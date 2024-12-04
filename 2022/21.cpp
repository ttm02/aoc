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


typedef long long int value_type;

class Monkey {
public:
    Monkey(std::string input);


    void set_connections(std::vector<std::shared_ptr<Monkey>> others);

    value_type get_value();

    value_type calculate_value_to_get(value_type target_value);

private:
public:
    const std::string &getIdS() const {
        return ID_s;
    }

private:
    std::string ID_s;
    std::string left_name;
    std::string right_name;
    char op;
    std::shared_ptr<Monkey> left;
    std::shared_ptr<Monkey> right;
    value_type value;
};

Monkey::Monkey(std::string input) {

    auto delim = input.find(':');
    assert(delim == 4);

    ID_s = input.substr(0, delim);
    right_name = "";
    left_name = "";
    left = nullptr;
    right = nullptr;
    op = '=';

    try {
        // number
        value = std::stol(input.substr(delim + 1, input.size() - delim - 1));
    }
    catch (std::invalid_argument &e) {
        // operation
        std::stringstream iss(input);
        std::string word;

        iss >> word;// name of monkey
        iss >> left_name;
        iss >> word; // operator
        assert(word.length() == 1);
        op = word[0];
        iss >> right_name;

    }

}

value_type Monkey::calculate_value_to_get(value_type target_value) {

    value_type result = 0;
    if (ID_s == "humn") {
        return target_value;
    } else if (ID_s == "root") {
        try {
            result = left->get_value();
            std::cout << "Target: " << result << "\n";
            return right->calculate_value_to_get(result);
        }
        catch (std::domain_error &e) {
            result = right->get_value();
            std::cout << "Target: " << result << "\n";
            return left->calculate_value_to_get(result);
        }
    } else {
        value_type known_val = 0;
        bool is_known_right = true;
        try {
            known_val = right->get_value();
        } catch (std::domain_error &e) {
            known_val = left->get_value();
            is_known_right = false;
        }
        switch (op) {
            case '+':
                // commutative
                result = target_value - known_val;
                assert(result + known_val == target_value);
                break;
            case '-':
                // X=a-b --> x+b=a
                // x=b-a --> b-x=a
                if (is_known_right) {
                    result = target_value + known_val;
                    assert(result - known_val == target_value);
                } else {
                    result = known_val - target_value;
                    assert(known_val - result == target_value);
                }
                break;
            case '*':
                // commutative
                //assert(target_value % known_val == 0);
                result = target_value / known_val;
                assert(result * known_val == target_value);
                break;
            case '/':
                //X= A / B-->x * B=A
                //X=B / A --> B/x=A
                if (is_known_right) {
                    result = target_value * known_val;
                    assert(result / known_val == target_value);
                } else {
                    result = known_val / target_value;
                    assert(known_val / result == target_value);
                }
                break;
            default:
                assert(false);
        }

        if (is_known_right) {
            return left->calculate_value_to_get(result);

        } else {
            return right->calculate_value_to_get(result);
        }

    }
}


void Monkey::set_connections(std::vector<std::shared_ptr<Monkey>> others) {

    if (right_name != "") {

        for (auto m: others) {
            if (left_name == m->ID_s) {
                left = m;
            }
            if (right_name == m->ID_s) {
                right = m;
            }
        }
    }
}

value_type Monkey::get_value() {

    if (ID_s == "humn")
        throw std::domain_error("Value to be calculated");

    if (op == '=')
        return value;

    else {
        auto val_left = left->get_value();
        auto val_right = right->get_value();
        value_type result = 0;
        switch (op) {
            case '+':
                result = val_left + val_right;
                assert(result > val_left);
                break;
            case '-':
                result = val_left - val_right;
                assert(result < val_left);
                break;
            case '*':
                result = val_left * val_right;
                break;
            case '/':
                result = val_left / val_right;
                break;
            default:
                assert(false);
        }
        // save result
        value = result;
        op = '=';
        return result;
    }

}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<std::shared_ptr<Monkey>> monkeys;
    std::transform(input.begin(), input.end(), std::back_inserter(monkeys),
                   [](auto s) { return std::make_shared<Monkey>(s); });

    for (auto m: monkeys) {
        m->set_connections(monkeys);
    }

    auto root_pos = std::find_if(monkeys.begin(), monkeys.end(), [](auto m) {
        return m->getIdS() == "root";
    });
    assert(root_pos != monkeys.end());
    auto root = *root_pos;

    auto humn_pos = std::find_if(monkeys.begin(), monkeys.end(), [](auto m) {
        return m->getIdS() == "humn";
    });
    assert(humn_pos != monkeys.end());
    auto humn = *humn_pos;

    auto score = root->calculate_value_to_get(1);

    std::cout << "Result Score: " << score << "\n";

    return 0;
}