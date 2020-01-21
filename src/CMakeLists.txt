cmake_minimum_required(VERSION 3.13)
project(src)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS " -std=c++14 -Wall -Wextra -Werror -Wpedantic -pedantic-errors ")
include_directories(include)
include_directories(test)
include_directories(src)
add_subdirectory(Google_tests)
set (SOURCE_FILES
        include/package.hpp
        include/types.hpp
        include/storage_types.hpp
        include/nodes.hpp
        include/helpers.hpp
        include/simulation.hpp
        include/factory.hpp
        include/reports.hpp
        src/factory.cpp
        src/storage_types.cpp
        src/package.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/simulation.cpp
        src/reports.cpp)

add_executable(netsim main.cpp ${SOURCE_FILES})
target_compile_definitions(netsim PUBLIC EXERCISE_ID=SIMULATION)

set(SOURCES_FILES_TESTS
        test/config.hpp
        test/global_functions_mock.hpp
        test/main_gtest.cpp
        test/nodes_mocks.hpp
        test/test_Factory.cpp
        test/test_factory_io.cpp
        test/test_Nodecollection.cpp
        test/test_nodes.cpp
        test/test_package.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_storage_types.cpp)

add_executable(netsim__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)
target_compile_definitions(netsim__test PUBLIC EXERCISE_ID=REPORTING)
target_include_directories(netsim__test PUBLIC
        ../../googletest-master/googlemock/include
        ../../googletest-master/googletest/include
        )

target_link_libraries(netsim__test gmock)

