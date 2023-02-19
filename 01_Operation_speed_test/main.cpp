#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include <cstring>
#include <string>
#include <forward_list> // single-linked list datastructure

#include "operations.cpp" // my operation templates

#define x10(ex) ex; ex; ex; ex; ex; ex; ex; ex; ex; ex
#define x100(ex) x10(x10(ex))
#define x1000(ex) x10(x10(x10(ex)))
#define xtimes(ex) x1000(ex); x1000(ex)

template<typename T>
union Magic {
    uint64_t source;
    T view;
};

using time_unit = std::chrono::microseconds;

template<typename T>
long long test_operation(const std::function<T(const T&, const T&)> operation,/* uint64_t& count,*/ uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    long long time{0};
    Magic<T> left, right;
    T result;

    uint64_t overflow = (uint64_t)1 << (sizeof(T)*8); // 0001 -> 1'0000 // -x> 0'1111

    while (repeat--) {
        for (left.source = 0; !(left.source & overflow); left.source += do_one_over) {
            for (right.source = left.source; !(right.source & overflow); right.source += do_one_over) {
                start = std::chrono::high_resolution_clock::now();
                xtimes(result = operation(left.view, right.view));
                finish = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<time_unit>(finish - start).count();
            }
        }
    }

    return time;
}

template<>
long long test_operation(const std::function<uint64_t(const uint64_t&, const uint64_t&)> operation, uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    long long time{0};
    Magic<uint64_t> left, right;
    uint64_t result;

    while (repeat--) {
        left.source = 0;
        do {
            right.source = left.source;
            do {
                start = std::chrono::high_resolution_clock::now();
                xtimes(result = operation(left.view, right.view));
                finish = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<time_unit>(finish - start).count();
                if (right.source > (right.source += do_one_over)) {
                    break;
                }
            } while(1);
            if (left.source > (left.source += do_one_over)) {
                break;
            }
        } while(1);
    }

    return time;
}

template<>
long long test_operation(const std::function<int64_t(const int64_t&, const int64_t&)> operation, uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    long long time{0};
    Magic<int64_t> left, right;
    int64_t result;

    while (repeat--) {
        left.source = 0;
        do {
            right.source = left.source;
            do {
                start = std::chrono::high_resolution_clock::now();
                xtimes(result = operation(left.view, right.view));
                finish = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<time_unit>(finish - start).count();
                if (right.source > (right.source += do_one_over)) {
                    break;
                }
            } while(1);
            if (left.source > (left.source += do_one_over)) {
                break;
            }
        } while(1);
    }

    return time;
}

template<>
long long test_operation(const std::function<double(const double&, const double&)> operation, uint64_t do_one_over, uint64_t repeat) {
    if (do_one_over < 1) {
        do_one_over = 1;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    long long time{0};
    Magic<double> left, right;
    double result;

    while (repeat--) {
        left.source = 0;
        do {
            right.source = left.source;
            do {
                start = std::chrono::high_resolution_clock::now();
                xtimes(result = operation(left.view, right.view));
                finish = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<time_unit>(finish - start).count();
                if (right.source > (right.source += do_one_over)) {
                    break;
                }
            } while(1);
            if (left.source > (left.source += do_one_over)) {
                break;
            }
        } while(1);
    }

    return time;
}

template<typename T>
void operation_test_wrapper(const std::string& type_label, const std::string& operation_label, const std::function<T(const T&, const T&)> operation, const uint64_t& do_one_over, const uint64_t& repeat) {
    auto time = test_operation<T>(operation,/* count,*/ do_one_over, repeat);

    // type, sizeof, operation, time, do_one_over, repeat
    std::forward_list<std::string> data {
        type_label,
        std::to_string(sizeof(T) * 8),
        std::to_string(do_one_over),
        std::to_string(repeat),
        operation_label,
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

int main(int argc, const char *argv[]) {

    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];

        if (!strcmp(arg, "labels")) {
            std::cout << "type,size,do_one_over,repeat,operation,time" << std::endl;
            continue;
        }
        if (!strcmp(arg, "x")) {
            std::cout << "x = 1000" << std::endl;
            continue;
        }

        if (!strcmp(arg, "uint8_t")) {
            type_test_wrapper<uint8_t>(arg, 1, 48);
            continue;
        }
        if (!strcmp(arg, "int8_t")) {
            type_test_wrapper<int8_t>(arg, 1, 48);
            continue;
        }

        const uint64_t do_16_over = 18 * 6;
        if (!strcmp(arg, "uint16_t")) {
            type_test_wrapper<uint16_t>(arg, do_16_over, 1);
            continue;
        }
        if (!strcmp(arg, "int16_t")) {
            type_test_wrapper<int16_t>(arg, do_16_over, 1);
            continue;
        }

        const uint64_t do_32_over = 1'200'481 * 6;
        if (!strcmp(arg, "uint32_t")) {
            type_test_wrapper<uint32_t>(arg, do_32_over, 1);
            continue;
        }
        if (!strcmp(arg, "int32_t")) {
            type_test_wrapper<int32_t>(arg, do_32_over, 1);
            continue;
        }
        if (!strcmp(arg, "float")) {
            type_test_wrapper<float>(arg, do_32_over, 1);
            continue;
        }

        const uint64_t do_64_over = 5'156'024'596'349'061 * 6;
        if (!strcmp(arg, "uint64_t")) {
            type_test_wrapper<uint64_t>(arg, do_64_over, 1);
            continue;
        }
        if (!strcmp(arg, "int64_t")) {
            type_test_wrapper<int64_t>(arg, do_64_over, 1);
            continue;
        }
        if (!strcmp(arg, "double")) {
            type_test_wrapper<double>(arg, do_64_over, 1);
            continue;
        }
    }

    /*
    type_test_wrapper<uint8_t>("uint8_t", 1, 256);
    type_test_wrapper<int8_t>("int8_t", 1, 256);

    const uint64_t do_16_over = 18;
    type_test_wrapper<uint16_t>("uint16_t", do_16_over, 1); // 4, 1
    type_test_wrapper<int16_t>("int16_t", do_16_over, 1);

    const uint64_t do_32_over = 1'200'481;
    type_test_wrapper<uint32_t>("uint32_t", do_32_over, 1);
    type_test_wrapper<int32_t>("int32_t", do_32_over, 1);
    type_test_wrapper<float>("float", do_32_over, 1);

    const uint64_t do_64_over = 5'156'024'596'349'061;
    type_test_wrapper<uint64_t>("uint64_t", do_64_over, 1);
    type_test_wrapper<int64_t>("int64_t", do_64_over, 1);
    type_test_wrapper<double>("double", do_64_over, 1);
    //*/

    // std::cout << "count = repeat * (2^bits / do_one_over + 1) * (2^bits / do_one_over) / 2" << std::endl;
    // std::cout << "do_one_over = repeat * 2^(bits - 2) * (1 + (1 + 8*count/repeat)^0.5) / count" << std::endl;

    return 0;
}