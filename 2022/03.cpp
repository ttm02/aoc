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
    assert(lines.size()>0);
    return lines;
}

int get_prio(char c){

    if (std::islower(c))
        return c-'a'+1;
        else
    return c-'A'+26+1;
}

int get_score(std::string a, std::string b,std::string c){



    std::set<char> in_1;
    std::copy(a.begin(),a.end(),std::inserter(in_1,in_1.begin()));

    std::set<char> in_2;
    std::copy(b.begin(),b.end(),std::inserter(in_2,in_2.begin()));
    std::set<char> in_3;
    std::copy(c.begin(),c.end(),std::inserter(in_3,in_3.begin()));

    std::set<char>  in_both;
    std::set_intersection(in_1.begin(),in_1.end(),in_2.begin(),in_2.end(),std::inserter(in_both,in_both.begin()));

    std::set<char>  in_all;
    std::set_intersection(in_3.begin(),in_3.end(),in_both.begin(),in_both.end(),std::inserter(in_all,in_all.begin()));

    assert(in_all.size()==1);

    return get_prio(*in_all.begin());
}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<int> score;


    for (int i = 0; i < input.size(); i+=3) {
        score.push_back(get_score(input[i+0],input[i+1],input[i+2]));
    }
  //  std::transform(input.begin(), input.end(), std::back_inserter(score), get_score);

    std::accumulate(score.begin(),score.end(),0);

    int result = std::accumulate(score.begin(),score.end(),0);
    std::cout <<  result << "\n";

    return 0;
}