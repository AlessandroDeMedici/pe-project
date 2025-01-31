#include "BackendServer.h"

namespace cloudcomputingworkloads {

Define_Module(BackendServer);

// Module initialization
void BackendServer::initialize()
{
    // Serving rate S: Number of tasks served, on average, in a second
    rate = par("S");

    // Distribuition type of the serving rate 
    // 0 = constant at value 1/S
    // 1 = exponential with mean 1/S
    // 2 = unifrom from 0 to 2/S
    backendDistributionType = par("backendDistributionType");

    // Fifo queue for incoming tasks
    fifoQueue = std::queue<Task *>();

    // Support variable used to track whether the server is processing a task
    scheduled = false;

    // Signals used to keep track of statistics
    Nq = registerSignal("backendNq"); // Number of elements in the queue
    W = registerSignal("backendW"); // Task waiting time
    R = registerSignal("backendR"); // Task response time
    CPU = registerSignal("backendCPU"); // Utilization
}

// Message handler (both internal and external)
void BackendServer::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) { // Internal message handler (elaboration end events)

        // Record response time of the task
        emit(R, (simTime() - check_and_cast<Task *>(msg)->getNodeArrivalTime()).dbl());

        // Forward packet to output port
        send(msg, "out");

        // Update state variable
        scheduled = false;
        
        // If the queue is not empty schedule the next task
        if (!fifoQueue.empty()) {
            Task *nextTask = fifoQueue.front();
            // Record waiting time (time spent in the queue)
            emit(W, (simTime() - nextTask->getArrivalTime()).dbl());
            fifoQueue.pop();
            scheduleElaborationEnd(nextTask);
        }

    } else { // External message handler (packet arrival event)
        Task *task = check_and_cast<Task *>(msg);

        // Save arrival time to later compute statistics
        task->setNodeArrivalTime(simTime());

        // If the server is idle schedule the task for elaboration, otherwise insert it in the queue
        if (fifoQueue.empty() && !scheduled) {
            scheduleElaborationEnd(task);

            // Record 0 waiting time as the task doesn't enter the queue
            emit(W, 0.0); 
        } else {
            fifoQueue.push(task);
        }
    }
    
    #ifdef DEBUG_GUI_STATE
    char status[32];
    sprintf(status, "Queue: %ld", fifoQueue.size());
    getDisplayString().setTagArg("t", 0, status);
    #endif

    // Record Queue Size and Utilization
    emit(Nq, fifoQueue.size());
    emit(CPU, (int) scheduled);
    
    #ifdef DEBUG_COHERENCE
    setNTasks(fifoQueue.size() + scheduled);
    #endif
}

// Function to schedule task elaboration
void BackendServer::scheduleElaborationEnd(Task *msg)
{
    double backendDelay;
    // Determine the task's elaboration time based on backendDistributionType 
    switch (backendDistributionType) {
        case 0: // Constant at value 1/rate (1/S)
            backendDelay = 1/rate;
            break;
        case 1: // Exponentially distributed with mean 1/rate (1/S)
            backendDelay = exponential(1/rate, 0);
            break;
        case 2: // Unifromly distributed from 0 to 2/rate (2/S)
            backendDelay = uniform(0, 2/rate, 0);
            break;
        default:
            throw cRuntimeError("Unknown distribution type: %c", backendDistributionType);
    }

    // Update status variable
    scheduled = true;

    // Schedule an event after the elaboration time has passed
    scheduleAt(simTime() + backendDelay, msg);
}

// Simulation end handler
void BackendServer::finish()
{
    // Deallocate messages in the queue
    while (!fifoQueue.empty()) {
        Task *task = fifoQueue.front();
        fifoQueue.pop();
        delete task;
    }
}

}; // Namespace
