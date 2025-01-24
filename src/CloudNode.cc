#include "CloudNode.h"
#include "Task_m.h"
#include <vector>
#include <queue>
#include <omnetpp.h>
#include "VMs.h"

namespace cloudcomputingworkloads {

Define_Module(CloudNode);

void CloudNode::initialize()
{
  numVMs = par("numVMs");
  processingRate = par("processingRate");

  pRandomStream = par("pRandomStream");
  p = par("backendProcessingProbability");
  
  fairSharing = par("fairSharing");

  nextEvent = new cMessage("taskFinished");

  fifoQueue = std::queue<Task *>();
  vm = VMs(numVMs, processingRate, fairSharing);

  Nq = registerSignal("cloudNodeNq");
  W = registerSignal("cloudNodeW");
  R = registerSignal("cloudNodeR");
  activeVMs = registerSignal("cloudNodeActiveVMs");
}

void CloudNode::handleMessage(cMessage *msg)
{
    simtime_t t = -1;
    if (msg == nextEvent) {
        Task * finishedTask = vm.taskFinished(t, simTime());

        if (!fifoQueue.empty()) {
            Task *task = fifoQueue.front();
            fifoQueue.pop();
            t = vm.addTask(task, simTime());
            emit(W, (simTime() - task->getArrivalTime()).dbl());  // time spent in the queue
        }
        
        emit(R, (simTime() - finishedTask->getArrivalTime()).dbl());  // time spent in the system
        forwardFinishedTask(finishedTask);
    } else {
      if (vm.availableVM()) {
          t = vm.addTask(check_and_cast<Task *>(msg), simTime());
          emit(W, 0.0);     // 0 means that the task was processed immediately
      } else {
          fifoQueue.push(check_and_cast<Task *>(msg));
      } 
    }

    if (t > 0) {
        cancelEvent(nextEvent);
        scheduleAt(simTime() + t, nextEvent);
    }
    EV << (msg==nextEvent ? "self message" : "external message") << endl;
    EV << "Current queue length: " << fifoQueue.size() << endl;
    EV << "Current active VMs: " << vm.nActiveVMs << "  VM list length: " << vm.runningTasks.size() << endl;
    
    char status[32];
    sprintf(status, "Active VMs %lu/%d\nQueue %lu", vm.runningTasks.size(), numVMs, fifoQueue.size());
    getDisplayString().setTagArg("t", 0, status);
  
    emit(Nq, fifoQueue.size());
    emit(activeVMs, vm.runningTasks.size());
}

void CloudNode::finish()
{
    cancelAndDelete(nextEvent);
    while (!fifoQueue.empty()) {
        delete fifoQueue.front();
        fifoQueue.pop();
    }
    while (vm.nActiveVMs > 0) {
        simtime_t t;
        Task *finishedTask = vm.taskFinished(t, simTime());
        cancelAndDelete(finishedTask);
    }
}

void CloudNode::forwardFinishedTask(Task *task)
{
    double random_val = uniform(0, 1, pRandomStream);
    if (random_val < p) {
        send(task, "backend");
    } else {
        send(task, "sink");
    }
}
};
