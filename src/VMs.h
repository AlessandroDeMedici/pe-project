#ifndef __CLOUDCOMPUTINGWORKLOADS_VMS_H
#define __CLOUDCOMPUTINGWORKLOADS_VMS_H

#include <list>
#include <omnetpp.h>
#include "Task_m.h"

using namespace omnetpp;

class VMs
{
  struct taskElement {
    double remainingInstructions;
    Task *task; 
  };
  private:
    int nVMs;
    
    simtime_t lastTaskUpdate;
    bool fairSharing;
    double processingRate;
    double currentProcessingRate();

  public:
    int nActiveVMs;
    std::list<taskElement> runningTasks;
    VMs(){};
    VMs(int nVMs, double processingRate, bool fairSharing);
    bool availableVM();
    simtime_t addTask(Task* task, simtime_t currentTime);
    Task* taskFinished(simtime_t &t, simtime_t currentTime);
    void printList();    
};

#endif


