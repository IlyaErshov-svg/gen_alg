#include "parser_e.h"

#include <iostream>
#include <sstream>


std::vector<std::vector<double>> NCVRP::NParser::TParserE::ParseDistanceMatrix(std::ifstream& file) {
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
    matrix.resize(Dimension_);
    for (std::size_t i = 0; i < Dimension_; ++i) {
        matrix[i].resize(Dimension_);
    }
    std::size_t index = 0;
    for (std::size_t i = 0; i < Dimension_; ++i) {
        matrix[i][i] = 0;
        for (std::size_t j = i + 1; j < Dimension_; ++j) {
            matrix[i][j] = tempWeights[index++];
            matrix[j][i] = matrix[i][j];
        }
    }
    return matrix;
}

std::vector<double> NCVRP::NParser::TParserE::ParseDemandVector(std::ifstream& file) {
    std::string line;
    std::vector<double> vector;
    vector.resize(Dimension_);
    for (std::size_t i = 0; i < Dimension_; ++i) {
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

const std::string& NCVRP::NParser::TParserE::GetFileName() const {
    return FileName_;
}

const std::string& NCVRP::NParser::TParserE::GetFileType() const {
    return FileType_;
}

double NCVRP::NParser::TParserE::GetOptimalSolution() const {
    return OptimalSolution_;
}

const std::vector<std::vector<double>>& NCVRP::NParser::TParserE::GetDistanceMatrix() const {
    return DistanceMatrix_;
}

const std::vector<double>& NCVRP::NParser::TParserE::GetDemandVector() const {
    return DemandVector_;
}

unsigned int NCVRP::NParser::TParserE::GetDimension() const {
    return Dimension_;
}


unsigned int NCVRP::NParser::TParserE::GetCapacity() const {
    return Capacity_;
}

bool NCVRP::NParser::TParserE::Parse(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line;
    while (std::getline(file, line)) {
        if (line.starts_with("NAME")) {
            FileName_ = line.substr(line.find(":") + 2);
        } else if (line.starts_with("COMMENT")) {
            std::size_t posLeft = line.find("Optimal value: ") + 15;
            std::size_t posRight = line.find(")") - 1;
            OptimalSolution_ = std::stod(line.substr(posLeft, posRight - posLeft));
        } else if (line.starts_with("TYPE")) {
            FileType_ = line.substr(line.find(":") + 2);
        } else if (line.starts_with("DIMENSION")) {
            Dimension_ = std::stoi(line.substr(line.find(":") + 2));
        } else if (line.starts_with("CAPACITY")) {
            Capacity_ = std::stoi(line.substr(line.find(":") + 2));
        } else if (line.starts_with("EDGE_WEIGHT_SECTION")) {
            DistanceMatrix_ = ParseDistanceMatrix(file);
            DemandVector_ = ParseDemandVector(file);
        }
    }
    if (DistanceMatrix_.empty() || DemandVector_.empty() || Dimension_ == 0 || Capacity_ == 0) {
        return false;
    }
    return true;
}
