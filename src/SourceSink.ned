package cloudcomputingworkloads;

simple TaskGenerator
{
    parameters:
        int timeDistributionType;
        double timeDistributionMean @unit("s");
        int timeRandomStream;
        
        int instrDistributionType;
        double instrDistributionMean;
        int instrRandomStream;
    gates:
        output out;
}

simple Sink
{
    parameters:
        @signal[taskTime](type=double);
        @statistic[taskTimeStat](source="taskTime";record=vector?, mean, vector(mean););
    gates:
        input inCloudNode;
        input inBackend;
}
