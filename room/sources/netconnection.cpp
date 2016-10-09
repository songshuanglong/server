#include "log.h"
#include "buffer.h"
#include "player.h"
#include "server.h"
#include "netconnection.h"

CNetConnection::CNetConnection(const ServerPtr &svr) :
    _server(svr),
    _sock(svr->service()),
    _isClosed(true),
    _isWritting(false),
    _isDispatching(false)
{

}

CNetConnection::NetConnectionPtr CNetConnection::get(const ServerPtr &svr)
{
    NetConnectionPtr _new(new CNetConnection(svr));

    return _new;
}

CNetConnection::Socket &CNetConnection::sock()
{
    return _sock;
}

void CNetConnection::read()
{
    Mutex::scoped_lock lock(_objectLock);

    if(!_isClosed) return;

    _isClosed = false;

    asyncRead();
}

bool CNetConnection::write(void *data, int len)
{
    {
        Mutex::scoped_lock lock(_objectLock);

        if(_isClosed) return false;
    }

    BufferPtr buf = _server->allocBuffer();

    buf->reset();
    memcpy(buf->memory(), data, len);
    buf->setCurrrent(len);

    {
        Mutex::scoped_lock lock(_writeQueueLock);

        _writeQueue.push_back(buf);
    }

    asyncWrite();

    return true;
}

void CNetConnection::close()
{
    ErrorCode ec;
    Mutex::scoped_lock lock(_objectLock);

    if(_isClosed) return;

    _isClosed = true;

    _sock.cancel(ec);
    if(ec){
        LOG_WARNING() << ec;
    }

    ec.clear();
    _sock.close(ec);
    if(ec){
        LOG_WARNING() << ec;
    }
}

void CNetConnection::asyncRead()
{
    BufferPtr buf = _server->allocBuffer();

    boost::asio::async_read
            (
                _sock,
                boost::asio::buffer
                (
                    buf->memory(),
                    buf->total()
                ),
                boost::bind
                (
                    &CNetConnection::isReadComplete,
                    this,
                    buf,
                    _1,
                    _2
                ),
                boost::bind
                (
                    &CNetConnection::onReadComplete,
                    this,
                    buf,
                    _1,
                    _2
                )
            );
}

void CNetConnection::asyncWrite()
{
    BufferPtr buf;

    Mutex::scoped_lock lock(_writeQueueLock);

    if(_isWritting) return;

    if(_writeQueue.empty()) return;

    _isWritting = true;

    buf = _writeQueue.front();
    _writeQueue.pop_front();

    boost::asio::async_write
            (
                _sock,
                boost::asio::buffer
                (
                    buf->memory(),
                    buf->current()
                ),
                boost::bind
                (
                    &CNetConnection::isWriteComplete,
                    this,
                    buf,
                    _1,
                    _2
                ),
                boost::bind
                (
                    &CNetConnection::onWriteComplete,
                    this,
                    buf,
                    _1,
                    _2
                )
            );
}

void CNetConnection::asyncDispatch()
{
    {
        Mutex::scoped_lock lock(_objectLock);

        if(_isClosed)
            return;
    }

    BufferPtr buf;

    Mutex::scoped_lock lock(_readQueueLock);

    if(_isDispatching)
        return;

    if(_readQueue.empty())
        return;

    buf = _readQueue.front();
    _readQueue.pop_front();

    _server->service().post
            (
                boost::bind
                (
                    &CNetConnection::onDispatch,
                    this,
                    buf
                )
            );
}

size_t CNetConnection::isReadComplete(BufferPtr buf, const ErrorCode &ec, size_t size)
{
    if(ec){
        if(ec.value() == boost::system::errc::operation_canceled){
            LOG_WARNING() << ec;
        }else{
            close();
            LOG_ERROR() << ec;
        }
        return 0;
    }

    if(size < sizeof(NMCommonHeader)){
        return sizeof(NMCommonHeader) - size;
    }

    NMCommonHeader *h = (NMCommonHeader *)buf->memory();
    if(h->len > sizeof(NMCommonHeader) + NM_LIMITS_BODY_LEN){
        close();
        return 0;
    }

    if(size < h->len){
        return h->len - size;
    }

    return 0;
}

size_t CNetConnection::isWriteComplete(BufferPtr buf, const ErrorCode &ec, size_t size)
{
    if(ec){
        if(ec.value() == boost::system::errc::operation_canceled){
            LOG_WARNING() << ec;
        }else{
            close();
            LOG_ERROR() << ec;
        }

        return 0;
    }

    if(size < buf->current()){
        return buf->current() - size;
    }

    return 0;
}

void CNetConnection::onReadComplete(BufferPtr buf, const ErrorCode &ec, size_t size)
{
    if(ec){
        close();
        _server->releaseBuffer(buf);
        _server->releaseNetConnection(shared_from_this());
        _server->removeNetConnection(shared_from_this());
        return;
    }

    buf->setCurrrent(size);

    {
        Mutex::scoped_lock lock(_readQueueLock);

        _readQueue.push_back(buf);
    }

    asyncDispatch();
}

void CNetConnection::onWriteComplete(BufferPtr buf, const ErrorCode &ec, size_t)
{
    if(ec){
        _server->releaseBuffer(buf);
        _server->releaseNetConnection(shared_from_this());
        _server->removeNetConnection(shared_from_this());
        return;
    }

    _server->releaseBuffer(buf);

    buf.reset();

    {
        Mutex::scoped_lock lock(_writeQueueLock);

        if(_writeQueue.empty()){
            _isWritting = false;
            return;
        }

        buf = _writeQueue.front();
        _writeQueue.pop_front();
    }

    boost::asio::async_write
            (
                _sock,
                boost::asio::buffer
                (
                    buf->memory(),
                    buf->current()
                ),
                boost::bind
                (
                    &CNetConnection::isWriteComplete,
                    this,
                    buf,
                    _1,
                    _2
                ),
                boost::bind
                (
                    &CNetConnection::onWriteComplete,
                    this,
                    buf,
                    _1,
                    _2
                )
            );
}

void CNetConnection::onDispatch(BufferPtr buf)
{
    bool ret = true;

    {
        Mutex::scoped_lock lock(_objectLock);

        if(_isClosed){
            _server->releaseBuffer(buf);
            return;
        }
    }

    NMCommonHeader *h = (NMCommonHeader *)buf->memory();
    switch(h->cmd >> 16){
    case NM_SYS:{
            ret = _server->onSysMessage(h->cmd & 0xffff, h->data, h->len - sizeof(NMCommonHeader));
        }
        break;
    case NM_USER:{
            ret = _player->onUserMessage(h->cmd & 0xffff, h->data, h->len - sizeof(NMCommonHeader));
        }
        break;
    case NM_GAME:{
            ret = _player->onGameMessage(h->cmd & 0xffff, h->data, h->len - sizeof(NMCommonHeader));
        }
        break;
    case NM_BEAT:{
            ret = onBeat(h->cmd & 0xffff, h->data, h->len - sizeof(NMCommonHeader));
        }
        break;
    default:{
            LOG_WARNING() << "Unknown message type! Ignore.";
        }
        break;
    }
    _server->releaseBuffer(buf);

    if(!ret){
        close();
        _server->releaseNetConnection(shared_from_this());
        _server->removeNetConnection(shared_from_this());
        return;
    }

    buf.reset();

    Mutex::scoped_lock lock(_readQueueLock);

    if(_readQueue.empty()){
        _isDispatching = false;
        return;
    }

    buf = _readQueue.front();
    _readQueue.pop_front();

    _server->service().post
            (
                boost::bind
                (
                    &CNetConnection::onDispatch,
                    this,
                    buf
                )
            );
}
