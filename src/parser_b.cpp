#include "parser_b.h"

#include <iostream>
#include <sstream>


std::vector<std::vector<double>> NCVRP::NParser::TParserB::ParseDistanceMatrix(std::ifstream& file) {
    std::string line;
    std::vector<std::vector<double>> matrix;
    std::vector<std::pair<double, double>> vectorNodes;
    vectorNodes.resize(Dimension_);
    for (std::size_t i = 0; i < Dimension_; ++i) {
        std::getline(file, line);
        std::stringstream ss(line);
        std::size_t index;
        ss >> index;
        double x, y;
        ss >> x >> y;
        vectorNodes[index - 1].first = x;
        vectorNodes[index - 1].second = y;
    }
    matrix.resize(Dimension_);
    for (std::size_t i = 0; i < Dimension_; ++i) {
        matrix[i].resize(Dimension_);
        for (std::size_t j = 0; j < Dimension_; ++j) {
            matrix[i][j] = std::sqrt(std::pow(vectorNodes[i].first - vectorNodes[j].first, 2) + std::pow(vectorNodes[i].second - vectorNodes[j].second, 2));
        }
    }
    return matrix;
}

std::vector<double> NCVRP::NParser::TParserB::ParseDemandVector(std::ifstream& file) {
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

const std::string& NCVRP::NParser::TParserB::GetFileName() const {
    return FileName_;
}

const std::string& NCVRP::NParser::TParserB::GetFileType() const {
    return FileType_;
}

double NCVRP::NParser::TParserB::GetOptimalSolution() const {
    return OptimalSolution_;
}

const std::vector<std::vector<double>>& NCVRP::NParser::TParserB::GetDistanceMatrix() const {
    return DistanceMatrix_;
}

const std::vector<double>& NCVRP::NParser::TParserB::GetDemandVector() const {
    return DemandVector_;
}

unsigned int NCVRP::NParser::TParserB::GetDimension() const {
    return Dimension_;
}


unsigned int NCVRP::NParser::TParserB::GetCapacity() const {
    return Capacity_;
}

bool NCVRP::NParser::TParserB::Parse(const std::string& fileName) {
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
        } else if (line.starts_with("NODE_COORD_SECTION")) {
            DistanceMatrix_ = ParseDistanceMatrix(file);
        } else if (line.starts_with("DEMAND_SECTION")) {
            DemandVector_ = ParseDemandVector(file);
        }
    }
    if (DistanceMatrix_.empty() || DemandVector_.empty() || Dimension_ == 0 || Capacity_ == 0) {
        return false;
    }
    return true;
}
