#include <cmath>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <string>
#include "csv_writer.h"
#include "parser_factory.h"
#include "species.h"
#include "genetic.h"
#include "fitness.h"
#include "crossing.h"
#include "mutation.h"
#include "selector.h"
#include "utility.h"
#include "benchmark.h"


int main(int argc, char* argv[]) {
    constexpr std::size_t populationSize = 2000;
    constexpr double mutationProbability = 0.2;
    constexpr double crossingProbability = 0.7;
    constexpr std::size_t iterations = 3000; 

     if (argc < 2) {
        std::cerr << "Error: specify path to directory in arguments!\n";
        return 1;
    }

    if (argc == 3 && std::string(argv[2]) == "search_optimal_parameters") {
        std::cout << "Searching optimal parameters...\n";
        NCVRP::NBenchmark::Benchmark benchmark(400, 2000, 400, 0.1, 0.3, 0.1, 0.5, 0.8, 0.1, 500, 2000, 500);
        std::string directoryPath = argv[1];
        std::string outputFilePath = "output.csv";
        std::cout << "Output file: " << outputFilePath << std::endl;
        benchmark.Run(directoryPath, outputFilePath, 10);
        return 0;
    }

    std::string directoryPath = argv[1]; 

    if (!std::filesystem::exists(directoryPath)) {
        std::cerr << "Error: the specified path does not exist!\n";
        return 1;
    }

    if (!std::filesystem::is_directory(directoryPath)) {
        std::cerr << "Error: the specified path is not a directory!\n";
        return 1;
    }

    CSVWriter writer("calculation_30_P.csv");

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        std::cout << "Parsing file " << entry.path().filename() << std::endl;
        std::string filename = entry.path().filename().string();
        std::string type;
        type += filename[0];
        auto parser = NCVRP::NParser::ParserFactory::createParser(type);
        auto result = parser->Parse( entry.path().string());
        TFitnessFunction fitnessFunction(result.DistanceMatrix, result.DemandVector, result.Capacity);
        NCVRP::NGenetic::TGeneticAlgorithm<TSpecies> geneticAlgorithm(populationSize, mutationProbability, crossingProbability, iterations);
        std::cout << "Genetic algorithm started" << std::endl;
        clock_t start = std::clock();
        auto res = geneticAlgorithm.Calculation(result.Dimension, TChooserRandom(), Cross<TSpecies>(), Mutation<TSpecies>(), fitnessFunction);
        clock_t end = std::clock();
        double elapsed_seconds = double(end - start) / CLOCKS_PER_SEC;
        Data data;
        data.timeCalcultation = elapsed_seconds;
        data.filename = filename;
        data.calculatedSolution = res.second;
        data.optimalSolution = result.OptimalSolution;
        data.dimension = result.Dimension;
        data.decision = (res.second / (static_cast<double>(result.OptimalSolution))  - 1.0) * 100;
        writer.WriteRow(data);
        std::cout << "Result: " << res.second << std::endl;
        std::cout << "Capacity: " << result.Capacity << std::endl;
        std::cout << "Time: " << elapsed_seconds << std::endl;
        FitnessPrint(res.first.GetGens(), result.DistanceMatrix, result.DemandVector, result.Capacity);
    }
    return 0;
}