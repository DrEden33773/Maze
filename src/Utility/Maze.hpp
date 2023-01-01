/**
 * @file Maze.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <list>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Utility {

using std::list;
using std::pair;
using std::queue;
using std::tuple;
using std::unordered_set;
using std::vector;

template <class T>
using matrix = vector<vector<T>>;
template <class T>
using sub_matrix = vector<T>;

using coordinate = pair<int, int>;

class Maze {
    enum class direction {
        nil, /* special case */
        up,
        down,
        left,
        right,
    };

    struct CoordinateHash {
        size_t operator()(const coordinate& cord) const {
            size_t x_hash = std::hash<int> {}(cord.first);
            size_t y_hash = std::hash<int> {}(cord.second);
            return x_hash ^ (y_hash << 1);
        };
    };

    matrix<int>       data       = {};
    matrix<direction> route_data = {};
    coordinate        entry      = { -1, -1 };
    coordinate        exit       = { -1, -1 };
    size_t            size       = 0;

    void init_size() {
        size = data.size();
    }
    void init_route_data() {
        route_data.reserve(size);
        auto temp = sub_matrix<direction>(
            size,
            direction::nil
        );
        for (size_t i = 0; i < size; ++i) {
            route_data.emplace_back(temp);
        }
    }
    void set_data(const matrix<int>& matrix) {
        this->data = matrix;
        init_size();
        init_route_data();
    }
    void reset_data() {
        data.clear();
        route_data.clear();
        size = 0;
    }
    void assert_data_init() const {
        if (data.size() == 0 || data.front().size() == 0) {
            throw std::runtime_error("Data Matrix has not been initialized!");
        }
    }
    void assert_route_data_init() const {
        if (route_data.size() == 0 || route_data.front().size() == 0) {
            throw std::runtime_error("Route Data Matrix has not been initialized!");
        }
    }

    void assert_coordinate_connectivity(const coordinate& input) const {
        int x = input.first;
        int y = input.second;
        if (x < 0 || x >= size || y < 0 || y >= size) {
            throw std::out_of_range("Coordinate out of range!");
        }
        if (data.at(x).at(y) == 0) {
            throw std::invalid_argument("Coordinate is not `connected`!");
        }
    }
    void set_entry(const coordinate& input) {
        assert_data_init();
        assert_coordinate_connectivity(input);
        this->entry = input;
    }
    void set_exit(const coordinate& input) {
        assert_data_init();
        assert_coordinate_connectivity(input);
        this->exit = input;
    }
    void reset_entry() {
        entry = { -1, -1 };
    }
    void reset_exit() {
        exit = { -1, -1 };
    }
    void assert_entry_init() const {
        if (entry.first == -1 || entry.second == -1) {
            throw std::runtime_error("Entry has not been initialized!");
        }
    }
    void assert_exit_init() const {
        if (exit.first == -1 || exit.second == -1) {
            throw std::runtime_error("Exit has not been initialized!");
        }
    }

    vector<coordinate> get_all_adj(const coordinate& input) {
        int                x = input.first;
        int                y = input.second;
        vector<coordinate> ret;
        vector<coordinate> possible {
            { x - 1, y },
            { x + 1, y },
            { x, y - 1 },
            { x, y + 1 },
        };

        auto if_in_range = [this](const coordinate& input) {
            int  x             = input.first;
            int  y             = input.second;
            bool if_x_in_range = x >= 0 && x < size;
            bool if_y_in_range = y >= 0 && y < size;
            return if_x_in_range && if_y_in_range;
        };

        for (const auto& cord : possible) {
            if (if_in_range(cord)) {
                auto x = cord.first;
                auto y = cord.second;
                if (data.at(x).at(y)) {
                    ret.push_back(cord);
                }
            }
        }
        return ret;
    };
    int m_dist(const coordinate& lhs, const coordinate& rhs) {
        int x_abs = std::abs(lhs.first - rhs.first);
        int y_abs = std::abs(lhs.second - rhs.second);
        return x_abs + y_abs;
    }
    direction trace_direction(const coordinate& to, const coordinate& from) {
        if (from.first + 1 == to.first) {
            return direction::left;
        }
        if (to.first + 1 == from.first) {
            return direction::right;
        }
        if (from.second + 1 == to.second) {
            return direction::down;
        }
        if (to.second + 1 == from.second) {
            return direction::up;
        }
        return direction::nil;
    }
    coordinate move_to(const coordinate& from, direction direction) {
        int        x   = from.first;
        int        y   = from.second;
        coordinate ret = { x, y };
        switch (direction) {
        case direction::up:
            ++ret.second;
            break;
        case direction::down:
            --ret.second;
            break;
        case direction::right:
            ++ret.first;
            break;
        case direction::left:
            --ret.first;
            break;
        case direction::nil:
            throw std::runtime_error("direction == nil, exception occurred!");
            break;
        }
        return ret;
    }
    coordinate get_lowest_cost(const vector<coordinate>& input) {
        struct cost_info {
            coordinate cord {};
            int        g_cost = 0;
            int        h_cost = 0;
            int        cost   = 0;
            cost_info()       = delete;
            explicit cost_info(coordinate cord)
                : cord(std::move(cord)) { }
        };
        vector<cost_info> all_info;
        all_info.reserve(input.size());
        for (const coordinate& cord : input) {
            all_info.emplace_back(cord);
            cost_info& back = all_info.back();
            back.g_cost     = m_dist(entry, back.cord);
            back.h_cost     = m_dist(back.cord, exit);
            back.cost       = back.g_cost + back.h_cost;
        }
        std::stable_sort(
            all_info.begin(),
            all_info.end(),
            [](const cost_info& lhs, const cost_info& rhs) {
                return lhs.cost < rhs.cost;
            }
        );
        std::stable_sort(
            all_info.begin(),
            all_info.end(),
            [](const cost_info& lhs, const cost_info& rhs) {
                return lhs.h_cost < rhs.h_cost;
            }
        );
        return all_info.front().cord;
    }
    list<coordinate> get_route() {
        list<coordinate> ret  = {};
        coordinate       cord = exit;
        while (cord != entry) {
            int x = cord.first;
            int y = cord.second;
            ret.push_front(cord);
            cord = move_to(
                cord,
                route_data.at(x).at(y)
            );
        }
        ret.push_front(entry);
        return ret;
    }

    void bfs_algo() {
        unordered_set<coordinate, CoordinateHash> visited;
        queue<coordinate>                         queue;
        queue.push(entry);
        visited.insert(entry);

        auto if_unvisited = [&](const coordinate& cord) {
            return !visited.contains(cord);
        };

        while (!queue.empty()) {
            coordinate from = queue.front();
            if (from == exit) {
                return;
            }
            vector<coordinate> all_adj  = get_all_adj(from);
            auto               filtered = all_adj | std::views::filter(if_unvisited);
            vector<coordinate> all_unvisited_adj {
                filtered.begin(),
                filtered.end()
            };
            for (const coordinate& to : all_unvisited_adj) {
                /* aliases */
                int x = to.first;
                int y = to.second;

                /* trace the direction */
                route_data.at(x).at(y) = trace_direction(to, from);

                /* push unvisited adj into the queue */
                queue.push(to);
                visited.insert(to);
            }
            queue.pop();
        }

        // if reached here, no route found
        throw std::runtime_error("No route found!");
    }
    void a_star_algo() {
        unordered_set<coordinate, CoordinateHash> visited;

        auto if_unvisited = [&](const coordinate& cord) {
            return !visited.contains(cord);
        };

        coordinate curr = entry;

        while (curr != exit) {
            vector<coordinate> all_adj  = get_all_adj(curr);
            auto               filtered = all_adj | std::views::filter(if_unvisited);
            vector<coordinate> all_unvisited_adj {
                filtered.begin(),
                filtered.end()
            };

            // if no unvisited adj, no route found
            if (all_unvisited_adj.empty()) {
                throw std::runtime_error("No route found!");
            }

            coordinate lowest_cost = get_lowest_cost(all_unvisited_adj);

            int x = lowest_cost.first;
            int y = lowest_cost.second;

            route_data.at(x).at(y) = trace_direction(lowest_cost, curr);

            visited.insert(curr);
            curr = lowest_cost;
        }
    }

    matrix<int> export_solved_maze() {
        matrix<int> ret = data;
        for (const coordinate& cord : get_route()) {
            int x           = cord.first;
            int y           = cord.second;
            ret.at(x).at(y) = 2;
        }
        return ret;
    }

public:
    /**
     * @brief Default constructor
     *
     */
    Maze() = default;

    void assert_maze_initialized() {
        assert_data_init();
        assert_route_data_init();
        assert_entry_init();
        assert_exit_init();
    }

    /**
     * @brief create from { matrix, entry, exit }
     *
     * @param matrix
     * @param entry
     * @param exit
     * @return Map
     */
    static Maze create(
        const matrix<int>& matrix,
        const coordinate&  entry,
        const coordinate&  exit
    ) {
        Maze ret;
        ret.set_data(matrix);
        ret.set_entry(entry);
        ret.set_exit(exit);
        return ret;
    }

    /**
     * @brief set => { matrix, entry, exit }
     *
     * @param matrix
     * @param entry
     * @param exit
     */
    void set(
        const matrix<int>& matrix,
        const coordinate&  entry,
        const coordinate&  exit
    ) {
        set_data(matrix);
        set_entry(entry);
        set_exit(exit);
    }

    /**
     * @brief reset the maze
     *
     */
    void reset() {
        reset_data();
        reset_entry();
        reset_exit();
    }

    using result_tuple = tuple<matrix<int>, coordinate, coordinate>;

    /**
     * @brief solve the maze by `bfs` algorithm
     *
     * @return matrix<int>
     */
    result_tuple bfs_solution() {
        assert_entry_init();
        assert_exit_init();
        bfs_algo();
        return { export_solved_maze(), entry, exit };
    }

    /**
     * @brief solve the maze by `a*` algorithm
     *
     * @return matrix<int>
     */
    result_tuple a_star_solution() {
        assert_entry_init();
        assert_exit_init();
        a_star_algo();
        return { export_solved_maze(), entry, exit };
    }
};

} // namespace Utility