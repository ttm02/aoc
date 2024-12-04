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

class Blueprint {
public:
    Blueprint(std::string input);

    int ID;
    std::tuple<int, int, int> ore_bot_cost;
    std::tuple<int, int, int> clay_bot_cost;
    std::tuple<int, int, int> obsidian_bot_cost;
    std::tuple<int, int, int> geode_bot_cost;

};

Blueprint::Blueprint(std::string input) {

    std::stringstream iss(input);
    std::string word;
    iss >> word;// blueprint
    iss >> word;// ID:
    ID = std::stoi(word.substr(0, word.length() - 1));
    iss >> word;// each
    iss >> word;// ore
    iss >> word;// robot
    iss >> word;// costs
    iss >> word;// N
    ore_bot_cost = std::make_tuple(std::stoi(word), 0, 0);
    iss >> word;// ore.
    assert(word == "ore.");
    iss >> word;// each
    iss >> word;// clay
    iss >> word;// bot
    iss >> word;// costs
    iss >> word;// N
    clay_bot_cost = std::make_tuple(std::stoi(word), 0, 0);
    iss >> word;// ore.
    assert(word == "ore.");
    iss >> word;// each
    iss >> word;// obsidian
    iss >> word;// bot
    iss >> word;// costs
    iss >> word;// N
    int ore_cost = std::stoi(word);
    iss >> word;// ore
    assert(word == "ore");
    iss >> word;// and
    assert(word == "and");
    iss >> word;// N
    int clay_cost = std::stoi(word);
    iss >> word;// clay
    assert(word == "clay.");
    obsidian_bot_cost = std::make_tuple(ore_cost, clay_cost, 0);
    iss >> word;// each
    iss >> word;// geode
    iss >> word;// bot
    iss >> word;// costs
    iss >> word;// N
    ore_cost = std::stoi(word);
    iss >> word;// ore
    assert(word == "ore");
    iss >> word;// and
    assert(word == "and");
    iss >> word;// N
    int obsidian_cost = std::stoi(word);
    iss >> word;// obsidian
    assert(word == "obsidian.");
    geode_bot_cost = std::make_tuple(ore_cost, 0, obsidian_cost);

}

int best_strategy(const std::shared_ptr<Blueprint> &blueprint, int ore, int clay, int obsidian, int ore_bots,
                  int clay_bots, int obsidian_bots, int geode_bots, int minutes_left);

int
build_ore_bot(const std::shared_ptr<Blueprint> &blueprint, int ore, int clay, int obsidian, int ore_bots, int clay_bots,
              int obsidian_bots, int geode_bots, int minutes_left) {//  try to build new ore bot

    int max_cost = std::max(std::get<0>(blueprint->ore_bot_cost), std::max(std::get<0>(blueprint->clay_bot_cost),
                                                                           std::max(std::get<0>(
                                                                                            blueprint->obsidian_bot_cost),
                                                                                    std::get<0>(
                                                                                            blueprint->geode_bot_cost))));

    if (ore_bots >= max_cost) {
        return geode_bots * minutes_left; // no value in building additional bot
    }
    int ore_bot_cost = std::get<0>(blueprint->ore_bot_cost);
    int minutes_to_wait =
            (std::max(ore_bot_cost - ore, 0)) / ore_bots;// wait until resources are sufficient to build one
    if ((ore_bots * minutes_to_wait) + ore < ore_bot_cost) minutes_to_wait++;// if devision rounded down
    if (minutes_left - minutes_to_wait > 0) {
        assert((ore_bots * minutes_to_wait) + ore >= ore_bot_cost);
        // spend one minute building the new bot
        minutes_to_wait++;
        return geode_bots * minutes_to_wait +
               +best_strategy(blueprint, ore + (ore_bots * minutes_to_wait) - ore_bot_cost,
                              clay + (clay_bots * minutes_to_wait),
                              obsidian + (obsidian_bots * minutes_to_wait), ore_bots + 1, clay_bots,
                              obsidian_bots, geode_bots, minutes_left - minutes_to_wait);

    } else {
        return geode_bots * minutes_left; // adds no value
    }
}

int
build_clay_bot(const std::shared_ptr<Blueprint> &blueprint, int ore, int clay, int obsidian, int ore_bots,
               int clay_bots,
               int obsidian_bots, int geode_bots, int minutes_left) {//  try to build new ore bot

    int max_cost = std::get<1>(blueprint->obsidian_bot_cost);
    if (clay_bots >= max_cost) {
        return geode_bots * minutes_left; // no value in building additional bot
    }

    int clay_bot_cost = std::get<0>(blueprint->clay_bot_cost);
    int minutes_to_wait =
            (std::max(clay_bot_cost - ore, 0)) / ore_bots;// wait until resources are sufficient to build one
    if ((ore_bots * minutes_to_wait) + ore < clay_bot_cost) minutes_to_wait++;// if devision rounded down
    if (minutes_left - minutes_to_wait > 0) {
        assert((ore_bots * minutes_to_wait) + ore >= clay_bot_cost);
        // spend one minute building the new bot
        minutes_to_wait++;
        return geode_bots * minutes_to_wait +
               best_strategy(blueprint, ore + (ore_bots * minutes_to_wait) - clay_bot_cost,
                             clay + (clay_bots * minutes_to_wait),
                             obsidian + (obsidian_bots * minutes_to_wait), ore_bots, clay_bots + 1,
                             obsidian_bots, geode_bots, minutes_left - minutes_to_wait);

    } else {

        return geode_bots * minutes_left; // adds no value
    }
}

int
build_obsidian_bot(const std::shared_ptr<Blueprint> &blueprint, int ore, int clay, int obsidian, int ore_bots,
                   int clay_bots,
                   int obsidian_bots, int geode_bots, int minutes_left) {
    int max_cost = std::get<2>(blueprint->geode_bot_cost);
    if (obsidian_bots >= max_cost) {
        return geode_bots * minutes_left; // no value in building additional bot
    }
    //  try to build new obsidian bot

    if (clay_bots == 0) {
        // will never have enough resources to build new bot
        return geode_bots * minutes_left; // probably 0
    }

    int ore_cost = std::get<0>(blueprint->obsidian_bot_cost);
    int clay_cost = std::get<1>(blueprint->obsidian_bot_cost);

    int minutes_to_wait_ore =
            (std::max(ore_cost - ore, 0)) / ore_bots;// wait until resources are sufficient to build one
    if ((ore_bots * minutes_to_wait_ore) + ore < ore_cost) minutes_to_wait_ore++;// if devision rounded down
    int minutes_to_wait_clay =
            (std::max(clay_cost - clay, 0)) / clay_bots;// wait until resources are sufficient to build one
    if ((clay_bots * minutes_to_wait_clay) + clay < clay_cost) minutes_to_wait_clay++;// if devision rounded down
    int minutes_to_wait = std::max(minutes_to_wait_ore, minutes_to_wait_clay);
    if (minutes_left - minutes_to_wait > 0) {
        assert((ore_bots * minutes_to_wait) + ore >= ore_cost);
        assert((clay_bots * minutes_to_wait) + clay >= clay_cost);
        // spend one minute building the new bot
        minutes_to_wait++;
        return geode_bots * minutes_to_wait +
               best_strategy(blueprint, ore + (ore_bots * minutes_to_wait) - ore_cost,
                             clay + (clay_bots * minutes_to_wait) - clay_cost,
                             obsidian + (obsidian_bots * minutes_to_wait), ore_bots, clay_bots,
                             obsidian_bots + 1, geode_bots, minutes_left - minutes_to_wait);

    } else {
        return geode_bots * minutes_left; // adds no value
    }
}

int
build_geode_bot(const std::shared_ptr<Blueprint> &blueprint, int ore, int clay, int obsidian, int ore_bots,
                int clay_bots,
                int obsidian_bots, int geode_bots, int minutes_left) {
    //  try to build new geode bot

    if (obsidian_bots == 0) {
        // will never have enough resources to build new bot
        return geode_bots * minutes_left; // probably 0
    }

    int ore_cost = std::get<0>(blueprint->geode_bot_cost);
    int obsidian_cost = std::get<2>(blueprint->geode_bot_cost);

    int minutes_to_wait_ore =
            (std::max(ore_cost - ore, 0)) / ore_bots;// wait until resources are sufficient to build one
    if ((ore_bots * minutes_to_wait_ore) + ore < ore_cost) minutes_to_wait_ore++;// if devision rounded down
    int minutes_to_wait_obsidian =
            (std::max(obsidian_cost - obsidian, 0)) / obsidian_bots;// wait until resources are sufficient to build one
    if ((obsidian_bots * minutes_to_wait_obsidian) + obsidian < obsidian_cost)
        minutes_to_wait_obsidian++;// if devision rounded down
    int minutes_to_wait = std::max(minutes_to_wait_ore, minutes_to_wait_obsidian);
    if (minutes_left - minutes_to_wait > 0) {
        assert((ore_bots * minutes_to_wait) + ore >= ore_cost);
        assert((obsidian_bots * minutes_to_wait) + obsidian >= obsidian_cost);
        // spend one minute building the new bot
        minutes_to_wait++;
        return geode_bots * minutes_to_wait +
               best_strategy(blueprint, ore + (ore_bots * minutes_to_wait) - ore_cost,
                             clay + (clay_bots * minutes_to_wait),
                             obsidian + (obsidian_bots * minutes_to_wait) - obsidian_cost, ore_bots, clay_bots,
                             obsidian_bots, geode_bots + 1, minutes_left - minutes_to_wait);
    } else {
        return geode_bots * minutes_left; // adds no value
    }
}

// try analysis of the full search space
// takes some amount of time but i think <2 minutes is still fine
int best_strategy(const std::shared_ptr<Blueprint> &blueprint, int ore, int clay, int obsidian, int ore_bots,
                  int clay_bots, int obsidian_bots, int geode_bots, int minutes_left) {

    // basically, there are only 4 things to do, build either one of the 4 robots, the build order determines the final score

    if (minutes_left < 2) {
        // nothing to gain by building new bots
        return geode_bots * minutes_left;
    }

    int best_score = geode_bots * minutes_left;// if nothing is done

    best_score = std::max(best_score,
                          build_ore_bot(blueprint, ore, clay, obsidian, ore_bots, clay_bots, obsidian_bots,
                                        geode_bots, minutes_left));
    best_score = std::max(best_score,
                          build_clay_bot(blueprint, ore, clay, obsidian, ore_bots, clay_bots, obsidian_bots,
                                         geode_bots, minutes_left));
    best_score = std::max(best_score,
                          build_obsidian_bot(blueprint, ore, clay, obsidian, ore_bots, clay_bots, obsidian_bots,
                                             geode_bots, minutes_left));
    best_score = std::max(best_score,
                          build_geode_bot(blueprint, ore, clay, obsidian, ore_bots, clay_bots, obsidian_bots,
                                          geode_bots, minutes_left));


    return best_score;
}

int main() {
    std::vector<std::string> input = read_input_file();

    std::vector<std::shared_ptr<Blueprint>> blueprints;

    std::transform(input.begin(), input.end(), std::back_inserter(blueprints),
                   [](auto s) { return std::make_shared<Blueprint>(s); });

    std::vector<int> scores;

    std::transform(blueprints.begin(), blueprints.end(), std::back_inserter(scores), [](auto blueprint) {

        int result = best_strategy(blueprint, 0, 0, 0, 1, 0, 0, 0, 32);
        std::cout << "Blueprint " << blueprint->ID << ": " << result << "\n";
        return result;
    });


    int score = std::accumulate(scores.begin(), scores.end(), 1, std::multiplies<>());


    std::cout << "Result Score: " << score << "\n";
    return 0;
}