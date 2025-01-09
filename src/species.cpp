#include "species.h"

#include <algorithm>
#include <random>

TSpecies::TSpecies(std::size_t size) {
    Gens.resize(size);
}

TSpecies::TSpecies(const std::vector<std::size_t>& gens) {
    Gens = gens;
}

std::vector<std::size_t>& TSpecies::GetGens() {
    return Gens;
}

void TSpecies::SetFitness(double fitness) {
    Fitness_ = fitness;
}

double TSpecies::GetFitness() const {
    return Fitness_;
}

const std::vector<std::size_t>& TSpecies::GetGens() const {
    return Gens;
}

std::size_t TSpecies::GetSize() const {
    return Gens.size();
}

void TSpecies::GenerateGens() {
    for (int i = 0; i < Gens.size(); ++i) {
        Gens[i] = i + 1; 
    }
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::shuffle(Gens.begin(), Gens.end(), rng);
}