#ifndef __CLOUDCOMPUTINGWORKLOADS_SOURCESINK_H
#define __CLOUDCOMPUTINGWORKLOADS_SOURCESINK_H

#include "debugOptions.h"

#ifdef DEBUG_COHERENCE
#include "CoherenceCheck.h"
#endif

#include <omnetpp.h>
#include "Task_m.h"

using namespace omnetpp;

namespace cloudcomputingworkloads {


// Module that generates the tasks and sends them to CloudNode
class TaskGenerator : public cSimpleModule
#ifdef DEBUG_COHERENCE
, public CoherenceCheck 
#endif
{
    // Mean time T between two consecutive arrivals in the network
    double timeDistributionMean;
    // Distribuition type of the time between two consecutive arrivals in the network
    // 0 = constant at value T
    // 1 = exponential with mean T
    // 2 = unifrom from 0 to 2T
    int timeDistributionType;
    
    // Mean number I of instruction needed to process the packet in the CloudNode
    double instrDistributionMean;
    // Distribuition type of the number of instruction needed to process the packet in the CloudNode
    // 0 = constant at value I
    // 1 = exponential with mean I
    // 2 = unifrom from 0 to 2I
    int instrDistributionType;

    // Timer task used to schedule events for the next packet to send out
    Task *timer_;

    protected:
        // Module initialization
        virtual void initialize();
        // Timer end handler
        virtual void handleMessage(cMessage *timer);
        // Next timer event scheduler
        virtual void scheduleNext();
};

// Sink module that collects completed tasks that exit the network and computes statistics
class Sink : public cSimpleModule
#ifdef DEBUG_COHERENCE
, public CoherenceCheck 
#endif
{
    private:
        // Signal for task response time statistic
        simsignal_t taskTime;
    protected:
        // Module initialization
        virtual void initialize();
        // Incoming message handler
        virtual void handleMessage(cMessage *msg);
};

}; // Namespace

#endif
