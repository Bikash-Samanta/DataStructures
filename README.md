# DataStructures Library

A modern, high-performance, **header-only C++26 template library** designed for zero-overhead data manipulation. Built specifically to be lean, portable, and easily integrable as a dependency for performance-critical systems like neural networks or physics engines.

[![C++ Version](https://img.shields.io/badge/C%2B%2B-26-blue.svg)](https://en.cppreference.com/w/cpp/compiler_support/26)
[![Build System](https://img.shields.io/badge/CMake-3.14%2B-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-purple.svg)](LICENSE)

---

## Features
* **Header-Only Architecture:** No pre-compilation required. Just include and compile directly into your application.
* **C++26 Native:** Built from the ground up to utilize modern language features, advanced template manipulation, and optimal performance semantics.
---

## Project Architecture
```text
DataStructures/
├── CMakeLists.txt
├── include/               # Public template headers
│   └── array.hpp
└── tests/
    └── test_array.cpp     # Testing logic

```

---

## Local Compilation & Testing

This project uses **CMake** to handle building and compiling the local test suite (`ds_tests`).

### Prerequisites

* A C++26 compliant compiler (GCC 14+, Clang 18+, or MSVC 2022+)
* CMake 3.14 or higher 



### Building the Test Runner

1. Clone or navigate to the directory.
2. Generate the build files and compile using the following standard commands:

```bash
# 1. Generate the build system configuration (Run this ONCE)
cmake -B build -DCMAKE_BUILD_TYPE=Release
# 2. Compile the executable (Run this for all future builds)
cmake --build build

```

### Running the Tests

Once compiled, you can run the standalone test runner executable directly:

```bash
./build/ds_tests

```

---

## Integration Guide (e.g., Incorporating into Another Project)

Because this library exports an `INTERFACE` target and uses the specialized `$<BUILD_INTERFACE:...>` generator expression , integration into downstream executables like **TrainX** is entirely automated.

### Step 1: Add as a Git Submodule

Navigate to your main project directory (e.g., `TrainX`) and attach this repository locally:

```bash
cd /path/to/your/TrainX
git submodule add <repository-url-or-local-path> external/DataStructures

```

### Step 2: Configure your `CMakeLists.txt`

Simply add the subdirectory and link against the exported `datastructures` target inside your main project's `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.15)
project(TrainX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

# 1. Process the submodule's CMake setup
add_subdirectory(external/DataStructures)

add_executable(TrainX main.cpp)

# 2. Link against the library target.
# This automatically forwards all header search paths to TrainX!
target_link_libraries(TrainX PRIVATE datastructures)

```

### Step 3: Clean Includes in Source Code

You can now include headers cleanly inside your source files without messy relative path chains:

```cpp
#include "Array.hpp" // Instantly resolves to the submodule's include path
#include <iostream>

int main() {
    // Your C++26 high-performance logic goes here!
    return 0;
}

```

---
