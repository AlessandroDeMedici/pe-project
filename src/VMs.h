#ifndef __VMs_h
#define __VMs_h

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
    ~VMs();
    bool availableVM();
    simtime_t addTask(Task* task, simtime_t currentTime);
    Task* taskFinished(simtime_t &t, simtime_t currentTime);
    
};

#endif


