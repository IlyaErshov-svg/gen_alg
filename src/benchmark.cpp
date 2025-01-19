#include "benchmark.h"
#include <fstream>
#include <iostream>
#include "parser_factory.h"
#include "genetic.h"
#include "fitness.h"
#include "crossing.h"
#include "mutation.h"
#include "selector.h"
#include "species.h"

NCVRP::NBenchmark::Benchmark::Benchmark(int minPopulationSize, int maxPopulationSize, int stepPopulationSize,
                     double minMutationProbability, double maxMutationProbability, double stepMutationProbability,
                     double minCrossingProbability, double maxCrossingProbability, double stepCrossingProbability,
                     int minIterations, int maxIterations, int stepIterations)
    : minPopulationSize_(minPopulationSize)
    , maxPopulationSize_(maxPopulationSize)
    , stepPopulationSize_(stepPopulationSize)
    , minMutationProbability_(minMutationProbability)
    , maxMutationProbability_(maxMutationProbability)
    , stepMutationProbability_(stepMutationProbability)
    , minCrossingProbability_(minCrossingProbability)
    , maxCrossingProbability_(maxCrossingProbability)
    , stepCrossingProbability_(stepCrossingProbability)
    , minIterations_(minIterations)
    , maxIterations_(maxIterations)
    , stepIterations_(stepIterations)
    {}

void NCVRP::NBenchmark::Benchmark::Run(std::string& directoryPath, std::string& outputFilePath, int threads) {
    std::ofstream outFile(outputFilePath, std::ios::app);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open the file for writing: " + outputFilePath);
    }
    outFile << "PopulationSize,Mutation,Crossing,Iteration,Decision\n";
    std::cout << "Starting benchmark...\n" << std::endl;
    for (double mutationProbability = minMutationProbability_; mutationProbability <= maxMutationProbability_ + stepMutationProbability_ / 2; mutationProbability += stepMutationProbability_) {
        for (int iterations = minIterations_; iterations <= maxIterations_; iterations += stepIterations_) {
            for (double crossingProbability = minCrossingProbability_; crossingProbability <= maxCrossingProbability_ + stepCrossingProbability_ / 2; crossingProbability += stepCrossingProbability_) {
                #pragma omp parallel for num_threads(threads) schedule(dynamic, 1) 
                for (int populationSize = minPopulationSize_; populationSize <= maxPopulationSize_; populationSize += stepPopulationSize_){
                    std::size_t filesCount = 0;
                    double decisionCount = 0;
                    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
                        std::string filename = entry.path().filename().string();
                        std::string type;
                        type += filename[0];
                        auto parser = NCVRP::NParser::ParserFactory::createParser(type);
                        auto result = parser->Parse( entry.path().string());
                        TFitnessFunction fitnessFunction(result.DistanceMatrix, result.DemandVector, result.Capacity);
                        NCVRP::NGenetic::TGeneticAlgorithm<TSpecies> geneticAlgorithm(populationSize, mutationProbability, crossingProbability, iterations);
                        auto res = geneticAlgorithm.Calculation(result.Dimension, TChooserRandom(), Cross<TSpecies>(), Mutation<TSpecies>(), fitnessFunction);
                        decisionCount += (res.second / (static_cast<double>(result.OptimalSolution))  - 1.0) * 100;
                        filesCount++;
                    }
                    #pragma omp critical 
                    {
                        outFile << populationSize << "," << mutationProbability << "," << crossingProbability << "," << iterations << "," << decisionCount / filesCount << std::endl;
                    }
                }
            }
        }
    }
    std::cout << "Benchmark finished!\n" << std::endl;
}