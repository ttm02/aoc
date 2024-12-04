//
// Created by tim on 02.12.22.
//

#include <iostream>

#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

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
    assert(lines.size() > 0);
    return lines;
}

int main() {
    std::vector<std::string> input = read_input_file();

    const int FIELD_SIZE = 1000;

    std::vector<std::vector<char>> field(FIELD_SIZE, std::vector<char>(FIELD_SIZE, '.'));

    // include the rock formations

    int max_y = 0;

    for (auto line: input) {
        int pos = 0;
        int delim = line.find("->", pos + 2);

        auto inter_delim = line.find(',', pos);
        int prev_i = std::stoi(line.substr(pos, inter_delim));
        int prev_j = std::stoi(line.substr(inter_delim + 1, delim - inter_delim));
        max_y = std::max(max_y, prev_j);

        while (delim < line.length()) {
            pos = delim;
            delim = line.find("->", pos + 2);

            inter_delim = line.find(',', pos);
            int cur_i = std::stoi(line.substr(pos + 2, inter_delim - pos));
            int cur_j = std::stoi(line.substr(inter_delim + 1, delim - inter_delim));

            // need to be in a straight line
            assert((cur_i == prev_i) || (cur_j == prev_j));

            for (int i = std::min(cur_i, prev_i); i <= std::max(cur_i, prev_i); ++i) {
                for (int j = std::min(cur_j, prev_j); j <= std::max(cur_j, prev_j); ++j) {
                    field[i][j] = '#';
                }
            }
            prev_i = cur_i;
            prev_j = cur_j;
            max_y = std::max(max_y, prev_j);
        }
    }

    // add floor
    for (int i = 0; i < FIELD_SIZE; ++i) {
        field[i][max_y + 2] = '#';
    }

    // simulate_sand
    field[500][0] = '+';
    int sand_count = 0;


    std::cout << "\n";
    for (int k = 493; k < 504; ++k) {
        for (int l = 0; l < 11; ++l) {
            std::cout << field[k][l];
        }
        std::cout << "\n";

    }

    int i = 500;
    int j = 0;
    bool has_finished = false;
    while (!has_finished) {
        sand_count++;
        i = 500;
        j = 0;
        if (field[i][j] != '+') {
            // filled up fuly
            sand_count--;
            has_finished = true;
            break;
        }
        while (true) {
            if (i < 1 || i > (FIELD_SIZE - 2) || j < 0 || j > (FIELD_SIZE - 2)) {
                // this sand falls into the abyss
                sand_count--;
                has_finished = true;
                break;
            }
            // try bottom, left, right
            if (field[i][j + 1] == '.') {
                j++;
            } else if (field[i - 1][j + 1] == '.') {
                j++;
                i--;
            } else if (field[i + 1][j + 1] == '.') {
                j++;
                i++;
            } else {
                // all blocked: come to rest
                field[i][j] = 'o';
                std::cout << "Rest at " << i << "," << j << "\n";
                break;
            }
        }
        /*
        // print excerpt of field
        std::cout << "\n";
        for (int k = 493; k < 504; ++k) {
            for (int l = 0; l < 11; ++l) {
                std::cout << field[k][l];
            }
            std::cout << "\n";
        }
        */
    }

    // they start counting at 1
    int score = 0;
    std::cout << "Result Score: " << sand_count << "\n";
    return 0;
}