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
#include <boost/tokenizer.hpp>

std::vector<std::string> read_input_file() {
    std::ifstream file_in("../input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(lines.size() > 0);
    return lines;
}


int main() {
    std::vector<std::string> input = read_input_file();

    // Last line contains operators
    std::string ops = input.back();
    input.pop_back();

    long score = 0;

    std::vector<long> current_problem = {};
    const int rows = input.size();
    const int cols = ops.size();
    for (int c = cols - 1; c >= 0; --c) {
        std::string num;
        for (int i = 0; i < rows; ++i) {
            if (input[i][c] != ' ') {
                num += input[i][c];
            }
        }
        if (num != "") {
            current_problem.push_back(std::stol(num));
        }
        if (ops[c] != ' ') {
            long result = 0;
            if (ops[c] == '*') {
                result = 1;
            }
            for (auto n: current_problem) {
                if (ops[c] == '*') {
                    result = result * n;
                } else if (ops[c] == '+') {
                    result = result + n;
                } else {
                    assert(0);
                }
            }
            score += result;
            current_problem.clear();
        }
    }


    std::cout << "Result Score: " << score << "\n";

    return 0;
}
