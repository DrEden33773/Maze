/**
 * @file Scanner.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief Scan from the file, to generate maze
 * @version 0.1
 * @date 2022-12-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../Resource/Maze.hpp"
#include "../Utility/FileManager.hpp"

namespace Module {

using std::cout;
using std::endl;

class Scanner {
    Utility::matrix<int> matrix = {};
    Utility::coordinate  entry  = { -1, -1 };
    Utility::coordinate  exit   = { -1, -1 };

    void assert_matrix_non_empty() const {
        if (matrix.empty()) {
            throw std::runtime_error("matrix is empty");
        }
    }
    void assert_matrix_validity() const {
        size_t row = matrix.size();
        for (const auto& curr : matrix) {
            if (curr.size() != row) {
                throw std::runtime_error("matrix is not square");
            }
        }
    }

    void scan_matrix_from_file() {
        std::ifstream file { FileManager::Filename::MazeData };
        if (!file.is_open()) {
            throw std::runtime_error("file is not open");
        }

        std::string line {};
        while (std::getline(file, line)) {
            std::stringstream ss { line };
            std::vector<int>  curr_row {};
            int               curr_num {};
            while (ss >> curr_num) {
                curr_row.push_back(curr_num);
            }
            matrix.push_back(curr_row);
        }

        assert_matrix_non_empty();
        assert_matrix_validity();

        file.close();

        std::cout << "Scan from file successfully!" << std::endl;
        std::cout << std::endl;
    }
    void full_scan_from_file() {
        static constexpr const char* DROP_SIGN    = "#";
        static constexpr int         line_of_tips = 10;

        std::ifstream file { FileManager::Filename::MazeData };
        if (!file.is_open()) {
            throw std::runtime_error("file is not open");
        }

        // pass first 10 lines
        std::string line;
        for (int i = 0; i < line_of_tips; ++i) {
            std::getline(file, line);
        }
        // get the size of maze
        int size = 0;
        file >> size;
        // import the matrix
        matrix = Utility::matrix<int>(
            size,
            std::vector<int>(size, 0)
        );
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                file >> matrix[i][j];
            }
        }
        // import the entry
        file >> entry.first >> entry.second;
        // import the exit
        file >> exit.first >> exit.second;
    }
    void register_the_maze() {
        Resource::set(matrix, entry, exit);
        cout << "Successfully registered the maze..." << endl;
        cout << endl;
    }
    void show_the_maze_info() {
        cout << "We'll first show you the info of current maze: " << endl;
        cout << endl;
        // 1. size
        cout << "size => " << matrix.size() << endl;
        cout << endl;
        // 2. matrix
        cout << "matrix (0 for wall, 1 for available path) :" << endl;
        cout << endl;
        for (const auto& vec : matrix) {
            for (auto num : vec) {
                cout << num << " ";
            }
            cout << endl;
        }
        cout << endl;
        // 3. entry
        cout << "entry => (" << entry.first << ", " << entry.second << ")" << endl;
        cout << endl;
        // 4. exit
        cout << "exit => (" << exit.first << ", " << exit.second << ")" << endl;
        cout << endl;
        // 5. tips
        cout << "Now, we'll try to solve the maze..." << endl;
        cout << endl;
    }

public:
    static auto matrix_scan_only() {
        Scanner scanner;
        scanner.scan_matrix_from_file();
        return scanner.matrix;
    }
    static void full_scan_and_register() {
        Scanner scanner;
        scanner.full_scan_from_file();
        scanner.register_the_maze();
        scanner.show_the_maze_info();
    }
};

} // namespace Module