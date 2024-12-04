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

int calculate_scenic_score(int i, int j, int max_i, int max_j, std::vector<std::string> input){

    int c1=0,c2=0,c3=0,c4 =0;
    // below
    int ii=i;
    char c = input[i][j];
    int cur_height = c-'0';
    for (int jj = j+1; jj < max_j; ++jj) {
        char c = input[ii][jj];
        int num = c-'0';
        c1++;
        if (num >= cur_height){
            break;
        }

    }
    // above
    for (int jj = j-1; jj>=0; --jj) {
        char c = input[ii][jj];
        int num = c-'0';
        c2++;
        if (num >= cur_height){
            break;
        }

    }
    int jj =j;
    // right
    for (int ii = i+1; ii < max_i; ++ii) {
        char c = input[ii][jj];
        int num = c-'0';
        c3++;
        if (num >= cur_height){
            break;
        }

    }
    // left
    for (int ii = i-1; ii>=0; --ii) {
        char c = input[ii][jj];
        int num = c-'0';
            c4++;
        if (num >= cur_height){
            break;
        }
    }

    return c1*c2*c3*c4;
}

int main() {
    std::vector<std::string> input = read_input_file();

    auto max_i= input[0].length();
    auto max_j = input.size();

    int score=0;
    for (int i = 0; i < max_i; ++i) {

        for (int j = 0; j < max_j; ++j) {
            int new_score = calculate_scenic_score(i,j,max_i,max_j,input);
            std::cout << new_score << " ";
            score = std::max(
                    score,
                    new_score
                    );
        }
std::cout <<"\n";
    }


std::cout << score <<"\n";
    return 0;
}