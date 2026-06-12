cmake_minimum_required(VERSION 3.14)
project(DataStructures VERSION 1.0.0 LANGUAGES CXX)

# Require C++26 for clean template manipulation and performance features
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------
# This is the magic line for portability. The INTERFACE keyword tells CMake that this library doesn't compile into a binary file (like a .so or .a file).
# It consists entirely of header/template source code that will be compiled directly inside whatever project includes it.
# -------------------------------------------------------------------
add_library(datastructures INTERFACE)

# Expose the include/ directory to any project linking against this target
# target_include_directories(...): This tells CMake where your header files (like array.hpp) live.
# $<BUILD_INTERFACE:${...}>: This is a CMake generator expression.
# It tells CMake: "As long as this project is sitting in its local folder on this hard drive,
# any external project looking at it can automatically read the headers inside the /include folder."
# You don't have to manually point to include paths inside Other Projects later.

target_include_directories(datastructures INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# add_executable(ds_tests ...): Creates a completely separate, local runnable program called ds_tests.
# It compiles your testing code (tests/test_array.cpp) so you can isolate and stress-test your arrays before using them in neural networks.
add_executable(ds_tests tests/test_array.cpp)

# target_link_libraries(...): This instructs CMake to connect your test executable (ds_tests) with your core data structure library (datastructures).
# Because of the INTERFACE setup above, the compiler will automatically feed the test runner your /include directory path.
target_link_libraries(ds_tests PRIVATE datastructures)