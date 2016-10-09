#ifndef CNETACCEPTOR_H
#define CNETACCEPTOR_H

#include "common.h"
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

class CNetConnection;
class CServer;

class CNetAcceptor :
        protected boost::noncopyable
{
public:
    typedef boost::shared_ptr< CNetAcceptor > NetAcceptorPtr;
    typedef boost::shared_ptr< CNetConnection > NetConnectionPtr;
    typedef boost::shared_ptr< CServer > ServerPtr;

    typedef boost::asio::ip::tcp::endpoint Endpoint;
    typedef boost::asio::ip::tcp::acceptor Acceptor;
    typedef boost::system::error_code ErrorCode;

public:
    static NetAcceptorPtr get(const ServerPtr &svr, const Endpoint &ep);

    void accept();

protected:
    CNetAcceptor(const ServerPtr &svr, const Endpoint &ep);

    void onAccept(NetConnectionPtr con, const ErrorCode &ec);

private:
    // Room server instance
    ServerPtr _server;

    // TCP acceptor
    Acceptor _acceptor;
};

#endif // CNETACCEPTOR_H
