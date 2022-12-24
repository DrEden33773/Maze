/**
 * @file Map.hpp
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
using std::unordered_set;
using std::vector;

template <class T>
using matrix = vector<vector<T>>;
template <class T>
using sub_matrix = vector<T>;

using coordinate = pair<int, int>;

class Map {
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

    matrix<int>       data {};
    matrix<direction> route_data {};
    coordinate        entry {};
    coordinate        exit {};
    size_t            size = 0;

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
        for (const auto& cord : possible) {
            auto x = cord.first;
            auto y = cord.second;
            if (data.at(x).at(y)) {
                ret.push_back(cord);
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

    void bfs_solution() {
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
    }
    void a_star_solution() {
        coordinate curr = entry;
        while (curr != exit) {
            coordinate lowest_cost = get_lowest_cost(
                get_all_adj(
                    curr
                )
            );
            int x = lowest_cost.first;
            int y = lowest_cost.second;

            route_data.at(x).at(y) = trace_direction(lowest_cost, curr);

            curr = lowest_cost;
        }
    }
    void dfs_solution() {
        // TODO(eden):
    }

public:
};

} // namespace Utility