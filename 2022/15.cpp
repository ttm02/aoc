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


std::tuple<long, long, long, long> parse_line(std::string line) {

    int pos = 0;
    int equal_sign = line.find('=', pos);
    pos = line.find(',', pos);
    long sx = std::stoi(line.substr(equal_sign + 1, pos - equal_sign));
    equal_sign = line.find('=', pos);
    pos = line.find(':', pos);
    long sy = std::stoi(line.substr(equal_sign + 1, pos - equal_sign));

    equal_sign = line.find('=', pos);
    pos = line.find(',', pos);
    long bx = std::stoi(line.substr(equal_sign + 1, pos - equal_sign));
    equal_sign = line.find('=', pos);
    pos = line.find(':', pos);
    long by = std::stoi(line.substr(equal_sign + 1, pos - equal_sign));

    return std::make_tuple(sx, sy, bx, by);

}

long distance(long ax, long ay, long bx, long by) {

    return std::abs(ax - bx) + std::abs(ay - by);
}

long distance(std::tuple<long, long, long, long> tup) {
    return distance(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup), std::get<3>(tup));
}


int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<std::tuple<long, long, long, long>> coordinates;
    std::transform(input.begin(), input.end(), std::back_inserter(coordinates), parse_line);


    const long field_size = 2 * std::accumulate(coordinates.begin(), coordinates.end(), 0, [](long accu, auto tup) {

        return std::max(accu, std::max(std::get<0>(tup),
                                       std::max(std::get<1>(tup), std::max(std::get<2>(tup), std::get<3>(tup)))));

    });

    std::cout << field_size << "\n";

    //there is the possibility of negative coordinates, therefore the "actual" coordinates are x = x+(field_size/4)

    int score = 0;

    long i = 0;
    long j = 0;

    const long max_coord=1+ 4000000;
    while (true) {

        bool found = false;
        for (auto sensor: coordinates) {
            // search radius can be precomputed
            int scan_dist = distance(sensor);

            long my_dist = distance(i, j, std::get<0>(sensor), std::get<1>(sensor));
            if (my_dist <= scan_dist) {

                found = true;
                // calculate next point that is out of radius
                j += scan_dist - my_dist + 1;
                if (j > max_coord) {
                    i++;
                    j = 0;
                }

                break;
            }
        }
        if (!found) {
            std::cout << "magnitude: " << i * 4000000 + j << "\n";
            return 0;
        }

    }



    //std::cout << "Result Score: " <<score << "\n";
    return 0;
}