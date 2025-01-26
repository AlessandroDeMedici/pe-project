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

class TaskGenerator : public cSimpleModule
#ifdef DEBUG_COHERENCE
, public CoherenceCheck 
#endif
{
  int timeDistributionType;
  double timeDistributionMean;
  int timeRandomStream;

  int instrDistributionType;
  double instrDistributionMean;
  int instrRandomStream;

  Task *timer_;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *timer);
    virtual void scheduleNext();
};

class Sink : public cSimpleModule
#ifdef DEBUG_COHERENCE
, public CoherenceCheck 
#endif
{
  private:
    simsignal_t taskTime;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

};

#endif
