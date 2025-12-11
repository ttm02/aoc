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


std::pair<std::vector<int>,std::vector<int>> parse_input(std::vector<std::string>& input){
    std::vector<int> a,b;
    a.reserve(input.size());
    b.reserve(input.size());

    for (const auto& s :input) {
        if (s[0]=='L') {
            a.push_back(-1);
        }
        else if (s[0]=='R') {
            a.push_back(1);
        }
        else {
            assert(false);
        }


        b.push_back(std::stoi(s.substr(1)));

        }


    return std::make_pair(a,b);

}



int main() {
    std::vector<std::string> input = read_input_file();
    auto [a,b] = parse_input(input);

    int current_pos=50;
    int number_of_0=0;

    for (int i = 0; i < a.size(); ++i) {

        std::cout << a[i] << " " << b[i] << "\n";

        int number_of_rounds= b[i] / 100;
        int to_move= a[i] * (b[i] % 100);
        if ((current_pos + to_move < 0 && current_pos!=0) || current_pos +to_move > 100) {
            number_of_rounds++;
        }

        number_of_0 += number_of_rounds;

        std::cout << number_of_rounds << "\n";
        current_pos = (100+ current_pos + to_move) % 100; // +100 to avoid negative numbers

        if (current_pos==0) {
            number_of_0++;
            std::cout << 1 << "\n";
        }
    }




    std::cout << "Result Score: " << number_of_0 << "\n";

    return 0;
}