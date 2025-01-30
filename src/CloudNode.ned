package cloudcomputingworkloads;

simple CloudNode
{
    parameters:
        int numVMs = default(5);
        double processingRate;
        double backendProcessingProbability;
        int pRandomStream;
        bool fairSharing;

		@signal[cloudNodeNq](type=unsigned long);
        @statistic[cloudNodeStatNq](source="cloudNodeNq";record=vector?,timeavg,vector(timeavg)?;);
                
        @signal[cloudNodeW](type=double);
        @statistic[cloudNodeStatW](source="cloudNodeW";record=vector?,mean,vector(mean)?;);
        
        @signal[cloudNodeR](type=double);
        @statistic[cloudNodeStatR](source="cloudNodeR";record=vector?,mean,vector(mean)?;);
        
        @signal[cloudNodeActiveVMs](type=unsigned long);
        @statistic[cloudNodeStatActiveVM](source="cloudNodeActiveVMs";record=vector?,timeavg,vector(timeavg)?;);
        
        @statistic[cloudNodeStatN](source="cloudNodeNq+cloudNodeActiveVMs";record=vector?,timeavg,vector(timeavg)?;);
        
        @signal[cloudNodeInterDepartureTime](type=double);
        @statistic[cloudNodeStatInterDepartureTime](source="cloudNodeInterDepartureTime";record=vector?,mean,vector(mean)?;);
        
        @display("bgb=347,170");
    gates:
        input in;
        output sink;
        output backend;
}