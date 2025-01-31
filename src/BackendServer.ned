package cloudcomputingworkloads;

simple BackendServer
{
    parameters:
        // Serving rate S: Number of tasks served, on average, in a second
        double S @unit("Hz");
        
        // Distribuition type of the serving rate 
        // 0 = constant at value 1/S
        // 1 = exponential with mean 1/S
        // 2 = unifrom from 0 to 2/S
        int backendDistributionType;
        
        // Signals used to keep track of statistics
        // Number of elements in the queue
        @signal[backendNq](type=unsigned long);
        @statistic[backendStatNq](source="backendNq";record=vector?,timeavg,vector(timeavg)?;);
        // Task waiting time
        @signal[backendW](type=double);
        @statistic[backendStatW](source="backendW";record=vector?,mean,vector(mean)?;);
        // Task response time
        @signal[backendR](type=double);
        @statistic[backendStatR](source="backendR";record=vector?,mean,vector(mean)?;);
        // Utilization
        @signal[backendCPU](type=unsigned int);
        @statistic[backendStatCPU](source="backendCPU";record=vector?,timeavg,vector(timeavg)?;);
        // Total number of tasks in the node
        @statistic[backendStatN](source="backendNq+backendCPU";record=vector?,timeavg,vector(timeavg)?;);
        
    gates:
        input in;
        output out;
}


