#pragma once

#include "utility.h"
#include <vector>
#include <random>


class TChooserWeighted {
public:
    TChooserWeighted() = default;
    TChooserWeighted(std::size_t populationSize) : PopulationSize_(populationSize) {
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
        double genNum = GetRandomDouble(0.0, 1.0);
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


class TChooserRandom {
public:
    TChooserRandom() = default;
    TChooserRandom(std::size_t populationSize) : PopulationSize_(populationSize) {
    }

    std::size_t operator()() const {
        return getRandomIndex();
    }

    void setPopulationSize(std::size_t populationSize) {
        PopulationSize_ = populationSize;
    }

    std::size_t getRandomIndex() const {
        return rand() % PopulationSize_;
    }
private:
    std::size_t PopulationSize_;
};