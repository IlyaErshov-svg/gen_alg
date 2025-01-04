#include "parser.h"

#include <sstream>
#include <iostream>


namespace NCVRP::NParser {
    std::vector<std::vector<double>> NCVRP::NParser::TParserB::ParseDistanceMatrix(std::ifstream& file, std::size_t dimension) {
        std::string line;
        std::vector<std::vector<double>> matrix;
        std::vector<std::pair<double, double>> vectorNodes;
        vectorNodes.resize(dimension);
        for (std::size_t i = 0; i < dimension; ++i) {
            std::getline(file, line);
            std::stringstream ss(line);
            std::size_t index;
            ss >> index;
            double x, y;
            ss >> x >> y;
            vectorNodes[index - 1].first = x;
            vectorNodes[index - 1].second = y;
        }
        matrix.resize(dimension);
        for (std::size_t i = 0; i < dimension; ++i) {
            matrix[i].resize(dimension);
            for (std::size_t j = 0; j < dimension; ++j) {
                matrix[i][j] = std::sqrt(std::pow(vectorNodes[i].first - vectorNodes[j].first, 2) + std::pow(vectorNodes[i].second - vectorNodes[j].second, 2));
            }
        }
        return matrix;
    }

    std::vector<double> NCVRP::NParser::TParserB::ParseDemandVector(std::ifstream& file, std::size_t dimension) {
        std::string line;
        std::vector<double> vector;
        vector.resize(dimension);
        for (std::size_t i = 0; i < dimension; ++i) {
            std::getline(file, line);
            std::stringstream ss(line);
            std::size_t index;
            ss >> index;
            double demand;
            ss >> demand;
            vector[index - 1] = demand;
        }
        return vector;
    }

    TParserResult TParserB::Parse(const std::string& fileName) {
        TParserResult result;
        std::ifstream file(fileName);
        std::string line;
        while (std::getline(file, line)) {
            if (line.starts_with("NAME")) {
                result.FileName = line.substr(line.find(":") + 2);
            } else if (line.starts_with("COMMENT")) {
                std::size_t posLeft = line.find("Optimal value: ") + 15;
                std::size_t posRight = line.find(")") - 1;
                result.OptimalSolution = std::stod(line.substr(posLeft, posRight - posLeft));
            } else if (line.starts_with("TYPE")) {
                result.FileType = line.substr(line.find(":") + 2);
            } else if (line.starts_with("DIMENSION")) {
                result.Dimension = std::stoi(line.substr(line.find(":") + 2));
            } else if (line.starts_with("CAPACITY")) {
                result.Capacity = std::stoi(line.substr(line.find(":") + 2));
            } else if (line.starts_with("NODE_COORD_SECTION") && result.Dimension > 0) {
                result.DistanceMatrix = ParseDistanceMatrix(file, result.Dimension);
            } else if (line.starts_with("DEMAND_SECTION") && result.Dimension > 0) {
                result.DemandVector = ParseDemandVector(file, result.Dimension);
            }
        }
        if (result.DistanceMatrix.empty() || result.DemandVector.empty() || result.Dimension == 0 || result.Capacity == 0) {
            throw std::runtime_error("Invalid input file");
        }
        return result;
    } 

    std::vector<std::vector<double>> NCVRP::NParser::TParserE::ParseDistanceMatrix(std::ifstream& file, std::size_t dimension) {
        std::string line;
        std::vector<double> tempWeights;
        while(std::getline(file, line)) {
            if (line.find("DEMAND_SECTION") != std::string::npos) {
                break;
            }
            std::stringstream ss(line);
            double weight;
            while (ss >> weight) {
                tempWeights.push_back(weight);
            }
        }
        std::vector<std::vector<double>> matrix;
        matrix.resize(dimension);
        for (std::size_t i = 0; i < dimension; ++i) {
            matrix[i].resize(dimension);
        }
        std::size_t index = 0;
        for (std::size_t i = 0; i < dimension; ++i) {
            matrix[i][i] = 0;
            for (std::size_t j = i + 1; j < dimension; ++j) {
                matrix[i][j] = tempWeights[index++];
                matrix[j][i] = matrix[i][j];
            }
        }
        return matrix;
    }

    std::vector<double> NCVRP::NParser::TParserE::ParseDemandVector(std::ifstream& file, std::size_t dimension) {
        std::string line;
        std::vector<double> vector;
        vector.resize(dimension);
        for (std::size_t i = 0; i < dimension; ++i) {
            std::getline(file, line);
            std::stringstream ss(line);
            std::size_t index;
            ss >> index;
            double demand;
            ss >> demand;
            vector[index - 1] = demand;
        }
        return vector;
    }


    TParserResult NCVRP::NParser::TParserE::Parse(const std::string& fileName) {
        TParserResult result;
        std::ifstream file(fileName);
        std::string line;
        while (std::getline(file, line)) {
            if (line.starts_with("NAME")) {
                result.FileName = line.substr(line.find(":") + 2);
            } else if (line.starts_with("COMMENT")) {
                std::size_t posLeft = line.find("Optimal value: ") + 15;
                std::size_t posRight = line.find(")") - 1;
                result.OptimalSolution = std::stod(line.substr(posLeft, posRight - posLeft));
            } else if (line.starts_with("TYPE")) {
                result.FileType = line.substr(line.find(":") + 2);
            } else if (line.starts_with("DIMENSION")) {
                result.Dimension = std::stoi(line.substr(line.find(":") + 2));
            } else if (line.starts_with("CAPACITY")) {
                result.Capacity = std::stoi(line.substr(line.find(":") + 2));
            } else if (line.starts_with("EDGE_WEIGHT_SECTION") && result.Dimension > 0) {
                result.DistanceMatrix = ParseDistanceMatrix(file, result.Dimension);
                result.DemandVector = ParseDemandVector(file, result.Dimension);
            }
        }

        if (result.DistanceMatrix.empty() || result.DemandVector.empty() || result.Dimension == 0 || result.Capacity == 0) {
            throw std::runtime_error("Invalid input file");
        }
        return result;
    }
}