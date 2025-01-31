#ifndef __CLOUDCOMPUTINGWORKLOADS_BACKENDSERVER_H
#define __CLOUDCOMPUTINGWORKLOADS_BACKENDSERVER_H

#include "debugOptions.h"

#ifdef DEBUG_COHERENCE
#include "CoherenceCheck.h"
#endif

#include "Task_m.h"
#include <omnetpp.h>
#include <queue>

using namespace omnetpp;

namespace cloudcomputingworkloads {

// BackendServer module: takes packets as inputs, serves them at the specified rate and then forwards them
class BackendServer : public cSimpleModule
#ifdef DEBUG_COHERENCE
, public CoherenceCheck 
#endif
{
    private:
        // Serving rate S: Number of tasks served, on average, in a second
        double rate;

        // Distribuition type of the serving rate 
        // 0 = constant at value 1/S
        // 1 = exponential with mean 1/S
        // 2 = unifrom from 0 to 2/S
        int backendDistributionType;

        // Fifo queue for incoming tasks
        std::queue<Task *> fifoQueue;

        // Support variable used to track whether the server is processing a task
        bool scheduled;
        
        // Signals used to keep track of statistics
        simsignal_t Nq; // Number of elements in the queue
        simsignal_t W; // Task waiting time
        simsignal_t R; // Task response time
        simsignal_t CPU; // Utilization

    protected:
        // Module initialization
        virtual void initialize();
        // Message handler (both internal and external)
        virtual void handleMessage(cMessage *msg);
        // Function to schedule task elaboration
        virtual void scheduleElaborationEnd(Task *msg);
        // Simulation end handler
        virtual void finish();
};

}; // Namespace

#endif
