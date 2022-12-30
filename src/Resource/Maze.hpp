/**
 * @file Maze.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "../Utility/Maze.hpp"

#include <memory>
#include <stdexcept>
#include <utility>

namespace Resource {

using std::shared_ptr;
using Utility::coordinate;
using Utility::matrix;
using Utility::Maze;

/**
 * @brief instance of single object => maze
    (using it's default constructor)
 *
 */
static const shared_ptr<Maze> instance = std::make_shared<Maze>();

/**
 * @brief assert that the maze instance has been initialized
 *
 */
static void assert_instance_initialized() {
    instance->assert_maze_initialized();
}

/**
 * @brief get the maze instance
 *
 * @return shared_ptr<Maze>
 */
static shared_ptr<Maze> get() {
    assert_instance_initialized();
    return instance;
}

/**
 * @brief set the maze instance
 *
 * @param matrix
 * @param entry
 * @param exit
 */
static void set(
    const matrix<int>& matrix,
    const coordinate&  entry,
    const coordinate&  exit
) {
    instance->set(matrix, entry, exit);
}

/**
 * @brief reset the maze instance
 *
 */
static void reset() {
    instance->reset();
}

} // namespace Resource