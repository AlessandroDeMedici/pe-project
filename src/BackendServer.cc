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
  scheduled = false;

  delaySignal = registerSignal("backendDelay");
  queueSignal = registerSignal("backendQueue");
}

void BackendServer::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        send(msg, "out");
        scheduled = false;
        
        if (!fifoQueue.empty()) {
            Task *nextTask = fifoQueue.front();
            fifoQueue.pop();
            scheduleElaborationEnd(nextTask);
        }
    } else {
        Task *task = check_and_cast<Task *>(msg);

        if (fifoQueue.empty() && !scheduled) {
          scheduleElaborationEnd(task);
        } else {
          fifoQueue.push(task);
        }
    }
    
    char status[32];
    sprintf(status, "Queue: %ld", fifoQueue.size());
    getDisplayString().setTagArg("t", 0, status);

    emit(queueSignal, fifoQueue.size());
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
    
    simtime_t backendDelay =  msg->getTaskLength() / processingRate;
    emit(delaySignal, backendDelay.dbl());

    scheduled = true;
    scheduleAt(simTime() + backendDelay, msg);
}

void BackendServer::finish()
{
  // deallocate messages in the queue
  while (!fifoQueue.empty()) {
    Task *task = fifoQueue.front();
    fifoQueue.pop();
    delete task;
  }
}

}; // namespace
