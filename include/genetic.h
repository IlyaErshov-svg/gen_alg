#pragma once

#include "utility.h"
#include <iostream>
#include <new>
#include <vector>
#include <unordered_set>
#include <vector>
#include <random>


namespace NCVRP::NGenetic {
    template<typename T>
    class TGeneticAlgorithm {
    public:
        TGeneticAlgorithm() = delete;
        TGeneticAlgorithm(const TGeneticAlgorithm&) = delete;
        TGeneticAlgorithm(TGeneticAlgorithm&&) = delete;
        TGeneticAlgorithm operator=(const TGeneticAlgorithm&) = delete;
        TGeneticAlgorithm operator=(TGeneticAlgorithm&&) = delete;
        TGeneticAlgorithm(std::size_t populationSize, double mutationProbability, double crossover, std::size_t iteration);
        template <typename Selector, typename Crossing, typename Mutation, typename Fitness>
        std::pair<T, int> Calculation(const std::size_t dimention, Selector selector, Crossing crossing, Mutation mutation, Fitness fitness);
        ~TGeneticAlgorithm() = default;
    private:
        std::size_t PopulationSize_;
        std::vector<T> Population_;
        std::size_t PopulationCrossingSize_;
        double MutationProbability_;
        double Crossover_;
        std::size_t Iteration_;
    };
}

template<typename T>
NCVRP::NGenetic::TGeneticAlgorithm<T>::TGeneticAlgorithm(std::size_t populationSize, double mutationProbability, double crossover, std::size_t iteration) {
    PopulationSize_ = populationSize;
    MutationProbability_ = mutationProbability;
    Crossover_ = crossover;
    Iteration_ = iteration;
    PopulationCrossingSize_ = static_cast<std::size_t>(populationSize * crossover);
    Population_.reserve(PopulationSize_);
}


template<typename T>
template<typename Selector, typename Crossing, typename Mutation, typename Fitness>
std::pair<T, int> NCVRP::NGenetic::TGeneticAlgorithm<T>::Calculation(const std::size_t dimention, Selector selector, Crossing crossing, Mutation mutation, Fitness fitness) {
    T tempResult(dimention - 1);
    selector.setPopulationSize(PopulationCrossingSize_);

    for(std::size_t i = 0; i < PopulationCrossingSize_; ++i) {
        //Выкидываем ноль
        T gen(dimention - 1);
        gen.GenerateGens();
        gen.SetFitness(fitness(gen.GetGens()));
        Population_.emplace_back(gen);
    }
    std::sort(Population_.begin(), Population_.begin() + PopulationCrossingSize_ - 1, [fitness](const T& a, const T& b) mutable {
        return a.GetFitness() < b.GetFitness();
    });
    for(std::size_t i = 0; i < Iteration_; ++i) {
        for (std::size_t j = PopulationCrossingSize_; j < PopulationSize_; ++j) {
            std::size_t index1 = selector();
            std::size_t index2 = selector();
            while ( index1 == index2 ) {
                index2 = selector();
            }
            T newGen = crossing(Population_[index1], Population_[index2]);
            if (GetRandomDouble(0.0, 1.0) < MutationProbability_) {
                newGen = mutation(newGen);
            }
            newGen.SetFitness(fitness(newGen.GetGens()));
            Population_.emplace_back(newGen);
        }

        std::sort(Population_.begin(), Population_.end(), [fitness](const T& a, const T& b) mutable {
            return a.GetFitness() < b.GetFitness();
        });
        Population_.resize(PopulationCrossingSize_);
        tempResult =  Population_[0].GetFitness() < tempResult.GetFitness() ? Population_[0] : tempResult;
    }

    return std::pair<T, int>(Population_[0], fitness(Population_[0].GetGens()));
}