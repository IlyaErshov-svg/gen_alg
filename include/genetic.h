#pragma once

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
        TGeneticAlgorithm(std::size_t populationSize, std::size_t mutationProbability, double crossover, std::size_t iteration, std::size_t capacity);
        template <typename Selector, typename Crossing, typename Mutation, typename Fitness>
        std::pair<T, double> Calculation(const std::size_t dimention, Selector selector, Crossing crossing, Mutation mutation, Fitness fitness);
        void FitnessPrint(const std::vector<std::size_t>& gen, const std::vector<std::vector<double>>& distanceMatrix, const std::vector<double>& demandVector);
        ~TGeneticAlgorithm() = default;
    private:
        std::size_t PopulationSize_;
        std::vector<T> Population_;
        std::size_t MutationProbability_;
        std::size_t Capacity_;
        double Crossover_;
        std::size_t Iteration_;
    };
}

template<typename T>
NCVRP::NGenetic::TGeneticAlgorithm<T>::TGeneticAlgorithm(std::size_t populationSize, std::size_t mutationProbability, double crossover, std::size_t iteration, std::size_t capacity) {
    PopulationSize_ = populationSize;
    MutationProbability_ = mutationProbability;
    Crossover_ = crossover;
    Iteration_ = iteration;
    Capacity_ = capacity;
    Population_.reserve(PopulationSize_);
}


template<typename T>
void NCVRP::NGenetic::TGeneticAlgorithm<T>::FitnessPrint(const std::vector<std::size_t> &gens, const std::vector<std::vector<double>> &distanceMatrix, const std::vector<double> &demandVector) {
    double fitness = 0.0;
    double capacity = 0.0;
    std::size_t from = 0;
    std::cout << "Gens: ";
    for (std::size_t i = 0; i < gens.size(); ++i) {
        if (capacity + demandVector[gens[i]] > Capacity_) {
            std::cout << " (" << capacity << ") ";
            std::cout << "| ";
            fitness += distanceMatrix[from][0] + distanceMatrix[0][gens[i]];
            capacity = demandVector[gens[i]];
            from = gens[i];
        } else {
            fitness += distanceMatrix[from][gens[i]];
            capacity += demandVector[gens[i]];
            from = gens[i];
        }
        std::cout << gens[i] + 1 << " ";
    }
    fitness += distanceMatrix[from][0];
    std::cout << "Fitness: " << fitness << std::endl;
}

template<typename T>
template<typename Selector, typename Crossing, typename Mutation, typename Fitness>
std::pair<T, double> NCVRP::NGenetic::TGeneticAlgorithm<T>::Calculation(const std::size_t dimention, Selector selector, Crossing crossing, Mutation mutation, Fitness fitness) {
    T tempResult(dimention - 1);
    selector.setPopulationSize(PopulationSize_ / 2);
    for(std::size_t i = 0; i < PopulationSize_ / 2; ++i) {
        //Выкидываем ноль
        T gen(dimention - 1);
        gen.GenerateGens();
        Population_.emplace_back(gen);
    }
    for(std::size_t i = 0; i < Iteration_; ++i) {
        for (std::size_t j = 0; j < PopulationSize_ / 2; ++j) {
            T newGen = crossing(Population_[selector()], Population_[selector()]);
            if (rand() % 100 < MutationProbability_) {
                newGen = mutation(newGen);
            }
            Population_.emplace_back(newGen);
        }
        std::sort(Population_.begin(), Population_.end(), [fitness](const T& a, const T& b) mutable {
            return fitness(a.GetGens()) < fitness(b.GetGens());
        });
        Population_.resize(PopulationSize_ / 2);
        tempResult =  fitness(Population_[0].GetGens()) < fitness(tempResult.GetGens()) ? Population_[0] : tempResult;
    }
    return std::pair<T, double>(Population_[0], fitness(Population_[0].GetGens()));
}