/**
 * @file TaskManager.hpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-01-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "Module/Generator.hpp"
#include "Module/Initializer.hpp"
#include "Module/Scanner.hpp"
#include "Module/Solver.hpp"
#include "Utility/FileManager.hpp"

namespace Task {

void run_all_tasks() {
    FileManager::init_all();
    // Module::Generator::generate_matrix_only();
    // auto matrix = Module::Scanner::matrix_scan_only();
    // Module::Initializer::init(matrix);
    // Module::Solver::solve();
    Module::Generator::fully_generate();
    Module::Scanner::full_scan_and_register();
    Module::Solver::solve();
}

} // namespace Task