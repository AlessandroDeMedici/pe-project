#include "SourceSink.h"


namespace cloudcomputingworkloads {

Define_Module(TaskGenerator);
Define_Module(Sink);

// Module initialization
void TaskGenerator::initialize()
{
    // Mean time T between two consecutive arrivals in the network
    timeDistributionMean = par("T");
    // Distribuition type of the time between two consecutive arrivals in the network
    // 0 = constant at value T
    // 1 = exponential with mean T
    // 2 = unifrom from 0 to 2T
    timeDistributionType = par("timeDistributionType");

    // Mean number I of instruction needed to process the packet in the CloudNode
    instrDistributionMean = par("I");
    // Distribuition type of the number of instruction needed to process the packet in the CloudNode
    // 0 = constant at value I
    // 1 = exponential with mean I
    // 2 = unifrom from 0 to 2I
    instrDistributionType = par("instrDistributionType");

    // Timer task used to schedule events for the next packet to send out
    timer_ = new Task("timer");

    // Schedule the timer for the first packet to send out
    scheduleNext();
}

// Timer end handler
void TaskGenerator::handleMessage(cMessage *timer)
{
    // Generate a new task
    Task *task = new Task("taskPacket");
    
    double instr;
    // Determine the task's number of instructions based on instrDistributionType 
    switch (instrDistributionType) {
        case 0: // Constant at value instrDistributionMean (I)
            instr = instrDistributionMean;
            break;
        case 1: // Exponentially distributed with mean instrDistributionMean (I)
            instr = exponential(instrDistributionMean, 1);
            break;
        case 2: // Unifromly distributed from 0 to 2*instrDistributionMean (2*I)
            instr = uniform(0, 2*instrDistributionMean, 1);
            break;
        default:
            throw cRuntimeError("Unknown distribution type: %c", instrDistributionType);
    }
    task->setTaskLength(instr);
    
    #ifdef DEBUG_COHERENCE
    // Debug function: counts the total number of tasks in the system and confronts it with the expected value
    // An exception is thrown in case of mismatch
    cModule *parent = getParentModule();
    unsigned long totTasks = 0;

    for (cModule::SubmoduleIterator it(parent); !it.end(); ++it) {
        cModule *submodule = *it;
        if (submodule != this) { // Exclude taskGenerator
            CoherenceCheck *module = dynamic_cast<CoherenceCheck *>(submodule);
            if (module) {
                totTasks += module->getNTasks();
            }
        }
    }

    EV << "Total processed/ing tasks: " << totTasks << " vs " << getNTasks() << endl;
    
    if (totTasks != getNTasks()) {
        throw cRuntimeError("Mismatch generated %lu tasks, processed %lu", getNTasks(), totTasks);
    }
    
    incrementNTasks();
    #endif
    
    // Send out the task
    send(task, "out");
    
    // Schedule the timer for the next packet to send out
    scheduleNext();
}

// Next timer event scheduler
void TaskGenerator::scheduleNext()
{
    
    simtime_t delay;
    // Determine the next task delay based on timeDistributionType 
    switch (timeDistributionType) {
        case 0: // Constant at value timeDistributionMean (T)
            delay = timeDistributionMean;
            break;
        case 1: // Exponentially distributed with mean timeDistributionMean (T)
            delay = exponential(timeDistributionMean, 0);
            break;
        case 2: // Unifromly distributed from 0 to 2*timeDistributionMean (2*T)
            delay = uniform(0, 2*timeDistributionMean, 0);
            break;
        default:
            throw cRuntimeError("Unknown distribution type: %c", timeDistributionType);
    }
    
    scheduleAt(simTime() + delay, timer_);
}

// Module initialization
void Sink::initialize()
{
    // Signal for task response time statistic
    taskTime = registerSignal("taskTime");
}

// Incoming message handler
void Sink::handleMessage(cMessage *msg)
{
    // Record the task response time
    emit(taskTime, (simTime() - msg->getCreationTime()).dbl());
    
    cancelAndDelete(msg);
    
    #ifdef DEBUG_COHERENCE
    incrementNTasks();
    #endif
}

}; // Namespace
