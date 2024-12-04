//
// Created by tim on 02.12.22.
//

#include <iostream>

#include <fstream>
#include <vector>
#include <iterator>

#include <algorithm>
#include <numeric>
#include <cassert>

std::vector<std::string> read_input_file() {

    std::ifstream file_in("../input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(lines.size()>0);
    return lines;
}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<std::vector<int>> elves;
    std::vector<int> current_elve;

    for (auto line: input) {

        if (line.empty()) {
            std::vector<int> v(current_elve);
            elves.push_back(v);
            current_elve.clear();
        } else {
            current_elve.push_back(std::stoi(line));
        }

    }

    std::vector<int> elves_total;

    std::transform(elves.begin(), elves.end(), std::back_inserter(elves_total),
                   [](auto x) { return std::accumulate(x.begin(),x.end(),
                           0); });
    assert(elves_total.size()>0);

    std::partial_sort(elves_total.begin(),elves_total.begin()+3,elves_total.end(),std::greater<>());

    int result = elves_total[0]+elves_total[1]+elves_total[2];
    std::cout <<  result << "\n";

    return 0;
}