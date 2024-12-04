//
// Created by tim on 02.12.22.
//

#include <iostream>

#include <sstream>
#include <fstream>
#include <vector>
#include <map>
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

std::tuple<int, int, int> read_coordinate(std::string s) {

    int delim1 = s.find(',', 0);
    int delim2 = s.find(',', delim1 + 1);

    return std::make_tuple(std::stoi(s.substr(0, delim1)),
                           std::stoi(s.substr(delim1 + 1, delim2 - delim1)),
                           std::stoi(s.substr(delim2 + 1, s.length() - delim2)));

}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<std::tuple<int, int, int>> coordinates;

    std::transform(input.begin(), input.end(), std::back_inserter(coordinates),
                   [](auto s) { return read_coordinate(s); });

    const int max_coord = 1 + std::accumulate(coordinates.begin(), coordinates.end(), 0, [](int accu, auto tup) {

        return std::max(accu, std::max(std::get<0>(tup), std::max(std::get<1>(tup), std::get<2>(tup))));
    });

    std::vector<std::vector<std::vector<int>>> shape(max_coord,
                                                     std::vector<std::vector<int>>(max_coord,
                                                                                   std::vector<int>(
                                                                                           max_coord,
                                                                                           0)));

    // set the shape
    for (auto coord: coordinates) {
        shape[std::get<0>(coord)][std::get<1>(coord)][std::get<2>(coord)] = 1;
    }

    // mark everything that is reachable
    // the bound may be smaller than n^3, which is the maximum length of any path throughout the space
    for (int n = 0; n < max_coord * max_coord * max_coord; ++n) {
        for (int i = 0; i < max_coord; ++i) {
            for (int j = 0; j < max_coord; ++j) {
                for (int k = 0; k < max_coord; ++k) {
                    if (shape[i][j][k] == 0) {
                        if (i - 1 < 0 || shape[i - 1][j][k] == -1) shape[i][j][k] = -1;
                        if (i + 1 >= max_coord || shape[i + 1][j][k] == -1) shape[i][j][k] = -1;

                        if (j - 1 < 0 || shape[i][j - 1][k] == -1) shape[i][j][k] = -1;
                        if (j + 1 >= max_coord || shape[i][j + 1][k] == -1) shape[i][j][k] = -1;

                        if (k - 1 < 0 || shape[i][j][k - 1] == -1) shape[i][j][k] = -1;
                        if (k + 1 >= max_coord || shape[i][j][k + 1] == -1) shape[i][j][k] = -1;
                    }
                }

            }
        }
    }

    int surface = 0;
    for (int i = 0; i < max_coord; ++i) {
        for (int j = 0; j < max_coord; ++j) {
            for (int k = 0; k < max_coord; ++k) {
                if (shape[i][j][k] == 1) {
                    if (i - 1 < 0 || shape[i - 1][j][k] == -1) surface++;
                    if (i + 1 >= max_coord || shape[i + 1][j][k] == -1) surface++;

                    if (j - 1 < 0 || shape[i][j - 1][k] == -1) surface++;
                    if (j + 1 >= max_coord || shape[i][j + 1][k] == -1) surface++;

                    if (k - 1 < 0 || shape[i][j][k - 1] == -1) surface++;
                    if (k + 1 >= max_coord || shape[i][j][k + 1] == -1) surface++;
                }
            }

        }
    }


    int score = surface;
    std::cout << "Result Score: " << score << "\n";
    return 0;
}