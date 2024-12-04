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


bool all_different(std::string s, int i, int n){

    std::set<char> set;

    for (int j = 0; j < n; ++j) {
        char c = s[i-j];

        if(set.find(c)!=set.end()){
            return false;
        }else{
            set.insert(c);
        }

    }

    return true;
}

int main() {
    std::vector<std::string> input = read_input_file();
    auto s = *input.begin();

    std::cout <<  s <<"\n";

    for (int i = 14; i < s.length(); ++i) {
        if(all_different(s,i,14)){
            std::cout << i+1  << "\n";
            break;
        }

    }




    std::cout <<"\n";

    return 0;
}