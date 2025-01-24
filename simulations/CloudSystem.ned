package cloudcomputingworkloads.simulations;

import cloudcomputingworkloads.TaskGenerator;
import cloudcomputingworkloads.CloudNode;
import cloudcomputingworkloads.BackendServer;
import cloudcomputingworkloads.Sink;


network CloudSystem
{
    parameters:
        int N;        					// Numero di macchine virtuali
        double R;       				// Tasso totale di elaborazione (instructions/sec)
        double T @unit("s");			// Tempo generazione task
        int T_type;
        double I;						// Lunghezza task
        int I_type;
        double S @unit("s");			// Backend
        int S_type;
        double p;        				// Probabilità di inoltro al backend
        bool fairSharing;

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
