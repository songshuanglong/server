#ifndef CNETCONNECTION_H
#define CNETCONNECTION_H

#include "common.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <deque>

class CBuffer;
class CPlayer;
class CServer;

class CNetConnection :
        public boost::enable_shared_from_this< CNetConnection >,
        protected boost::noncopyable
{
public:
    typedef boost::shared_ptr< CNetConnection > NetConnectionPtr;
    typedef boost::shared_ptr< CBuffer > BufferPtr;
    typedef boost::shared_ptr< CPlayer > PlayerPtr;
    typedef boost::shared_ptr< CServer > ServerPtr;

    typedef boost::asio::ip::tcp::socket Socket;
    typedef boost::system::error_code ErrorCode;
    typedef boost::mutex Mutex;

    typedef std::deque< BufferPtr > BufferQueue;

public:
    static NetConnectionPtr get(const ServerPtr &svr);

    Socket &sock();

    void read();
    bool write(void *data, int len);
    void close();

protected:
    CNetConnection(const ServerPtr &svr);

    void asyncRead();
    void asyncWrite();
    void asyncDispatch();

    size_t isReadComplete(BufferPtr buf, const ErrorCode &ec, size_t size);
    size_t isWriteComplete(BufferPtr buf, const ErrorCode &ec, size_t size);

    void onReadComplete(BufferPtr buf, const ErrorCode &ec, size_t size);
    void onWriteComplete(BufferPtr buf, const ErrorCode &ec, size_t size);
    void onDispatch(BufferPtr buf);

    bool onBeat(unsigned short type, void *data, int len);

private:
    ServerPtr _server;

    PlayerPtr _player;

    Socket _sock;

    BufferQueue _readQueue;

    BufferQueue _writeQueue;

    Mutex _objectLock;

    Mutex _readQueueLock;

    Mutex _writeQueueLock;

    bool _isClosed;

    bool _isWritting;

    bool _isDispatching;
};

#endif // CNETCONNECTION_H
