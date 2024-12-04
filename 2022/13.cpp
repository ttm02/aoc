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

class list_elem {

public:
    list_elem(std::string line);

    list_elem(int item, const std::vector<std::shared_ptr<list_elem>> &list);

    int is_order_correct(std::shared_ptr<list_elem> rval);

    friend auto operator<<(std::ostream &os, list_elem const &elem) -> std::ostream & {
        if (elem.item >= 0) {
            os << elem.item;
        } else {
            os << "[";
            for (auto le: elem.list) {
                os << *le << ",";
            }
            os << "]";

        }


        return os;
    }

private:
    int item;
    std::vector<std::shared_ptr<list_elem>> list;
};

list_elem::list_elem(std::string line) {

    //std::cout << line << "\n";
    item = -1;
    if (line == "[]") {
        // empty list
        item = -1;
        return;
    }

    if (line == "") {
        //empty list element
        assert (false);
        item = -2;
        return;
    }

    if (line[0] != '[') {
        // number
        item = std::stoi(line);
    } else {
        // list

        auto pos = 0;// opening '['

        while (pos < line.length() - 1) { // length -1 excludes closing ']'
            auto nxt = line.find(',', pos + 1);
            // sublist
            if (line[pos + 1] == '[') {
                int num_brackets = 1;// pos = , pos+1=[
                for (int i = pos + 2; i < line.length(); ++i) {
                    if (line[i] == '[') num_brackets++;
                    if (line[i] == ']') num_brackets--;

                    if (num_brackets == 0) {
                        assert(line[i + 1] == ',' || line[i + 1] == ']');
                        nxt = i + 1;
                        break;
                    }
                }
            }

            list.push_back(std::make_shared<list_elem>(line.substr(pos + 1, nxt - pos - 1)));

            pos = nxt;
        }
    }

}

// retrun -1 if not decided
// 0 if order is wrong
// 1 if correct
int list_elem::is_order_correct(std::shared_ptr<list_elem> rval) {

    std::cout << "compare: " << *this << "\t" << *rval << "\n";

    if (item != -1 && rval->item != -1) {
        if (item == rval->item) { return -1; }
        return item < rval->item;
    } else {
        std::shared_ptr<list_elem> lval = std::make_shared<list_elem>(*this);
        auto to_compare = rval;
        if (lval->item != -1) {
            lval = std::make_shared<list_elem>("[" + std::to_string(item) + "]");
        }
        if (rval->item != -1) {
            to_compare = std::make_shared<list_elem>("[" + std::to_string(rval->item) + "]");
        }

        auto smaller_list = std::min(lval->list.size(), to_compare->list.size());

        for (int i = 0; i < smaller_list; ++i) {
            auto result = lval->list[i]->is_order_correct(to_compare->list[i]);
            if (result != -1) return result;
        }
        // which ever list ends first is smaller
        if (lval->list.size() == to_compare->list.size())
            return -1;
        else return lval->list.size() < to_compare->list.size();


    }

}

int main() {
    std::vector<std::string> input = read_input_file();

    // all empty lines
    input.erase(
            std::remove(input.begin(), input.end(), ""),
            input.end());

    std::vector<std::shared_ptr<list_elem>> packet_list;

    std::transform(input.begin(), input.end(), std::back_inserter(packet_list),
                   [](auto line) { return std::make_shared<list_elem>(line); });

    auto devider1 = std::make_shared<list_elem>("[[2]]");
    auto devider2 = std::make_shared<list_elem>("[[6]]");


    packet_list.push_back(devider1);
    packet_list.push_back(devider2);

    std::sort(packet_list.begin(), packet_list.end(), [](auto a, auto b) {
        return a->is_order_correct(b);
    });

    int i1 = std::find(packet_list.begin(), packet_list.end(), devider1) - packet_list.begin();
    int i2 = std::find(packet_list.begin(), packet_list.end(), devider2) - packet_list.begin();

    // they start counting at 1
    int score = (i1 + 1) * (i2 + 1);
    std::cout << "Result Score: " << score << "\n";
    return 0;
}