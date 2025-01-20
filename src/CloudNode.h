//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __MYFIRSTPROJECT_CLOUDNODE_H
#define __MYFIRSTPROJECT_CLOUDNODE_H

#include <omnetpp.h>
#include "Task_m.h"
#include <queue>
#include "VMs.h"

using namespace omnetpp;

namespace cloudcomputingworkloads {

class CloudNode : public cSimpleModule
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

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void forwardFinishedTask(Task *task);
    virtual void finish();
};

};

#endif
