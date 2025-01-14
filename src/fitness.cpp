#include "fitness.h"


int TFitnessFunction::operator()(const std::vector<std::size_t>& gens) {
    double fitness = 0;
    int capacity = 0;
    std::size_t from = 0;
    for (std::size_t i = 0; i < gens.size(); ++i) {
        if (capacity + demandVector_[gens[i]] > Capacity_) {
            fitness += distanceMatrix_[from][0] + distanceMatrix_[0][gens[i]];
            capacity = demandVector_[gens[i]];
            from = gens[i];
        } else {
            fitness += distanceMatrix_[from][gens[i]];
            capacity += demandVector_[gens[i]];
            from = gens[i];
        }
    }
    fitness += distanceMatrix_[from][0];
    return static_cast<int>(fitness);
}