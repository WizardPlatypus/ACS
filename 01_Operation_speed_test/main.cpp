#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include <cstring>
#include <string>
#include <forward_list> // single-linked list datastructure

#define OP_NOTHING 0
#define OP_ADD 1
#define OP_SUBTRACT 2
#define OP_MULTIPLY 3
#define OP_DIVIDE 4
// #define OP_MODULO 5

// #define OP_TYPE OP_ADD

#if OP_TYPE == OP_NOTHING
#define OP(a, b) (0)
#define OP_LABEL ("nothing")
#elif OP_TYPE == OP_ADD
#define OP(a, b) ((a) + (b))
#define OP_LABEL ("add")
#elif OP_TYPE == OP_SUBTRACT
#define OP(a, b) ((a) - (b))
#define OP_LABEL ("subtract")
#elif OP_TYPE == OP_MULTIPLY
#define OP(a, b) ((a) * (b))
#define OP_LABEL ("multiply")
#elif OP_TYPE == OP_DIVIDE
#define OP(a, b) ((b) == 0 ? 0 : (a) / (b))
#define OP_LABEL ("divide")
//#elif OP_TYPE == OP_MODULO
//#define OP(a, b) ((b) == 0 ? 0 : (a) % (b))
//#define OP_LABEL ("modulo")
#endif

#define x10(ex) ex; ex; ex; ex; ex; ex; ex; ex; ex; ex
#define x100(ex) x10(x10(ex))
#define x1000(ex) x10(x10(x10(ex)))
#define xtimes(ex) x1000(ex);

template<typename T>
union Magic {
    uint64_t source;
    T view;
};

using time_unit = std::chrono::microseconds;

template<typename T>
time_unit test_operation(uint64_t do_one_over, uint64_t repeat, T _type_value = 0) {
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
                //x10(result = operation(left.view, right.view));
                xtimes(result = OP(left.view, right.view));
                // count++;
            }
        }
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<>
time_unit test_operation(uint64_t do_one_over, uint64_t repeat, uint64_t _type_value) {
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
                xtimes(result = OP(left.view, right.view));
                if (right.source > (right.source += do_one_over)) {
                    break;
                }
            } while(1);
            if (left.source > (left.source += do_one_over)) {
                break;
            }
        } while(1);
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<>
time_unit test_operation(uint64_t do_one_over, uint64_t repeat, int64_t _type_value) {
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
                xtimes(result = OP(left.view, right.view));
                if (right.source > (right.source += do_one_over)) {
                    break;
                }
            } while(1);
            if (left.source > (left.source += do_one_over)) {
                break;
            }
        } while(1);
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<>
time_unit test_operation(uint64_t do_one_over, uint64_t repeat, double _type_value) {
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
                xtimes(result = OP(left.view, right.view));
                if (right.source > (right.source += do_one_over)) {
                    break;
                }
            } while(1);
            if (left.source > (left.source += do_one_over)) {
                break;
            }
        } while(1);
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void operation_test_wrapper(const std::string& type_label, const uint64_t& do_one_over, const uint64_t& repeat) {
    auto time = test_operation<T>(do_one_over, repeat).count();

    // type, sizeof, operation, time, do_one_over, repeat
    std::forward_list<std::string> data {
        type_label,
        std::to_string(sizeof(T) * 8),
        std::to_string(do_one_over),
        std::to_string(repeat),
        OP_LABEL,
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
    operation_test_wrapper<T>(type_label, do_one_over, repeat);
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

    return 0;
}