package cloudcomputingworkloads;

simple CloudNode
{
    parameters:
        int numVMs = default(5);
        double processingRate;
        double backendProcessingProbability;
        int pRandomStream;

        @display("bgb=347,170");
    gates:
        input in;
        output sink;
        output backend;
}