#pragma once

#include <vector>

class TFitnessFunction {
public:
    TFitnessFunction(const std::vector<std::vector<double>>& distanceMatrix, const std::vector<int>& demandVector, std::size_t capacity) : distanceMatrix_(distanceMatrix), demandVector_(demandVector), Capacity_(capacity) {}
    int operator()(const std::vector<std::size_t>& gens);
private:
    std::vector<std::vector<double>> distanceMatrix_;
    std::vector<int> demandVector_;
    std::size_t Capacity_;
};