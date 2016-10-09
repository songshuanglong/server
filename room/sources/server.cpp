#include "log.h"
#include "netacceptor.h"
#include "server.h"

CServer::CServer()
{

}

CServer::ServerPtr CServer::get()
{
    ServerPtr _new(new CServer());

    return _new;
}

CServer::IOService &CServer::service()
{
    return _service;
}

int CServer::start(int argc, char **argv)
{
    if(!parseCommandLine(argc, argv)){
        LOG_ERROR() << "Parse command line error!";
        return -1;
    }

    if(!initialize()){
        LOG_ERROR() << "Server initialize failed!";
        return -1;
    }

    return run();
}

bool CServer::parseCommandLine(int argc, char **argv)
{
    LOG_TRACE() << "Server boot argument count: " << argc;

    for(int i = 0; i < argc; ++i){
        LOG_TRACE() << "Argument " << i + 1 << ": " << argv[i];
    }

    return true;
}

bool CServer::initialize()
{
    return true;
}

bool CServer::initSettings()
{
    return true;
}

bool CServer::initPools()
{
    return true;
}

bool CServer::initNetwork()
{
    _netAcceptor = CNetAcceptor::get
            (
                shared_from_this(),
                Endpoint
                (
                    boost::asio::ip::tcp::v4(),
                    19001
                )
            );
    if(!_netAcceptor.get()){
        LOG_ERROR() << "Create net acceptor instance failed!";
        return false;
    }

    _netAcceptor->accept();
    return true;
}

int CServer::run()
{
    if(!startThreads(16)){
        LOG_ERROR() << "All work threads create failed!";
        return -1;
    }

    routine();
    return 0;
}

int CServer::startThreads(int count)
{
    int ret = 0;

    for(int i = 0; i < count; ++i){
        if(!_threadPool.create_thread(boost::bind(&CServer::routine, this))){
            LOG_WARNING() << "Thread " << i + 1 << " create failed!";
        }else{
            ++ret;
        }
    }

    if(ret){
        LOG_TRACE() << "Ashenvale game room server use " << ret << " count threads to work.";
    }

    return ret;
}

void CServer::routine()
{
    _service.run();
}
