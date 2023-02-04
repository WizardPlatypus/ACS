#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include <string>
#include <forward_list> // single-linked list datastructure

#include "operations.cpp" // my operation templates

using time_unit = std::chrono::milliseconds;


template<typename T>
time_unit test_operation(const std::function<T(const T, const T)>& operation, const T begin, const T end, const T step = 1) {
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

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void operation_test_wrapper(const std::string type_label, const std::string operation_label, const int count, const std::function<T(const T, const T)>& operation, const T begin, const T end, const T step = 1) {
    auto time = test_operation<T>(operation, begin, end, step).count();

    // type, operation, count, time
    std::forward_list<std::string> data {
        type_label,
        operation_label,
        std::to_string(count),
        std::to_string(time),
    };

    std::cout << data.front();
    data.pop_front();
    for (auto item: data) {
        std::cout << "," << item;
    }
    std::cout << std::endl;
}

int main() {
    int begin = 0, end = 10'000, step = 1;
    size_t count = (end - begin + 1) / step;
    count *= count;

    operation_test_wrapper<int>("int", "add", count, add<int>, begin, end, step);

    return 0;
}