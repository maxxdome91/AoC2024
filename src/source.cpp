//
// Created by Maximilian Dornheim on 17.02.25.
//

#include "source.h"

#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <ranges>
#include <chrono>
#include <algorithm>
#include <map>
#include <list>


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
     * @return bool weather both conditions are fulfilled considering one mulligan opportunity
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
        //check each line against regex occurrences
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
    //used for bound check
    int maxX{};
    int maxY{};

    /**
     * Takes an origin (which should already be checked) and computes the number of occurrences regarding all 8 2D directions
     * @param x x-coordinate of origin
     * @param y y-coordinate of origin
     * @param input vector of strings
     * @param word string to check against
     * @return number of word occurrences regarding all 8 2D directions
     */
    int checkForXMAS(const int x, const int y, const std::vector<std::string> &input, const std::string_view word) {
        int result{0};
        static const std::vector xDiff{-1, 0, 1, -1, 1, -1, 0, 1};
        static const std::vector yDiff{-1, -1, -1, 0, 0, 1, 1, 1};
        for (auto direction = 0; direction < 8; ++direction) {
            auto wordIndex{0};
            for (; wordIndex < word.size(); ++wordIndex) {
                const auto newX{x + xDiff[direction] * (wordIndex + 1)};
                const auto newY{y + yDiff[direction] * (wordIndex + 1)};
                if (newX > maxX || newY > maxY || newX < 0 || newY < 0) {
                    break;
                }
                if (input[newY][newX] == word[wordIndex]) {
                    continue;
                }
                break;
            }
            if (wordIndex == word.size()) {
                ++result;
            }
        }
        return result;
    }

    int solvePartOne(const std::vector<std::string> &input) {
        maxX = static_cast<int>(input[0].size()) - 1;
        maxY = static_cast<int>(input.size() - 1);
        int result{0};
        for (int y = 0; const auto &line: input) {
            for (int x = 0; const auto character: line) {
                if (character == 'X') {
                    result += checkForXMAS(x, y, input, "MAS");
                }
                ++x;
            }
            ++y;
        }
        return result;
    }

    /**
     *
     * @param x x coordinate of origin
     * @param y y coordinate of origin
     * @param input vector of strings
     * @param word 3 letter string to check against
     * @return bool weather or not the word is crossed in any way against the origin
     */
    bool checkForCrossedWord(const int x, const int y, const std::vector<std::string> &input,
                             const std::string_view word) {
        //algorithm only viable for word size 3
        if (word.size() != 3) return false;
        std::string reversedWord{word};
        std::ranges::reverse(reversedWord);
        if (const auto toCompare = std::string{input[y - 1][x - 1], input[y][x], input[y + 1][x + 1]};
            toCompare != reversedWord && toCompare != word) {
            return false;
        }
        if (const auto toCompare = std::string{input[y + 1][x - 1], input[y][x], input[y - 1][x + 1]};
            toCompare != reversedWord && toCompare != word) {
            return false;
        }
        return true;
    }

    int solvePartTwo(const std::vector<std::string> &input) {
        //check for maximum x and y coordinates
        maxX = static_cast<int>(input[0].size()) - 1;
        maxY = static_cast<int>(input.size() - 1);
        int result{0};

        //both x and y beginning at index 1 and counting to max - 1 since there are no crossed words at those indexes
        for (int y = 0; const auto &line: input) {
            if (y == 0 || y == maxY) {
                ++y;
                continue;
            }
            for (int x = 0; const auto character: line) {
                if (x == 0 || x == maxX) {
                    ++x;
                    continue;
                }
                if (character == 'A') {
                    result += checkForCrossedWord(x, y, input, "MAS");
                }
                ++x;
            }
            ++y;
        }
        return result;
    }
}

namespace day5 {
    //one Rule; first must be updated before second if both in same update
    struct Rule {
        int first{};
        int second{};

        explicit Rule(const std::string &string) {
            first = std::stoi(string.substr(0, string.find('|')));
            second = std::stoi(string.substr(string.find('|') + 1));
        }
    };

    //one Update containing several pages to be updated sequentially
    struct Update {
        std::vector<int> pages{};

        explicit Update(std::string input) {
            auto v = input | std::ranges::views::split(',') | std::ranges::views::transform([](auto r) {
                return std::string(r.data(), r.size());
            });
            for (const auto vector = std::vector<std::string>{v.begin(), v.end()}; const auto &part: vector) {
                pages.emplace_back(std::stoi(part));
            }
        }
    };

    //wrapper for a set of rules
    struct RuleBook {
        std::vector<Rule> rules{};

        void emplace_rule(const std::string &rule) {
            rules.emplace_back(rule);
        }

        /**
         *
         * @param update Update object
         * @return all applicable rules regarding the Update
         */
        [[nodiscard]] std::vector<Rule> getApplicableRules(Update update) const {
            std::vector<Rule> result{};
            for (const auto &rule: rules) {
                if (std::ranges::find(update.pages, rule.first) != update.pages.end() &&
                    std::ranges::find(update.pages, rule.second) != update.pages.end()) {
                    result.emplace_back(rule);
                }
            }
            return result;
        }
    };

    /**
     * 
     * @param update Update containing pages to be updated
     * @param rules set of rules applicable for given Update
     * @return value of page at middle index after reordering
     */
    int repairUpdate(Update update, const std::vector<Rule> &rules) {
    beginning:
        for (const auto &rule: rules) {
            const auto itFirst = std::ranges::find(update.pages, rule.first);
            if (const auto itSecond = std::ranges::find(update.pages, rule.second);
                std::distance(itFirst, itSecond) < 0) {
                std::swap(*itFirst, *itSecond);
                goto beginning;
            }
        }
        return update.pages.at((update.pages.size() - 1) / 2);
    }

    /**
     *
     * @param update Update containing pages in order of update
     * @param ruleBook complete rulebook
     * @param repair bool weather or not the update should be repaired 
     * @return value of middle page of Update if correct, 0 else
     */
    int checkUpdate(Update &update, const RuleBook &ruleBook, const bool repair) {
        for (const auto rules{ruleBook.getApplicableRules(update)}; const auto &rule: rules) {
            const auto itFirst = std::ranges::find(update.pages, rule.first);
            if (const auto itSecond = std::ranges::find(update.pages, rule.second);
                std::distance(itFirst, itSecond) < 0) {
                if (repair) {
                    return repairUpdate(update, rules);
                }
                return 0;
            }
        }
        if (repair) {
            return 0;
        }
        return update.pages.at((update.pages.size() - 1) / 2);
    }

    int solvePartOne(const std::vector<std::string> &input) {
        int result{0};
        RuleBook rules{};
        auto it = input.begin();
        //populate rulebook
        for (; it != std::ranges::find(input, ""); ++it) {
            rules.emplace_rule(*it);
        }
        ++it;
        std::vector<Update> updates{};
        //populate updates
        for (; it != input.end(); ++it) {
            updates.emplace_back(*it);
        }
        for (auto &update: updates) {
            result += checkUpdate(update, rules, false);
        }
        return result;
    }

    int solvePartTwo(const std::vector<std::string> &input) {
        int result{0};
        RuleBook rules{};
        auto it = input.begin();
        //populate rulebook
        for (; it != std::ranges::find(input, ""); ++it) {
            rules.emplace_rule(*it);
        }
        ++it;
        std::vector<Update> updates{};
        //populate updates
        for (; it != input.end(); ++it) {
            updates.emplace_back(*it);
        }
        for (auto &update: updates) {
            result += checkUpdate(update, rules, true);
        }
        return result;
    }
}

namespace day6 {
    size_t maxY;
    size_t maxX;

    enum Direction { UP, RIGHT, DOWN, LEFT };

    struct Position {
        int x;
        int y;
        Direction direction;

        Position(const int _x, const int _y, const Direction _direction = UP) : x{_x}, y{_y}, direction{_direction} {
        }

        friend bool operator<(const Position &lhs, const Position &rhs) {
            if (lhs.x < rhs.x)
                return true;
            if (rhs.x < lhs.x)
                return false;
            return lhs.y < rhs.y;
        }

        friend bool operator<=(const Position &lhs, const Position &rhs) {
            return rhs >= lhs;
        }

        friend bool operator>(const Position &lhs, const Position &rhs) {
            return rhs < lhs;
        }

        friend bool operator>=(const Position &lhs, const Position &rhs) {
            return !(lhs < rhs);
        }

        friend bool operator==(const Position &lhs, const Position &rhs) {
            return lhs.x == rhs.x
                   && lhs.y == rhs.y;
        }

        friend bool operator!=(const Position &lhs, const Position &rhs) {
            return !(lhs == rhs);
        }
    };

    class Guard {
    public:
        std::vector<Position> path;
        std::set<Position> visited;
        Position position;

        [[nodiscard]] std::vector<Position> getPath() const {
            return path;
        }

        [[nodiscard]] Position getPosition() const {
            return position;
        }

        void setPosition(const Position &position) {
            this->position = position;
        }

        explicit Guard(const Position position) : path{position}, visited{position}, position{position} {
        }

        /**
         * turns the guard 90 degrees to the right
         */
        void turnRight() {
            switch (position.direction) {
                case UP: position.direction = RIGHT;
                    break;
                case RIGHT: position.direction = DOWN;
                    break;
                case DOWN: position.direction = LEFT;
                    break;
                case LEFT: position.direction = UP;
                    break;
            }
        }

        /**
         *
         * @param board play board
         * @return weather or not there is an obstacle ahead
         */
        [[nodiscard]] bool obstacleAhead(const std::vector<std::string> &board) const {
            switch (position.direction) {
                case UP:
                    if (board[position.y - 1][position.x] == '#') return true;
                    break;
                case RIGHT:
                    if (board[position.y][position.x + 1] == '#') return true;
                    break;
                case DOWN:
                    if (board[position.y + 1][position.x] == '#') return true;
                    break;
                case LEFT:
                    if (board[position.y][position.x - 1] == '#') return true;
                    break;
            }
            return false;
        }

        /**
         * function which performs one step ahead or one turn to the right, if there is an obstacle ahead. does not
         * update path variable, only visited variable
         * @param board arena of the guard
         * @return weather or not the step can be made inside the arena
         */
        bool step(const std::vector<std::string> &board) {
            switch (position.direction) {
                case UP:
                    if (position.y == 0) return false;
                    if (obstacleAhead(board)) {
                        turnRight();
                        return true;
                    }
                    --position.y;
                    visited.insert(position);
                    return true;
                case RIGHT:
                    if (position.x == maxX - 1) return false;
                    if (obstacleAhead(board)) {
                        turnRight();
                        return true;
                    }
                    ++position.x;
                    visited.insert(position);
                    return true;
                case DOWN:
                    if (position.y == maxY - 1) return false;
                    if (obstacleAhead(board)) {
                        turnRight();
                        return true;
                    }
                    ++position.y;
                    visited.insert(position);
                    return true;
                case LEFT:
                    if (position.x == 0) return false;
                    if (obstacleAhead(board)) {
                        turnRight();
                        return true;
                    }
                    --position.x;
                    visited.insert(position);
                    return true;
                default: return false;
            }
        }
    };

    /**
     *
     * @param board arena of the guard
     * @return starting coordinates of the guard
     */
    std::pair<int, int> findStart(const std::vector<std::string> &board) {
        for (int y = 0; y < board.size(); ++y) {
            for (int x = 0; x < board[0].size(); ++x) {
                if (board[y][x] == '^') {
                    return {x, y};
                }
            }
        }
        return {0, 0};
    }

    /**
     *
     * @param position position to check against
     * @param path walked path
     * @return weather or not exact position has already been visited
     */
    bool posInVec(const Position position, const std::vector<Position> &path) {
        return std::ranges::any_of(path, [&position](const Position &pos) {
            return pos.x == position.x && pos.y == position.y && pos.direction == position.direction;
        });
    }


    int solvePartOne(const std::vector<std::string> &input) {
        maxY = input.size();
        maxX = input[0].size();
        auto [fst, snd] = findStart(input);
        Guard guard{Position{fst, snd, UP}};
        while (guard.step(input)) {
        };
        return static_cast<int>(guard.visited.size());
    }


    int solvePartTwo(const std::vector<std::string> &input) {
        int result{0};
        maxY = input.size();
        maxX = input[0].size();
        auto [fst, snd] = findStart(input);
        Guard guard{Position{fst, snd, UP}};
        while (guard.step(input)) {
        };

        //erase starting Position; crucial for loop detection
        guard.visited.erase(Position{fst, snd, UP});

        //loop through all visited sports as obstacles need to be placed in these locations
        for (const auto spot: guard.visited) {
            std::vector alteredBoard{input};
            alteredBoard[spot.y][spot.x] = '#';
            auto circleGuard = Guard{Position{fst, snd, UP}};
            int i{0};
            while (circleGuard.step(alteredBoard)) {
                //checking only every 2500th time reduces runtime
                if (i % 2500 == 0 && posInVec(circleGuard.position, circleGuard.getPath())) {
                    ++result;
                    break;
                }
                circleGuard.path.emplace_back(circleGuard.position);
                ++i;
            }
        }
        return result;
    }
}

namespace day7 {
    enum OPERATIONS { ADD, MUL };

    enum EXTENDED_OPERATIONS { ADDITION, MULTIPLICATION, CONCATENATION };

    struct OperationWrapper {
        std::vector<EXTENDED_OPERATIONS> operations;

        explicit OperationWrapper(const int n) : operations(n, ADDITION) {
        }

        OperationWrapper() = delete;

        bool nextCombination(int i) {
        begin:
            switch (operations[i]) {
                case ADDITION:
                    operations[i] = MULTIPLICATION;
                    return true;
                case MULTIPLICATION:
                    operations[i] = CONCATENATION;
                    return true;
                case CONCATENATION:
                    if (operations.size() > i + 1) {
                        operations[i] = ADDITION;
                        ++i;
                        goto begin;
                    }
                    return false;
            }
            return false;
        }

        /**
         * cycles through all combinations of the EXTENDED_OPERATIONS
         * @return weather or not there is a next combination
         */
        bool nextCombination() {
            switch (operations[0]) {
                case ADDITION:
                    operations[0] = MULTIPLICATION;
                    return true;
                case MULTIPLICATION:
                    operations[0] = CONCATENATION;
                    return true;
                case CONCATENATION:
                    if (operations.size() > 1) {
                        operations[0] = ADDITION;
                        return nextCombination(1);
                    }
                    return false;
            }
            return false;
        }
    };


    /**
     * generates a vector of i * ADD and (size - i) * MUL Operations
     * @param i integer of how many ADD Operations to ADD
     * @param size size of output vector
     * @return vector of operations
     */
    std::vector<OPERATIONS> generateOperations(const int i, const size_t size) {
        std::vector<OPERATIONS> operations{};
        operations.reserve(size);
        for (int j = 0; j < i; ++j) {
            operations.emplace_back(ADD);
        }
        while (operations.size() < size) {
            operations.emplace_back(MUL);
        }
        return operations;
    }


    /**
     *
     * @param numbers vector of numbers
     * @param operations vector of operations
     * @return result of calculation
     */
    unsigned long performCalculation(const std::vector<unsigned long> &numbers,
                                     const std::vector<OPERATIONS> &operations) {
        unsigned long result{0};
        for (int i = 0; i < operations.size(); ++i) {
            if (i == 0) {
                switch (operations[i]) {
                    case ADD:
                        result += numbers[i] + numbers[i + 1];
                        break;
                    case MUL:
                        result += numbers[i] * numbers[i + 1];
                        break;
                }
            } else {
                switch (operations[i]) {
                    case ADD:
                        result += numbers[i + 1];
                        break;
                    case MUL:
                        result *= numbers[i + 1];
                        break;
                }
            }
        }
        return result;
    }

    /**
     * performs the same as performCalculation, but includes the concatenation operator
     * @param numbers vector of numbers
     * @param operations vector of operations
     * @return result of calculation
     */
    unsigned long performExtendedCalculation(const std::vector<unsigned long> &numbers,
                                             const std::vector<EXTENDED_OPERATIONS> &operations) {
        unsigned long result{0};
        if (operations.empty()) {
            return numbers[0];
        }
        for (int i = 0; i < operations.size(); ++i) {
            if (i == 0) {
                switch (operations[i]) {
                    case ADDITION:
                        result += numbers[i] + numbers[i + 1];
                        break;
                    case MULTIPLICATION:
                        result += numbers[i] * numbers[i + 1];
                        break;
                    case CONCATENATION:
                        result += std::stoul(std::to_string(numbers[i]) + std::to_string(numbers[i + 1]));
                        break;
                }
            } else {
                switch (operations[i]) {
                    case ADDITION:
                        result += numbers[i + 1];
                        break;
                    case MULTIPLICATION:
                        result *= numbers[i + 1];
                        break;
                    case CONCATENATION:
                        result = std::stoul(std::to_string(result) + std::to_string(numbers[i + 1]));
                        break;
                }
            }
        }
        return result;
    }

    /**
     *
     * @param line string with a target and numbers, seperated by 2 spaces
     * @return pair of unsigned long target and vector<unsigned long> numbers
     */
    auto getTargetAndNumbers(const std::string &line) {
        unsigned long target{};
        unsigned long toAdd{};
        std::vector<unsigned long> numbers{};
        std::stringstream ss(line);
        ss >> target;
        ss.ignore(2);
        while (ss >> toAdd) {
            numbers.emplace_back(toAdd);
        }
        return std::make_pair(target, numbers);
    }

    unsigned long solvePartOne(const std::vector<std::string> &input) {
        unsigned long result{0};
        for (const auto &line: input) {
            auto [target, numbers] = getTargetAndNumbers(line);
            for (int i = 0; i < numbers.size(); ++i) {
                std::vector<OPERATIONS> operations = generateOperations(i, numbers.size() - 1);
                do {
                    if (performCalculation(numbers, operations) == target) {
                        result += target;
                        goto end;
                    }
                } while (std::ranges::next_permutation(operations).found);
            }
        end:



        }
        return result;
    }

    unsigned long solvePartTwo(const std::vector<std::string> &input) {
        unsigned long result{0};
        for (const auto &line: input) {
            auto [target, numbers] = getTargetAndNumbers(line);
            OperationWrapper operations{static_cast<int>(numbers.size() - 1)};
            do {
                if (performExtendedCalculation(numbers, operations.operations) == target) {
                    result += target;
                    goto end;
                }
            } while (operations.nextCombination());
        end:



        }
        return result;
    }
}

namespace day8 {
    struct location {
        int x;
        int y;

        auto operator<=>(const location &) const = default;
    };

    struct antenna {
        char frequency;
        location location;

        auto operator<=>(const antenna &) const = default;
    };


    /**
     *
     * @param input vector of strings containing input
     * @return pair<set<char>, multimap<char, antenna>>; the set containing the distinct frequencies and the map each
     * antenna location.
     */
    auto getFrequencies(const std::vector<std::string> &input) {
        std::multimap<char, antenna> result{};
        std::set<char> frequencies;
        for (int i = 0; i < input.size(); ++i) {
            for (int j = 0; j < input[i].size(); ++j) {
                if (input[i][j] == '.') {
                    continue;
                }
                result.emplace(input[i][j], antenna{input[i][j], j, i});
                frequencies.emplace(input[i][j]);
            }
        }
        return std::make_pair(frequencies, result);
    }

    size_t maxY{0};
    size_t maxX{0};

    /**
     *
     * @param antennaLocations multimap containing all antenna locations
     * @param frequency specific frequency to look for
     * @param result set of locations taking antinode locations
     */
    void getAntinodes(std::multimap<char, antenna> &antennaLocations, const char frequency,
                      std::set<location> &result) {
        auto [fst, snd] = antennaLocations.equal_range(frequency);
        std::vector<std::multimap<char, antenna>::iterator> locations{};
        for (auto it = fst; it != snd; ++it) {
            locations.emplace_back(it);
        }
        for (int i = 0; i < locations.size(); ++i) {
            antenna first = locations[i]->second;
            for (int j = i + 1; j < locations.size(); ++j) {
                antenna second = locations[j]->second;
                auto firstY = first.location.y + (first.location.y - second.location.y);
                auto firstX = first.location.x + (first.location.x - second.location.x);
                auto secondY = second.location.y + (second.location.y - first.location.y);
                auto secondX = second.location.x + (second.location.x - first.location.x);
                if (firstY >= 0 && firstY <= maxY && firstX >= 0 && firstX <= maxX) {
                    result.emplace(firstX, firstY);
                }
                if (secondY >= 0 && secondY <= maxY && secondX >= 0 && secondX <= maxX) {
                    result.emplace(secondX, secondY);
                }
            }
        }
    }

    int solvePartOne(const std::vector<std::string> &input) {
        maxY = input.size() - 1;
        maxX = input[0].size() - 1;
        std::set<location> result{};
        for (auto [frequencies, antennaMap] = getFrequencies(input); const auto frequency: frequencies) {
            getAntinodes(antennaMap, frequency, result);
        }
        return static_cast<int>(result.size());
    }

    /**
     *
     * @param antennaLocations multimap containing all antenna locations
     * @param frequency specific frequency to look for
     * @param result set of locations taking antinode locations
     */
    void getAntinodesPartTwo(std::multimap<char, antenna> &antennaLocations, const char frequency,
                             std::set<location> &result) {
        auto [fst, snd] = antennaLocations.equal_range(frequency);
        std::vector<std::multimap<char, antenna>::iterator> locations{};
        for (auto it = fst; it != snd; ++it) {
            locations.emplace_back(it);
        }
        for (int i = 0; i < locations.size(); ++i) {
            antenna first = locations[i]->second;
            result.emplace(first.location.x, first.location.y);
            for (int j = i + 1; j < locations.size(); ++j) {
                antenna second = locations[j]->second;
                result.emplace(second.location.x, second.location.y);
                auto firstY = first.location.y + (first.location.y - second.location.y);
                auto firstX = first.location.x + (first.location.x - second.location.x);
                auto secondY = second.location.y + (second.location.y - first.location.y);
                auto secondX = second.location.x + (second.location.x - first.location.x);
                while (firstY >= 0 && firstY <= maxY && firstX >= 0 && firstX <= maxX) {
                    result.emplace(firstX, firstY);
                    firstY += (first.location.y - second.location.y);
                    firstX += (first.location.x - second.location.x);
                }
                while (secondY >= 0 && secondY <= maxY && secondX >= 0 && secondX <= maxX) {
                    result.emplace(secondX, secondY);
                    secondY += (second.location.y - first.location.y);
                    secondX += (second.location.x - first.location.x);
                }
            }
        }
    }

    int solvePartTwo(const std::vector<std::string> &input) {
        maxY = input.size() - 1;
        maxX = input[0].size() - 1;
        std::set<location> result{};
        for (auto [frequencies, antennaMap] = getFrequencies(input); auto frequency: frequencies) {
            getAntinodesPartTwo(antennaMap, frequency, result);
        }
        return static_cast<int>(result.size());
    }
}

namespace day9 {
    struct ID {
        int id;

        auto operator<=>(const ID &) const = default;
    };

    struct Entry {
        int id;
        int size;

        auto operator<=>(const Entry &) const = default;
    };

    /**
     * 
     * @param input input string
     * @return vector<ID> of block-formatted output
     */
    std::vector<ID> formatToBlocks(const std::string &input) {
        std::vector<ID> result{};
        int index{0};
        bool data{true};
        std::stringstream ss{};
        for (const auto character: input) {
            const int length = character - '0';
            if (data) {
                for (int i = 0; i < length; ++i) {
                    result.emplace_back(index);
                }
                ++index;
                data = false;
            } else {
                for (int i = 0; i < length; ++i) {
                    result.emplace_back(-1);
                }
                data = true;
            }
        }
        return result;
    }

    /**
     * moves the IDs to free spaces on the left side
     * @param input vector<ID> block-formatted by formatToBlocks
     */
    void moveBlocks(std::vector<ID> &input) {
        auto front = input.begin();
        auto back = input.end();
        --back;
        while (back->id == -1) {
            --back;
        }
        while (front <= back) {
            if (front->id == -1 && back->id != -1) {
                std::swap(*front, *back);
                ++front;
                --back;
            } else {
                while (front->id != -1) {
                    ++front;
                }
                while (back->id == -1) {
                    --back;
                }
            }
        }
    }

    unsigned long solvePartOne(const std::vector<std::string> &input) {
        unsigned long result{0};
        std::vector<ID> blocks = formatToBlocks(input[0]);
        moveBlocks(blocks);
        std::erase(blocks, ID{-1});
        for (int i = 0; i < blocks.size(); ++i) {
            result += i * (blocks[i].id);
        }
        return result;
    }

    /**
     *
     * @param input input string
     * @return list<Entry> of block-formatted output
     */
    std::list<Entry> formatToEntries(const std::string &input) {
        std::list<Entry> result{};
        int index{0};
        bool data{true};
        for (const auto character: input) {
            int length = character - '0';
            if (data) {
                if (length != 0) {
                    result.emplace_back(index, length);
                }
                ++index;
                data = false;
            } else {
                if (length != 0) {
                    result.emplace_back(-1, length);
                }
                data = true;
            }
        }
        return result;
    }

    /**
     * tries to move Entries from right to left one time
     * @param input list<Entry> block-formatted by formatToEntries
     */
    void moveEntries(std::list<Entry> &input) {
        const auto front = input.begin();
        auto back = input.end();
        --back;
        while (back->id == -1) {
            --back;
        }
        while (back != front) {
            while (back->id == -1) {
                --back;
            }
            auto searchIt = input.begin();
            while (searchIt != back) {
                if (searchIt->id != -1) {
                    ++searchIt;
                    continue;
                }
                if (searchIt->size < back->size) {
                    ++searchIt;
                    continue;
                }
                searchIt->size -= back->size;
                const auto id = back->id;
                const auto size = back->size;
                back->id = -1;
                input.insert(searchIt, {id, size});
                if (searchIt->size == 0) {
                    input.erase(searchIt);
                }
                break;
            }
            --back;
        }
    }

    unsigned long solvePartTwo(const std::vector<std::string> &input) {
        unsigned long result{0};
        std::list<Entry> entries = formatToEntries(input[0]);
        moveEntries(entries);
        unsigned long index{0};
        for (const auto [id, size]: entries) {
            if (id == -1) {
                index += size;
                continue;
            }
            for (int i = 0; i < size; ++i, ++index) {
                result += index * id;
            }
        }
        return result;
    }
}
