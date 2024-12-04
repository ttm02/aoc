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

typedef unsigned long long ITEM_TYPE;
class Monkey{

public:
    Monkey(std::vector<std::string>::iterator& input_position);

    void add_connections(std::vector<std::shared_ptr<Monkey>>all_monkeys);

    void make_turn();

    int getActivityCount() const {
        return activity_count;
    }

    friend auto operator<<(std::ostream& os, Monkey const& m) -> std::ostream& {
        os << "Monkey "<<m.ID<<": ";

        for (auto v:m.item_values) {
            os << v<<", ";
        }

        return os;
    }
private:
    void catch_item(ITEM_TYPE item_value);

    std::vector<ITEM_TYPE> item_values;
    int ID;
    ITEM_TYPE activity_count;
    long operation_value;
    bool operation_is_addition;
    bool op_value_is_item;
    int test_devisor;
    ITEM_TYPE modulo_value;
public:
    void setModuloValue(ITEM_TYPE moduloValue);

public:
    int getTestDevisor() const;

private:
    int id_true;
    int id_false;
    std::shared_ptr<Monkey> throw_true;
    std::shared_ptr<Monkey> throw_false;
};



Monkey::Monkey(std::vector<std::string>::iterator& input_position) {

    auto line = *input_position;

    std::cout << line << "\n";

    ID= std::stoi(line.substr(6,line.length()-1));

    // starting items
    input_position++;
    line =*input_position;
    int delim_pos= line.find(":");
    std::string tmp;
    std::stringstream ss(line.substr(delim_pos+1,line.length()));
    while(getline(ss, tmp, ',')){
        item_values.push_back(std::stoi(tmp));
    }

    // operation
    input_position++;
    line =*input_position;
    delim_pos= line.find(":") +std::string(": new = old ").length();

    operation_is_addition = line[delim_pos]=='+';
    auto op_val_s = line.substr(delim_pos+2,line.length());
    if (op_val_s=="old"){
        op_value_is_item= true;
        // neutral element of addition or multiplication
        operation_value=!operation_is_addition;
    }
    else{
        op_value_is_item=false;
        operation_value = std::stoi(op_val_s);}

    // test
    input_position++;
    line =*input_position;
    delim_pos= line.find(":") +std::string(": divisible by ").length();
    test_devisor = std::stoi(line.substr(delim_pos,line.length()));

    // true ID
    input_position++;
    line =*input_position;
    delim_pos= line.find(":") +std::string(": throw to monkey ").length();
    id_true = std::stoi(line.substr(delim_pos,line.length()));
    // false ID
    input_position++;
    line =*input_position;
    delim_pos= line.find(":") +std::string(": throw to monkey ").length();
    id_false = std::stoi(line.substr(delim_pos,line.length()));


    throw_false=nullptr;
    throw_true=nullptr;
    modulo_value=std::numeric_limits<ITEM_TYPE>::max();

    std::cout << ID <<" "<< operation_is_addition<<" "<<operation_value<<" "<<test_devisor<<" "<<id_true<<" "<<id_false<<" "<< "\n";

    input_position++;
    activity_count=0;
}

void Monkey::add_connections(std::vector<std::shared_ptr<Monkey>> all_monkeys) {

    for (auto m : all_monkeys)
    {if (m->ID == id_true) throw_true=m;
        if (m->ID == id_false) throw_false=m;
    }
    assert(throw_true!= nullptr && throw_false!= nullptr);
}

void Monkey::make_turn() {
    for (auto item: item_values) {
        activity_count++;
        ITEM_TYPE result_value=0;
        if(operation_is_addition){
            if(op_value_is_item)
                result_value = item+item;
            else
                result_value = item+operation_value;
        }
        else {
            if(op_value_is_item)
                result_value = item*item;
            else
                result_value = item*operation_value;

        }
        // only for fists part:
        //result_value = result_value / 3;
        // for second part:
        result_value= result_value % modulo_value;

        if (result_value<0){
            std::cout << ID <<" Throws negative Item " << result_value << " before: "<<item<< "\n";
        }
        if (result_value % test_devisor ==0){
            throw_true->catch_item(result_value);
        }else{
            throw_false->catch_item(result_value);
        }


    }

    item_values.clear();

}

void Monkey::catch_item(ITEM_TYPE  item_value) {item_values.push_back(item_value);}

int Monkey::getTestDevisor() const {
    return test_devisor;
}

void Monkey::setModuloValue(ITEM_TYPE moduloValue) {
    modulo_value = moduloValue;
}

int main() {
    std::vector<std::string> input = read_input_file();

    const int ROUNDS=10000;

    std::vector<std::shared_ptr<Monkey>> monkeys;

    auto iter = input.begin();
    while (iter < input.end()){
        monkeys.push_back(std::make_shared<Monkey>(iter));
        iter++;// skip empty line
    }


    for (auto m : monkeys){
        m->add_connections(monkeys);
    }

    int max_modulo_op=1;
    for (auto m : monkeys){
        max_modulo_op= max_modulo_op *m->getTestDevisor();
    }
    // this gets a number which includes all prime-factors relevant in all monkeys
    // maybe a smaller number is possible, but the only important thing is,
    // that we find any valid number with all prime factors necessary for the monkey tests smaller than 64 bit
    for (auto m : monkeys){
       m->setModuloValue(max_modulo_op);
    }

    for (int i = 0; i < ROUNDS; ++i) {
        for (auto m : monkeys){
            m->make_turn();
        }
        std::cout << "\n";
        for (auto m : monkeys){
            std::cout <<*m<< "\n";
        }
        std::cout << "\n";
    }

    for (auto m : monkeys){
        std::cout << m->getActivityCount() <<"\n";
    }

    std::vector<ITEM_TYPE> scores;
    std::transform(monkeys.begin(), monkeys.end(), std::back_inserter(scores), [](auto m){
        return m->getActivityCount();
    });

    std::partial_sort(scores.begin(),scores.begin()+2,scores.end(),std::greater<int>());

    std::cout << "Result Score: " <<scores[0]*scores[1] << "\n";
    return 0;
}