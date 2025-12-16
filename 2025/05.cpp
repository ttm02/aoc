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

std::pair<long, long> parse_range(const std::string &s) {
    auto pos = s.find('-');
    long first = std::stol(s.substr(0, pos));
    long second = std::stol(s.substr(pos + 1));

    return {first, second};
}


int main() {
    std::vector<std::string> input = read_input_file();
    auto empty_line = std::find(input.begin(), input.end(), "");

    std::vector<std::pair<long, long> > ranges;

    for (auto s = input.begin(); s != empty_line; ++s) {
        ranges.push_back(parse_range(*s));
    }

    std::vector<std::pair<long, long> > merged_ranges;
    std::sort(ranges.begin(), ranges.end());
    merged_ranges.push_back(ranges[0]);


    for (size_t i = 1; i < ranges.size(); ++i) {
        auto &[cur_start, cur_end] = ranges[i];
        auto &[last_start, last_end] = merged_ranges.back();

        if (cur_start <= last_end) {
            // Overlapping or touching → merge
            last_end = std::max(last_end, cur_end);
        } else {
            // No overlap → add new range
            merged_ranges.emplace_back(cur_start, cur_end);
        }
    }


    long score = 0;
    for (auto [a,b]: merged_ranges) {
        score += b - a + 1;
        std::cout << a << " " << b << " " << b - a + 1 << std::endl;
    }


    std::cout << "Result Score: " << score << "\n";

    return 0;
}
