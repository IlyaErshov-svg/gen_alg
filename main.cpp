#include <iostream>
#include <filesystem>
#include "parser_factory.h"
#include "species.h"
#include "genetic.h"
#include "fitness.h"
#include "crossing.h"
#include "mutation.h"
#include "selector.h"


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

    std::cout << "Список файлов в директории: " << directoryPath << "\n";
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        std::cout << entry.path().filename() << "\n";
        std::string filename = entry.path().filename().string();
        std::string type;
        type += filename[0];
        std::cout << type << std::endl;
        auto parser = NCVRP::NParser::ParserFactory::createParser(type);
        auto result = parser->Parse( entry.path().string());
        TFitnessFunction fitnessFunction(result.DistanceMatrix, result.DemandVector, result.Capacity);
        NCVRP::NGenetic::TGeneticAlgorithm<TSpecies> geneticAlgorithm(200, 10, 0.5, 1000, result.Capacity);
        std::cout << "Genetic algorithm started" << std::endl;
        auto res = geneticAlgorithm.Calculation(result.Dimension, TChooser(), Cross<TSpecies>(), Mutation<TSpecies>(), fitnessFunction);
        std::cout << "Result: " << res.second << std::endl;
        geneticAlgorithm.FitnessPrint(res.first.GetGens(), result.DistanceMatrix, result.DemandVector);
    }
    return 0;
}