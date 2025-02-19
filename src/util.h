//
// Created by Maximilian Dornheim on 17.02.25.
//

#ifndef UTIL_H
#define UTIL_H
#include <vector>

#endif //UTIL_H

/**
 * Creates a std::vector<std::string> containing all the lines of the input file
 *
 * @param filePath absolute file path of input file
 * @return new std::vector<std::string> containing all the lines of the input file
 */
std::vector<std::string> readInputFile(std::string_view filePath);
