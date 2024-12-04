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

const int FIELD_WIDTH = 7;


void print_field(const std::vector<std::string> &field) {
    for (auto it = field.rbegin(); it != field.rend() - 5; ++it) {
        std::cout << *it << "\n";
    }
    std::cout << "\n";
}

void push(std::vector<std::string> &field, int lr, int target_at_count) {


    int i = field.size();
    int at_count = 0;
    while (at_count < target_at_count) {
        i--;
        for (int j = 1; j <= FIELD_WIDTH; ++j) {
            if (field[i][j] == '@') {
                at_count++;
                if (field[i][j + lr] != '@' && field[i][j + lr] != '-') {
                    return;
                }
            }
        }
    }

    // there is room: move
    i = field.size();

    at_count = 0;
    while (at_count < target_at_count) {
        i--;
        std::string new_row = "|-------|";
        for (int j = 1; j <= FIELD_WIDTH; ++j) {
            if (field[i][j] == '@') {
                new_row[j + lr] = '@';
                at_count++;
            } else if (field[i][j] == '#') {
                new_row[j] = '#';
                // no conflict possible, this was checked before
            } else if (field[i][j] == ':') {
                new_row[j] = ':';
            }
        }
        field[i] = new_row;
    }
}

void push_left(std::vector<std::string> &field, int target_at_count) {
    push(field, -1, target_at_count);
}

void push_right(std::vector<std::string> &field, int target_at_count) {
    push(field, +1, target_at_count);
}

bool fall_down(std::vector<std::string> &field, int target_at_count) {
    bool room = true;

    std::vector<std::tuple<int, int>> at_positions;
    at_positions.reserve(target_at_count);
    int i = field.size();
    int at_count = 0;
    while (at_count < target_at_count) {
        i--;
        for (int j = 1; j <= FIELD_WIDTH; ++j) {
            if (field[i][j] == '@') {
                at_count++;
                at_positions.push_back(std::make_tuple(i, j));
                if (field[i - 1][j] != '@' && field[i - 1][j] != '-') {
                    room = false;
                }
            }
        }

    }

    if (room) {

        // there is room: move
        for (auto point: at_positions) {
            field[std::get<0>(point)][std::get<1>(point)] = '-';
        }
        for (auto point: at_positions) {
            field[std::get<0>(point) - 1][std::get<1>(point)] = '@';
        }
        if (field[field.size() - 1] == "|-------|") {
            field.pop_back();// row is empty now
        }
        return true;
    } else {

        //print_field(field);
        // come to rest
        for (auto point: at_positions) {
            field[std::get<0>(point)][std::get<1>(point)] = '#';
        }
        return false;
    }
}

int spawn_figure(int figure, std::vector<std::string> &field) {
    // always 3 empty lines
    field.push_back("|-------|");
    field.push_back("|-------|");
    field.push_back("|-------|");

    switch (figure) {
        case 1:
            field.push_back("|--@@@@-|");
            return 4;
            break;
        case 2:
            field.push_back("|---@---|");
            field.push_back("|--@@@--|");
            field.push_back("|---@---|");
            return 5;
            break;
        case 3:
            field.push_back("|--@@@--|");
            field.push_back("|----@--|");
            field.push_back("|----@--|");
            return 5;
            break;
        case 4:
            field.push_back("|--@----|");
            field.push_back("|--@----|");
            field.push_back("|--@----|");
            field.push_back("|--@----|");
            return 4;
            break;
        case 5:
            field.push_back("|--@@---|");
            field.push_back("|--@@---|");
            return 4;
            break;
        default:
            assert(false);
            return 0;
            break;
    }

}

bool compare_cycle(std::vector<std::string> &field, int begin, int middle, int end) {

    if (middle - begin == end - middle) {

        // maybe this can be acheived with a memcompare, if we change the data layout
        for (int i = 0; i < middle - begin; ++i) {
            if (field[begin + i] != field[middle + i]) {
                return false;
            }
        }
        return true;
    } else
        return false;
}

std::tuple<unsigned long, unsigned long>
find_cycle(std::vector<std::string> &field, std::vector<std::tuple<unsigned long, int, int>> &cycle_points,
           unsigned long current_rock,
           int wind_pattern_position) {
    // try to find a cycle

    // cycle_point = current_field_height ,rock that has rested, current wind_pattern pos


    for (int len = 1; len * 10 < cycle_points.size(); ++len) {
        int end = cycle_points.size() - 1;
        int middle = end - (len * 5);
        int begin = middle - (len * 5);
        assert(std::get<1>(cycle_points[begin]) == std::get<1>(cycle_points[middle]) &&
               std::get<1>(cycle_points[middle]) == std::get<1>(cycle_points[end])); // must be at the same point


        if (std::get<2>(cycle_points[begin]) == std::get<2>(cycle_points[middle]) &&
            std::get<2>(cycle_points[middle]) == std::get<2>(cycle_points[end])) {
            if (compare_cycle(field, std::get<0>(cycle_points[begin]), std::get<0>(cycle_points[middle]),
                              std::get<0>(cycle_points[end]))) {
                std::cout << "Found cycle len" << len << "\n";

                unsigned long cycle_height = std::get<0>(cycle_points[middle]) - std::get<0>(cycle_points[begin]);
                assert(cycle_height == std::get<0>(cycle_points[end]) - std::get<0>(cycle_points[middle]));
                unsigned long max_rocks = 1000000000000;
                unsigned long remaining = max_rocks - current_rock;
                unsigned long repetitions = remaining / (len * 5);
                return std::make_tuple(repetitions * cycle_height, current_rock + (repetitions * len * 5));
            }
        }
    }


    return std::make_tuple(0, current_rock);
}


int main() {
    std::vector<std::string> input = read_input_file();

    assert(input.size() == 1);
    int x;

    std::vector<std::string> field;
    field.push_back("+:::::::+");
    field.push_back("+:::::::+");
    field.push_back("+:::::::+");
    field.push_back("+:::::::+");
    field.push_back("+:::::::+");
    field.push_back("+:::::::+");
    // 6 layers of floor, so that no bounds checking is necessary

    std::vector<std::tuple<unsigned long, int, int>> cycle_points;

    int target_at_count = 0;
    unsigned long len_modifier = 0;
    std::string pattern = input[0];

    auto s = pattern.begin();

    for (unsigned long i = 0; i < 1000000000000; ++i) {
        target_at_count = spawn_figure((i % 5) + 1, field);
        //print_field(field);
        //std::cout << i << "\n";
        //std::cin >> x;
        do {
            if (*s == '<') {
                push_left(field, target_at_count);
            } else {
                assert(*s == '>');
                push_right(field, target_at_count);
            }
            ++s;
            if (s == pattern.end()) {
                s = pattern.begin();// wrap around if necessary
            }
        } while (fall_down(field, target_at_count));

        if (len_modifier == 0) {
            // if a cycle was detected, only the remainder needs to be simulated
            cycle_points.push_back(std::make_tuple(field.size(), (i % 5) + 1, s - pattern.begin()));
            auto [new_len, new_i] = find_cycle(field, cycle_points, i, s - pattern.begin());
            len_modifier = new_len;
            i = new_i;
        }

    }


    unsigned long score = field.size() - 6 + len_modifier;// -6 as the floor does not count

    //print_field(field);
    std::cout << "Result Score: " << score << "\n";
    return 0;
}