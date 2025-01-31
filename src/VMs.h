#ifndef __CLOUDCOMPUTINGWORKLOADS_VMS_H
#define __CLOUDCOMPUTINGWORKLOADS_VMS_H

#include <list>
#include <omnetpp.h>
#include "Task_m.h"

using namespace omnetpp;

// VMs class that keeps track of what VMs are active, their rate and the remaining instruction to end the elaboration
class VMs
{
    // List element struct for the runningTasks list
    struct taskElement {
        double remainingInstructions;
        Task *task; 
    };

    private:
        // Number of VMs N
        int nVMs;
        
        // Time of last update used to comupte the progress between updates
        simtime_t lastTaskUpdate;
        // If true use fair sharing mode, else segregation mode
        bool fairSharing;
        // Total processing rate R (instructions per second)
        double processingRate;
        // Function to get the current processing rate for the single VM
        // Returns processingRate/nActiveVMs if fairSharing is true, processingRate/nVMs otherwise
        double currentProcessingRate();

    public:
        // Number of active VMs
        int nActiveVMs;

        // List of tasks being served
        // The list is ordered and for each task is saved the number of remaining instructions needed to complete it
        // The number is saved relative to the previous element in the list so that updating the first element updates all values
        std::list<taskElement> runningTasks;
        
        // Constructors
        VMs(){};
        VMs(int nVMs, double processingRate, bool fairSharing);

        // Function that returns whether there is an idle VM that can accept tasks
        bool availableVM();
        // Function to add a task to the list of tasks being served by a VM
        simtime_t addTask(Task* task, simtime_t currentTime);
        // Remove the first task from the list of tasks being served
        // A pointer to the task is returned
        // The time t is passed as reference, so the function updates it to the time of next elaboration end (or -1 if all VMs are idle)
        Task* taskFinished(simtime_t &t, simtime_t currentTime);
        // Support function that prints the list of tasks being served
        void printList();    
};

#endif


