#pragma once

#include <vector>

void FitnessPrint(const std::vector<std::size_t> &gens, const std::vector<std::vector<double>> &distanceMatrix, const std::vector<int> &demandVector, std::size_t capacity);

double GetRandomDouble(double left, double right);