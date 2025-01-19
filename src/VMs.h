#ifndef __VMs_h
#define __VMs_h

#include <list>
#include <omnetpp.h>

class VMs
{
  private:
    int nVMs;
    int nActiveVMs;
    // list
    std::list<double> *runningTasks;
    simtime_t lastTaskUpdate;
  
  public:
    VMs(int nVMs);
    ~VMs();
    bool availableVM();
    simtime_t addTask(double taskLength, simtime_t currentTime);
    void taskFinished(simtime_t currentTime);
};

#endif


