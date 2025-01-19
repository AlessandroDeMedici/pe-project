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


  timer_ = new cMessage("generationTimer");
  scheduleNext(timer_);
}

void TaskGenerator::handleMessage(cMessage *msg_timer)
{
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

    Task *task = new Task("task");
    task->setTaskLength(instr);

    send(task, "out");
    scheduleNext(msg_timer);

}

void TaskGenerator::scheduleNext(cMessage *timer)
{

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

    EV_INFO << "Scheduling next message after " << delay << "s\n";

    scheduleAt(simTime() + delay, timer);
}



void Sink::initialize()
{

}

void Sink::handleMessage(cMessage *msg)
{
  // destroy
  delete msg;
}
}; // namespace
