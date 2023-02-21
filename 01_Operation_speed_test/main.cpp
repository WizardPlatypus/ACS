#include <iostream> // input/output
#include <chrono> // timers
#include <string>
#include <forward_list> // single-linked list datastructure
#include <random>
#include <limits>

#define OP_NOTHING 0
#define OP_ADD 1
#define OP_SUBTRACT 2
#define OP_MULTIPLY 3
#define OP_DIVIDE 4
#define OP_MODULO 5

#if OP_TYPE == OP_NOTHING
#define OP(r, a, b) ;
#define OP_LABEL ("nothing")
#elif OP_TYPE == OP_ADD
#define OP(r, a, b) (r = (a) + (b))
#define OP_LABEL ("add")
#elif OP_TYPE == OP_SUBTRACT
#define OP(r, a, b) (r = (a) - (b))
#define OP_LABEL ("subtract")
#elif OP_TYPE == OP_MULTIPLY
#define OP(r, a, b) (r = (a) * (b))
#define OP_LABEL ("multiply")
#elif OP_TYPE == OP_DIVIDE
#define OP(r, a, b) (r = (a) / (b))
#define OP_LABEL ("divide")
#elif OP_TYPE == OP_MODULO
#define OP(r, a, b) (r = (a) % (b))
#define OP_LABEL ("modulo")
#endif

#define x10(ex) ex; ex; ex; ex; ex; ex; ex; ex; ex; ex
#define x100(ex) x10(x10(ex))
#define x1000(ex) x10(x10(x10(ex)))
#define xtimes(ex) x1000(ex); x1000(ex)
#define XTIMES_LABEL ("2000")

using time_unit = std::chrono::microseconds;

template<typename T>
time_unit test_int(uint64_t repeat) {
    // fiddling with random number generation
    // code from cppreference/.../uniform_int_distribution
    std::random_device device;
    std::mt19937_64 gen(device());
    std::uniform_int_distribution<T> distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

    std::chrono::high_resolution_clock::time_point start, finish;
    T left, right;
    T result;

    start = std::chrono::high_resolution_clock::now();
    while (repeat--) {
        left = distribution(gen);
        right = distribution(gen);
        #if OP_TYPE == OP_DIVIDE || OP_TYPE == OP_MODULO
        if (right == 0) {
            right = 1;
        }
        #endif
        xtimes(OP(result, left, right));
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
time_unit test_real(uint64_t repeat) {
    // fiddling with random number generation
    // code from cppreference/.../uniform_real_distribution
    std::random_device device;
    std::mt19937_64 gen(device());
    std::uniform_real_distribution<T> distribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

    std::chrono::high_resolution_clock::time_point start, finish;
    T left, right;
    T result;

    start = std::chrono::high_resolution_clock::now();
    while (repeat--) {
        left = distribution(gen);
        right = distribution(gen);
        xtimes(OP(result, left, right));
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<time_unit>(finish - start);
}

template<typename T>
void test_int_wrapper(const std::string& type_label, const uint64_t& repeat) {
    auto time = test_int<T>(repeat).count();

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

template<typename T>
void test_real_wrapper(const std::string& type_label, const uint64_t& repeat) {
    #if OP_TYPE == OP_MODULO
    auto time = 0;
    #else
    auto time = test_real<T>(repeat).count();
    #endif

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

    for (int i = 1; i < argc; i++) {
        auto arg = std::string(argv[i]);

        if (!arg.compare("labels")) {
            std::cout << "type,size,x,repeat,operation,time" << std::endl;
            continue;
        }

        // const uint64_t repeat = (1ULL << 24);
        const uint64_t repeat = (1ULL << 12);

        if (!arg.compare("uint8_t")) {
            test_int_wrapper<uint8_t>(arg, repeat);
            continue;
        }
        if (!arg.compare("int8_t")) {
            test_int_wrapper<int8_t>(arg, repeat);
            continue;
        }

        if (!arg.compare("uint16_t")) {
            test_int_wrapper<uint16_t>(arg, repeat);
            continue;
        }
        if (!arg.compare("int16_t")) {
            test_int_wrapper<int16_t>(arg, repeat);
            continue;
        }

        if (!arg.compare("uint32_t")) {
            test_int_wrapper<uint32_t>(arg, repeat);
            continue;
        }
        if (!arg.compare("int32_t")) {
            test_int_wrapper<int32_t>(arg, repeat);
            continue;
        }

        if (!arg.compare("uint64_t")) {
            test_int_wrapper<uint64_t>(arg, repeat);
            continue;
        }
        if (!arg.compare("int64_t")) {
            test_int_wrapper<int64_t>(arg, repeat);
            continue;
        }

        if (!arg.compare("float")) {
            test_real_wrapper<float>(arg, repeat);
            continue;
        }
        if (!arg.compare("double")) {
            test_real_wrapper<double>(arg, repeat);
            continue;
        }
    }

    return 0;
}
