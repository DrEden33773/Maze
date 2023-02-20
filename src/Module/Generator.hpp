/**
 * @file Generator.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief Generate a random Maze
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "../Utility/FileManager.hpp"
#include "../Utility/Maze.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <ranges>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace Module {

using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::unordered_set;
using std::vector;
using Utility::coordinate;
using Utility::matrix;

class Generator {
    /// @brief size of the maze (it's best be odd number)
    static constexpr int size = 23;

    /// @brief the row data of maze to be generated (0 for wall, 1 for path)
    matrix<int> data;

    /// @brief the entry
    coordinate entry = { -1, -1 };

    /// @brief the exit
    coordinate exit = { -1, -1 };

    /// @brief visited cells
    unordered_set<coordinate, Utility::Maze::CoordinateHash> visited;

    void init_the_matrix() {
        // make the whole matrix to be wall
        data = matrix<int>(size, vector<int>(size, 0));
        // make a special row, whose form is `1 0 1 0 ... 1 0 1`
        vector<int> special_row(size, 0);
        for (int i = 0; i < size; i += 2) {
            special_row[i] = 1;
        }
        // make the (1, 3, 5, 7, ..., size<2n+1>)_th row to be special row
        for (int i = 0; i < size; i += 2) {
            data[i] = special_row;
        }
    }
    void generate_entry() {
        // list all available num in a vec
        vector<int> available_num_vec;
        for (int i = 0; i < size; i += 2) {
            available_num_vec.push_back(i);
        }
        // shuffle the vec
        std::random_device rd;
        std::mt19937       g(rd());
        std::shuffle(available_num_vec.begin(), available_num_vec.end(), g);
        // choose the first one as the entry
        entry = { available_num_vec[0], 0 };
    }
    void set_path_by_stack_dfs() {
        // helper lambda functions
        auto if_unvisited = [this](const coordinate& c) {
            return visited.find(c) == visited.end();
        };
        auto if_in_range = [this](const coordinate& c) {
            int x = c.first;
            int y = c.second;
            return x >= 0 && x < size && y >= 0 && y < size;
        };
        // init the stack
        vector<coordinate> stack;
        stack.push_back(entry);
        visited.insert(entry);
        // dfs
        while (!stack.empty()) {
            // get the top of the stack
            coordinate top = stack.back();
            int        x   = top.first;
            int        y   = top.second;
            // all neighbors
            vector<coordinate> all_neighbors = {
                { x - 2, y },
                { x + 2, y },
                { x, y - 2 },
                { x, y + 2 },
            };
            // filter
            auto available = all_neighbors
                | std::views::filter(if_in_range)
                | std::views::filter(if_unvisited);
            // get the available neighbors
            vector<coordinate> neighbors { available.begin(), available.end() };
            // if there is no available neighbors, pop the stack
            if (neighbors.empty()) {
                stack.pop_back();
                continue;
            }
            // choose a random neighbor
            std::random_device rd;
            std::mt19937       g(rd());
            std::shuffle(neighbors.begin(), neighbors.end(), g);
            coordinate chosen_neighbor = neighbors.front();
            int        nx              = chosen_neighbor.first;
            int        ny              = chosen_neighbor.second;
            // set the path
            data[(x + nx) / 2][(y + ny) / 2] = 1;
            // push the chosen neighbor to the stack
            stack.push_back(chosen_neighbor);
            visited.insert(chosen_neighbor);
            exit = chosen_neighbor;
        }
    }
    void generate_maze() {
        // init the matrix
        init_the_matrix();
        // generate the entry
        generate_entry();
        // set the path
        set_path_by_stack_dfs();
    }

    void output_matrix_for_test() {
        for (auto&& row : data) {
            for (auto&& num : row) {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << endl;

        cout << "entry => ";
        cout << "(" << entry.first << ", " << entry.second << ")" << endl;
        cout << "exit => ";
        cout << "(" << exit.first << ", " << exit.second << ")" << endl;
        cout << endl;
    }
    void write_matrix_into_file() {
        fstream file;
        file.open(FileManager::Filename::MazeData, fstream::out);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }
        for (auto&& row : data) {
            for (auto&& num : row) {
                file << num << " ";
            }
            file << endl;
        }
        file << endl;
        file.close();
    }
    void write_everything_into_file() {
        static constexpr const char* SIGN = "# ";

        fstream file;
        file.open(FileManager::Filename::MazeData, fstream::out);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }
        // 1. how to use the file (10 lines)
        file << SIGN << "This file is composed by 4 parts: " << endl;
        file << endl;
        file << SIGN << "1. size of maze (an integer)" << endl;
        file << SIGN << "2. the maze data (a matrix with 0 for wall, 1 for path)" << endl;
        file << SIGN << "3. the entry (two integer starts from 0, separated in <space>)" << endl;
        file << SIGN << "4. the exit (two integer starts from 0, separated in <space>)" << endl;
        file << endl;
        file << SIGN << "Different part should be separated by a blank line" << endl;
        file << SIGN << "Here are the meta data: " << endl;
        file << endl;
        // 2. size of the maze (2 lines)
        file << size << endl;
        file << endl;
        // 3. matrix (size + 1 lines)
        for (const auto& row : data) {
            for (auto curr : row) {
                file << curr << " ";
            }
            file << endl;
        }
        file << endl;
        // 4. entry (2 lines)
        file << entry.first << " " << entry.second << endl;
        file << endl;
        // 5. exit (2 lines)
        file << exit.first << " " << exit.second << endl;
        file << endl;
    }

public:
    static void test() {
        Generator generator;
        generator.generate_maze();
        generator.output_matrix_for_test();
    }
    static void generate_matrix_only() {
        Generator generator;
        generator.generate_maze();
        generator.write_matrix_into_file();
    }
    static void fully_generate() {
        Generator generator;
        generator.generate_maze();
        generator.write_everything_into_file();
    }
};

} // namespace Module
