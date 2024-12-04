//
// Created by tim on 02.12.22.
//

#include <iostream>

#include <sstream>
#include <fstream>
#include <vector>
#include <map>
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

class Valve {
public:
    Valve(std::string input);

    int getFlowRate() const {
        return flow_rate;
    }

    void set_connections(std::vector<std::shared_ptr<Valve>> others) {
        for (auto other: others) {
            for (auto conn_id: connected_to) {
                if (other->ID_s == conn_id) {
                    connections.push_back(other);
                }
            }
        }
        assert(connected_to.size() == connections.size());
    }

    const std::vector<std::shared_ptr<Valve>> &getConnections() const;

    int getId() const;

    const std::string &getIdS() const;

private:
    static int next_id;
    std::string ID_s;
    int ID;
    int flow_rate;
    std::vector<std::string> connected_to;
    std::vector<std::shared_ptr<Valve>> connections;
};

int Valve::next_id = 0;

Valve::Valve(std::string input) {

    int s1 = input.find(' ');
    int s2 = input.find(' ', s1 + 1);

    ID_s = input.substr(s1 + 1, s2 - s1 - 1);
    ID = next_id;
    next_id++;


    int eq = input.find('=', s2 + 1);
    int semi = input.find(';', eq + 1);
    flow_rate = std::stoi(input.substr(eq + 1, semi - eq - 1));

    int valve2 = input.find("valve", semi + 1);
    int s9 = input.find(' ', valve2 + 1);

    int pos = s9;
    int nxt = input.find(',', pos + 1);

    if (nxt == -1) {
        nxt = input.length();
    }

    while (pos < input.length()) {
        connected_to.push_back(input.substr(pos + 1, nxt - pos - 1));
        pos = nxt + 1;
        nxt = input.find(',', pos + 1);
        if (nxt == -1) {
            nxt = input.length();
        }

    }

}

const std::vector<std::shared_ptr<Valve>> &Valve::getConnections() const {
    return connections;
}

int Valve::getId() const {
    return ID;
}

const std::string &Valve::getIdS() const {
    return ID_s;
}

unsigned long calculate_path_value(int minutes,
                                   const std::vector<std::shared_ptr<Valve>> &relevant_valves,
                                   const std::vector<std::vector<int>> &adjecency) {
    unsigned long result = 0;
    int cur_time = minutes;

    std::shared_ptr<Valve> pos = relevant_valves[0];

    for (int i = 1; i < relevant_valves.size(); ++i) {
        auto v = relevant_valves[i];

        // travel to v
        int travel_time = adjecency[pos->getId()][v->getId()];
        cur_time -= travel_time;
        // open v
        cur_time--;
        if (cur_time > 0) {
            result += v->getFlowRate() * cur_time;

        } else break;
        pos = v;
    }
    //std::cout << result << "\n";
    return result;
}
const int maximum_minutes = 26;
static unsigned long bound = 0;// used for brnch and bound on a sublist

static std::vector<std::shared_ptr<Valve>> operation_sequence;
std::vector<int> sorted_flow_rates;

// one may come up with a mor tight bound than this
unsigned long calculate_bound(unsigned long minutes, unsigned long current_pressure,unsigned long current_pos,unsigned long num_valves) {
    // upper bound on the maximum releasable pressure
    unsigned long result = current_pressure;

    unsigned long to_open = num_valves-current_pos;
    for (int i = 0; i < std::min(minutes / 2, to_open); ++i) {
        result += sorted_flow_rates[i] * (minutes - (i * 2));
    }
    //std::cout << result<<"/"<<bound<<"\n";
    return result;
}

unsigned long
branch_and_bound(std::vector<std::shared_ptr<Valve>> &valves,unsigned long current_pos, unsigned long minutes, unsigned long current_pressure,std::vector<std::vector<int>> adjacency);

unsigned long
branch(std::vector<std::shared_ptr<Valve>> &valves, unsigned long current_pos, unsigned long minutes, unsigned long current_pressure, std::vector<std::vector<int>> adjacency) {

    unsigned long result = current_pressure;

    for (int i = current_pos; i < valves.size(); ++i) {
        std::swap(valves[current_pos], valves[i]);
        // 1 minute to open + travel time
        auto minutes_left = minutes - 1 - adjacency[valves[current_pos - 1]->getId()][valves[current_pos]->getId()];
        if (minutes_left < minutes)// otherwise: overflow, no time left to travel to this valve
        {
            auto released_pressure = current_pressure + valves[current_pos]->getFlowRate() * minutes_left;
            auto additional_pressure = branch_and_bound(valves,
                                                        current_pos + 1,
                                                        minutes_left,
                                                        released_pressure,
                                                        adjacency);
            result = std::max(result, additional_pressure);
        }
        // swap back
        std::swap(valves[i], valves[current_pos]);
    }

    return result;

}

unsigned long
branch_and_bound(std::vector<std::shared_ptr<Valve>> &valves,unsigned long current_pos, unsigned long minutes, unsigned long current_pressure,std::vector<std::vector<int>> adjacency) {

    // can update at anytime, if we found a better way, even if not at recursion end, as doing nothing from here on is a valid solution
    if (bound < current_pressure) {
        bound = calculate_path_value(maximum_minutes,valves,adjacency);

        operation_sequence.clear();
        operation_sequence.reserve(valves.size());
        std::copy(valves.begin(), valves.end(), std::back_inserter(operation_sequence));
    }

    if (minutes <= 1 || current_pos>=valves.size()) {
        // end of recursion, if only one minute left: no action could affect end result
        // also end if all valves open: nothing more to do
        assert(current_pressure== calculate_path_value(maximum_minutes,valves,adjacency));
        return current_pressure;
    }

    if (calculate_bound(minutes - 1, current_pressure, current_pos, valves.size()) <= bound) {
        //std::cout << "bounded\n";
        return bound;
    }
    // else: branch
    return branch(valves,current_pos,minutes,current_pressure,adjacency);
}

// greedy is not possible:
// e.g. it may be better to stop in between to open some valve
unsigned long
get_best_path_greedy(std::vector<std::shared_ptr<Valve>> &valves,int current_pos, unsigned long minutes, unsigned long current_pressure,std::vector<std::vector<int>> adjacency) {

    if(current_pos>= valves.size()){
        // everything visited
        return current_pressure;
    }

    long best_utility=0;
    int best_option=-1;
    for (int i = current_pos; i < valves.size(); ++i) {
        auto v= valves[i];
        long long utility= (minutes-1-adjacency[valves[current_pos-1]->getId()][v->getId()])*v->getFlowRate();
        if (utility>best_utility){
            best_option=i;
            best_utility=utility;
        }
    }
    if (best_utility<=0){
        // nothing more can be acheived
        return current_pressure;
    }
    assert(best_option!= -1);
    auto time_left= minutes-1-adjacency[valves[current_pos-1]->getId()][valves[best_option]->getId()];

    assert(best_utility== time_left*valves[best_option]->getFlowRate());
    // like selection sort
    std::swap(valves[current_pos],valves[best_option]);

    return get_best_path_greedy(valves,current_pos+1,time_left,current_pressure+best_utility,adjacency);
}

std::vector<std::vector<int>> build_adjecency(std::vector<std::shared_ptr<Valve>> valves) {


    const int n = valves.size();
    std::vector<std::vector<int>> result(n, std::vector<int>(n, n * n));

    for (int i = 0; i < n; ++i) {
        result[i][i] = 0;
    }

    for (auto v: valves) {
        for (auto c: v->getConnections()) {
            result[v->getId()][c, c->getId()] = 1;
        }
    }

    // floydWarshal for all pair shortest path

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (result[i][k] + result[k][j] < result[i][j])
                    result[i][j] = result[i][k] + result[k][j];
            }
        }

    }

    // should be fully connected
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            assert(result[i][j] < n * n);
        }
    }

    return result;

}


unsigned long best_score_for_partitioning(std::vector<std::shared_ptr<Valve>> valves1,std::vector<std::shared_ptr<Valve>> valves2, unsigned long minutes,unsigned long current_best,std::vector<std::vector<int>> adjacency){


    auto b1= calculate_bound(minutes,0,1,valves1.size());
    auto b2= calculate_bound(minutes,0,1,valves1.size());
    if (b1+b2<=current_best)
        return 0;// we have already found a better solution
    // prepare bnb
    bound = calculate_path_value(minutes,valves1,adjacency);
    // we don't care about the operation_list
    auto score1 = branch_and_bound(valves1,1,minutes,0,adjacency);

    if (score1+b2<=current_best)
        return 0;// we have already found a better solution

        // same for other part
    bound = calculate_path_value(minutes,valves2,adjacency);
    // we don't care about the operation_list
    auto score2 = branch_and_bound(valves2,1,minutes,0,adjacency);

    return score1+score2;
}


unsigned long try_all_partitions(std::vector<std::shared_ptr<Valve>> valves_all,unsigned long minutes,std::vector<std::vector<int>> adjacency){

    unsigned long result =0;
    std::vector<std::shared_ptr<Valve>> valves1;
    std::vector<std::shared_ptr<Valve>> valves2;
    valves1.reserve(valves_all.size());
    valves2.reserve(valves_all.size());

    assert(valves_all.size() <= std::numeric_limits<unsigned long>::digits);

    unsigned long max_val = 1<<(valves_all.size()-1);
    // 1<<valves_all.size() -1 == all bits set to 1, but it doesn't matter in which partition each element is, so we only need half of that


    for (unsigned long mask = 0; mask < max_val; ++mask) {
        valves1.clear();
        valves2.clear();
        // start item always the same;
        valves1.push_back(valves_all[0]);
        valves2.push_back(valves_all[0]);
        for (int j = 1; j < valves_all.size(); ++j) {
            if(mask & (1<<(j-1))) {
                valves1.push_back(valves_all[j]);
            }else{
                valves2.push_back(valves_all[j]);
            }
        }
        auto new_score = best_score_for_partitioning(valves1,valves2,minutes,result,adjacency);

        if (result< new_score){
            result=new_score;
            std::cout << new_score << "\n";
            for (auto v: valves1) {
            std::cout << v->getIdS()<<" ";
            }
            std::cout << "\n";
            for (auto v: valves2) {
                std::cout << v->getIdS()<<" ";
            }
            std::cout << "\n";
        }
        result= std::max(result, new_score);
    }

    return result;
}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<std::shared_ptr<Valve>> valves;
    std::transform(input.begin(), input.end(), std::back_inserter(valves),
                   [](auto s) { return std::make_shared<Valve>(s); });

    for (auto v: valves) {
        v->set_connections(valves);
    }

    auto adjacency = build_adjecency(valves);

    std::shared_ptr<Valve> start_pos = nullptr;
    for (auto v: valves) {
        if (v->getIdS() == "AA") {
            assert(start_pos == nullptr);
            start_pos = v;
        }
    }
    assert(start_pos != nullptr);
    assert(start_pos->getFlowRate()==0);// otherwise our implementation dont work

    std::vector<std::shared_ptr<Valve>> relevant_valves;
    relevant_valves.push_back(start_pos);

    // prepare branch and bound
    std::transform(valves.begin(), valves.end(), std::back_inserter(sorted_flow_rates),[](auto v) { return v->getFlowRate(); });
    std::sort(sorted_flow_rates.begin(), sorted_flow_rates.end(), std::greater<>());


    std::copy_if(valves.begin(), valves.end(), std::back_inserter(relevant_valves), [](auto v) {
        return v->getFlowRate() != 0;
    });

    operation_sequence.reserve(relevant_valves.size());
    std::copy(relevant_valves.begin(),relevant_valves.end(),std::back_inserter(operation_sequence));

    auto score = try_all_partitions(relevant_valves,maximum_minutes,adjacency);

    std::cout << "Result Score: " << score << "\n";

    return 0;
}