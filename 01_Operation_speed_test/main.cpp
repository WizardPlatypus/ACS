#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include "operations.cpp" // my operation templates

template<typename T>
std::chrono::milliseconds test_operation(std::function<T(const T&, const T&)> operation, T begin, T end, T step = 1) {
    size_t execution_count = 0;
    std::chrono::high_resolution_clock::time_point start, finish;
    T result;

    start = std::chrono::high_resolution_clock::now();
    for (int a = begin; a <= end; a += step) {
        for (int b = begin; b <= end; b += step) {
            result = operation(a, b);
            execution_count++;
        }
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
}

int main() {
    int begin = 0, end = 10'000, step = 1;
    size_t count = (end - begin + 1) / step;
    count *= count;

    auto add_time = test_operation<int>(add<int>, begin, end, step).count();

    auto nothing_time = test_operation<int>(nothing<int>, begin, end, step).count();

    std::cout << "Count: " << count << std::endl;
    std::cout << "Add: " << add_time << " ms" << std::endl;
    std::cout << "Nothing: " << nothing_time << " ms" << std::endl;
    std::cout << "Time difference is " << add_time - nothing_time << " ms" << std::endl;

    return 0;
}