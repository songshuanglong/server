// Ashenvale game room server
// Server start entry
#include "log.h"
#include "server.h"

int main(int argc, char **argv)
{
    LOG_TRACE() << "Ashenvale game room server.";

    CServer::ServerPtr server = CServer::get();

    if(!server.get()){
        LOG_ERROR() << "Create server instance failed!";
        return -1;
    }

    return server->start(argc, argv);
}
