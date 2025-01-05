#include "csv_writer.h"


CSVWriter::CSVWriter(const std::string& filename) : FileName_(filename) {
    std::ifstream file(filename);
    bool fileExists = file.good();
    file.close();

    if (!fileExists) {
        std::ofstream outFile(filename, std::ios::app);
        if (!outFile.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
        }
        outFile << "filename,dimension,optimalSolution,calculatedSolution,decision,timeCalcultation\n";
    }
}


void CSVWriter::WriteRow(const Data& data) {
    std::ofstream outFile(FileName_, std::ios::app); // Открываем файл на добавление
    if (!outFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + FileName_);
    }
    outFile << data.filename << "," << std::to_string(data.dimension) << "," << std::to_string(data.optimalSolution) << "," << std::to_string(data.calculatedSolution) << "," << std::to_string(data.decision) << "," << std::to_string(data.timeCalcultation) << "\n";
}