package cloudcomputingworkloads.simulations;

import cloudcomputingworkloads.TaskGenerator;
import cloudcomputingworkloads.CloudNode;
import cloudcomputingworkloads.BackendServer;
import cloudcomputingworkloads.Sink;


network CloudSystem
{
    parameters:        
        // Utility variables to help define certain simultaions in cloudSystem.ini
        int variable = default(0);
        int range = default(0);

    submodules:
        // Module that generates the tasks and sends them to CloudNode
        taskGenerator: TaskGenerator {
            @display("p=84,135;i=block/source");
        }
        // CloudNode Module, after elaboration forwards tasks to BackendServer with probability p or to Sink with probability 1-p
        cloudNode: CloudNode {
            @display("p=217,135;i=misc/cloud");
        }
        // BackendServer Module, after elaboration forwards tasks to Sink
        backendServer: BackendServer {
            @display("p=361,210;i=device/server");
        }
        // Sink module that collects completed tasks that exit the network and computes statistics
        sink: Sink {
            @display("p=498,135;i=block/sink");
        }
    connections:
        taskGenerator.out --> cloudNode.in;
        cloudNode.sink --> sink.inCloudNode;
        cloudNode.backend --> backendServer.in;
        backendServer.out --> sink.inBackend;
}
