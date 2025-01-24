#ifndef __CLOUDCOMPUTINGWORKLOADS_SOURCESINK_H
#define __CLOUDCOMPUTINGWORKLOADS_SOURCESINK_H

#include <omnetpp.h>
#include "Task_m.h"

using namespace omnetpp;

namespace cloudcomputingworkloads {

/**
 * Implements the Txc simple module. See the NED file for more information.
 */
class TaskGenerator : public cSimpleModule
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
{
  private:
    simsignal_t delaySignal;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

}; // namespace

#endif
