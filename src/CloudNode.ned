package cloudcomputingworkloads;

module CloudNode
{
    parameters:
        int numVMs = default(5);
        double processingRate;
        double backendProcessingProbability;

        @display("bgb=347,170");
    gates:
        input in;
        output sink;
        output backend;

    submodules:

        queue: Queue {
            numOutputs = parent.numVMs;
            @display("p=44,87");
        }
        vm[numVMs]: VM {
            @display("p=163,87");
        }
        mux: Mux {
            numInputs = parent.numVMs;
            backendProcessingProbability = parent.backendProcessingProbability;
            @display("p=285,87");
        }

    connections:
        for i=0..numVMs-1 {
            queue.out[i] --> vm[i].in;
            vm[i].out --> mux.in[i];
        }
        in --> queue.in;
        mux.sink --> sink;
        mux.backend --> backend;
}

simple Queue
{
    parameters:
        int numOutputs = default(5);
    gates:
        input in;
        output out[numOutputs];
}

simple VM
{
    gates:
        input in;
        output out;
}

simple Mux
{
    parameters:
        int numInputs = default(5);
        double backendProcessingProbability;
    gates:
        input in[numInputs];
        output sink;
        output backend;
}
