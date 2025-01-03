#include <iostream>
#include "species.h"
#include "genetic.h"
#include "parser_b.h"
#include "parser_e.h"

int main() {
    auto parser = NCVRP::NParser::TParserE();
    bool result = parser.Parse("E-n13-k4.vrp.txt");
    if (result) {
        std::cout << "Parsing successful" << std::endl;
    } else {
        std::cout << "Parsing failed" << std::endl;
        exit(1);
    }
    NCVRP::NGenetic::TGeneticAlgorithm<TSpecies> geneticAlgorithm(200, 10, 0.5, 1000, parser.GetCapacity());
    std::cout << "Genetic algorithm started" << std::endl;
    auto res = geneticAlgorithm.Calculation(parser.GetDistanceMatrix(), parser.GetDemandVector());

    std::cout << "Result: " << res.second << std::endl;
    geneticAlgorithm.FitnessPrint(res.first.GetGens(), parser.GetDistanceMatrix(), parser.GetDemandVector());
    return 0;
}