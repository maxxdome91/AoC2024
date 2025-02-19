//
// Created by Maximilian Dornheim on 17.02.25.
//

#include "util.h"

#include <fstream>

std::vector<std::string> readInputFile(std::string_view filePath) {
    std::string line;
    std::vector<std::string> result;
    std::ifstream inFile{filePath};
    while (std::getline(inFile, line)) {
        result.push_back(line);
    }
    return result;
}
