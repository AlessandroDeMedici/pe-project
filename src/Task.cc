#include "Task.h"
#include "Task_m.h"

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

}

void Sink::handleMessage(cMessage *msg)
{
  cancelAndDelete(msg);
}
};
