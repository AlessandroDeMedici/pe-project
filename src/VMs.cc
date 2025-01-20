#include "VMs.h"
#include <omnetpp.h>
#include <list>

using namespace omnetpp;
using namespace std;

// constructor
//

VMs::VMs(int nVMs, double processingRate, bool fairSharing)
{
    this->nVMs = nVMs;
    this->nActiveVMs = 0;
    this->runningTasks = new list<taskElement>(); 
    this->lastTaskUpdate = 0;
    this->fairSharing = fairSharing;
    this->processingRate = processingRate;
}

// destructor

VMs::~VMs()
{
    delete runningTasks;
}

bool VMs::availableVM()
{
    return nActiveVMs < nVMs;
}

simtime_t VMs::addTask(Task* task, simtime_t currentTime)
{
    if (nActiveVMs < nVMs) {


      // decrement first element by executedInstructions if exists
      if (!runningTasks->empty()) {
        runningTasks->front().remainingInstructions -= (currentTime - lastTaskUpdate).dbl() * currentProcessingRate();
      }
      
      double taskLength = task->getTaskLength();
      double accumulator = 0;
      
      taskElement newTask;
      newTask.task = task;

      for (list<taskElement>::iterator it = runningTasks->begin(); it != runningTasks->end(); it++) {
        
        // insert task when accumulator is greater than taskLength
        if (accumulator + it->remainingInstructions >= taskLength) {
          // non dovrei metterlo nello heap ? 
          newTask.remainingInstructions = taskLength - accumulator;
          runningTasks->insert(it, newTask);
          break;
        }
        accumulator += it->remainingInstructions;
      }

      // if task is not inserted, append it to the end
      if (accumulator < taskLength) {
        newTask.remainingInstructions = taskLength - accumulator;
        runningTasks->push_back(newTask);
      }

      lastTaskUpdate = currentTime;
      nActiveVMs++;

      return runningTasks->front().remainingInstructions / currentProcessingRate();

    } else {
        return -1;
    }
}


Task* VMs::taskFinished(simtime_t &t, simtime_t currentTime)
{
    // remove the task in front of the list and put to task pointer
    Task *finishedTask = runningTasks->front().task;

    runningTasks->pop_front();
    nActiveVMs--;
    lastTaskUpdate = currentTime;
    
    if (runningTasks->empty()) {
        t = -1;
    } else {

      t = runningTasks->front().remainingInstructions / currentProcessingRate();
    }

    return finishedTask;
}

double VMs::currentProcessingRate()
{
    if (fairSharing) {
        return processingRate / nActiveVMs;
    } else {
        return processingRate / nVMs;
    }
}


