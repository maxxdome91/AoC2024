//
// Created by Maximilian Dornheim on 17.02.25.
//

#include "source.h"

#include <regex>
#include <set>
#include <sstream>


namespace day1 {
    /**
     *
     * @param input vector containing input as strings
     * @return pair of multiset with converted integers
     */
    std::pair<std::multiset<int>, std::multiset<int> > createMultiset(const std::vector<std::string> &input) {
        std::multiset<int> left, right;
        for (const auto &line: input) {
            size_t static index{line.find(' ')};
            left.emplace(std::stoi(line.substr(0, index)));
            right.emplace(std::stoi(line.substr(index + 3)));
        }
        return std::make_pair(left, right);
    }

    int solvePartOne(const std::vector<std::string> &input) {
        int result{0};
        auto [left, right] = createMultiset(input);
        for (auto [leftIt, rightIt] = std::make_pair(left.begin(), right.begin()); leftIt != left.end();
             ++leftIt, ++rightIt) {
            result += std::abs(*leftIt - *rightIt);
        }
        return result;
    }

    int solvePartTwo(const std::vector<std::string> &input) {
        int result{0};
        for (auto [left, right] = createMultiset(input); auto value: left) {
            result += value * static_cast<int>(right.count(value));
        }
        return result;
    }
}

namespace day2 {
    /**
     *
     * @param input vector containing input as strings
     * @return vector containing vectors of parsed integers
     */
    auto formatInput(const std::vector<std::string> &input) {
        std::vector<std::vector<int> > result{};
        for (const auto &line: input) {
            result.emplace_back();
            std::stringstream ss{line};
            auto &vector = result.back();
            while (ss.good()) {
                auto toInsert{0};
                ss >> toInsert;
                vector.push_back(toInsert);
            }
        }
        return result;
    }

    /**
     *
     * @param input vector of integer
     * @param increasing bool weather the input numbers should increase or not
     * @return bool weather both conditions are fulfilled
     */
    bool conditionIsTrue(const std::vector<int> &input, const bool increasing) {
        for (int i = 0; i < input.size() - 1; ++i) {
            if (std::abs(input[i] - input[i + 1]) > 3) return false;
            if (increasing && input[i] >= input[i + 1]) return false;
            if (!increasing && input[i] <= input[i + 1]) return false;
        }
        return true;
    }

    int solvePartOne(const std::vector<std::string> &input) {
        const auto vector = formatInput(input);
        auto result{0};
        for (const auto &line: vector) {
            bool increasing{false};
            if (line.front() < line.back()) {
                increasing = true;
            }
            if (conditionIsTrue(line, increasing)) {
                result++;
            }
        }
        return result;
    }

    /**
     *
     * @param input vector of integers
     * @param i index signaling which values should be erased
     * @return pair of vectors with erased values i and i + 1
     */
    auto createMulliganVector(const std::vector<int> &input, const int i) {
        auto [first, second] = std::make_pair(std::vector{input}, std::vector{input});
        first.erase(first.begin() + i);
        second.erase(second.begin() + i + 1);
        return std::make_pair(first, second);
    }

    /**
     *
     * @param input vector of integers
     * @param increasing bool weather the input numbers should increase or not
     * @return bool weather both conditions are fulfilled considering one mulligan oportunity
     */
    bool conditionIsTrueSwap(const std::vector<int> &input, const bool increasing) {
        for (int i = 0; i < input.size() - 1; ++i) {
            if (std::abs(input[i] - input[i + 1]) > 3) {
                auto [first,second] = createMulliganVector(input, i);
                return (conditionIsTrue(first, increasing) || conditionIsTrue(second, increasing));
            }
            if (increasing && input[i] >= input[i + 1]) {
                auto [first,second] = createMulliganVector(input, i);
                return (conditionIsTrue(first, increasing) || conditionIsTrue(second, increasing));
            }
            if (!increasing && input[i] <= input[i + 1]) {
                auto [first,second] = createMulliganVector(input, i);
                return (conditionIsTrue(first, increasing) || conditionIsTrue(second, increasing));
            }
        }
        return true;
    }

    int solvePartTwo(const std::vector<std::string> &input) {
        const auto vector = formatInput(input);
        auto result{0};

        for (const auto &line: vector) {
            bool increasing{false};
            if (line.front() < line.back()) {
                increasing = true;
            }
            if (conditionIsTrueSwap(line, increasing)) {
                result++;
            }
        }
        return result;
    }
}

namespace day3 {
    /**
     * wrapper class for a multiplication Object
     */
    struct Multiplication {
        int x{};
        int y{};
        int result{};

        explicit Multiplication(const std::string &input) {
            std::stringstream ss{input};
            ss.ignore(4);
            ss >> x;
            ss.ignore(1);
            ss >> y;
            result = x * y;
        }

        Multiplication() = delete;
    };

    int solvePartOne(const std::vector<std::string> &input) {
        int result{0};
        std::regex regex(R"(mul\((\d{1,3}),(\d{1,3})\))");
        for (auto line: input) {
            auto mulIt = std::sregex_iterator(line.begin(), line.end(), regex);
            auto mulEnd = std::sregex_iterator();
            while (mulIt != mulEnd) {
                Multiplication test{mulIt->str()};
                result += test.result;
                ++mulIt;
            }
        }
        return result;
    }

    int solvePartTwo(const std::vector<std::string> &input) {
        bool mulEnabled{true};
        int result{0};
        std::regex regex(R"((mul\((\d{1,3}),(\d{1,3})\))|(do\(\))|(don't\(\)))");
        for (auto line: input) {
            auto regexIt = std::sregex_iterator(line.begin(), line.end(), regex);
            auto regexItEnd = std::sregex_iterator();
            while (regexIt != regexItEnd) {
                switch (regexIt->str()[2]) {
                    case '(': mulEnabled = true;
                        break;
                    case 'n': mulEnabled = false;
                        break;
                    case 'l': {
                        if (mulEnabled) {
                            Multiplication test{regexIt->str()};
                            result += test.result;
                            break;
                        }
                    }
                    default: break;
                }
                ++regexIt;
            }
        }
        return result;
    }
}

namespace day4 {
    auto transformVector(const std::vector<std::string> &input) {
        
    }

    int solvePartOne(const std::vector<std::string> &input) {

    }

    int solvePartTwo(const std::vector<std::string> &input) {
        return 0;
    }
}
