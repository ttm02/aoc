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

    std::ifstream file_in("../input.txt");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file_in, line))
        lines.push_back(line);
    assert(lines.size() > 0);
    return lines;
}

typedef std::tuple<int, int> Point;

int field_width;
int field_height;

class Blizzard {
public:
    Blizzard(const Point &position, char dir) : position(position), dir(dir) {}

    // copy constructor:
    Blizzard(Blizzard const &copy) : position(copy.position), dir(copy.dir) {}

    bool move(std::vector<std::vector<int>> &field, const Point &expedition_pos);

private:
    Point position;
    char dir;

};

bool Blizzard::move(std::vector<std::vector<int>> &field, const Point &expedition_pos) {

    auto [x, y] = position;

    field[x][y] -= 1;
    assert(field[x][y] >= 0);

    switch (dir) {
        case 'v':
            x += 1;
            x = x >= field_height ? 0 : x;
            break;
        case '>':
            y += 1;
            y = y >= field_width ? 0 : y;
            break;
        case '^':
            x -= 1;
            x = x < 0 ? field_height - 1 : x;
            break;
        case '<':
            y -= 1;
            y = y < 0 ? field_width - 1 : y;
            break;
        default:
            assert(false);
    }

    field[x][y] += 1;

    position = std::make_tuple(x, y);

    return expedition_pos == position;
}


int best_solution = std::numeric_limits<int>::max();

// for the statistics: the number of branch points taken in the search spaces
unsigned long search_space_size;

// the steps taken
std::vector<char> best_steps;

int calculate_bound(Point pos, const int exit_pos, int current_time) {
    // manhatten-distance to exit

    //+1 for the last step to the exit
    return 1 + current_time + field_height - std::get<0>(pos) + std::abs(exit_pos - std::get<1>(pos));
}

bool move_blizzards(std::vector<std::vector<int>> &field, const Point current_position,
                    std::vector<Blizzard> &blizzards) {
    bool result = false;
    for (auto &b: blizzards) {
        bool moved_over = b.move(field, current_position);
        result = result | moved_over;
    }

    return result;
    // true if a blizzard moved over this expedition
}

void draw_field(const std::vector<std::vector<int>> &field, const Point current_position) {

    for (int i = 0; i < field_height; ++i) {
        for (int j = 0; j < field_width; ++j) {
            if (std::make_tuple(i, j) == current_position) {
                assert(field[i][j] == 0);
                std::cout << "E";
            } else {
                std::cout << field[i][j];
                assert(field[i][j] < 10);
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// floydWarshal for all pair shortest path
// assumes that all nodes without connections have a resonably large initialization
// updates the adjacency matrix
void all_pair_shortest_path(std::vector<std::vector<int>> &adjacency) {
    int n = adjacency.size();
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (adjacency[i][k] + adjacency[k][j] < adjacency[i][j])
                    adjacency[i][j] = adjacency[i][k] + adjacency[k][j];
            }
        }

    }
}


// dijkstra
// as all edges have weight 1, dijkstra is a breadth first search
// retunrs minimum path len until one end_pos is reached
int
single_source_shortest_path(std::vector<std::vector<int>> &adjacency, const int source,
                            const std::vector<int> &end_positions, const int num_nodes,
                            const int max_node_degree = 5) {
    std::vector<int> distance(num_nodes, -1);
    distance[source] = 0;
    for (int end: end_positions) {
        distance[end] = -2;// will show that we can stop
    }

    // point is an alias for tuple int,int
    // first value is the distance, second the node index
    std::queue<Point> to_visit;
    to_visit.push(std::make_tuple(0, source));

    while (!to_visit.empty()) {
        auto [cost, node] = to_visit.front();
        for (int i = 0; i < max_node_degree; ++i) {
            int other = adjacency[node][i];
            if (other != -1) {
                if (distance[other] == -2) {
                    return cost + 1;
                } else if (distance[other] == -1) {
                    distance[other] = cost + 1;
                    // not in queue yet
                    to_visit.push(std::make_tuple(cost + 1, other));
                } else {
                    // anready visited, finding a better path is not possible
                    assert(distance[other] <= cost + 1);
                }
            }
        }
        to_visit.pop();
    }
    // no possible path
    return -1;
}

int get_node_id(int state, int i, int j) {
    int field_size = field_width * field_height;
    return state * field_size + i * field_width + j;
}

//adds adjacency a->b
void add_adjacency_entry(const int node_a, const int node_b, std::vector<std::vector<int>> &adjacency) {
    for (int i = 0; i < 5; ++i) {
        if (adjacency[node_a][i] == -1) {
            adjacency[node_a][i] = node_b;
            return;
        }
    }
    // must return earlier and find empty spot to add adjacency
    assert(false);
}

int main() {
    std::vector<std::string> input = read_input_file();

    field_height = input.size() - 2;// do not count the border
    field_width = input[0].length() - 2;

    std::vector<Blizzard> blizzards;

    std::vector<std::vector<int>> field(field_height, std::vector<int>(field_width, 0));

    for (int i = 0; i < field_height; ++i) {
        for (int j = 0; j < field_width; ++j) {
            char d = input[i + 1][j + 1];
            if (d != '.') {
                blizzards.push_back(Blizzard(std::make_tuple(i, j), d));
                field[i][j] = 1;
            }
        }
    }

    int start_pos = input[0].find('.') - 1;
    int end_pos = input[input.size() - 1].find('.') - 1;

    auto initial_blizzards = blizzards;
    auto initial_field = field;

    // lcm is least common multiple / kgV auf deutsch
    const int num_blizzard_positions = std::lcm(field_height, field_width);

    // after num_blizzard_positions the blizzards will all have their original positions

    const int num_states = num_blizzard_positions * field_height * field_width;// number of possible states
    const int field_size = field_height * field_width;
    std::cout << "Theoretical Graph size: " << num_states << " as a result of " << num_blizzard_positions
              << " possible blizzard positionings" << "\n";

    std::vector<int> node_idx_list(num_states, -1);
    int cur_node = 0;

    for (int n = 0; n < num_blizzard_positions; ++n) {
        for (int i = 0; i < field_height; ++i) {
            for (int j = 0; j < field_width; ++j) {
                if (field[i][j] == 0) {
                    // allowed to go there
                    int this_node_id = get_node_id(n, i, j);
                    node_idx_list[this_node_id] = cur_node;
                    cur_node++;
                }
            }
        }
        move_blizzards(field, std::make_tuple(0, 0), blizzards);
    }

    assert(field == initial_field);
    int num_nodes = cur_node;

    std::cout << "Unreachable: " << num_states - num_nodes << " so only " << num_nodes << " Left" << "\n";
    std::cout << "Results in approx " << (num_nodes * num_nodes * sizeof(int)) / 1000000000.0
              << "GB for the full adjacency Matrix" << "\n";
    std::cout << "But the graph is spare so only in approx " << (num_nodes * 5 * sizeof(int)) / 1000000000.0
              << "GB are needed" << "\n";

    // each node is adjacent to at maximum 5 other nodes
    std::vector<std::vector<int>> adjacency(num_nodes, std::vector<int>(5, -1));

    for (int n = 0; n < num_blizzard_positions; ++n) {
        for (int i = 0; i < field_height; ++i) {
            for (int j = 0; j < field_width; ++j) {
                int this_node_idx = node_idx_list[get_node_id(n, i, j)];
                if (field[i][j] == 0) {
                    assert(this_node_idx != -1);
                    // if allowed to go here
                    // wrap around n-1
                    int n_before = (n - 1 == -1) ? num_blizzard_positions - 1 : n - 1;
                    // come from the 4 directions
                    if (i - 1 >= 0) {
                        int other = node_idx_list[get_node_id(n_before, i - 1, j)];
                        if (other != -1) {
                            add_adjacency_entry(other, this_node_idx, adjacency);
                        }
                    }
                    if (i + 1 < field_height) {
                        int other = node_idx_list[get_node_id(n_before, i + 1, j)];
                        if (other != -1) {
                            add_adjacency_entry(other, this_node_idx, adjacency);
                        }
                    }
                    if (j - 1 >= 0) {
                        int other = node_idx_list[get_node_id(n_before, i, j - 1)];
                        if (other != -1) {
                            add_adjacency_entry(other, this_node_idx, adjacency);
                        }
                    }
                    if (j + 1 < field_width) {
                        int other = node_idx_list[get_node_id(n_before, i, j + 1)];
                        if (other != -1) {
                            add_adjacency_entry(other, this_node_idx, adjacency);
                        }
                    }
                    // and come from waiting
                    int other = node_idx_list[get_node_id(n_before, i, j)];
                    if (other != -1) {
                        add_adjacency_entry(other, this_node_idx, adjacency);
                    }
                } else {
                    // never allowed to visit this node
                    assert(this_node_idx == -1);
                }
                // adjacency matrix is NOT symmetrical, as it may not be allowed to go back where we came from
            }
        }
        move_blizzards(field, std::make_tuple(0, 0), blizzards);
    }
    assert(field == initial_field);

    std::vector<int> start_positions;
    std::vector<int> end_positions;
    start_positions.reserve(num_blizzard_positions);
    end_positions.reserve(num_blizzard_positions);

    for (int n = 0; n < num_blizzard_positions; ++n) {
        int s = get_node_id(n, 0, start_pos);
        if (node_idx_list[s] != -1) {
            start_positions.push_back(node_idx_list[s]);
        }
        int e = get_node_id(n, field_height - 1, end_pos);
        if (node_idx_list[e] != -1) {
            end_positions.push_back(node_idx_list[e]);
        }
    }


    int s = node_idx_list[get_node_id(1, 0, 0)];
    assert(s != -1);
    //+2 for entrance and exit
    int part1 = 2 + single_source_shortest_path(adjacency, s, end_positions, num_nodes);
    std::cout << "Start to goal: " << part1 << "\n";
    // state +1 for entering the field again
    int wait_at_end = -1;
    int traversal_time = -1;
    while (traversal_time == -1) {
        wait_at_end++;
        s = node_idx_list[get_node_id((part1 + 1 + wait_at_end) % num_blizzard_positions, field_height - 1, end_pos)];
        while (s == -1) {
            wait_at_end++;
            s = node_idx_list[get_node_id((part1 + 1 + wait_at_end) % num_blizzard_positions, field_height - 1,
                                          end_pos)];
        }
        traversal_time = single_source_shortest_path(adjacency, s, start_positions, num_nodes);
    }
    int part2 = 2 + traversal_time + wait_at_end;
    std::cout << "back to start: " << part2 << "\n";

    wait_at_end = -1;
    traversal_time = -1;
    while (traversal_time == -1) {
        wait_at_end++;
        s = node_idx_list[get_node_id((part1 + part2 + 1 + wait_at_end) % num_blizzard_positions, 0, start_pos)];
        while (s == -1) {
            wait_at_end++;
            s = node_idx_list[get_node_id((part1 + part2 + 1 + wait_at_end) % num_blizzard_positions, 0, start_pos)];
        }
        traversal_time = single_source_shortest_path(adjacency, s, end_positions, num_nodes);
    }

    assert(s != -1);
    int part3 = 2 + traversal_time + wait_at_end;
    std::cout << "to goal again: " << part3 << "\n";


    //+2 for entrance and exit
    int score = part1 + part2 + part3;
    std::cout << "Result Score: " << score << "\n";

    return 0;
}