#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>

namespace NCVRP::NParser {
    class TParserE {
    public:
        TParserE() = default;
        bool Parse(const std::string& fileName);
        const std::string& GetFileName() const;
        const std::string& GetFileType() const;
        double GetOptimalSolution() const;
        unsigned int GetDimension() const;
        unsigned int GetCapacity() const;
        const std::vector<std::vector<double>>& GetDistanceMatrix() const;
        const std::vector<double>& GetDemandVector() const; 
    private:
        std::string FileName_;
        std::string FileType_;
        double OptimalSolution_;
        unsigned int Dimension_;
        unsigned int Capacity_;
        std::vector<std::vector<double>> DistanceMatrix_;
        std::vector<double> DemandVector_;

        std::vector<std::vector<double>> ParseDistanceMatrix(std::ifstream& file);
        std::vector<double> ParseDemandVector(std::ifstream& file);
    };
}