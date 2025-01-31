package cloudcomputingworkloads;

simple CloudNode
{
    parameters:
        // Number of VMs inside the CloudNode N
        int N;
        // Processing rate R (instructions per second)
        double R;
        // If true use fair sharing mode, else segregation
        bool fairSharing;
        // Probability of forwarding to the BackendServer for extra elaboration
        double p;

        // Signals used to keep track of statistics
        // Number of elements in the queue
		@signal[cloudNodeNq](type=unsigned long);
        @statistic[cloudNodeStatNq](source="cloudNodeNq";record=vector?,timeavg,vector(timeavg)?;);
        // Task waiting time
        @signal[cloudNodeW](type=double);
        @statistic[cloudNodeStatW](source="cloudNodeW";record=vector?,mean,vector(mean)?;);
        // Task response time
        @signal[cloudNodeR](type=double);
        @statistic[cloudNodeStatR](source="cloudNodeR";record=vector?,mean,vector(mean)?;);
        // Number of active VMs
        @signal[cloudNodeActiveVMs](type=unsigned long);
        @statistic[cloudNodeStatActiveVM](source="cloudNodeActiveVMs";record=vector?,timeavg,vector(timeavg)?;);
        // Total number of tasks in the node
        @statistic[cloudNodeStatN](source="cloudNodeNq+cloudNodeActiveVMs";record=vector?,timeavg,vector(timeavg)?;);
        // Interdeparture time
        @signal[cloudNodeInterDepartureTime](type=double);
        @statistic[cloudNodeStatInterDepartureTime](source="cloudNodeInterDepartureTime";record=vector?,mean,vector(mean)?;);
        
        @display("bgb=347,170");
    gates:
        input in;
        output sink;
        output backend;
}