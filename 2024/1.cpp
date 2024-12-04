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
        boost::char_separator<char> sep(", ");
        boost::tokenizer<boost::char_separator<char>> tokens(s, sep);
        bool use_a= true;
         for (const auto &t: tokens) {
            if (! t.empty()){
                if (use_a){
                    a.push_back(std::stoi(t));
                    use_a= false;
                } else {
                    b.push_back(std::stoi(t));
                }
            }
        }
    }

    return std::make_pair(a,b);

}



int main() {
    std::vector<std::string> input = read_input_file();
    auto [a,b] = parse_input(input);

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    int sum =0;

    for (int i = 0; i < a.size(); ++i) {

        int count = std::accumulate(b.begin(), b.end(),0,
                                    [a,i](auto accu,auto x){return x==a[i] ? accu +1:accu;});

        int score=a[i]*count;

        sum+=score;
    }




    std::cout << "Result Score: " << sum << "\n";

    return 0;
}