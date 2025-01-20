package cloudcomputingworkloads;

simple BackendServer
{
    parameters:
        double backendDistributionMean;
        int backendDistributionType;
        int backendRandomStream;
    gates:
        input in;
        output out;
}


