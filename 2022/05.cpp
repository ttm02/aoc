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

std::vector<std::string> read_input_file() {

    std::ifstream file_in("../input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(lines.size() > 0);
    return lines;
}

int get_prio(char c) {

    if (std::islower(c))
        return c - 'a' + 1;
    else
        return c - 'A' + 26 + 1;
}


int get_score(std::string s) {

    int middle = s.find(',');

    int a, b, c, d;

    int left = s.find('-');
    int right = s.find('-', middle + 1);
    a = std::stoi(s.substr(0, left));
    b = std::stoi(s.substr(left + 1, middle));
    c = std::stoi(s.substr(middle + 1, right));
    d = std::stoi(s.substr(right + 1, s.length()));




    if (std::max(a,c) <= std::min(b,d))
        return 1;


    return 0;
}

template<class It> std::vector<std::vector<char>> parse_top_part(It begin,It end){
    std::vector<std::vector<char>> result;

    It pos=end-1;
    std::string s = *pos;

    // read num of buckets
    int in_str =s.find_last_of(' ',s.size()-2);
    int buckets = std::stoi(s.substr(in_str,s.length()));

    result.resize(buckets);


    // skip one line
    for (pos=end-1-1;pos >=begin;pos--) {
        s = *pos;
        for (int i = 0; i < buckets; ++i) {
            //[A]
            char c = s[i*4+1];
            if (c != ' ')
            result[i].push_back(c);
        }

    }

    return result;
}

int main() {
    std::vector<std::string> input = read_input_file();

    auto delim = std::find(input.begin(),input.end(),"");

    auto stacks= parse_top_part(input.begin(),delim);

     for (auto pos = delim+1; pos < input.end(); ++pos) {
        std::string line = *pos;
        // split is at space into vec
        std::stringstream ss(line);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> movement(begin, end);

        int num = std::stoi(movement[1]);
        // -1 as input does not start counting at 0
        int from = std::stoi(movement[3])-1;
        int to = std::stoi(movement[5])-1;

        assert(stacks[from].end()-num >= stacks[from].begin());
        std::move(stacks[from].end()-num,stacks[from].end(),std::back_inserter(stacks[to]));
        for (int i = 0; i < num; ++i) {
            stacks[from].pop_back();
        }
    }

    for (auto stack:stacks) {
        std::cout << *(stack.end()-1);

    }
    std::cout <<"\n";

    return 0;
}