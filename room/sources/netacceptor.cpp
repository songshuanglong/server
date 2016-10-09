#include "log.h"
#include "netconnection.h"
#include "server.h"
#include "netacceptor.h"

CNetAcceptor::CNetAcceptor(const ServerPtr &svr, const Endpoint &ep) :
    _server(svr),
    _acceptor(svr->service(), ep)
{

}

CNetAcceptor::NetAcceptorPtr CNetAcceptor::get(const ServerPtr &svr, const Endpoint &ep)
{
    NetAcceptorPtr _new(new CNetAcceptor(svr, ep));

    return _new;
}

void CNetAcceptor::accept()
{
    NetConnectionPtr con = _server->allocNetConnection();

    _acceptor.async_accept
            (
                con->sock(),
                boost::bind
                (
                    &CNetAcceptor::onAccept,
                    this,
                    con,
                    _1
                )
            );
}

void CNetAcceptor::onAccept(NetConnectionPtr con, const ErrorCode &ec)
{
    if(ec){
        _server->releaseNetConnection(con);

        if(ec.value() == boost::system::errc::operation_canceled){
            LOG_WARNING() << ec;
            return;
        }else{
            LOG_ERROR() << ec;
            exit(-1);
        }
    }

    _server->newNetConnection(con);

    accept();
}
