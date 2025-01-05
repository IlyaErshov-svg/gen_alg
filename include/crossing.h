#pragma once

#include <vector>
#include <unordered_set>

//Алгоритм частично сопоставленного кроссовера (PMX)
template <typename T>
class Cross {
public:
    T operator()(const T& specie_1, const T& specie_2);
};


template <typename T>
T Cross<T>::operator()(const T& specie_1, const T& specie_2) {
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