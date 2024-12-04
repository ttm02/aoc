//
// Created by tim on 02.12.22.
//

#include <iostream>

#include <sstream>
#include <fstream>
#include <vector>
#include <list>
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

// double linked list, need to self implement, as the original order does matter
struct list_elem {
    list_elem *prev;
    list_elem *nxt;
    long value;
};

void print_list(list_elem *head, unsigned long len) {
    auto elem = head;
    for (int i = 0; i < len + 1; ++i) {
        std::cout << elem->value << ", ";
        elem = elem->nxt;
    }
    std::cout << "\n";
}

void refreshOuterPointers(list_elem *A) {
    if (A->prev != NULL)
        A->prev->nxt = A;

    if (A->nxt != NULL)
        A->nxt->prev = A;
}

void swap(list_elem *A, list_elem *B) {
    list_elem *swapperVector[4];
    list_elem *temp;

    if (A == B) return;

    if (B->nxt == A) {
        temp = A;
        A = B;
        B = temp;
    }

    swapperVector[0] = A->prev;
    swapperVector[1] = B->prev;
    swapperVector[2] = A->nxt;
    swapperVector[3] = B->nxt;

    if (true) { //areTheyNeighbours(A,B)
        A->prev = swapperVector[2];
        B->prev = swapperVector[0];
        A->nxt = swapperVector[3];
        B->nxt = swapperVector[1];
    } else {
        A->prev = swapperVector[1];
        B->prev = swapperVector[0];
        A->nxt = swapperVector[3];
        B->nxt = swapperVector[2];
    }


    refreshOuterPointers(A);
    refreshOuterPointers(B);
}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<list_elem *> list_vec;


    std::transform(input.begin(), input.end(), std::back_inserter(list_vec), [](auto s) {
        long val = std::stol(s) * 811589153;
        list_elem *elem = (list_elem *) malloc(sizeof(list_elem));
        elem->value = val;
        elem->nxt = NULL;
        elem->prev = NULL;
        return elem;
    });

    //const int decrypt_key=811589153;


    // build connections
    for (int i = 0; i < list_vec.size(); ++i) {
        auto elem = list_vec[i];
        int prev_ind = i == 0 ? list_vec.size() - 1 : i - 1;
        int nxt_ind = i == list_vec.size() - 1 ? 0 : i + 1;
        elem->nxt = list_vec[nxt_ind];
        elem->prev = list_vec[prev_ind];
    }

    auto zero = *std::find_if(list_vec.begin(), list_vec.end(), [](auto elem) {
        return elem->value == 0;
    });

    print_list(zero, list_vec.size());

    for (int n = 0; n < 10; ++n) {
        for (int i = 0; i < list_vec.size(); ++i) {
            auto elem = list_vec[i];
            long val = elem->value;
            // module list size -1, as if i move -1 fields, it will have the same order
            auto num_mix = std::abs(val) % (list_vec.size() - 1);
            if (val < 0) {
                // forward
                for (int j = 0; j < num_mix; ++j) {
                    swap(elem->prev, elem);
                }
            } else {
                // backward
                for (int j = 0; j < num_mix; ++j) {
                    swap(elem, elem->nxt);
                }
            }

        }
        print_list(zero, list_vec.size());

    }

    long score = 0;
    auto elem = zero;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1000; ++j) {
            elem = elem->nxt;
        }
        std::cout << elem->value << "\n";
        score += elem->value;
    }


    std::cout << "Result Score: " << score << "\n";
    return 0;
}