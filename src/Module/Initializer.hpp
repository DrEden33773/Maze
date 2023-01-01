/**
 * @file Initializer.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief select `entry` and `exit` from the `matrix`
 * @version 0.1
 * @date 2022-12-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../Resource/Maze.hpp"
#include "../Utility/FileManager.hpp"

#include <ostream>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace Module {

using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::vector;
using Utility::coordinate;

class Initializer {
    Utility::matrix<int> matrix {};
    vector<coordinate>   available_options {};

    coordinate entry = { -1, -1 };
    coordinate exit  = { -1, -1 };

    friend std::ostream& operator<<(
        std::ostream&     os,
        const coordinate& cord
    ) {
        os << "(" << cord.first << ", " << cord.second << ")";
        return os;
    }

    void set_matrix(const Utility::matrix<int>& matrix) {
        this->matrix = matrix;
    }
    void show_matrix() {
        static constexpr std::string_view SEPERATOR = " ";

        cout << "Here's the maze (0 for wall, 1 for available path) : " << endl;
        cout << endl;

        for (const auto& curr_row : matrix) {
            for (const auto& curr_num : curr_row) {
                cout << curr_num << SEPERATOR;
            }
            cout << endl;
        }
        cout << endl;
    }
    void get_available_options() {
        size_t row = matrix.size();
        size_t col = matrix.front().size();
        available_options.reserve(4 * row);
        // scan top and bottom line
        for (size_t i = 0; i < row; ++i) {
            // top line
            if (matrix[0][i] == 1) {
                available_options.emplace_back(0, i);
            }
            // bottom line
            if (matrix[row - 1][i] == 1) {
                available_options.emplace_back(row - 1, i);
            }
        }
        // scan left and right line
        for (size_t i = 0; i < col; ++i) {
            // left line
            if (matrix[i][0] == 1) {
                available_options.emplace_back(i, 0);
            }
            // right line
            if (matrix[i][col - 1] == 1) {
                available_options.emplace_back(i, col - 1);
            }
        }
        // shrink to fit
        available_options.shrink_to_fit();
        // unique available_options
        auto it = std::unique(available_options.begin(), available_options.end());
        available_options.erase(it, available_options.end());
    }
    void show_available_options() {
        cout << "Here's the available options : " << endl;
        cout << endl;
        size_t index = 1;
        for (const auto& curr_option : available_options) {
            int x = curr_option.first;
            int y = curr_option.second;
            cout << index << ". (" << x << ", " << y << ")" << endl;
            ++index;
        }
        cout << endl;
    }
    pair<int, int> input_option() {
        pair<int, int> ret = { -1, -1 };
        cout << "Please select entry and exit by input index : ";
        string entry_idx_str;
        string exit_idx_str;
        cin >> entry_idx_str >> exit_idx_str;
        int entry_idx = std::stoi(entry_idx_str);
        int exit_idx  = std::stoi(exit_idx_str);
        if (entry_idx >= 1 && entry_idx <= available_options.size()) {
            ret.first = entry_idx - 1;
        }
        if (exit_idx >= 1 && exit_idx <= available_options.size()) {
            ret.second = exit_idx - 1;
        }
        return ret;
    }
    void select_entry_and_exit() {
        if (available_options.empty()) {
            throw std::runtime_error("No available options!");
        }
        while (true) {
            show_available_options();
            auto [entry_idx, exit_idx] = input_option();
            if (entry_idx == -1 || exit_idx == -1) {
                cout << "Invalid input!" << endl;
                cout << endl;
                continue;
            }
            if (entry_idx == exit_idx) {
                cout << "Entry and exit cannot be the same!" << endl;
                cout << endl;
                continue;
            }
            entry = available_options[entry_idx];
            exit  = available_options[exit_idx];
            break;
        }
        cout << endl;
    }
    void register_the_maze() {
        Resource::set(matrix, entry, exit);
        cout << "Successfully registered the maze, now trying to solve it..." << endl;
        cout << endl;
    }

public:
    static void init(const Utility::matrix<int>& matrix) {
        Initializer initializer;
        initializer.set_matrix(matrix);
        initializer.show_matrix();
        initializer.get_available_options();
        initializer.select_entry_and_exit();
        initializer.register_the_maze();
    }
};

} // namespace Module
