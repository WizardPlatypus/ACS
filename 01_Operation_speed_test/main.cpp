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
time_unit test_operation(const std::function<T(const T&, const T&)> operation, uint64_t& count, const uint64_t& skip, uint64_t repeat) {
    std::chrono::high_resolution_clock::time_point start, finish;
    uint64_t source_min, source_max;
    Magic<T> left, right;
    T result;
    count = 0;

    source_min = 0;
    /*
    0001 -> 1000 -> 0111 -> 1110 -> 1111
    */
    source_max = 1;
    source_max <<= (sizeof(T)*8 - 1);
    source_max -= 1;
    source_max <<= 1;
    source_max += 1;

    start = std::chrono::high_resolution_clock::now();
    while (repeat--) {
        left.source = source_min;
        right.source = source_max;
        while (left.source < right.source) {
            result = operation(left.view, right.view);
            left.source += 1 + skip;
            right.source -= 1 + skip;
            count++;
            /*
            if (count % 1000 == 0) {
                std::cerr << left.source << '\t' << right.source << std::endl;
            }
            //*/
        }
    }
    // std::cout << result << std::endl;
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void operation_test_wrapper(const std::string& type_label, const std::string& operation_label, const std::function<T(const T&, const T&)> operation, const uint64_t& skip, const uint64_t& repeat) {
    uint64_t count = -1;
    auto time = test_operation<T>(operation, count, skip, repeat).count();

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
void type_test_wrapper(const std::string& type_label, const uint64_t& skip, const uint64_t& repeat = 1) {
    operation_test_wrapper<T>(type_label, "nothing", operations::nothing<T>, skip, repeat);
    operation_test_wrapper<T>(type_label, "add", operations::add<T>, skip, repeat);
    operation_test_wrapper<T>(type_label, "subtract", operations::subtract<T>, skip, repeat);
    operation_test_wrapper<T>(type_label, "multiply", operations::multiply<T>, skip, repeat);
    operation_test_wrapper<T>(type_label, "divide", operations::divide<T>, skip, repeat);
    operation_test_wrapper<T>(type_label, "modulo", operations::modulo<T>, skip, repeat);
}

int main() {
    // type_test_wrapper<int>("int", 50);
    // type_test_wrapper<unsigned int>("unsigned int", 50);
    // type_test_wrapper<float>("float", 100);
    // type_test_wrapper<double>("double", 10 * ((uint64_t)1 << 32));
    /*
    type_test_wrapper<uint8_t>("uint8_t", 0);
    std::cout << std::endl;
    type_test_wrapper<int8_t>("uint8_t", 0);
    std::cout << std::endl;
    //*/
    type_test_wrapper<uint16_t>("uint16_t", 0, 1 << 12);
    std::cout << std::endl;
    type_test_wrapper<int16_t>("int16_t", 0, 1 << 12);
    std::cout << std::endl;
    /*
    type_test_wrapper<uint32_t>("uint32_t", 0);
    std::cout << std::endl;
    type_test_wrapper<int32_t>("int32_t", 0);
    std::cout << std::endl;
    //*/

    return 0;
}