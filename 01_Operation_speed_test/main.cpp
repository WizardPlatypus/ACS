#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include <string>
#include <forward_list> // single-linked list datastructure

#include "operations.cpp" // my operation templates

using time_unit = std::chrono::microseconds;

template<typename T>
union Magic {
    uint64_t source;
    T view;
};

template<typename T>
time_unit test_operation(const std::function<T(const T&, const T&)> operation,/* uint64_t& count,*/ uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    uint64_t source_min, source_max;
    Magic<T> left, right;
    T result;
    // count = 0;

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
            // this might happen because of overflow
            if (left.source != 0 && left.source - do_one_over > left.source) {
                // on first iteration that if would always pass ^^^
                break;
            }
            for (right.source = left.source; right.source <= source_max; right.source += do_one_over) {
                // this might happen because of overflow
                if (right.source != 0 && right.source - do_one_over > right.source) {
                    // on first iteration that if would always pass ^^^
                    break;
                }
                result = operation(left.view, right.view);
                // count++;
            }
        }
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void operation_test_wrapper(const std::string& type_label, const std::string& operation_label, const std::function<T(const T&, const T&)> operation, const uint64_t& do_one_over, const uint64_t& repeat) {
    // uint64_t count = -1;
    auto time = test_operation<T>(operation,/* count,*/ do_one_over, repeat).count();

    // type, sizeof, operation, time, do_one_over, repeat
    std::forward_list<std::string> data {
        type_label,
        std::to_string(sizeof(T) * 8),
        operation_label,
        std::to_string(time),
        std::to_string(do_one_over),
        std::to_string(repeat),
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
    std::cout << std::endl;
}

int main() {
    // count = repeat * (2^bits / do_one_over + 1) * (2^bits / do_one_over) / 2
    std::cout << "type,size,operation,time,do_one_over,repeat" << std::endl;

    //type_test_wrapper<uint8_t>("uint8_t", 1, 1 << 12);
    //type_test_wrapper<uint16_t>("uint16_t", 4, 1);
    //type_test_wrapper<uint32_t>("uint32_t", 261'641, 1);
    type_test_wrapper<uint64_t>("uint64_t", 1'123'741'531'702'044, 1);

    std::cout << "count = repeat * (2^bits / do_one_over + 1) * (2^bits / do_one_over) / 2" << std::endl;
    std::cout << "do_one_over = repeat * 2^(bits - 2) * (1 + (1 + 8*count/repeat)^0.5) / count" << std::endl;

    return 0;
}