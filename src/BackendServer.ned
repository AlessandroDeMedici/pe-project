package cloudcomputingworkloads;

simple BackendServer
{
    parameters:
        double S @unit("Hz");
        int backendDistributionType;
        
        @signal[backendNq](type=unsigned long);
        @statistic[backendStatNq](source="backendNq";record=vector?,timeavg,vector(timeavg)?;);
                
        @signal[backendW](type=double);
        @statistic[backendStatW](source="backendW";record=vector?,mean,vector(mean)?;);
        
        @signal[backendR](type=double);
        @statistic[backendStatR](source="backendR";record=vector?,mean,vector(mean)?;);
        
        @signal[backendCPU](type=unsigned int);
        @statistic[backendStatCPU](source="backendCPU";record=vector?,timeavg,vector(timeavg)?;);
        
        @statistic[backendStatN](source="backendNq+backendCPU";record=vector?,timeavg,vector(timeavg)?;);
        
    gates:
        input in;
        output out;
}


