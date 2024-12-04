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

int get_score_for_round(std::string s){
    assert(s.length()==3);

    const char opponent = s[0];
    const char you = s[2];

    int score=0;


    if (you=='X' && opponent=='A') score+=0+3;
    if (you=='Y' && opponent=='B') score+=3+2;
    if (you=='Z' && opponent=='C') score+=6+1;

    if (you=='Z' && opponent=='A') score+=6+2;
    if (you=='X' && opponent=='B') score+=0+1;
    if (you=='Y' && opponent=='C') score+=3+3;

    if (you=='Y' && opponent=='A') score+=3+1;
    if (you=='Z' && opponent=='B') score+=6+3;
    if (you=='X' && opponent=='C') score+=0+2;



    return score;
}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<int> score;

    std::transform(input.begin(), input.end(), std::back_inserter(score),
                   get_score_for_round);

    std::accumulate(score.begin(),score.end(),0);

    int result = std::accumulate(score.begin(),score.end(),0);
    std::cout <<  result << "\n";

    return 0;
}