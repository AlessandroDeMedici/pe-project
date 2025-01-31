#include "VMs.h"
#include <omnetpp.h>
#include <list>

using namespace omnetpp;
using namespace std;


// Constructor
VMs::VMs(int nVMs, double processingRate, bool fairSharing)
{
    // Use the input to initialize the parameters of the class
    this->nVMs = nVMs;
    this->fairSharing = fairSharing;
    this->processingRate = processingRate;

    // Initialize the support variables and the list
    this->nActiveVMs = 0;
    this->runningTasks = list<taskElement>(); 
    this->lastTaskUpdate = 0;
}

// Function that returns whether there is an idle VM that can accept tasks
bool VMs::availableVM()
{
    return nActiveVMs < nVMs;
}

// Function to add a task to the list of tasks being served by a VM
simtime_t VMs::addTask(Task* task, simtime_t currentTime)
{
    // Check if there is an available VM
    if (nActiveVMs < nVMs) {
        // If there is at least a running taskd update the number of remaining instructions by decrementing the first element
        if (!runningTasks.empty()) {
            runningTasks.front().remainingInstructions -= (currentTime - lastTaskUpdate).dbl() * currentProcessingRate();
        }
        
        // Rounding needed to compensate for approximation errors
        if (runningTasks.front().remainingInstructions < 0 && runningTasks.front().remainingInstructions > -0.001)
            runningTasks.front().remainingInstructions = 0;
        
        // Error check: if the remaining instructions are negative something went wrong
        if (runningTasks.front().remainingInstructions < 0)
            throw cRuntimeError("Negative remaining instructions %f (%f - %f), %f", runningTasks.front().remainingInstructions, currentTime.dbl(), lastTaskUpdate.dbl(), currentProcessingRate());
        
        // Generate the list element
        taskElement newTask;
        newTask.task = task;
        
        // Insert the task in the right spot
        double taskLength = task->getTaskLength();
        double accumulator = 0;

        #ifdef DEBUG_PRINT_STATE
        EV << "Task len: " << taskLength << endl;
        #endif

        // Traverse the list adding the remainingInstructions of each element
        for (list<taskElement>::iterator it = runningTasks.begin(); it != runningTasks.end(); it++) {
            
            // Insert the new element when:
            // - the cumulative remainingInstructions of the previous elements are less then the taskLength
            // - the cumulative remainingInstructions of the previous elements + the remainingInstructions of the next are more then the taskLength
            if (accumulator + it->remainingInstructions >= taskLength) {
                // Compute the relative remaining instructions
                newTask.remainingInstructions = taskLength - accumulator;
                // Insert the task
                runningTasks.insert(it, newTask);
                // Decrement the remaining instructions of the next task
                it->remainingInstructions -= newTask.remainingInstructions;

                // Exit from the loop
                accumulator += it->remainingInstructions;
                break;
            }
            // Update accumulator
            accumulator += it->remainingInstructions;
        }

        // If the task has not been inserted, append it to the end
        if (accumulator < taskLength) {
            newTask.remainingInstructions = taskLength - accumulator;
            runningTasks.push_back(newTask);
        }

        // Update support variables
        lastTaskUpdate = currentTime;
        nActiveVMs++;

        #ifdef DEBUG_PRINT_STATE
        printList();
        #endif
        
        // Return the time of the next elaboration end
        return runningTasks.front().remainingInstructions / currentProcessingRate();
    } else {
        return -1;
    }
}


// Remove the first task from the list of tasks being served
// A pointer to the task is returned
// The time t is passed as reference, so the function updates it to the time of next elaboration end (or -1 if all VMs are idle)
Task* VMs::taskFinished(simtime_t &t, simtime_t currentTime)
{
    // Remove the task in front of the list and save its pointer
    Task *finishedTask = runningTasks.front().task;
    runningTasks.pop_front();

    // Update status variables
    nActiveVMs--;
    lastTaskUpdate = currentTime;
    
    // Update the time of next elaboration end (if there are no active VMs sets it to -1)
    if (runningTasks.empty()) {
        t = -1;
    } else {
        t = runningTasks.front().remainingInstructions / currentProcessingRate();
    }

    #ifdef DEBUG_PRINT_STATE
    printList();
    #endif
    
    // Return the removed task
    return finishedTask;
}

// Function to get the current processing rate for the single VM
// Returns processingRate/nActiveVMs if fairSharing is true, processingRate/nVMs otherwise
double VMs::currentProcessingRate()
{
    if (fairSharing) {
        return processingRate / nActiveVMs;
    } else {
        return processingRate / nVMs;
    }
}

// Support function that prints the list of tasks being served
void VMs::printList()
{
    for (list<taskElement>::iterator it = runningTasks.begin(); it != runningTasks.end(); it++) {
        EV << it->remainingInstructions << " -> ";
    }
    EV << "end" << endl;
}
