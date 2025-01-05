#pragma once

#include <fstream>
#include <string>
#include <vector>


struct Data {
    std::string filename;
    std::size_t dimension;
    double optimalSolution;
    double calculatedSolution;
    double decision;
    double timeCalcultation;
};


class CSVWriter {
    public:
        CSVWriter(const std::string& filename);
        void WriteRow(const Data& data);
    private:
        void writeHeaders(std::ofstream& outFile);
        std::string FileName_;
};