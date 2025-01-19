#include "CloudNode.h"
#include "Task_m.h"

namespace cloudcomputingworkloads {

Define_Module(CloudNode);

void CloudNode::initialize()
{
  pRandomStream = par("pRandomStream");
  p = par("backendProcessingProbability");
}

void CloudNode::handleMessage(cMessage *msg)
{
    EV << "Received message " << msg->getName() << " from " << msg->getArrivalGate()->getName() << "length: " << check_and_cast<Task *>(msg)->getTaskLength() << endl;

    // just send back the message we received
    taskFinished(check_and_cast<Task *>(msg));
}


void CloudNode::taskFinished(Task *task)
{
    double random_val = uniform(0, 1, pRandomStream);
    if (random_val < p) {
        send(task, "backend");
    } else {
        send(task, "sink");
    }
}
}; // namespace
