#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include <string>
#include <forward_list> // single-linked list datastructure
#include <limits>

#include "operations.cpp" // my operation templates

using time_unit = std::chrono::microseconds;

template<typename T>
union Magic {
    uint64_t source;
    T view;
};

template<typename T>
time_unit test_operation(const std::function<T(const T&, const T&)> operation, const uint64_t& skip, size_t& count) {
    std::chrono::high_resolution_clock::time_point start, finish;
    Magic<T> left, right;
    T result;
    count = 0;
    bool coin = true;

    left.source = 0;
    /*
    0001 -> 1000 -> 0111 -> 1110 -> 1111
    */
    right.source = 1;
    right.source <<= (sizeof(T)*8 - 1);
    right.source -= 1;
    right.source <<= 1;
    right.source += 1;

    // std::cout << left.source << ' ' << right.source << std::endl;

    start = std::chrono::high_resolution_clock::now();
    while (left.source < right.source) {
        result = operation(left.view, right.view);
        if (coin) {
            left.source += 1 + skip;
        } else {
            right.source -= 1 + skip;
        }
        coin = !coin;
        count++;
        /*
        if (count % 1000 == 0) {
            std::cerr << left.source << '\t' << right.source << std::endl;
        }
        //*/
    }
    // std::cout << result << std::endl;
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void operation_test_wrapper(const std::string& type_label, const std::string& operation_label, const std::function<T(const T&, const T&)> operation, const uint64_t& skip) {
    size_t count = -1;
    auto time = test_operation<T>(operation, skip, count).count();

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
void type_test_wrapper(const std::string& type_label, const uint64_t& skip) {
    operation_test_wrapper<T>(type_label, "nothing", operations::nothing<T>, skip);
    operation_test_wrapper<T>(type_label, "add", operations::add<T>, skip);
    operation_test_wrapper<T>(type_label, "subtract", operations::subtract<T>, skip);
    operation_test_wrapper<T>(type_label, "multiply", operations::multiply<T>, skip);
    operation_test_wrapper<T>(type_label, "divide", operations::divide<T>, skip);
    operation_test_wrapper<T>(type_label, "modulo", operations::modulo<T>, skip);
}

int main() {
    // type_test_wrapper<int>("int", 50);
    // type_test_wrapper<unsigned int>("unsigned int", 50);
    // type_test_wrapper<float>("float", 100);
    type_test_wrapper<double>("double", 10 * ((uint64_t)1 << 32));

    return 0;
}