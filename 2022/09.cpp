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

typedef std::tuple<int, int> point;


point follow(point head, point tail) {
    if (( std::abs(std::get<0>(tail) - std::get<0>(head) )==2) && ( std::abs(std::get<1>(tail) - std::get<1>(head) )==2)) {
        // head moved diagonal: we also need to move diagonal
        int new_x= std::get<0>(tail) > std::get<0>(head) ? std::get<0>(head)+1: std::get<0>(head)-1;
        int new_y= std::get<1>(tail) > std::get<1>(head) ? std::get<1>(head)+1: std::get<1>(head)-1;
        return std::make_tuple(new_x,new_y);
    }
    else if (std::get<1>(tail) == std::get<1>(head) - 2) {
// in these cases it doesn't matter where the tail is exactly
// as wether it moves diagonal or not is not important
        return std::make_tuple(std::get<0>(head), std::get<1>(head) - 1);
    } else if (std::get<1>(tail) == std::get<1>(head) + 2) {

        return std::make_tuple(std::get<0>(head), std::get<1>(head) + 1);

    } else if (std::get<0>(tail) == std::get<0>(head) + 2) {

        return std::make_tuple(std::get<0>(head) + 1, std::get<1>(head));

    } else if (std::get<0>(tail) == std::get<0>(head) - 2) {

        return std::make_tuple(std::get<0>(head) - 1, std::get<1>(head));

    }

    // no need to move tail
    return tail;

}



void print_rope(point head,std::vector<point>tails){
const int FIELD_SIZE=6;

std::vector<std::vector<char>> output(FIELD_SIZE,std::vector<char>(FIELD_SIZE,'.'));

int i=tails.size();
auto it = tails.end()-1;

while (it >=tails.begin())
{
    output[std::get<0>(*it)][std::get<1>(*it)]='0'+i;
    --it;
    --i;
}

    output[std::get<0>(head)][std::get<1>(head)]='H';

    for (auto l :output) {
        for (auto c: l) {
            std::cout << c;
        }
        std::cout << "\n";
    }
    std::cout << "\n";


};

int main() {
    std::vector<std::string> input = read_input_file();

    const int ROPE_LENGTH = 9;

    std::set<point> locations_visited;

    point head = std::make_tuple(0, 0);
    std::vector<point> tails;
    for (int i = 0; i < ROPE_LENGTH; ++i) {
        tails.push_back(std::make_tuple(0, 0));
    }
    locations_visited.insert(head);// start position

    for (auto line: input) {

        char direction = line[0];
        int num_moves = std::stoi(line.substr(2, line.length()));
        for (int i = 0; i < num_moves; ++i) {
            // move head
            switch (direction) {
                case 'R':
                    head = std::make_tuple(std::get<0>(head), std::get<1>(head) + 1);
                    break;
                case 'L':
                    head = std::make_tuple(std::get<0>(head), std::get<1>(head) - 1);
                    break;
                case 'D':
                    head = std::make_tuple(std::get<0>(head) - 1, std::get<1>(head));
                    break;
                case 'U':
                    head = std::make_tuple(std::get<0>(head) + 1, std::get<1>(head));
                    break;
            }
            // tails must follow
            tails[0] = follow(head, tails[0]);
            //std::cout << std::get<0>(head)<<" "<<std::get<1>(head)<<"\n";
            //std::cout << std::get<0>(tails[0])<<" "<<std::get<1>(tails[0])<<"\n";
            for (int j = 1; j < ROPE_LENGTH; ++j) {
                tails[j]= follow(tails[j-1], tails[j]);
                //std::cout << std::get<0>(tails[j])<<" "<<std::get<1>(tails[j])<<"\n";
            }
            locations_visited.insert(tails[ROPE_LENGTH-1]);
            //std::cout << "\n";

            //print_rope(head,tails);
        }

    }


    std::cout << locations_visited.size() << "\n";
    return 0;
}