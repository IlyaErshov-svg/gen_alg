#pragma once

#include "parser.h"

namespace NCVRP::NParser {
class ParserFactory {
public:
    static std::unique_ptr<NCVRP::NParser::IParser> createParser(const std::string& type) {
        if (type == "B") {
            return std::make_unique<NCVRP::NParser::TParserB>();
        } else if (type == "E") {
            return std::make_unique<NCVRP::NParser::TParserE>();
        } else if (type == "P") {
            return std::make_unique<NCVRP::NParser::TParserB>();
        } else {
            throw std::invalid_argument("Unknown parser type: " + type);
        }
    }
};

}