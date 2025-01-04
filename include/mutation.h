#pragma once

#include <vector>


template <typename T>
class Mutation {
public:
    T operator()(const T& specie);
};


template <typename T>
T Mutation<T>::operator()(const T& specie) {
    std::vector<std::size_t> newGens(specie.GetGens());
    std::size_t index1 = rand() % specie.GetSize();
    std::size_t index2 = rand() % specie.GetSize();
    std::swap(newGens[index1], newGens[index2]);
    return T(newGens);
}