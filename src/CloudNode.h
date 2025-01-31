#ifndef __CLOUDCOMPUTINGWORKLOADS_CLOUDNODE_H
#define __CLOUDCOMPUTINGWORKLOADS_CLOUDNODE_H

#include "debugOptions.h"

#ifdef DEBUG_COHERENCE
#include "CoherenceCheck.h"
#endif

#include "Task_m.h"
#include "VMs.h"
#include <omnetpp.h>
#include <queue>

using namespace omnetpp;

namespace cloudcomputingworkloads {

// CloudNode module that is made of N VMs that elaborate tasks in parallel
// Has a FIFO queue and can run in either segregation or fair sharing mode
// after elaboration forwards tasks to BackendServer with probability p or to Sink with probability 1-p
class CloudNode : public cSimpleModule
#ifdef DEBUG_COHERENCE
, public CoherenceCheck 
#endif
{
    private:
        // Number of VMs inside the CloudNode N
        int numVMs;
        // Processing rate R (instructions per second)
        double processingRate;
        // If true use fair sharing mode, else segregation mode
        bool fairSharing;
        // Probability of forwarding to the BackendServer for extra elaboration
        double p;

        // Fifo queue for incoming tasks
        std::queue<Task *> fifoQueue;
        // VMs class that keeps track of what VMs are active, their rate and the remaining instruction to end the elaboration
        VMs vm;
        // Support message used internally to handle elaboration end events
        cMessage *nextEvent;

        // Time of last departure (used to compute interdeparture time)
        simtime_t lastDepartureTime;

        // Signals used to keep track of statistics
        simsignal_t Nq; // Number of elements in the queue
        simsignal_t W; // Task waiting time
        simsignal_t R; // Task response time
        simsignal_t activeVMs; // Number of active VMs
        simsignal_t interDepartureTime; // Interdeparture time

    protected:
        // Module initialization
        virtual void initialize();
        // Message handler (both internal and external)
        virtual void handleMessage(cMessage *msg);
        // Function to forward finished tasks to either Sink or BackendServer based on probability p
        virtual void forwardFinishedTask(Task *task);
        // Simulation end handler
        virtual void finish();
};

}; // Namespace

#endif
