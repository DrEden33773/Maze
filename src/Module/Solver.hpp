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

namespace Module {

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;
using Utility::coordinate;
using Utility::matrix;

class Solver {
    matrix<int> answer {};

    void solve_by_bfs() {
        answer = Resource::get()->bfs_solution();
    }
    void solve_by_a_star() {
        answer = Resource::get()->a_star_solution();
    }
    void show_mode() {
        cout << "Here's mode to solve the maze:" << endl;
        cout << "\t"
             << "1. BFS" << endl;
        cout << "\t"
             << "2. A*" << endl;
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
        static constexpr std::string_view SEPERATOR = "";

        fstream output;
        output.open(FileManager::Filename::Solved, fstream::out);
        if (!output.is_open()) {
            throw std::runtime_error("Cannot open output file");
        }
        for (const auto& curr_row : answer) {
            for (const auto& curr_num : curr_row) {
                output << curr_num << SEPERATOR;
            }
            output << endl;
        }
        output.close();

        cout << "Solved maze has been written into => "
             << FileManager::Filename::Solved << endl;
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