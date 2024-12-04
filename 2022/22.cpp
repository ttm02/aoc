//
// Created by tim on 02.12.22.
//

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

std::vector<std::string> read_input_file() {

    std::ifstream file_in("../input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(lines.size() > 0);
    return lines;
}

typedef std::tuple<int, int> Point;


class DiceSide : public std::enable_shared_from_this<DiceSide> {
public:
    DiceSide(std::vector<std::string> &field, int field_size, int start_x, int start_y);


    void start_in_upper_left_corner() {
        dir = 0;
        pos_x = start_x;
        pos_y = start_y;
    };

    void set_connections(std::shared_ptr<DiceSide> up, bool invert_up, std::shared_ptr<DiceSide> left,
                                   bool invert_left, std::shared_ptr<DiceSide> down, bool invert_down,
                                   std::shared_ptr<DiceSide> right, bool invert_right);

    std::shared_ptr<DiceSide> move();

    std::tuple<int, int, int> get_position_and_dir() {
        return std::make_tuple(pos_x, pos_y, dir);
    };

    void rotate_left();

    void rotate_right();

private:
    int pos_x;
    int pos_y;
    int dir;
    int field_size;
    int start_x;
    int start_y;
    bool invert_up;
    bool invert_left;
    bool invert_right;
    bool invert_down;
    std::shared_ptr<DiceSide> up;
    std::shared_ptr<DiceSide> left;
    std::shared_ptr<DiceSide> right;
    std::shared_ptr<DiceSide> down;

    std::vector<std::string> &field;

    // true, if transfer is allowed, false if blocked
    bool transfer(const std::shared_ptr<DiceSide> &other, const int n);


};

DiceSide::DiceSide(std::vector<std::string> &field, int field_size, int start_x, int start_y) : field(field) {
    this->field_size = field_size;
    this->start_x = start_x;
    this->start_y = start_y;
    pos_x = -1;
    pos_y = -1;
    dir = -1;
    up = nullptr;
    left = nullptr;
    right = nullptr;
    down = nullptr;
    invert_up = false;
    invert_left = false;
    invert_right = false;
    invert_down = false;
}

std::shared_ptr<DiceSide> DiceSide::move() {

    assert(pos_x != -1 && pos_y != -1 && dir != -1);
    switch (dir) {
        case 3:// up
        {
            field[pos_x][pos_y] = '^';
            int new_pos_x = pos_x - 1;
            if (new_pos_x < start_x) {
                // transfer to up
                if (up->transfer(shared_from_this(), pos_y - start_y)) {
                    pos_x = -1;
                    pos_y = -1;
                    dir = -1;
                    return up;
                }// else path is blocked: nothing to do
            } else {
                // inside my space
                if (field[new_pos_x][pos_y] != '#')
                    pos_x = new_pos_x;
            }
            break;
        }
        case 1:// down
        {
            field[pos_x][pos_y] = 'v';
            int new_pos_x = pos_x + 1;
            if (new_pos_x >= start_x + field_size) {
                // transfer to other
                if (down->transfer(shared_from_this(), pos_y - start_y)) {
                    pos_x = -1;
                    pos_y = -1;
                    dir = -1;
                    return down;
                }// else path is blocked: nothing to do
            } else {
                // inside my space
                if (field[new_pos_x][pos_y] != '#')
                    pos_x = new_pos_x;
            }
            break;
        }
        case 0:// right
        {
            field[pos_x][pos_y] = '>';
            int new_pos_y = pos_y + 1;
            if (new_pos_y >= start_y + field_size) {
                // transfer to other
                if (right->transfer(shared_from_this(), pos_x - start_x)) {
                    pos_x = -1;
                    pos_y = -1;
                    dir = -1;
                    return right;
                }// else path is blocked: nothing to do
            } else {
                // inside my space
                if (field[pos_x][new_pos_y] != '#')
                    pos_y = new_pos_y;
            }
            break;
        }
        case 2:// left
        {
            field[pos_x][pos_y] = '<';
            int new_pos_y = pos_y - 1;
            if (new_pos_y < start_y) {
                // transfer to other
                if (left->transfer(shared_from_this(), pos_x - start_x)) {
                    pos_x = -1;
                    pos_y = -1;
                    dir = -1;
                    return left;
                }// else path is blocked: nothing to do
            } else {
                // inside my space
                if (field[pos_x][new_pos_y] != '#')
                    pos_y = new_pos_y;
            }
            break;
        }
        default:
            assert(false);
    }
    return shared_from_this();
}

void DiceSide::rotate_left() {
    dir = dir - 1;
    // why is dir = (dir - 1) % 4; not sufficient?
    dir = (dir == -1) ? 3 : dir;

}

void DiceSide::rotate_right() {
    dir = dir + 1;
    dir = (dir == 4) ? 0 : dir;
}

bool DiceSide::transfer(const std::shared_ptr<DiceSide> &other, const int n) {
    assert(pos_x == -1 && pos_y == -1 && dir == -1);
    if (other == up) {
        int new_y = start_y + n;
        if (invert_up)
            new_y = start_y +field_size-1 - n;
        if (field[start_x][new_y] == '#') {
            return false;
        } else {
            dir = 1;//down
            pos_x = start_x;
            pos_y = new_y;
            return true;
        }
    } else if (other == down) {
        int new_y = start_y + n;
        if (invert_down)
            new_y = start_y +field_size-1 - n;
        if (field[start_x + field_size - 1][new_y] == '#') {
            return false;
        } else {
            dir = 3;//up
            pos_x = start_x + field_size - 1;
            pos_y = new_y;
            return true;
        }
    } else if (other == left) {
        int new_x = start_x + n;
        if (invert_left)
            new_x = start_x +field_size-1 - n;
        if (field[new_x][start_y] == '#') {
            return false;
        } else {
            dir = 0;//right
            pos_x = new_x;
            pos_y = start_y;
            return true;
        }
    } else if (other == right) {
        int new_x = start_x + n;
        if (invert_right)
            new_x = start_x +field_size -1- n;
        if (field[new_x][start_y + field_size - 1] == '#') {
            return false;
        } else {
            dir = 2;//left
            pos_x = new_x;
            pos_y = start_y + field_size - 1;
            return true;
        }
    } else {
        assert(false);// unreachable
    }
}

void
DiceSide::set_connections(std::shared_ptr<DiceSide> up, bool invert_up, std::shared_ptr<DiceSide> left,
                          bool invert_left, std::shared_ptr<DiceSide> down, bool invert_down,
                          std::shared_ptr<DiceSide> right, bool invert_right) {
    this->up = std::move(up);
    this->left = std::move(left);
    this->right = std::move(right);
    this->down = std::move(down);
    this->invert_up = invert_up;
    this->invert_left = invert_left;
    this->invert_down = invert_down;
    this->invert_right = invert_right;

}

//#define USE_TEST_LAYOUT

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<std::string> field;
    std::copy(input.begin(), input.end() - 2, std::back_inserter(field));

    std::string path = input[input.size() - 1];

    const int field_height = field.size();
    const int field_width = std::accumulate(field.begin(), field.end(), 0, [](size_t accu, auto line) {
        return std::max(accu, line.length());
    });

#ifdef USE_TEST_LAYOUT
    // Why has the example map a different layout on how it folds to a cube?

    // assert all sides of the cube have same quadratic size
    assert(field_height % 3 == 0);
    assert(field_width % 4 == 0);
    assert(field_height / 3 == field_width / 4);

    const int dice_side_length = field_height / 3;

    // create_the_dice_sides
    std::vector<std::shared_ptr<DiceSide>> sides;
    sides.reserve(7);
    sides.push_back(nullptr);// just to make idexing easier
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, 0, dice_side_length * 2));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length, 0));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length, dice_side_length));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length, dice_side_length * 2));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length * 2, dice_side_length * 2));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length * 2, dice_side_length * 3));

    //make the sides connect
    sides[1]->set_connections(sides[2], true, sides[3], false, sides[4],false, sides[6], true);
    sides[2]->set_connections(sides[1], true, sides[6], true, sides[5], true, sides[3], false);
    sides[3]->set_connections(sides[1], false, sides[2], false, sides[5], true, sides[4], false);
    sides[4]->set_connections(sides[1], false, sides[3], false, sides[5], false, sides[6], true);
    sides[5]->set_connections(sides[4], false, sides[3], true, sides[2], true, sides[6], false);
    sides[6]->set_connections(sides[4], true, sides[5], false, sides[2], true, sides[1], true);
#else
    // assert all sides of the cube have same quadratic size
    assert(field_height % 4 == 0);
    assert(field_width % 3 == 0);
    assert(field_height / 4 == field_width / 3);

    const int dice_side_length = field_height / 4;

    // create_the_dice_sides
    std::vector<std::shared_ptr<DiceSide>> sides;
    sides.reserve(7);
    sides.push_back(nullptr);// just to make idexing easier
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, 0, dice_side_length ));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, 0, dice_side_length*2));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length, dice_side_length));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length*2, 0));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length * 2, dice_side_length));
    sides.push_back(std::make_shared<DiceSide>(field, dice_side_length, dice_side_length * 3, 0));

    //make the sides connect
    sides[1]->set_connections(sides[6], false,sides[4], true,sides[3], false,sides[2], false);
    sides[2]->set_connections(sides[6], false,sides[1], false,sides[3], false,sides[5], true);
    sides[3]->set_connections(sides[1], false,sides[4], false,sides[5], false,sides[2], true);
    sides[4]->set_connections(sides[3], false,sides[1], true,sides[6], false,sides[5], false);
    sides[5]->set_connections(sides[3], false,sides[4], false,sides[6], false,sides[2], true);
    sides[6]->set_connections(sides[4], false,sides[1], true,sides[2], false,sides[5], true);
#endif


    // find start pos
    sides[1]->start_in_upper_left_corner();

    int i=0;
    std::shared_ptr<DiceSide> current_side = sides[1];
    int path_pos = 0;
    int nxt_pos = std::min(path.find('L', path_pos), path.find('R', path_pos));
    while (path_pos < path.length()) {

        int movement = 0;
        if (nxt_pos - path_pos > 0)
            movement = std::stoi(path.substr(path_pos, nxt_pos - path_pos));

        std::cout << "move " << movement << "\n";

        for (int m = 0; m < movement; ++m) {
            current_side = current_side->move();
        }

        // end move: Turn
        if (nxt_pos < path.length() && path[nxt_pos] == 'L') {
            current_side->rotate_left();
        }
        if (nxt_pos < path.length() && path[nxt_pos] == 'R') {
            current_side->rotate_right();
        }
        path_pos = nxt_pos + 1;
        nxt_pos = std::min(path.find('L', path_pos), path.find('R', path_pos));
        if (nxt_pos == -1)
            nxt_pos = path.length();

        // output status
        std::ofstream output("output_"+std::to_string(i)+".txt", std::ios_base::out);
        for (
            auto l
                : field) {
            output << l << "\n";
        }
        ++i;
    }

    auto [pos_i, pos_j, dir] = current_side->get_position_and_dir();
    std::cout << "Position: " << pos_i << " " << pos_j << " Dir" << dir << "\n";

    std::ofstream output("output.txt", std::ios_base::out);
    for (
        auto l
            : field) {
        output << l << "\n";
    }

    // +1 as counting is started at 1
    auto score = 1000 * (pos_i + 1) + 4 * (pos_j + 1) + dir;

    std::cout << "Result Score: " << score << "\n";

    return 0;
}