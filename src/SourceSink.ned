package cloudcomputingworkloads;

simple TaskGenerator
{
    parameters:
        int timeDistributionType;
        double T @unit("s");
        
        int instrDistributionType;
        double I;
    gates:
        output out;
}

simple Sink
{
    parameters:
        @signal[taskTime](type=double);
        @statistic[taskTimeStat](source="taskTime";record=vector?, mean, vector(mean)?;);
    gates:
        input inCloudNode;
        input inBackend;
}
