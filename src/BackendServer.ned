package cloudcomputingworkloads;

simple BackendServer
{
    parameters:
        double backendDistributionMean;
        int backendDistributionType;
        int backendRandomStream;
        @signal[backendDelay](type=double);
        @statistic[backendDelayStat](source="backendDelay";record=vector;);
        @signal[backendQueue](type=unsigned long);
        @statistic[backendQueueStat](source="backendQueue";record=vector,timeavg;);
    gates:
        input in;
        output out;
}


