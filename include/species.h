#pragma once

#include <vector>

class TSpecies {
    public:
        TSpecies() = default;
        TSpecies(const TSpecies&) = default; 
        TSpecies(std::size_t size);
        TSpecies(const std::vector<std::size_t>& gens);
        void SetFitness(int fitness);
        int GetFitness() const;
        std::vector<std::size_t>& GetGens();
        std::size_t GetSize() const;
        const std::vector<std::size_t>& GetGens() const;
        void GenerateGens();
    private:
        int Fitness_;
        std::vector<std::size_t> Gens;
};