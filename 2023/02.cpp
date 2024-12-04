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


class Game {
public:
    Game(const std::string &line) {
        assert(line.rfind("Game", 0) == 0);

        int current_pos = 5;
        int end_pos = 5;

        while (isdigit(line[end_pos])) {
            end_pos++;
        }
        assert(line[end_pos] == ':');

        id = std::stoi(line.substr(current_pos, end_pos - 1));

        current_pos = end_pos + 2;// +11 for : +1 for space

        while (current_pos < line.size()) {


            int r = 0, g = 0, b = 0;

            while (true) {
                end_pos = current_pos;
                while (isdigit(line[end_pos])) {
                    end_pos++;
                }
                assert(line[end_pos] == ' ');
                int num = std::stoi(line.substr(current_pos, end_pos - 1));
                current_pos = end_pos + 1;

                bool valid_color = false;
                if (line.rfind("red", current_pos) == current_pos) {
                    r = num;
                    valid_color = true;
                    current_pos = current_pos + 3;
                } else if (line.rfind("green", current_pos) == current_pos) {
                    g = num;
                    valid_color = true;
                    current_pos = current_pos + 5;
                } else if (line.rfind("blue", current_pos) == current_pos) {
                    b = num;
                    valid_color = true;
                    current_pos = current_pos + 4;
                }
                assert(valid_color);
                if (line[current_pos] == ';' | current_pos == line.size()) {
                    drawings.push_back(std::make_tuple(r, g, b));
                    current_pos = current_pos + 2;
                    break;
                } else {
                    assert(line[current_pos] == ',');
                    current_pos = current_pos + 2;
                }
            }
        }
    }

    int id;

    std::vector<std::tuple<int, int, int>> drawings;

    int get_power() {

        int r = 0, g = 0, b = 0;
        for (auto draw: drawings) {

            r = std::max(std::get<0>(draw), r);
            g = std::max(std::get<1>(draw), g);
            b = std::max(std::get<2>(draw), b);

        }

        return r * g * b;
    }

};


int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<std::shared_ptr<Game>> games_list;

    for (auto &s: input) {
        games_list.push_back(std::make_shared<Game>(s));
    }


    int sum = 0;
    for (auto &g: games_list) {
        sum = sum + g->get_power();
    }


    std::cout << "Result Score: " << sum << "\n";

    return 0;
}