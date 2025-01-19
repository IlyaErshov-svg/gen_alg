#include <string>

namespace NCVRP {
namespace NBenchmark {

class Benchmark {
    public:
        Benchmark(int minPopulationSize, int maxPopulationSize, int stepPopulationSize,
        double minMutationProbability, double maxMutationProbability, double stepMutationProbability,
        double minCrossingProbability, double maxCrossingProbability, double stepCrossingProbability,
        int minIterations, int maxIterations, int stepIterations
        );

        void Run(std::string& directoryPath, std::string& outputFilePath, int threads);
        
    private:
        int minPopulationSize_;
        int maxPopulationSize_;
        int stepPopulationSize_;
        double minMutationProbability_;
        double maxMutationProbability_;
        double stepMutationProbability_;
        double minCrossingProbability_;
        double maxCrossingProbability_;
        double stepCrossingProbability_;
        int minIterations_;
        int maxIterations_;
        int stepIterations_;
};

}
}