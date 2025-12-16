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

std::vector<std::string> parse_input(std::vector<std::string> input) {
    // pad input to avoid boundary checks later
    std::vector<std::string> result;
    int width = input[0].size();
    result.push_back(std::string(width + 2, '.'));

    for (int i = 0; i < input.size(); i++) {
        result.push_back("." + input[i] + ".");
    }
    result.push_back(std::string(width + 2, '.'));
    return result;
}


int main() {
    std::vector<std::string> input = read_input_file();
    auto field = parse_input(input);

    long score = 0;


    int removed_this_round = 1;
    while (removed_this_round) {
        removed_this_round = 0;
        for (int i = 1; i < field.size() - 1; i++) {
            for (int j = 1; j < field[i].size() - 1; j++) {
                if (field[i][j] == '@') {
                    int empty =
                            static_cast<int>(field[i - 1][j - 1] == '.') +
                            static_cast<int>(field[i - 1][j] == '.') +
                            static_cast<int>(field[i - 1][j + 1] == '.') +

                            static_cast<int>(field[i][j - 1] == '.') +
                            static_cast<int>(field[i][j + 1] == '.') +

                            static_cast<int>(field[i + 1][j - 1] == '.') +
                            static_cast<int>(field[i + 1][j] == '.') +
                            static_cast<int>(field[i + 1][j + 1] == '.');


                    if (empty > 4) {
                        field[i][j] = 'x';
                        score++;
                        removed_this_round++;
                    }
                }
            }
            // probably more eficient if we directly replace with "."
            // I choose to operate in real rounds as the example solution shows
            for (int i = 1; i < field.size() - 1; i++) {
                for (int j = 1; j < field[i].size() - 1; j++) {
                    if (field[i][j] == 'x') {
                        field[i][j] = '.';
                    }
                }
            }
        }
        //
    }

    std::cout << "Result Score: " << score << "\n";

    return 0;
}
