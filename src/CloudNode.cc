#include "CloudNode.h"

namespace cloudcomputingworkloads {

Define_Module(Queue);
Define_Module(VM);
Define_Module(Mux);

void Queue::initialize()
{
}

void Queue::handleMessage(cMessage *msg)
{
    // just send back the message we received
    send(msg, "out");
}

void VM::initialize()
{

}

void VM::handleMessage(cMessage *msg)
{
    // just send back the message we received
    send(msg, "out");
}

void Mux::initialize()
{

}

void Mux::handleMessage(cMessage *msg)
{
    // just send back the message we received
    send(msg, "out");
}

}; // namespace
