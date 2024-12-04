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

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<int> score;


    //  for (int i = 0; i < input.size(); i+=3) {
    //      score.push_back(get_score(input[i+0],input[i+1],input[i+2]));
    //  }
    std::transform(input.begin(), input.end(), std::back_inserter(score), get_score);

    std::accumulate(score.begin(), score.end(), 0);

    int result = std::accumulate(score.begin(), score.end(), 0);
    std::cout << result << "\n";

    return 0;
}