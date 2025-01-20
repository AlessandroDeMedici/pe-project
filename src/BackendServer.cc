#include "BackendServer.h"
#include "Task_m.h"
#include <queue>

namespace cloudcomputingworkloads {

Define_Module(BackendServer);

void BackendServer::initialize()
{
  backendDistributionMean = par("backendDistributionMean"); 
  backendDistributionType = par("backendDistributionType");
  backendRandomStream = par("backendRandomStream");

  fifoQueue = std::queue<Task *>();
}

void BackendServer::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        send(msg, "out");

        if (!fifoQueue.empty()) {
            Task *nextTask = fifoQueue.front();
            fifoQueue.pop();
            scheduleElaborationEnd(nextTask);
        }
    } else {
        Task *task = check_and_cast<Task *>(msg);

        if (fifoQueue.empty()) {
          scheduleElaborationEnd(task);
        } else {
          fifoQueue.push(task);
        }
    }
}

void BackendServer::scheduleElaborationEnd(Task *msg)
{
    double processingRate;
    switch (backendDistributionType) {
        case 0:
            processingRate = backendDistributionMean;
            break;
        case 1:
            processingRate = exponential(backendDistributionMean, backendRandomStream);
            break;
        case 2:
            processingRate = uniform(0, 2*backendDistributionMean, backendRandomStream);
            break;
        default:
            throw cRuntimeError("Unknown distribution type: %c", backendDistributionType);
    }
    
    scheduleAt(simTime() + msg->getTaskLength() / processingRate, msg);
}

void BackendServer::finish()
{
  // TODO: strano che non si lamenti che non vengono distrutti gli eventi nella coda, da indagare
}

}; // namespace
