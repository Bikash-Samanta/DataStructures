#include <dsa/array.hpp>
#include <cassert>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

struct TestResult {
    std::string test_name;
    double duration_ms;
    long long operations;

    double ops_per_second() const {
        return duration_ms > 0.0 ? (operations / duration_ms) * 1000.0 : 0.0;
    }
};

void print_result(const TestResult& result) {
    std::cout << std::left << std::setw(35) << result.test_name
              << std::setw(15) << std::fixed << std::setprecision(3) << result.duration_ms << " ms"
              << "  " << std::scientific << std::setprecision(3) << result.ops_per_second() << " ops/s"
              << std::endl;
}

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << std::endl;
        std::terminate();
    }
}

struct Counted {
    static int constructions;
    static int copies;
    static int moves;
    static int destructions;

    int value;

    Counted(int v = 0) noexcept : value(v) { ++constructions; }
    Counted(const Counted& other) noexcept : value(other.value) { ++copies; }
    Counted(Counted&& other) noexcept : value(other.value) { ++moves; other.value = -1; }
    ~Counted() noexcept { ++destructions; }

    Counted& operator=(const Counted& other) noexcept {
        value = other.value;
        ++copies;
        return *this;
    }

    Counted& operator=(Counted&& other) noexcept {
        value = other.value;
        other.value = -1;
        ++moves;
        return *this;
    }

    bool operator==(const Counted& other) const noexcept {
        return value == other.value;
    }
};

int Counted::constructions = 0;
int Counted::copies = 0;
int Counted::moves = 0;
int Counted::destructions = 0;

void validate_integer_edge_cases() {
    std::cout << "\n=== Integer edge-case validation ===" << std::endl;
    dsa::array<int> arr;
    expect(arr.size() == 0, "Default constructor should create empty array");

    arr.push_back(1);
    arr.push_back(2);
    expect(arr.size() == 2, "Size should update after push_back");
    expect(arr[0] == 1 && arr[1] == 2, "push_back should preserve values");

    arr.pop_back();
    expect(arr.size() == 1, "pop_back should shrink size");
    expect(arr[0] == 1, "Remaining element should be preserved after pop_back");

    arr.reserve(16);
    expect(arr.capacity() >= 16, "reserve should increase capacity");

    arr.resize(4, 7);
    expect(arr.size() == 4, "resize should expand size");
    expect(arr[1] == 7 && arr[2] == 7 && arr[3] == 7, "resize fill value should be assigned");

    arr.resize(2);
    expect(arr.size() == 2, "resize down should shrink size");
    expect(arr[0] == 1 && arr[1] == 7, "resize down should preserve first elements");

    arr.insert(1, 42);
    expect(arr.size() == 3, "insert should increase size");
    expect(arr[1] == 42, "insert should place value at position");

    arr.erase(1);
    expect(arr.size() == 2, "erase should decrease size");
    expect(arr[1] == 7, "erase should remove element in the middle");

    const dsa::array<int> const_arr = arr;
    expect(const_arr[0] == 1, "const operator[] should work");
}

void validate_custom_type_semantics() {
    std::cout << "\n=== Custom-type semantics validation ===" << std::endl;

    Counted::constructions = Counted::copies = Counted::moves = Counted::destructions = 0;

    dsa::array<Counted> arr;
    arr.push_back(Counted(10));
    arr.push_back(Counted(20));
    expect(arr.size() == 2, "Counted array should contain two elements");
    expect(arr[0].value == 10 && arr[1].value == 20, "Counted values should match inserted values");

    dsa::array<Counted> copy = arr;
    expect(copy.size() == 2, "Copy constructor should preserve size");
    expect(copy[1].value == 20, "Copy constructor should preserve element values");

    dsa::array<Counted> moved = std::move(arr);
    expect(moved.size() == 2, "Move constructor should preserve size");

    expect(Counted::moves > 0 || Counted::copies > 0, "Counted type should be moved or copied during operations");
}

void validate_iterator_and_addition() {
    std::cout << "\n=== Iterator and addition validation ===" << std::endl;

    dsa::array<int> arr = {1, 2, 3};
    auto it = arr.begin();
    expect(*it == 1, "begin iterator should point to first element");
    ++it;
    expect(*it == 2, "iterator increment should work");

    dsa::array<int> repeated = arr + 9;
    expect(repeated.size() == 4 && repeated[3] == 9, "operator+ should append value");

    dsa::array<int> combined = arr + repeated;
    expect(combined.size() == 7, "operator+ should combine sizes");
    expect(combined[0] == 1 && combined[3] == 1 && combined[6] == 9, "operator+ should combine values correctly");
}

template <typename F>
TestResult benchmark(const std::string& name, F func, size_t operations, size_t repeats = 5) {
    double total_ms = 0.0;
    for (size_t i = 0; i < repeats; ++i) {
        auto start = Clock::now();
        func();
        auto end = Clock::now();
        total_ms += std::chrono::duration<double, std::milli>(end - start).count();
    }
    return TestResult{name, total_ms / static_cast<double>(repeats), static_cast<long long>(operations)};
}

TestResult benchmark_push_back(size_t n, bool reserve_vector = false) {
    return benchmark("alpha::Array push_back(" + std::to_string(n) + ")", [n] {
        dsa::array<int> arr;
        for (size_t i = 0; i < n; ++i) {
            arr.push_back(static_cast<int>(i));
        }
    }, n);
}

TestResult benchmark_vector_push_back(size_t n, bool reserve_vector = false) {
    return benchmark("std::vector push_back(" + std::to_string(n) + (reserve_vector ? ", reserve" : "") + ")", [n, reserve_vector] {
        std::vector<int> vec;
        auto it = vec.begin();
        if (reserve_vector) vec.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            vec.push_back(static_cast<int>(i));
        }
    }, n);
}

TestResult benchmark_random_access(size_t n) {
    dsa::array<int> arr;
    for (size_t i = 0; i < n; ++i) {
        arr.push_back(static_cast<int>(i));
    }
    return benchmark("alpha::Array random_access(" + std::to_string(n) + ")", [n, &arr] {
        volatile int x = 0;
        for (size_t i = 0; i < n; ++i) {
            x += arr[i];
        }
    }, n);
}

TestResult benchmark_iteration(size_t n) {
    dsa::array<int> arr;
    for (size_t i = 0; i < n; ++i) {
        arr.push_back(static_cast<int>(i));
    }
    return benchmark("alpha::Array iteration(" + std::to_string(n) + ")", [n, &arr] {
        volatile long long sum = 0;
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            sum += *it;
        }
    }, n);
}

TestResult benchmark_copy(size_t n) {
    dsa::array<int> arr;
    for (size_t i = 0; i < n; ++i) arr.push_back(static_cast<int>(i));
    return benchmark("alpha::Array copy(" + std::to_string(n) + ")", [&arr] {
        dsa::array<int> copy = arr;
    }, n);
}

TestResult benchmark_move(size_t n) {
    dsa::array<int> arr;
    for (size_t i = 0; i < n; ++i) arr.push_back(static_cast<int>(i));
    return benchmark("alpha::Array move(" + std::to_string(n) + ")", [&arr] {
        dsa::array<int> moved = std::move(arr);
    }, n);
}

int main() {
    std::cout << "===============================================" << std::endl;
    std::cout << "      Alpha Array Full Validation & Benchmark" << std::endl;
    std::cout << "===============================================" << std::endl;

    validate_integer_edge_cases();
    validate_custom_type_semantics();
    validate_iterator_and_addition();

    std::cout << "\n=== Performance Benchmarks ===" << std::endl;
    std::cout << std::left << std::setw(35) << "Test" << std::setw(15) << "Time" << "Throughput" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    const std::vector<size_t> sizes = {1000, 10000, 100000, 1000000};
    for (size_t n : sizes) {
        print_result(benchmark_push_back(n));
        print_result(benchmark_vector_push_back(n, false));
        print_result(benchmark_vector_push_back(n, true));
        std::cout << std::string(80, '-') << std::endl;
    }

    std::cout << "\n=== Access & Iteration Benchmarks ===" << std::endl;
    for (size_t n : sizes) {
        print_result(benchmark_random_access(n));
        print_result(benchmark_iteration(n));
        std::cout << std::string(80, '-') << std::endl;
    }

    std::cout << "\n=== Copy/Move Benchmarks ===" << std::endl;
    for (size_t n : {100000ull, 1000000ull}) {
        print_result(benchmark_copy(n));
        print_result(benchmark_move(n));
        std::cout << std::string(80, '-') << std::endl;
    }

    std::cout << "\nAll tests completed successfully." << std::endl;
    return 0;
}
