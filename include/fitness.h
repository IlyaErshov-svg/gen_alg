#pragma once

#include <vector>

class TFitnessFunction {
public:
    TFitnessFunction(const std::vector<std::vector<double>>& distanceMatrix, const std::vector<double>& demandVector, std::size_t capacity) : distanceMatrix_(distanceMatrix), demandVector_(demandVector), Capacity_(capacity) {}
    double operator()(const std::vector<std::size_t>& gens);
private:
    std::vector<std::vector<double>> distanceMatrix_;
    std::vector<double> demandVector_;
    std::size_t Capacity_;
};