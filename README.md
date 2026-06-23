# DataStructures

A modern **header-only C++26 library** for data structures and algorithms.

> **⚠️ Under active development**
>
> This project is currently in its early stages. At the moment, only a dynamic array implementation is available. Additional data structures and algorithms will be added over time.

### Implemented

* Dynamic Array (`dsa::array`)

---

## Integration

It is recommended to integrate **DataStructures** into your project as a Git submodule.

## Example

```cpp
#include <dsa/array.hpp>

#include <iostream>

int main() {

    dsa::array<int> arr;

    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);

    for (const auto& value : arr)
        std::cout << value << ' ';
    std::cout << '\n';
}

```



## Building

Although **DataStructures** is a header-only library and does not require compilation, the repository provides examples and tests that can be built using CMake.

### Clone the Repository

```bash
git clone https://github.com/Bikash-Samanta/DataStructures.git

cd DataStructures
```

### Configure

```bash
cmake -B build
```

### Build

```bash
cmake --build build
```

### Run Tests

```bash
./build/tests/ds_tests
