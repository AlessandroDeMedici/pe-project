package cloudcomputingworkloads;

simple TaskGenerator
{
    parameters:
        // Mean time T between two consecutive arrivals in the network
        double T @unit("s");
        // Distribuition type of the time between two consecutive arrivals in the network
        // 0 = constant at value T
        // 1 = exponential with mean T
        // 2 = unifrom from 0 to 2T
        int timeDistributionType;
        
        // Mean number I of instruction needed to process the packet in the CloudNode
        double I;
        // Distribuition type of the number of instruction needed to process the packet in the CloudNode
        // 0 = constant at value I
        // 1 = exponential with mean I
        // 2 = unifrom from 0 to 2I
        int instrDistributionType;
    gates:
        output out;
}

simple Sink
{
    parameters:
        // Signal for task response time statistic
        @signal[taskTime](type=double);
        @statistic[taskTimeStat](source="taskTime";record=vector?, mean, vector(mean)?;);
    gates:
        input inCloudNode;
        input inBackend;
}
