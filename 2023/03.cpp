#include <iostream>

#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <iterator>
#include <stdexcept>

#include <algorithm>
#include <numeric>
#include <cassert>
#include <set>
#include <memory>
#include <limits>

std::vector<std::string> read_input_file() {

    std::ifstream file_in("../input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(!lines.empty());
    return lines;
}


int main() {
    std::vector<std::string> input = read_input_file();

    // include padding to remove the need for bounds checking
    size_t num_cols = input[0].size();
    std::vector<std::string> padded_input;
    padded_input.reserve(input.size() + 2);
    padded_input.push_back(std::string(num_cols + 2, '.'));
    for (const auto &s: input) {
        padded_input.push_back("." + s + ".");
    }
    padded_input.push_back(std::string(num_cols + 2, '.'));

    std::vector<unsigned long> part_numbers;
    std::map<std::pair<int, int>, unsigned long> gear_values;
    std::map<std::pair<int, int>, unsigned long> num_adjacent_parts;


    for (int i = 0; i < padded_input.size(); ++i) {
        int pos = 0;
        int digit_start_pos = -1;
        bool adjacency = false;
        bool gear_adjacent = false;
        std::set<std::pair<int, int>> gear_pos;
        while (pos < padded_input[i].size()) {
            if (std::isdigit(padded_input[i][pos])) {
                if (digit_start_pos == -1) {
                    digit_start_pos = pos;
                    assert(adjacency == false);
                    assert(gear_adjacent == false);
                }
                if ((not std::isdigit(padded_input[i][pos - 1]) && padded_input[i][pos - 1] != '.')
                    || (not std::isdigit(padded_input[i][pos + 1]) && padded_input[i][pos + 1] != '.')
                    || (not std::isdigit(padded_input[i - 1][pos]) && padded_input[i - 1][pos] != '.')
                    || (not std::isdigit(padded_input[i + 1][pos]) && padded_input[i + 1][pos] != '.')
                    // diagonal
                    || (not std::isdigit(padded_input[i + 1][pos + 1]) && padded_input[i + 1][pos + 1] != '.')
                    || (not std::isdigit(padded_input[i + 1][pos - 1]) && padded_input[i + 1][pos - 1] != '.')
                    || (not std::isdigit(padded_input[i - 1][pos + 1]) && padded_input[i - 1][pos + 1] != '.')
                    || (not std::isdigit(padded_input[i - 1][pos - 1]) && padded_input[i - 1][pos - 1] != '.')
                        ) { adjacency = true; }
                int condition = (padded_input[i][pos - 1] == '*') << 1
                                | (padded_input[i][pos + 1] == '*') << 2
                                | (padded_input[i - 1][pos] == '*') << 3
                                | (padded_input[i + 1][pos] == '*') << 4
                                | (padded_input[i - 1][pos - 1] == '*') << 5
                                | (padded_input[i - 1][pos + 1] == '*') << 6
                                | (padded_input[i + 1][pos - 1] == '*') << 7
                                | (padded_input[i + 1][pos + 1] == '*') << 8;
                if (condition) {
                    gear_adjacent = true;

                    if (condition & (1 << 1)) {
                        gear_pos.insert(std::make_pair(i, pos - 1));
                    }
                    if (condition & (1 << 2)) {
                        gear_pos.insert(std::make_pair(i, pos + 1));
                    }
                    if (condition & (1 << 3)) {
                        gear_pos.insert(std::make_pair(i - 1, pos));
                    }
                    if (condition & (1 << 4)) {
                        gear_pos.insert(std::make_pair(i + 1, pos));
                    }

                    if (condition & (1 << 5)) {
                        gear_pos.insert(std::make_pair(i - 1, pos - 1));
                    }
                    if (condition & (1 << 6)) {
                        gear_pos.insert(std::make_pair(i - 1, pos + 1));
                    }
                    if (condition & (1 << 7)) {
                        gear_pos.insert(std::make_pair(i + 1, pos - 1));
                    }
                    if (condition & (1 << 8)) {
                        gear_pos.insert(std::make_pair(i + 1, pos + 1));
                    }
                }

            } else if (digit_start_pos != -1) {
                int num = std::stoi(padded_input[i].substr(digit_start_pos, pos - 1));
                if (adjacency) {
                    part_numbers.push_back(num);
                }
                if (gear_adjacent) {
                    for (auto g: gear_pos) {
                        if (num_adjacent_parts.find(g) != num_adjacent_parts.end()) {
                            num_adjacent_parts[g] = num_adjacent_parts[g] + 1;
                            gear_values[g] = gear_values[g] * num;
                        } else {
                            num_adjacent_parts[g] = 1;
                            gear_values[g] = num;
                        }
                    }
                }

                digit_start_pos = -1;
                adjacency = false;
                gear_adjacent= false;
            }
            ++pos;
        }
    }

    unsigned long sum = 0;

    for (auto pair: num_adjacent_parts) {
        if (pair.second == 2) {
            sum = sum + gear_values[pair.first];
        }

    }




// operator + is default for accumulate
    int sum_part_number = std::accumulate(part_numbers.begin(), part_numbers.end(), 0);
    std::cout << "Sum of Part numbers: " << sum_part_number << "\n";

    std::cout << "Result Score: " << sum << "\n";

    return 0;
}