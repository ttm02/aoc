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
#include <tuple>

std::vector<std::string> read_input_file() {

    std::ifstream file_in("../input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(lines.size() > 0);
    return lines;
}

typedef std::tuple<int,int> Point;

int main() {
    std::vector<std::string> input = read_input_file();

    Point Start, Finish;

    int rows= input.size();
    int cols = input[0].length();

    // input is the heightmap, we only need to replace S and E
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (input[i][j]=='S'){
                Start = std::make_tuple(i,j);
                input[i][j]='a';
            }
            if (input[i][j]=='E'){
                Finish = std::make_tuple(i,j);
                input[i][j]='z';
            }
        }
    }

    // single source shortest path
    std::vector<std::vector<int> > path_costs(
            rows,
            std::vector<int>(cols,  rows*cols*2));
    // could also initialize to int_max

    path_costs[std::get<0>(Finish)][std::get<1>(Finish)] = 0;

// maximum possible path length
    for (int n = 0; n < rows*cols; ++n) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                // up
                if(i-1>=0){
                    if((input[i-1][j]-input[i][j])<=1)
                    {
                        path_costs[i][j] = std::min(path_costs[i-1][j]+1 , path_costs[i][j]);
                    }
                }
                //down
                if(i+1<rows){
                    if((input[i+1][j]-input[i][j])<=1)
                    {
                        path_costs[i][j] = std::min(path_costs[i+1][j]+1 , path_costs[i][j]);
                    }
                }
                // left
                if(j-1>=0){
                    if((input[i][j-1]-input[i][j])<=1)
                    {
                        path_costs[i][j] = std::min(path_costs[i][j-1]+1 , path_costs[i][j]);
                    }
                }
                //right
                if(j+1<cols){
                    if((input[i][j+1]-input[i][j])<=1)
                    {
                        path_costs[i][j] = std::min(path_costs[i][j+1]+1 , path_costs[i][j]);
                    }
                }

            }
        }

        /*
        for(auto v :path_costs){
            for (auto vv : v){
                std::cout << vv <<"\t";
            }

            std::cout << "\n";
        }
        std::cout << "\n";
         */

    }

    int best_score=rows*cols*2;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (input[i][j]=='a'){
                best_score=std::min(best_score,path_costs[i][j]);
            }
        }
    }

    std::cout <<best_score <<"\n";

    return 0;
}