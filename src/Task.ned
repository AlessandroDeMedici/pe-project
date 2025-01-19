package cloudcomputingworkloads;

simple TaskGenerator
{
    parameters:
        int timeDistributionType;
        double timeDistributionMean @unit(s);
        int timeRandomStream;
        
        int instrDistributionType;
        double instrDistributionMean;
        int instrRandomStream;
    gates:
        output out;
}

simple Sink
{
    gates:
        input in;
}
