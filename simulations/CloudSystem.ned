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
        double T @unit("s");						// Tempo generazione task
        int T_type;
        double I;						// Lunghezza task
        int I_type;
        double S;						// Backend
        int S_type;
        double p = default(0.5);        // Probabilità di inoltro al backend
        bool fairSharing = default(false);
    submodules:
        taskGenerator: TaskGenerator {
            timeDistributionMean = parent.T;
            timeDistributionType = parent.T_type;
            timeRandomStream = 0;

            instrDistributionMean = parent.I;
            instrDistributionType = parent.I_type;
            instrRandomStream = 1;

            @display("p=42,135;i=block/source");
        }
        cloudNode: CloudNode {
            numVMs = parent.N;
            processingRate = parent.R;
            backendProcessingProbability = parent.p;
            pRandomStream = 2;
            fairSharing = parent.fairSharing;

            @display("p=217,135;i=misc/cloud");
        }
        backendServer: BackendServer {
            backendDistributionMean = parent.S;
            backendDistributionType = parent.S_type;
            backendRandomStream = 3;

            @display("p=359,169;i=device/server");
        }
        sink1: Sink {
            @display("p=514,68;i=block/sink");
        }
        sink2: Sink {
            @display("p=514,188;i=block/sink");
        }
    connections:
        taskGenerator.out --> cloudNode.in;
        cloudNode.sink --> sink1.in;
        cloudNode.backend --> backendServer.in;
        backendServer.out --> sink2.in;
}
