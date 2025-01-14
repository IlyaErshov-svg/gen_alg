#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace NCVRP::NParser {
    struct TParserResult {
        std::string FileName;
        std::string FileType;
        double OptimalSolution;
        std::size_t Dimension;
        std::size_t Capacity;
        std::vector<std::vector<double>> DistanceMatrix;
        std::vector<int> DemandVector;
    };

    class IParser {
    public:
        virtual ~IParser() = default;
        virtual TParserResult Parse(const std::string& fileName) = 0;
    };

    class TParserB : public IParser {
    public:
        TParserResult Parse(const std::string& fileName) override;
    private:
        std::vector<std::vector<double>> ParseDistanceMatrix(std::ifstream& file, std::size_t dimension);
        std::vector<int> ParseDemandVector(std::ifstream& file, std::size_t dimension);
    };

    class TParserE : public IParser {
    public:
        TParserResult Parse(const std::string& fileName) override;
    private:
        std::vector<std::vector<double>> ParseDistanceMatrix(std::ifstream& file, std::size_t dimension);
        std::vector<int> ParseDemandVector(std::ifstream& file, std::size_t dimension);
    };
}