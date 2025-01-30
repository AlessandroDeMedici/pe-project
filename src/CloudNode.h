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

class CloudNode : public cSimpleModule
#ifdef DEBUG_COHERENCE
, public CoherenceCheck 
#endif
{
  private:
    int numVMs;
    double processingRate;
    bool fairSharing;
    double p;
    int pRandomStream;
    std::queue<Task *> fifoQueue;
    VMs vm;
    cMessage *nextEvent;

    simtime_t lastDepartureTime;

    simsignal_t Nq;
    simsignal_t W;
    simsignal_t R;
    simsignal_t activeVMs;
    simsignal_t interDepartureTime;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void forwardFinishedTask(Task *task);
    virtual void finish();
};

};

#endif
