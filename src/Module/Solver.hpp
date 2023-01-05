/**
 * @file Solver.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "../Resource/Maze.hpp"
#include "../Utility/FileManager.hpp"
#include <filesystem>

namespace Module {

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;
using Utility::coordinate;
using Utility::matrix;

class Solver {
    matrix<int> answer           = {};
    coordinate  entry            = { -1, -1 };
    coordinate  exit             = { -1, -1 };
    bool        if_have_solution = true;

    void solve_by_bfs() {
        auto&& [_if_have_solution, _answer, _entry, _exit]
            = Resource::get()->bfs_solution();
        if_have_solution = _if_have_solution;
        answer           = std::move(_answer);
        entry            = std::move(_entry);
        exit             = std::move(_exit);
    }
    void solve_by_a_star() {
        auto&& [_if_have_solution, _answer, _entry, _exit]
            = Resource::get()->a_star_solution();
        if_have_solution = _if_have_solution;
        answer           = std::move(_answer);
        entry            = std::move(_entry);
        exit             = std::move(_exit);
    }
    void show_mode() {
        cout << "Here's mode to solve the maze:" << endl;
        cout << endl;
        cout << "1. BFS" << endl;
        cout << "2. A*" << endl;
        cout << endl;
        cout << "Please select a mode >>> ";
    }
    void solve_by_selected_mode() {
        string mode;
        while (true) {
            show_mode();
            cin >> mode;
            if (mode == "1" || mode == "2") {
                break;
            } else {
                cout << "Invalid mode, please try again." << endl;
                cout << endl;
            }
        }
        cout << endl;
        if (mode == "1") {
            solve_by_bfs();
        } else {
            solve_by_a_star();
        }
    }
    void write_into_output_file() {
        static constexpr std::string_view SEPERATOR = " ";

        fstream output;
        output.open(FileManager::Filename::Solved, fstream::out);
        if (!output.is_open()) {
            throw std::runtime_error("Cannot open output file");
        }

        if (if_have_solution) {
            output << "Successfully solved the maze!" << endl;
        } else {
            output << "Cannot solve the maze! But we could show the original data." << endl;
        }
        output << endl;

        output << "Here's the maze (0 for wall, 1 for available path, * for picked path) : " << endl;
        output << endl;

        for (const auto& curr_row : answer) {
            for (const auto& curr_num : curr_row) {
                if (curr_num != 2) {
                    output << curr_num << SEPERATOR;
                } else {
                    output << "*" << SEPERATOR;
                }
            }
            output << endl;
        }
        output << endl;

        output << "Entry: "
               << "(" << entry.first << ", " << entry.second << ")" << endl;
        output << "Exit: "
               << "(" << exit.first << ", " << exit.second << ")" << endl;

        output.close();

        if (if_have_solution) {
            cout << "Solved maze has been written into => " << endl;
        } else {
            cout << "Maze cannot be solved."
                 << " However, the original data has been copied to => "
                 << endl;
        }
        cout << FileManager::fs::absolute(FileManager::Filename::Solved) << endl;
        cout << endl;
    }

public:
    static void solve() {
        Solver solver;
        solver.solve_by_selected_mode();
        solver.write_into_output_file();
    }
};

} // namespace Module