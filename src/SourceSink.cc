#include "SourceSink.h"


namespace cloudcomputingworkloads {

Define_Module(TaskGenerator);
Define_Module(Sink);

void TaskGenerator::initialize()
{
  timeDistributionType = par("timeDistributionType");
  timeDistributionMean = par("timeDistributionMean");
  timeRandomStream = par("timeRandomStream");

  instrDistributionType = par("instrDistributionType");
  instrDistributionMean = par("instrDistributionMean");
  instrRandomStream = par("instrRandomStream");


  timer_ = new Task("timer");
  scheduleNext();
}

void TaskGenerator::handleMessage(cMessage *timer)
{
    Task *task = new Task("taskPacket");
    
    double instr;

    switch (instrDistributionType) {
        case 0:
            instr = instrDistributionMean;
            break;
        case 1:
            instr = exponential(instrDistributionMean, instrRandomStream);
            break;
        case 2:
            instr = uniform(0, 2*instrDistributionMean, instrRandomStream);
            break;
        default:
            throw cRuntimeError("Unknown distribution type: %c", instrDistributionType);
    }
    
    task->setTaskLength(instr);
    
    #ifdef DEBUG_COHERENCE
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
    
    send(task, "out");
    
    scheduleNext();
}

void TaskGenerator::scheduleNext()
{
    // TODO farlo con i nomi ivece che con i numeri
    simtime_t delay;

    switch (timeDistributionType) {
        case 0:
            delay = timeDistributionMean;
            break;
        case 1:
            delay = exponential(timeDistributionMean, timeRandomStream);
            break;
        case 2:
            delay = uniform(0, 2*timeDistributionMean, timeRandomStream);
            break;
        default:
            throw cRuntimeError("Unknown distribution type: %c", timeDistributionType);
    }
    
    scheduleAt(simTime() + delay, timer_);
}



void Sink::initialize()
{
  taskTime = registerSignal("taskTime");
}

void Sink::handleMessage(cMessage *msg)
{
  emit(taskTime, (simTime() - msg->getCreationTime()).dbl());
  
  cancelAndDelete(msg);
  
  #ifdef DEBUG_COHERENCE
  incrementNTasks();
  #endif
}
};
