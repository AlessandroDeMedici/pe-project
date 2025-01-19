#include "VMs.h"
#include <omnetpp.h>
#include <list>

using namespace omnetpp;

// constructor
//

VMs::VMs(int nVMs)
{
    this->nVMs = nVMs;
    this->nActiveVMs = 0;
    this->runningTasks = new std::list<double>();
    this->lastTaskUpdate = 0;
}

// destructor

VMs::~VMs()
{
    delete runningTasks;
}

VMs::availableVM()
{
    return nActiveVMs < nVMs;
}



