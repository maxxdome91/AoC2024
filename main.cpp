#include <iostream>

#include "src/util.h"
#include "src/source.h"
#include <map>
#include <regex>

static constexpr int DAYS = 5;
static const std::string INPUT_DIRECTORY = "/Users/max/CLionProjects/AoC2024/inputs/";

std::map<std::string, std::function<int(std::vector<std::string>)> > functions = {
    {"d1p1", day1::solvePartOne}, {"d1p2", day1::solvePartTwo},
    {"d2p1", day2::solvePartOne}, {"d2p2", day2::solvePartTwo},
    {"d3p1", day3::solvePartOne}, {"d3p2", day3::solvePartTwo},
    {"d4p1", day4::solvePartOne}, {"d4p2", day4::solvePartTwo},
    {"d5p1", day5::solvePartOne}, {"d5p2", day5::solvePartTwo}
};

int main() {
    for (int i = 1; i <= DAYS; ++i) {
        std::string inputPath = INPUT_DIRECTORY + std::format("input{}.txt", i);
        std::vector<std::string> input = readInputFile(inputPath);
        std::cout << "Day " << i << ":\n";
        std::cout << '\t' << "Part 1: " << functions[std::format("d{}p1", i)](input) << "\n";
        std::cout << '\t' << "Part 2: " << functions[std::format("d{}p2", i)](input) << "\n";
        std::cout << std::endl;
    }
    return 0;
}
