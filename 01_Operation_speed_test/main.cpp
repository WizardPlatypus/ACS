#include <iostream> // input/output
#include <chrono> // timers
#include <cstring>
#include <string>
#include <forward_list> // single-linked list datastructure

#include "mt19937-64.c" // Mesenne Twister — PRNG

#define OP_NOTHING 0
#define OP_ADD 1
#define OP_SUBTRACT 2
#define OP_MULTIPLY 3
#define OP_DIVIDE 4
// #define OP_MODULO 5

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
#define OP(a, b) ((a) / (b))
#define OP_LABEL ("divide")
//#elif OP_TYPE == OP_MODULO
//#define OP(a, b) ((b) == 0 ? 0 : (a) % (b))
//#define OP_LABEL ("modulo")
#endif

#define x10(ex) ex; ex; ex; ex; ex; ex; ex; ex; ex; ex
#define x100(ex) x10(x10(ex))
#define x1000(ex) x10(x10(x10(ex)))
#define xtimes(ex) x1000(ex); x1000(ex)
#define XTIMES_LABEL ("2000")

template<typename T>
union Magic {
    uint64_t source;
    T view;
};

using time_unit = std::chrono::microseconds;

template<typename T>
time_unit test(uint64_t repeat) {
    std::chrono::high_resolution_clock::time_point start, finish;
    Magic<T> left, right;
    T result;

    start = std::chrono::high_resolution_clock::now();
    while (repeat--) {
        left.source = genrand64_int64();
        right.source = genrand64_int64();
        #if OP_TYPE == OP_DIVIDE
        if (right.view == 0) {
            right.source = 1;
        }
        #endif
        xtimes(result = OP(left.view, right.view));
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void test_wrapper(const std::string& type_label, const uint64_t& repeat) {
    auto time = test<T>(repeat).count();

    std::forward_list<std::string> data {
        type_label,
        std::to_string(sizeof(T) * 8),
        XTIMES_LABEL,
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

int main(int argc, const char *argv[]) {
    unsigned long long init[4]={0x12345ULL, 0x23456ULL, 0x34567ULL, 0x45678ULL}, length=4;
    init_by_array64(init, length);

    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];

        if (!strcmp(arg, "labels")) {
            std::cout << "type,size,x,repeat,operation,time" << std::endl;
            continue;
        }

        const uint64_t repeat = (1 << 16);

        if (!strcmp(arg, "uint8_t")) {
            test_wrapper<uint8_t>(arg, repeat);
            continue;
        }
        if (!strcmp(arg, "int8_t")) {
            test_wrapper<int8_t>(arg, repeat);
            continue;
        }

        const uint64_t do_16_over = 8;
        if (!strcmp(arg, "uint16_t")) {
            test_wrapper<uint16_t>(arg, repeat);
            continue;
        }
        if (!strcmp(arg, "int16_t")) {
            test_wrapper<int16_t>(arg, repeat);
            continue;
        }

        const uint64_t do_32_over = do_16_over << 16;
        if (!strcmp(arg, "uint32_t")) {
            test_wrapper<uint32_t>(arg, repeat);
            continue;
        }
        if (!strcmp(arg, "int32_t")) {
            test_wrapper<int32_t>(arg, repeat);
            continue;
        }
        if (!strcmp(arg, "float")) {
            test_wrapper<float>(arg, repeat);
            continue;
        }

        const uint64_t do_64_over = do_32_over << 32;
        if (!strcmp(arg, "uint64_t")) {
            test_wrapper<uint64_t>(arg, repeat);
            continue;
        }
        if (!strcmp(arg, "int64_t")) {
            test_wrapper<int64_t>(arg, repeat);
            continue;
        }
        if (!strcmp(arg, "double")) {
            test_wrapper<double>(arg, repeat);
            continue;
        }
    }

    return 0;
}