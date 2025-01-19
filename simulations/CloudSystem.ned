package cloudcomputingworkloads.simulations;

import cloudcomputingworkloads.TaskGenerator;
import cloudcomputingworkloads.CloudNode;
import cloudcomputingworkloads.BackendServer;
import cloudcomputingworkloads.Sink;


network CloudSystem
{
    parameters:
        int N = default(5);        		// Numero di macchine virtuali
        double R = default(1000);       // Tasso totale di elaborazione (instructions/sec)
        double T @unit(s);						// Tempo generazione task
        double I;						// Lunghezza task
        double S;						// Backend
        double p = default(0.5);        // Probabilità di inoltro al backend
    submodules:
        taskGenerator: TaskGenerator {
            timeDistribution = parent.T;
            taskSize = parent.I;
            
            @display("p=39,106");
        }
        cloudNode: CloudNode {
            numVMs = parent.N;
            processingRate = parent.R;
            backendProcessingProbability = parent.p;
            
            @display("p=140,106");
        }
        backendServer: BackendServer {
            serverRate = parent.S;
            
            @display("p=245,145");
        }
        sink1: Sink {
            @display("p=338,58");
        }
        sink2: Sink {
            @display("p=338,153");
        }
    connections:
        taskGenerator.out --> cloudNode.in;
        cloudNode.sink --> sink1.in;
        cloudNode.backend --> backendServer.in;
        backendServer.out --> sink2.in;
}
