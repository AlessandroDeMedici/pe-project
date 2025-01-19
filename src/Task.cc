#include "Task.h"

namespace cloudcomputingworkloads {

Define_Module(TaskGenerator);
Define_Module(Sink);

void TaskGenerator::initialize()
{

}

void TaskGenerator::handleMessage(cMessage *msg)
{
    // just send back the message we received
    send(msg, "out");
}

void Sink::initialize()
{

}

void Sink::handleMessage(cMessage *msg)
{
    // just send back the message we received
    send(msg, "out");
}

}; // namespace
