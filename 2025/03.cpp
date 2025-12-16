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
    //auto ranges = parse_input(input);

    long score = 0;


    for (auto s: input) {
        std::string concat = "";
        auto d = s.begin();
        for (int digits_left = 11; digits_left >= 0; --digits_left) {
            d = std::max_element(d, s.end() - digits_left, [](char a, char b) { return a < b; });

            concat += *d;
            ++d;
        }
        score += std::stol(concat);
        std::cout << concat << std::endl;
    }


    std::cout << "Result Score: " << score << "\n";

    return 0;
}
