#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include <string>
#include <forward_list> // single-linked list datastructure
#include <limits>

#include "operations.cpp" // my operation templates

using time_unit = std::chrono::microseconds;


template<typename T>
time_unit test_operation(const std::function<T(const T, const T)>& operation, const T begin, const T end, const T step, size_t& count) {
    std::chrono::high_resolution_clock::time_point start, finish;
    T result, left = begin, right = end;
    count = 0;
    bool coin = false;

    start = std::chrono::high_resolution_clock::now();
    while (left < right) {
        // std::cerr << left << ' ';
        result = operation(left, right);
        if (coin) {
            left += step;
        } else {
            right -= step;
        }
        coin = !coin;
        count++;
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void operation_test_wrapper(const std::string type_label, const std::string operation_label, const std::function<T(const T, const T)>& operation, const T begin, const T end, const T step) {
    size_t count = -1;
    auto time = test_operation<T>(operation, begin, end, step, count).count();

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

template<typename T>
void type_test_wrapper(const std::string type_label, const T begin, const T end, const T step) {
    operation_test_wrapper<T>(type_label, "nothing", nothing<int>, begin, end, step);
    operation_test_wrapper<T>(type_label, "add", add<int>, begin, end, step);
    operation_test_wrapper<T>(type_label, "subtract", subtract<int>, begin, end, step);
    operation_test_wrapper<T>(type_label, "multiply", multiply<int>, begin, end, step);
    // operation_test_wrapper<T>(type_label, "divide", divide<int>, count, begin, end, step);
    // operation_test_wrapper<T>(type_label, "modulo", modulo<int>, count, begin, end, step);
}

int main() {
    // type_test_wrapper<int>("int", std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 10);
    type_test_wrapper<double>("double", (double)std::numeric_limits<int>::min(), (double)std::numeric_limits<int>::max(), 10) ;

    return 0;
}