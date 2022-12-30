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

class Scanner {
    Utility::matrix<int> matrix {};

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

    void scan_from_file() {
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

public:
    static auto scan() {
        Scanner scanner;
        scanner.scan_from_file();
        return scanner.matrix;
    }
};

} // namespace Module