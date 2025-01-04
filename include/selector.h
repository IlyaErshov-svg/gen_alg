#pragma once

#include <vector>
#include <random>


class TChooser {
public:
    TChooser() = default;
    TChooser(std::size_t populationSize) : PopulationSize_(populationSize) {
        weights_.resize(populationSize);
        std::size_t sum = 0;
        for (std::size_t i = 1; i <= populationSize; ++i) {
            sum += i;
        }
        for (std::size_t i = 1; i <= populationSize; ++i) {
            weights_[populationSize - i] = 1.0 / sum * i;
        }
    }

    std::size_t operator()() const {
        return getRandomIndex();
    }

    void setPopulationSize(std::size_t populationSize) {
        PopulationSize_ = populationSize;
        weights_.resize(populationSize);
        std::size_t sum = 0;
        for (std::size_t i = 1; i <= populationSize; ++i) {
            sum += i;
        }
        for (std::size_t i = 1; i <= populationSize; ++i) {
            weights_[populationSize - i] = 1.0 / sum * i;
        }
    }

    std::size_t getRandomIndex() const {
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        double genNum = dis(gen);
        std::size_t index = 0;
        double weight = weights_[index];
        while (weight < genNum) {
            ++index;
            weight += weights_[index];
        }
        return index != PopulationSize_ ? index : PopulationSize_ - 1;
    }
private:
    std::size_t PopulationSize_;
    std::vector<double> weights_;
};