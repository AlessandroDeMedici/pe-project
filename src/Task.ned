package cloudcomputingworkloads;

simple TaskGenerator
{
    parameters:
        double timeDistribution @unit(s);
        double taskSize;
    gates:
        output out;
}

simple Sink
{
    gates:
        input in;
}
