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



void draw_pt(int reg,int cycles){

    const int display_width=40;
    // get correct position of linebreak, as cycles start at 1 instead of 0
    int pos_in_line = ((cycles-1) % display_width);

    //std::cout << reg << " " << pos_in_line <<" ";

    if (std::abs(pos_in_line-reg)<=1){
        std::cout <<"#";
    }else{
        std::cout << ".";
    }
    if (pos_in_line==display_width-1)
        std::cout << "\n";

}

int main() {
    std::vector<std::string> input = read_input_file();

    int cycles = 0;
    int reg = 1;

    for (auto line: input) {

        if (line == "noop") {
            cycles++;
            draw_pt(reg,cycles);
        } else {
            long to_add = std::stoi(line.substr(4, line.length()));
            cycles++;
            draw_pt(reg,cycles);
            cycles++;
            draw_pt(reg,cycles);
            reg+=to_add;

        }
    }


    std::cout << 0 << "\n";
    return 0;
}