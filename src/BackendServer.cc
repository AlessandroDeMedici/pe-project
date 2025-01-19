#include "BackendServer.h"

namespace cloudcomputingworkloads {

Define_Module(BackendServer);

void BackendServer::initialize()
{

}

void BackendServer::handleMessage(cMessage *msg)
{
    // just send back the message we received
    send(msg, "out");
}

}; // namespace
