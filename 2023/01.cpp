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

std::vector<std::string> strings_to_look_for = {
        "0",
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
        "1", "2", "3", "4", "5", "6", "7", "8", "9"
};


int main() {
    std::vector<std::string> input = read_input_file();

    int sum = 0;

    for (auto s: input) {

        size_t pos_1 = s.length();
        size_t pos_2 = 0;
        for (const auto &to_find: strings_to_look_for) {
            auto p1 = s.find(to_find);
            auto p2 = s.rfind(to_find);
            if (p1 != std::string::npos) {
                pos_1 = std::min(pos_1, p1);
            }
            if (p2 != std::string::npos) {
                pos_2 = std::max(pos_2, p2);
            }
        }

        int num_1 = 0;
        if (isdigit(s[pos_1])) {
            num_1 = s[pos_1] - '0';
        } else {
            while (true) {
                num_1++;
                if (pos_1 == s.find(strings_to_look_for[num_1])) {
                    break;
                }
            }
        }

        int num_2 = 0;
        if (isdigit(s[pos_2])) {
            num_2 = s[pos_2] - '0';
        } else {
            while (true) {
                num_2++;
                if (pos_2 == s.rfind(strings_to_look_for[num_2])) {
                    break;
                }
            }
        }

        int num = num_1 * 10 + num_2;
        std::cout << num << "\n";

        sum = sum + num;

    }


    std::cout << "Result Score: " << sum << "\n";

    return 0;
}