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
time_unit test_operation(const std::function<T(const T&, const T&)> operation, uint64_t& count, uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

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
        for (left.source = source_min; left.source <= source_max; left.source += do_one_over) {
            for (right.source = left.source; right.source <= source_max; right.source += do_one_over) {
                result = operation(left.view, right.view);
                count++;
                /*
                if (count % 1000 == 0) {
                    std::cerr << left.source << '\t' << right.source << std::endl;
                }
                //*/
            }
        }
    }
    // std::cout << result << std::endl;
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void operation_test_wrapper(const std::string& type_label, const std::string& operation_label, const std::function<T(const T&, const T&)> operation, const uint64_t& do_one_over, const uint64_t& repeat) {
    uint64_t count = -1;
    auto time = test_operation<T>(operation, count, do_one_over, repeat).count();

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
void type_test_wrapper(const std::string& type_label, const uint64_t& do_one_over, const uint64_t& repeat) {
    operation_test_wrapper<T>(type_label, "nothing", operations::nothing<T>, do_one_over, repeat);
    operation_test_wrapper<T>(type_label, "add", operations::add<T>, do_one_over, repeat);
    operation_test_wrapper<T>(type_label, "subtract", operations::subtract<T>, do_one_over, repeat);
    operation_test_wrapper<T>(type_label, "multiply", operations::multiply<T>, do_one_over, repeat);
    operation_test_wrapper<T>(type_label, "divide", operations::divide<T>, do_one_over, repeat);
    operation_test_wrapper<T>(type_label, "modulo", operations::modulo<T>, do_one_over, repeat);
}

int main() {
    //*
    type_test_wrapper<uint8_t>("uint8_t", 1, 1 << 12);
    std::cout << std::endl;
    //type_test_wrapper<int8_t>("uint8_t", 0, 1 << 12);
    //std::cout << std::endl;
    //*/
    /*
    type_test_wrapper<uint16_t>("uint16_t", 0);
    std::cout << std::endl;
    type_test_wrapper<int16_t>("int16_t", 0);
    std::cout << std::endl;
    //*/
    /*
    type_test_wrapper<uint32_t>("uint32_t", 0);
    std::cout << std::endl;
    type_test_wrapper<int32_t>("int32_t", 0);
    std::cout << std::endl;
    //*/

    return 0;
}