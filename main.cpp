#include <cmath>
#include <iostream>
#include <filesystem>
#include <ctime>
#include "csv_writer.h"
#include "parser_factory.h"
#include "species.h"
#include "genetic.h"
#include "fitness.h"
#include "crossing.h"
#include "mutation.h"
#include "selector.h"
#include "utility.h"


int main(int argc, char* argv[]) {
     if (argc < 2) {
        std::cerr << "Ошибка: укажите путь до директории в аргументах!\n";
        return 1;
    }

    std::string directoryPath = argv[1]; 

    if (!std::filesystem::exists(directoryPath)) {
        std::cerr << "Ошибка: указанный путь не существует!\n";
        return 1;
    }

    if (!std::filesystem::is_directory(directoryPath)) {
        std::cerr << "Ошибка: указанный путь не является директорией!\n";
        return 1;
    }
    CSVWriter writer("calculation_20_P.csv");
    std::cout << "Список файлов в директории: " << directoryPath << "\n";
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        Data data;
        std::cout << entry.path().filename() << "\n";
        std::string filename = entry.path().filename().string();
        std::string type;
        type += filename[0];
        std::cout << type << std::endl;
        auto parser = NCVRP::NParser::ParserFactory::createParser(type);
        auto result = parser->Parse( entry.path().string());
        TFitnessFunction fitnessFunction(result.DistanceMatrix, result.DemandVector, result.Capacity);
        NCVRP::NGenetic::TGeneticAlgorithm<TSpecies> geneticAlgorithm(2000, 0.2, 0.7, 3000);
        std::cout << "Genetic algorithm started" << std::endl;
        clock_t start = std::clock();
        auto res = geneticAlgorithm.Calculation(result.Dimension, TChooserRandom(), Cross<TSpecies>(), Mutation<TSpecies>(), fitnessFunction);
        clock_t end = std::clock();
        double elapsed_seconds = double(end - start) / CLOCKS_PER_SEC;
        data.timeCalcultation = elapsed_seconds;
        data.filename = filename;
        data.calculatedSolution = res.second;
        data.optimalSolution = result.OptimalSolution;
        data.dimension = result.Dimension;
        data.decision = (res.second / (static_cast<double>(result.OptimalSolution))  - 1.0) * 100;
        writer.WriteRow(data);
        std::cout << "Result: " << res.second << std::endl;
        std::cout << "Capacity: " << result.Capacity << std::endl;
        FitnessPrint(res.first.GetGens(), result.DistanceMatrix, result.DemandVector, result.Capacity);
    }
    return 0;
}