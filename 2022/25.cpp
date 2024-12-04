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
#include <numeric>

#include <algorithm>
#include <numeric>
#include <cassert>
#include <set>
#include <queue>
#include <memory>
#include <limits>

std::vector<std::string> read_input_file() {

    std::ifstream file_in("input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(lines.size() > 0);
    return lines;
}

typedef long long num_type;


class SNAFU {
public:
    explicit SNAFU(const std::string &snafu);

    explicit SNAFU(const num_type as_int);


    [[nodiscard]] const std::string &getAsSnafu() const {
        return snafu;
    }

    [[nodiscard]] num_type getAsInt() const {
        return as_int;
    }

private:
    std::string snafu;
    num_type as_int;
};

SNAFU::SNAFU(const std::string &snafu) {
    this->snafu = snafu;

    as_int = 0;
    num_type current_factor = 1;
    for (int i = snafu.length() - 1; i >= 0; --i) {
        switch (snafu[i]) {
            case '0':
                as_int += 0 * current_factor;
                break;
            case '1':
                assert(as_int + current_factor > as_int);// no overfloww
                as_int += 1 * current_factor;
                break;
            case '2':
                assert(as_int + current_factor + current_factor > as_int);// no overfloww
                as_int += 2 * current_factor;
                break;
            case '-':
                assert(as_int - current_factor < as_int);// no underflow
                as_int += -1 * current_factor;
                break;
            case '=':
                assert(as_int - current_factor - current_factor < as_int);// no underflow
                as_int += -2 * current_factor;
                break;
            default:
                assert(false);
        }
        assert(current_factor * 5 > current_factor);// check for overflow
        current_factor = current_factor * 5;
    }
}

SNAFU::SNAFU(const num_type as_int) {

    this->as_int = as_int;
    if (as_int == 0) {
        snafu = "0";
    } else {
        assert(as_int > 0);
        num_type current_value = as_int;

        num_type current_factor = 1;
        snafu = "";
        while (current_value != 0) {
            assert(current_value % current_factor == 0);
            num_type reminder = (current_value / current_factor) % 5;
            switch (reminder) {
                case 0:
                    snafu += '0';
                    current_value = current_value - current_factor * 0;
                    break;
                case 1:
                    snafu += '1';
                    current_value = current_value - current_factor * 1;
                    break;
                case 2:
                    snafu += '2';
                    current_value = current_value - current_factor * 2;
                    break;
                case 3:
                    snafu += '=';
                    current_value = current_value - current_factor * 3;
                    current_value = current_value + current_factor * 5;
                    break;
                case 4:
                    snafu += '-';
                    current_value = current_value - current_factor * 4;
                    current_value = current_value + current_factor * 5;
                    break;
                default:
                    assert(false);

            }
            assert(current_factor * 5 > current_factor);// check for overflow
            current_factor = current_factor * 5;
        }
        std::reverse(snafu.begin(), snafu.end());

        assert(SNAFU(snafu).getAsInt()==as_int);
    }
}

int main() {
    std::vector<std::string> input = read_input_file();
    std::vector<SNAFU> snafu_numbers;
    snafu_numbers.reserve(input.size());

    std::transform(input.begin(), input.end(), std::back_inserter(snafu_numbers), [](auto s) { return SNAFU(s); });

    
    num_type sum = std::accumulate(snafu_numbers.begin(), snafu_numbers.end(), 0,
                                   [](num_type accu, auto num) {
                                       assert(accu + num.getAsInt() > accu);// check for overflow
                                       return accu + num.getAsInt();
                                   });

    num_type sum2 = 0;
    for (auto num: snafu_numbers) {
        assert(sum2 + num.getAsInt() > sum2);// check for overflow
        sum2 = sum2 + num.getAsInt();
    }
    assert(sum == sum2);

    num_type score = sum;
    std::cout << "Result Score: " << score << "\n";
    std::string snafu = SNAFU(score).getAsSnafu();
    std::cout << "Result Score: " << snafu << "\n";


    return 0;
}
