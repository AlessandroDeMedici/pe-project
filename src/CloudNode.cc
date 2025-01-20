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

  // initialize fifo queue
  fifoQueue = new std::queue<Task *>();
  vm = new VMs(numVMs, processingRate, fairSharing);
}

void CloudNode::handleMessage(cMessage *msg)
{
    //EV << "Received message " << msg->getName() << " from " << msg->getArrivalGate()->getName() << "length: " << check_and_cast<Task *>(msg)->getTaskLength() << endl;
    
    simtime_t t = -1;
    if (msg->isSelfMessage()) {
        EV << "Self message" << endl;
        Task * finishedTask = vm->taskFinished(t, simTime());

        // check if there are tasks in the queue
        if (!fifoQueue->empty()) {
            Task *task = fifoQueue->front();
            fifoQueue->pop();
            t = vm->addTask(task, simTime());
            EV << "Task added to VMs after ended job, next task end in " << t << endl;
        }
        EV << "Task finished, next task end in " << t << endl;
        taskFinished(finishedTask);
    } else {

      if (vm->availableVM()) {
          t = vm->addTask(check_and_cast<Task *>(msg), simTime());
          EV << "Task added to VMs, next task end in " << t << endl;
      } else {
          fifoQueue->push(check_and_cast<Task *>(msg));
          EV << "Task added to queue, current queue length: " << fifoQueue->size() << endl;
      } 
    }
    // schedule next event after time t
    msg = new cMessage("taskFinished");

    // create nextEvent variable

    if (t > 0) {
        // reschedule the message and save it to delete it later
        cancelAndDelete(nextEvent);
        nextEvent = new cMessage("taskFinished");
        scheduleAt(simTime() + t, nextEvent);
    }
    cancelAndDelete(msg);
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
