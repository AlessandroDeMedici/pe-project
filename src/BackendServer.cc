#include "BackendServer.h"

namespace cloudcomputingworkloads {

Define_Module(BackendServer);

void BackendServer::initialize()
{
  rate = par("S");
  backendDistributionType = par("backendDistributionType");

  fifoQueue = std::queue<Task *>();
  scheduled = false;

  Nq = registerSignal("backendNq");
  W = registerSignal("backendW");
  R = registerSignal("backendR");
  CPU = registerSignal("backendCPU");
}

void BackendServer::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        emit(R, (simTime() - check_and_cast<Task *>(msg)->getNodeArrivalTime()).dbl());
        send(msg, "out");

        scheduled = false;
        
        if (!fifoQueue.empty()) {
            Task *nextTask = fifoQueue.front();
            emit(W, (simTime() - nextTask->getArrivalTime()).dbl());  // time spent in the queue
            fifoQueue.pop();
            scheduleElaborationEnd(nextTask);
        }
    } else {
        Task *task = check_and_cast<Task *>(msg);

        task->setNodeArrivalTime(simTime());

        if (fifoQueue.empty() && !scheduled) {
          scheduleElaborationEnd(task);

          emit(W, 0.0); // task is being processed immediately
        } else {
          fifoQueue.push(task);
        }
    }
    
    char status[32];
    sprintf(status, "Queue: %ld", fifoQueue.size());
    getDisplayString().setTagArg("t", 0, status);

    emit(Nq, fifoQueue.size());
    emit(CPU, (int) scheduled);
    
    #ifdef DEBUG_COHERENCE
    setNTasks(fifoQueue.size() + scheduled);
    #endif
}

void BackendServer::scheduleElaborationEnd(Task *msg)
{
    double backendDelay;
    switch (backendDistributionType) {
        case 0:
            backendDelay = 1/rate;
            break;
        case 1:
            backendDelay = exponential(1/rate, 0);
            break;
        case 2:
            backendDelay = uniform(0, 2/rate, 0);
            break;
        default:
            throw cRuntimeError("Unknown distribution type: %c", backendDistributionType);
    }

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
