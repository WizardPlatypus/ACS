#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include <string>
#include <forward_list> // single-linked list datastructure

#include "operations.cpp" // my operation templates

#define x10(ex) ex; ex; ex; ex; ex; ex; ex; ex; ex; ex

template<typename T>
union Magic {
    uint64_t source;
    T view;
};

using time_unit = std::chrono::microseconds;

template<typename T>
time_unit test_operation(const std::function<T(const T&, const T&)> operation,/* uint64_t& count,*/ uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    Magic<T> left, right;
    T result;
    // count = 0;

    uint64_t overflow = (uint64_t)1 << (sizeof(T)*8); // 0001 -> 1'0000 // -x> 0'1111

    start = std::chrono::high_resolution_clock::now();
    while (repeat--) {
        for (left.source = 0; !(left.source & overflow); left.source += do_one_over) {
            for (right.source = left.source; !(right.source & overflow); right.source += do_one_over) {
                x10(result = operation(left.view, right.view));
                // count++;
            }
        }
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<>
time_unit test_operation(const std::function<uint64_t(const uint64_t&, const uint64_t&)> operation,/* uint64_t& count,*/ uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    Magic<uint64_t> left, right;
    uint64_t result;
    // count = 0;

    start = std::chrono::high_resolution_clock::now();
    while (repeat--) {
        left.source = 0;
        do {
            right.source = left.source;
            do {
                x10(result = operation(left.view, right.view));
                if (right.source + do_one_over < right.source) {
                    break;
                }
                right.source += do_one_over;
            } while(1);
            if (left.source + do_one_over < left.source) {
                break;
            }
            left.source += do_one_over;
        } while(1);
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<>
time_unit test_operation(const std::function<int64_t(const int64_t&, const int64_t&)> operation,/* uint64_t& count,*/ uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    Magic<int64_t> left, right;
    int64_t result;
    // count = 0;

    start = std::chrono::high_resolution_clock::now();
    while (repeat--) {
        left.source = 0;
        do {
            right.source = left.source;
            do {
                x10(result = operation(left.view, right.view));
                if (right.source + do_one_over < right.source) {
                    break;
                }
                right.source += do_one_over;
            } while(1);
            if (left.source + do_one_over < left.source) {
                break;
            }
            left.source += do_one_over;
        } while(1);
    }
    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<>
time_unit test_operation(const std::function<double(const double&, const double&)> operation,/* uint64_t& count,*/ uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    Magic<double> left, right;
    double result;
    // count = 0;

    start = std::chrono::high_resolution_clock::now();
    while (repeat--) {
        left.source = 0;
        do {
            right.source = left.source;
            do {
                x10(result = operation(left.view, right.view));
                if (right.source + do_one_over < right.source) {
                    break;
                }
                right.source += do_one_over;
            } while(1);
            if (left.source + do_one_over < left.source) {
                break;
            }
            left.source += do_one_over;
        } while(1);
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
    std::cout << "type,size,operation,time,do_one_over,repeat" << std::endl;

    /*
    type_test_wrapper<uint8_t>("uint8_t", 0, (uint64_t)1 << 8); // 1, 1 << 12
    type_test_wrapper<int8_t>("int8_t", 0, (uint64_t)1 << 12);

    type_test_wrapper<uint16_t>("uint16_t", 1, 2); // 4, 1
    type_test_wrapper<int16_t>("int16_t", 1, 2);

    uint64_t do_32_over = 47'768; // 261'641
    type_test_wrapper<uint32_t>("uint32_t", do_32_over, 1);
    type_test_wrapper<int32_t>("int32_t", do_32_over, 1);
    type_test_wrapper<float>("float", do_32_over, 1);
    //*/

    uint64_t do_64_over = 5'123'741'531'702'044;//205'161'087'223'729;
    type_test_wrapper<uint64_t>("uint64_t", do_64_over, 1);
    //type_test_wrapper<int64_t>("int64_t", do_64_over, 1);
    //type_test_wrapper<double>("double", do_64_over, 1);
    //*/

    // std::cout << "count = repeat * (2^bits / do_one_over + 1) * (2^bits / do_one_over) / 2" << std::endl;
    // std::cout << "do_one_over = repeat * 2^(bits - 2) * (1 + (1 + 8*count/repeat)^0.5) / count" << std::endl;

    return 0;
}