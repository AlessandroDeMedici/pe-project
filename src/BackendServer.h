#ifndef __MYFIRSTPROJECT_BACKENDSERVER_H
#define __MYFIRSTPROJECT_BACKENDSERVER_H

#include "Task_m.h"
#include <omnetpp.h>
#include <queue>

using namespace omnetpp;

namespace cloudcomputingworkloads {

class BackendServer : public cSimpleModule
{
  double backendDistributionMean;
  int backendDistributionType;
  int backendRandomStream;
  std::queue<Task *> fifoQueue;
  bool scheduled;
    
    simsignal_t Nq;
    simsignal_t W;
    simsignal_t R;
    simsignal_t CPU;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void scheduleElaborationEnd(Task *msg);
    virtual void finish();
};

};

#endif
