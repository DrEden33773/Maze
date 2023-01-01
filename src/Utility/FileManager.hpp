/**
 * @file FileManager.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace FileManager {

namespace fs = std::filesystem;

namespace Dir {
    /* root */
    static const fs::path Root = "Files";
} // namespace Path

namespace Filename {
    static const fs::path MazeData = Dir::Root / "MazeData.txt";
    static const fs::path Solved   = Dir::Root / "Solved.txt";
} // namespace Filename

/* all_path in a vec */
static const std::vector<fs::path> all_path {
    Dir::Root,
};

static void create_all_dir() {
    std::for_each(
        all_path.begin(),
        all_path.end(),
        [](const fs::path& curr) {
            if (!fs::exists(curr)) {
                try {
                    fs::create_directories(curr);
                } catch (fs::filesystem_error& e) {
                    std::cout << "Could not create directory, because: " << e.what() << std::endl;
                }
            }
        }
    );
}
static void check_all_dir_existence() {
    std::for_each(
        all_path.begin(),
        all_path.end(),
        [](const fs::path& curr) {
            if (!fs::exists(curr)) {
                throw std::runtime_error("A directory is NOT exist!");
            }
        }
    );
}

static void dir_init() {
    create_all_dir();
    check_all_dir_existence();
    std::cout << "Dir_Init Succeeded!" << std::endl;
    std::cout << std::endl;
}
static void maze_data_file_init() {
    using std::cout;
    using std::endl;
    using std::fstream;

    if (!fs::exists(Filename::MazeData)) {
        fstream maze_data;
        maze_data.open(Filename::MazeData, fstream::out | fstream::trunc);
        if (!maze_data.is_open()) {
            throw std::runtime_error("Cannot create `MazeData.txt`!");
        }
        maze_data.close();
    }
    cout << "Successfully => Init `MazeData.txt`" << endl;
    cout << endl;
}
static void output_file_init() {
    /// @attention `Solved.txt` will be created/refreshed

    using std::cout;
    using std::endl;
    using std::fstream;

    fstream solved;
    solved.open(Filename::Solved, fstream::out | fstream::trunc);
    if (!solved.is_open()) {
        throw std::runtime_error("Cannot create `Solved.txt`!");
    }
    solved.close();
    cout << "Successfully => Init `Solved.txt`" << endl;
    cout << endl;
}
static void init_all() {
    dir_init();
    maze_data_file_init();
    output_file_init();
}

} // namespace FileManager