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
#include <string_view>
#include <vector>

namespace Module {

using std::cout;
using std::endl;
using std::vector;
using Utility::coordinate;

class Initializer {
    Utility::matrix<int> matrix {};
    vector<coordinate>   available_options {};

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
        static constexpr std::string_view SEPERATOR = "";

        cout << "Here's the maze (0 for wall, 1 for path) : " << endl;
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

public:
    static void init(const Utility::matrix<int>& matrix) {
        Initializer initializer;
        initializer.set_matrix(matrix);
        initializer.show_matrix();
        initializer.get_available_options();
    }
};

} // namespace Module
