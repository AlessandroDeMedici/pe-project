package cloudcomputingworkloads.simulations;

import cloudcomputingworkloads.TaskGenerator;
import cloudcomputingworkloads.CloudNode;
import cloudcomputingworkloads.BackendServer;
import cloudcomputingworkloads.Sink;


network CloudSystem
{
    parameters:        
        // Utility for some simulations
        int variable = default(0);
        int range = default(0);

    submodules:
        taskGenerator: TaskGenerator {
            @display("p=84,135;i=block/source");
        }
        cloudNode: CloudNode {
            @display("p=217,135;i=misc/cloud");
        }
        backendServer: BackendServer {
            @display("p=361,210;i=device/server");
        }
        sink: Sink {
            @display("p=498,135;i=block/sink");
        }
    connections:
        taskGenerator.out --> cloudNode.in;
        cloudNode.sink --> sink.inCloudNode;
        cloudNode.backend --> backendServer.in;
        backendServer.out --> sink.inBackend;
}
