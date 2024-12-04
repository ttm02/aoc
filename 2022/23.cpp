//
// Created by tim on 02.12.22.
//

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
    assert(lines.size() > 0);
    return lines;
}

typedef std::tuple<int, int> Point;

std::tuple<std::vector<std::tuple<Point, int>>, bool>
get_proposed_positions(std::vector<Point> &current_positions, int round_number) {

    std::vector<std::tuple<Point, int>> proposals;
    proposals.reserve(current_positions.size());
    int stationary_counter = 0;

    std::sort(current_positions.begin(), current_positions.end());
    // the order of the elves is not relevant
    // sorts ascending regarding x coordinate, meaning we only need to consider the other elves if x is one below


    for (int n = 0; n < current_positions.size(); ++n) {
        auto [pos_i, pos_j] = current_positions[n];

        bool north = false, east = false, south = false, west = false;
        int j = n - 1;
        while (j >= 0 && std::get<0>(current_positions[j]) >= pos_i - 1) {
            // there shouldnt be that many elves, so n^2 should be OK
            auto [other_pos_i, other_pos_j] = current_positions[j];
            if (other_pos_i == pos_i - 1 && std::abs(other_pos_j - pos_j) <= 1)
                north = true;
            if (other_pos_i == pos_i + 1 && std::abs(other_pos_j - pos_j) <= 1)
                south = true;
            if (other_pos_j == pos_j - 1 && std::abs(other_pos_i - pos_i) <= 1)
                west = true;
            if (other_pos_j == pos_j + 1 && std::abs(other_pos_i - pos_i) <= 1)
                east = true;
            j--;
        }
        j = n + 1;
        while (j < current_positions.size() && std::get<0>(current_positions[j]) <= pos_i + 1) {
            // there shouldnt be that many elves, so n^2 should be OK
            auto [other_pos_i, other_pos_j] = current_positions[j];
            if (other_pos_i == pos_i - 1 && std::abs(other_pos_j - pos_j) <= 1)
                north = true;
            if (other_pos_i == pos_i + 1 && std::abs(other_pos_j - pos_j) <= 1)
                south = true;
            if (other_pos_j == pos_j - 1 && std::abs(other_pos_i - pos_i) <= 1)
                west = true;
            if (other_pos_j == pos_j + 1 && std::abs(other_pos_i - pos_i) <= 1)
                east = true;
            j++;
        }

        if (!north && !east && !south && !west) {
            // everything Empty: dont move
            //proposals.push_back(std::make_tuple(std::make_tuple(pos_i, pos_j), n));
            stationary_counter++;

        } else {
            int i = 0;
            for (; i < 4; ++i) {

                int dir_to_check = (round_number + i) % 4;

                switch (dir_to_check) {
                    case 0:
                        if (!north) {
                            proposals.push_back(std::make_tuple(std::make_tuple(pos_i - 1, pos_j), n));
                            i = 6;
                        }
                        break;
                    case 1:
                        if (!south) {
                            proposals.push_back(std::make_tuple(std::make_tuple(pos_i + 1, pos_j), n));
                            i = 6;
                        }
                        break;
                    case 2:
                        if (!west) {
                            proposals.push_back(std::make_tuple(std::make_tuple(pos_i, pos_j - 1), n));
                            i = 6;
                        }
                        break;
                    case 3:
                        if (!east) {
                            proposals.push_back(std::make_tuple(std::make_tuple(pos_i, pos_j + 1), n));
                            i = 6;
                        }
                        break;
                    default:
                        assert(false);
                }

            }
            // none of the above: movement is not valid
            //if (i != 6 + 1)
            //proposals.push_back(std::make_tuple(std::make_tuple(pos_i, pos_j), n));
        }
    }
    bool moved = stationary_counter < current_positions.size();
    return std::make_tuple(proposals, moved);

}

void execute_proposals(std::vector<std::tuple<Point, int>> proposals, std::vector<Point> &current_positions,
                       int round_number) {
    //assert(proposals.size() == current_positions.size());

    std::sort(proposals.begin(), proposals.end());
    // this will have the same proposals in hte same area

    for (auto It = proposals.begin(); It < proposals.end(); ++It) {
        auto [pos, idx] = *It;
        if ((It - 1 == proposals.begin() || pos != std::get<0>(*(It - 1))) &&
            (It + 1 == proposals.end() || pos != std::get<0>(*(It + 1)))) {
            current_positions[idx] = pos;
        }
    }


}

void draw_field(const std::vector<Point> &positions) {
    Point max = std::accumulate(positions.begin(), positions.end(), std::make_tuple(0, 0), [](auto accu, auto elem) {
        return std::make_tuple(std::max(std::get<0>(accu), std::get<0>(elem)),
                               std::max(std::get<1>(accu), std::get<1>(elem)));
    });
    Point min = std::accumulate(positions.begin(), positions.end(), positions[0], [](auto accu, auto elem) {
        return std::make_tuple(std::min(std::get<0>(accu), std::get<0>(elem)),
                               std::min(std::get<1>(accu), std::get<1>(elem)));
    });


    std::vector<Point> scaled_points;
    scaled_points.reserve(positions.size());

    std::transform(positions.begin(), positions.end(), std::back_inserter(scaled_points), [min](auto p) {
        return std::make_tuple(std::get<0>(p) - std::get<0>(min), std::get<1>(p) - std::get<1>(min));
    });

    const auto num_rows = 1 + std::get<0>(max) - std::get<0>(min);
    const auto num_cols = 1 + std::get<1>(max) - std::get<1>(min);

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            if (std::find(scaled_points.begin(), scaled_points.end(), std::make_tuple(i, j)) != scaled_points.end())
                std::cout << "#";
            else
                std::cout << ".";

        }
        std::cout << "\n";

    }


}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<Point> positions;

    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[i].length(); ++j) {
            if (input[i][j] == '#')
                positions.push_back(std::make_tuple(i, j));
        }
    }

    const int elve_count = positions.size();
    std::cout << "Elves: " << elve_count << "\n";
    int round = 0;
    bool moved = true;
    while (moved) {
        std::cout << "Round " << round << "\n";
        //draw_field(positions);
        auto [proposals, moved_this_time] = get_proposed_positions(positions, round);
        execute_proposals(proposals, positions, round);
        moved = moved_this_time;
        ++round;
    }

    Point max = std::accumulate(positions.begin(), positions.end(), std::make_tuple(0, 0), [](auto accu, auto elem) {
        return std::make_tuple(std::max(std::get<0>(accu), std::get<0>(elem)),
                               std::max(std::get<1>(accu), std::get<1>(elem)));
    });
    Point min = std::accumulate(positions.begin(), positions.end(), positions[0], [](auto accu, auto elem) {
        return std::make_tuple(std::min(std::get<0>(accu), std::get<0>(elem)),
                               std::min(std::get<1>(accu), std::get<1>(elem)));
    });

    auto area = (1 + std::get<0>(max) - std::get<0>(min)) * (1 + std::get<1>(max) - std::get<1>(min));
    std::cout << "End Position: \n";
    draw_field(positions);

    auto score = area - positions.size();

    std::cout << "Result Score: " << score << "\n";
    std::cout << "Stationary Round: " << round << "\n";

    return 0;
}