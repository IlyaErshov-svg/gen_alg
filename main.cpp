#include <iostream>
#include <filesystem>
#include "species.h"
#include "genetic.h"
#include "parser_b.h"
#include "parser_e.h"

int main(int argc, char* argv[]) {
     if (argc < 2) {
        std::cerr << "Ошибка: укажите путь до директории в аргументах!\n";
        return 1;
    }

    std::string directoryPath = argv[1]; // Получаем путь из аргументов

    // Проверяем, существует ли путь и это ли директория
    if (!std::filesystem::exists(directoryPath)) {
        std::cerr << "Ошибка: указанный путь не существует!\n";
        return 1;
    }

    if (!std::filesystem::is_directory(directoryPath)) {
        std::cerr << "Ошибка: указанный путь не является директорией!\n";
        return 1;
    }

    // Выводим список всех файлов в директории
    std::cout << "Список файлов в директории: " << directoryPath << "\n";
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        std::cout << entry.path().filename() << "\n";
        if (entry.path().string().find("E") == std::string::npos) {
            continue;
        }
        auto parser = NCVRP::NParser::TParserE();
        bool result = parser.Parse(entry.path().string());
        NCVRP::NGenetic::TGeneticAlgorithm<TSpecies> geneticAlgorithm(200, 10, 0.5, 1000, parser.GetCapacity());
        std::cout << "Genetic algorithm started" << std::endl;
        auto res = geneticAlgorithm.Calculation(parser.GetDistanceMatrix(), parser.GetDemandVector());
        std::cout << "Result: " << res.second << std::endl;
        geneticAlgorithm.FitnessPrint(res.first.GetGens(), parser.GetDistanceMatrix(), parser.GetDemandVector());
    }
    return 0;
}