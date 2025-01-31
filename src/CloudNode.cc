#include "CloudNode.h"
#include "Task_m.h"
#include <vector>
#include <queue>
#include <omnetpp.h>
#include "VMs.h"

namespace cloudcomputingworkloads {

Define_Module(CloudNode);

// Module initialization
void CloudNode::initialize()
{
    // Number of VMs inside the CloudNode
    numVMs = par("N");
    // Processing rate R (instructions per second)
    processingRate = par("R");
    // If true use fair sharing mode, else segregation mode
    fairSharing = par("fairSharing");
    // Probability of forwarding to the BackendServer for extra elaboration
    p = par("p");

    // Support message used internally to handle elaboration end events
    nextEvent = new cMessage("taskFinished");
    // Fifo queue for incoming tasks
    fifoQueue = std::queue<Task *>();
    // VMs class that keeps track of what VMs are active, their rate and the remaining instruction to end the elaboration
    vm = VMs(numVMs, processingRate, fairSharing);

    // Time of last departure (used to compute interdeparture time)
    lastDepartureTime = 0;

    // Signals used to keep track of statistics
    Nq = registerSignal("cloudNodeNq");// Number of elements in the queue
    W = registerSignal("cloudNodeW"); // Task waiting time
    R = registerSignal("cloudNodeR"); // Task response time
    activeVMs = registerSignal("cloudNodeActiveVMs"); // Number of active VMs
    interDepartureTime = registerSignal("cloudNodeInterDepartureTime"); // Interdeparture time
}

// Message handler (both internal and external)
void CloudNode::handleMessage(cMessage *msg)
{
    simtime_t t = -1;
    if (msg == nextEvent) { // If the message is internal (a VM finished elaboration)
        // Get the finished task from the VMs class
        // The time t is passed as reference, so the function updates it to the time of next elaboration end (or -1 if all VMs are idle)
        Task * finishedTask = vm.taskFinished(t, simTime());

        // If the queue isn't empty remove its first element and send it to the VMs for elaboration
        if (!fifoQueue.empty()) {
            Task *task = fifoQueue.front();
            fifoQueue.pop();
            // Sends the tasks to the VMs
            // The time t is updated to the time of next elaboration end (or -1 if all VMs are idle)
            t = vm.addTask(task, simTime());

            // Record waiting time (time spent in the queue)
            emit(W, (simTime() - task->getArrivalTime()).dbl()); 
        }
        
        // Compute and record interdeparture time
        if (lastDepartureTime != 0) {
            emit(interDepartureTime, (simTime() - lastDepartureTime).dbl());
        }
        lastDepartureTime = simTime();

        // Record response time
        emit(R, (simTime() - finishedTask->getArrivalTime()).dbl()); 

        // Foward the finished task to either Sink or BackendServer
        forwardFinishedTask(finishedTask);

    } else { // If the message is external (a new task arrived)

        // If a VM is idle schedule the task for elaboration, otherwise insert it in the queue
        if (vm.availableVM()) {
            // Sends the tasks to the VMs
            // The time t is updated to the time of next elaboration end (or -1 if all VMs are idle)
            t = vm.addTask(check_and_cast<Task *>(msg), simTime());

            // Record 0 waiting time as the task doesn't enter the queue
            emit(W, 0.0);
        } else {
            fifoQueue.push(check_and_cast<Task *>(msg));
        } 
    }

    // Schedule an event when the next VM ends its elaboration
    if (t >= 0) {
        // If there is already an event scheduled, cancels it, otherwise if no event is scheduled does nothing
        cancelEvent(nextEvent);

        scheduleAt(simTime() + t, nextEvent);
    }
    
    #ifdef DEBUG_PRINT_STATE
    EV << (msg==nextEvent ? "self message" : "external message") << endl;
    EV << "Current queue length: " << fifoQueue.size() << endl;
    EV << "Current active VMs: " << vm.nActiveVMs << "  VM list length: " << vm.runningTasks.size() << endl;
    #endif
    
    #ifdef DEBUG_GUI_STATE
    char status[32];
    sprintf(status, "Active VMs %lu/%d\nQueue %lu", vm.runningTasks.size(), numVMs, fifoQueue.size());
    getDisplayString().setTagArg("t", 0, status);
    #endif
    
    // Record Queue Size and Number of active VMs
    emit(Nq, fifoQueue.size());
    emit(activeVMs, vm.runningTasks.size());
    
    #ifdef DEBUG_COHERENCE
    setNTasks(fifoQueue.size() + vm.runningTasks.size());
    #endif
}

// Function to forward finished tasks to either Sink or BackendServer based on probability p
void CloudNode::forwardFinishedTask(Task *task)
{
    double random_val = uniform(0, 1, 0);
    if (random_val < p) {
        send(task, "backend");
    } else {
        send(task, "sink");
    }
}

// Simulation end handler
void CloudNode::finish()
{
    // Cancel events and deallocate all memory
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

}; // Namespace