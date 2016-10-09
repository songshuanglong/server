#ifndef CSERVER_H
#define CSERVER_H

#include "common.h"
#include "pool.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <set>

class CBuffer;
class CPlayer;
class CTable;
class CNetConnection;
class CNetAcceptor;

class CServer :
        public boost::enable_shared_from_this< CServer >,
        protected boost::noncopyable
{
public:
    typedef boost::shared_ptr< CServer > ServerPtr;
    typedef boost::shared_ptr< CBuffer > BufferPtr;
    typedef boost::shared_ptr< CPlayer > PlayerPtr;
    typedef boost::shared_ptr< CTable > TablePtr;
    typedef boost::shared_ptr< CNetConnection > NetConnectionPtr;
    typedef boost::shared_ptr< CNetAcceptor > NetAcceptorPtr;

    typedef boost::asio::ip::tcp::endpoint Endpoint;
    typedef boost::asio::io_service IOService;
    typedef boost::mutex Mutex;
    typedef boost::thread_group ThreadPool;
    typedef CPool< CBuffer > BufferPool;
    typedef CPool< CPlayer > PlayerPool;
    typedef CPool< CTable > TablePool;
    typedef CPool< CNetConnection > NetConnectionPool;

    typedef boost::shared_ptr< BufferPool > BufferPoolPtr;
    typedef boost::shared_ptr< PlayerPool > PlayerPoolPtr;
    typedef boost::shared_ptr< TablePool > TablePoolPtr;
    typedef boost::shared_ptr< NetConnectionPool > NetConnectionPoolPtr;

    typedef std::set< NetConnectionPtr > NetConnectionQueue;
    typedef std::set< TablePtr > TableQueue;
    typedef std::set< PlayerPtr > PlayerQueue;

public:
    static ServerPtr get();

    IOService &service();

    void newNetConnection(const NetConnectionPtr &con);
    void removeNetConnection(const NetConnectionPtr &con);

    NetConnectionPtr allocNetConnection();
    void releaseNetConnection(const NetConnectionPtr &con);

    BufferPtr allocBuffer();
    void releaseBuffer(const BufferPtr &buf);

    bool onSysMessage(unsigned short type, void *data, int len);

    int start(int argc, char **argv);
protected:
    bool parseCommandLine(int argc, char **argv);
    bool initialize();
    bool initSettings();
    bool initPools();
    bool initNetwork();
    int run();

    int startThreads(int count);
    void routine();

    CServer();

private:
    IOService _service;

    NetAcceptorPtr _netAcceptor;

    ThreadPool _threadPool;

    BufferPoolPtr _bufferPool;

    PlayerPoolPtr _playerPool;

    TablePoolPtr _tablePool;

    NetConnectionPoolPtr _netConnectionPool;

    PlayerQueue _playerQueue;

    TableQueue _tableQueue;

    NetConnectionQueue _netConnectionQueue;
};

#endif // CSERVER_H
