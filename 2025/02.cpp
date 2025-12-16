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

std::pair<long, long> parse_range(std::string &s, int start, int minus, int end) {
    long a = std::stol(s.substr(start + 1, minus));
    long b = std::stol(s.substr(minus + 1, end));
    std::cout << a << " " << b << "\n";
    return {a, b};
}

std::vector<std::pair<long, long> > parse_input(std::vector<std::string> &input) {
    std::vector<std::pair<long, long> > result;

    auto s = input[0];

    //for (const auto& s :input) {
    //  }

    int prev_comma = -1;
    int comma = -1;
    int minus = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == ',') {
            prev_comma = comma;
            comma = i;
            // PARSE
            result.push_back(parse_range(s, prev_comma, minus, i));
        }
        if (s[i] == '-') {
            minus = i;
        }
    }
    prev_comma = comma;
    result.push_back(parse_range(s, prev_comma, minus, s.size()));

    return result;
}

bool is_id_valid(long id) {
    auto s = std::to_string(id);

    for (int rep = 2; rep <= s.size(); rep++) {
        if (s.size() % rep == 0) {
            auto s_prefix = s.substr(0, s.size() / rep);
            bool is_repetition = true;
            for (int i = 1; i < rep; ++i) {
                auto next_part = s.substr(i * (s.size() / rep), s.size() / rep);
                if (s_prefix == s.substr(i * (s.size() / rep), s.size() / rep)) {
                    is_repetition = is_repetition && true;
                } else {
                    is_repetition = is_repetition && false;
                }
            }
            if (is_repetition) {
                std::cout << s << "\n";
                return false;
            }
        }
    }

    return true;
}


int main() {
    std::vector<std::string> input = read_input_file();
    auto ranges = parse_input(input);

    long score = 0;

    for (auto &[a, b]: ranges) {
        for (long j = a; j <= b; ++j) {
            if (not is_id_valid(j)) {
                score += j;
            }
        }
    }


    std::cout << "Result Score: " << score << "\n";

    return 0;
}
