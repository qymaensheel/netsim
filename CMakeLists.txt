cmake_minimum_required(VERSION 3.15)
project(netsim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/factory.cpp
        src/simulation.cpp
        src/helpers.cpp)

add_executable(netsim main.cpp)

add_executable(netsim__debug ${SOURCE_FILES} main.cpp)


set(SOURCES_FILES_TESTS
        #test/test_factory_io.cpp
        #test/test_Factory.cpp
        #test/test_nodes.cpp
        #test/test_package.cpp
        test/test_simulate.cpp
        #test/test_storage_types.cpp

        )

add_executable(netsim__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)
target_compile_definitions(netsim__test PUBLIC EXERCISE_ID=4)

target_include_directories(netsim__test PUBLIC
        ../googletest-master/googlemock/include
        )

add_subdirectory(../googletest-master googletest-master)

target_link_libraries(netsim__test gmock)


