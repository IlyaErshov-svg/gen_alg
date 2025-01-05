#include "utility.h"


#include <iostream>
#include <random>


void FitnessPrint(const std::vector<std::size_t> &gens, const std::vector<std::vector<double>> &distanceMatrix, const std::vector<double> &demandVector, std::size_t capacity) {
    double fitness = 0.0;
    double localCapacity = 0.0;
    std::size_t from = 0;
    std::cout << "Gens: ";
    for (std::size_t i = 0; i < gens.size(); ++i) {
        if (localCapacity + demandVector[gens[i]] > capacity) {
            std::cout << " (" << capacity << ") ";
            std::cout << "| ";
            fitness += distanceMatrix[from][0] + distanceMatrix[0][gens[i]];
            localCapacity = demandVector[gens[i]];
            from = gens[i];
        } else {
            fitness += distanceMatrix[from][gens[i]];
            localCapacity += demandVector[gens[i]];
            from = gens[i];
        }
        std::cout << gens[i] + 1 << " ";
    }
    fitness += distanceMatrix[from][0];
    std::cout << "Fitness: " << fitness << std::endl;
}


double GetRandomDouble(double left, double right) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<double> dis(left, right);
    return dis(gen);
}