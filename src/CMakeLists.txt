cmake_minimum_required(VERSION 3.13)
project(src)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_subdirectory(Google_tests)
set (SOURCE_FILES
        include/package.hpp
        include/types.hpp
        include/storage_types.hpp
        package.cpp
        storage_types.cpp
        include/nodes.hpp
        include/helpers.hpp
        nodes.cpp
        helpers.cpp)

add_executable(src main.cpp ${SOURCE_FILES} )
target_compile_definitions(src PUBLIC EXERCISE_ID=EXERCISE_ID_NODES)

set(SOURCES_FILES_TESTS
        test/config.hpp
        test/global_functions_mock.hpp
        test/main_gtest.cpp
        test/test_package.cpp
        test/test_storage_types.cpp
        test/test_nodes.cpp
        test/nodes_mocks.hpp
        )

set(SOURCES_FILES_TESTS2
        test/config.hpp
        test/global_functions_mock.hpp
        test/main_gtest.cpp
        test/test_nodes.cpp
        test/nodes_mocks.hpp
        )


add_executable(netsim__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS2} test/main_gtest.cpp)

target_include_directories(netsim__test PUBLIC
        ../../googletest-master/googlemock/include
        ../../googletest-master/googletest/include
        )

target_link_libraries(netsim__test gmock)

