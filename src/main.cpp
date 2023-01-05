/**
 * @file main.cpp
 * @author Eden (edwardwang33773@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "TaskManager.hpp"
#include "Test/GeneratorTest.hpp"

int main(int argc, char** argv) {
    Task::run_all_tasks();
    // Test::GeneratorTest();
    return 0;
}
