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
        std::pair<T, double> Calculation(const std::vector<std::vector<double>>& distanceMatrix, const std::vector<double>& demandVector);
        void FitnessPrint(const std::vector<std::size_t>& gen, const std::vector<std::vector<double>>& distanceMatrix, const std::vector<double>& demandVector);
        ~TGeneticAlgorithm() = default;
    private:
        double FitnessCount(const std::vector<std::size_t>& gen, const std::vector<std::vector<double>>& distanceMatrix, const std::vector<double>& demandVector);
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


//Алгоритм частично сопоставленного кроссовера (PMX)
template <typename T>
T Cross(const T& specie_1, const T& specie_2) {
    std::vector<std::size_t> newSpecie(specie_1.GetSize());
    const std::vector<std::size_t>& specie1 = specie_1.GetGens();
    const std::vector<std::size_t>& specie2 = specie_2.GetGens();
    std::unordered_set<std::size_t> indexes;
    std::pair<std::size_t, std::size_t> crossPoint = std::make_pair(rand() % specie1.size(), rand() % specie2.size());
    for(std::size_t i = crossPoint.first; i <= crossPoint.second; ++i) {
        newSpecie[i] = specie1[i];
        indexes.insert(specie1[i]);
    }
    std::size_t i = 0, j = 0;
    while (i < specie2.size() && j < specie1.size()) {
        if (j >= crossPoint.first && j <= crossPoint.second) {
            ++j;
            continue;
        }
        if (indexes.find(specie2[i]) == indexes.end()) {
            newSpecie[j] = specie2[i];
            ++j;
        }
        ++i;
    }
    return T(newSpecie);
}


template <typename T>
T Mutation(const T& specie) {
    std::vector<std::size_t> newGens(specie.GetGens());
    std::size_t index1 = rand() % specie.GetSize();
    std::size_t index2 = rand() % specie.GetSize();
    std::swap(newGens[index1], newGens[index2]);
    return T(newGens);
}

class TChooser {
public:
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
    std::size_t getIndex() {
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        double genNum = dis(gen);
        std::size_t index = 0;
        double weight = weights_[index];
        while (weight < genNum) {
            //std::cout << "Index: " << index << std::endl;
            //std::cout << "Weight: " << weight << std::endl;
            ++index;
            weight += weights_[index];
        }
        return index != PopulationSize_ ? index : PopulationSize_ - 1;
    }
private:
    std::size_t PopulationSize_;
    std::vector<double> weights_;
};


template<typename T>
double NCVRP::NGenetic::TGeneticAlgorithm<T>::FitnessCount(const std::vector<std::size_t> &gens, const std::vector<std::vector<double>> &distanceMatrix, const std::vector<double> &demandVector) {
    double fitness = 0.0;
    double capacity = 0.0;
    std::size_t from = 0;
    for (std::size_t i = 0; i < gens.size(); ++i) {
        if (capacity + demandVector[gens[i]] > Capacity_) {
            fitness += distanceMatrix[from][0] + distanceMatrix[0][gens[i]];
            capacity = demandVector[gens[i]];
            from = gens[i];
        } else {
            fitness += distanceMatrix[from][gens[i]];
            capacity += demandVector[gens[i]];
            from = gens[i];
        }
    }
    fitness += distanceMatrix[from][0];
    return fitness;
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
std::pair<T, double> NCVRP::NGenetic::TGeneticAlgorithm<T>::Calculation(const std::vector<std::vector<double>> &distanceMatrix, const std::vector<double> &demandVector) {
    T tempResult(demandVector.size() - 1);
    TChooser chooser(PopulationSize_ / 2);
    for(std::size_t i = 0; i < PopulationSize_ / 2; ++i) {
        //Выкидываем ноль
        T gen(demandVector.size() - 1);
        gen.GenerateGens();
        Population_.emplace_back(gen);
    }
    for(std::size_t i = 0; i < Iteration_; ++i) {
        //std::cout << "Iteration: " << i << std::endl;
        for (std::size_t j = 0; j < PopulationSize_ / 2; ++j) {
            //std::cout << "Start Crossing: " << j << std::endl;
            T newGen = Cross<T>(Population_[chooser.getIndex()], Population_[chooser.getIndex()]);
            //std::cout << "Crossed" << std::endl;
            //std::cout << "Start Mutation: " << j << std::endl;
            if (rand() % 100 < MutationProbability_) {
                newGen = Mutation(newGen);
            }
            //std::cout << "Mutated" << std::endl;
            Population_.emplace_back(newGen);
            //std::cout << "Pushed" << std::endl;
        }
        //std::cout << "Population size: " << Population_.size() << std::endl;
        //std::cout << "Population sort" << std::endl;
        std::sort(Population_.begin(), Population_.end(), [&](const T& a, const T& b) {
            return FitnessCount(a.GetGens(), distanceMatrix, demandVector) < FitnessCount(b.GetGens(), distanceMatrix, demandVector);
        });
        //std::cout << "Population sorted" << std::endl;
        Population_.resize(PopulationSize_ / 2);
        //std::cout << "Population resized" << std::endl;
        tempResult =  FitnessCount(Population_[0].GetGens(), distanceMatrix, demandVector) < FitnessCount(tempResult.GetGens(), distanceMatrix, demandVector) ? Population_[0] : tempResult;
    }
    return std::pair<T, double>(Population_[0], FitnessCount(Population_[0].GetGens(), distanceMatrix, demandVector));
}