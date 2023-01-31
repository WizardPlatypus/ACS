#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>

/* This is how arguments work
int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
    return 0;
}
// */

std::chrono::milliseconds performance_test(std::function<int(int, int)> operation, int begin, int end, int step = 1) {
    if (begin > end) {
        auto _ = begin;
        begin = end;
        end = _;
    }

    std::chrono::high_resolution_clock::time_point start, finish;
    int result;

    start = std::chrono::high_resolution_clock::now();
    for (int a = begin; a <= end; a += step) {
        for (int b = begin; b <= end; b += step) {
            result = operation(a, b);
        }
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
}

int main() {
    std::cout << "Hallo! Enter two numbers separated by spaces to define testing range." << std::endl;
    int begin, end;
    std::cin >> begin >> end;

    auto addition_time = performance_test([](int a, int b) { return a + b; }, begin, end);
    auto hollow_time = performance_test([](int a, int b) { return 0; }, begin, end);

    std::cout << "Test with operation took " << addition_time.count() << std::endl;
    std::cout << "Test without operation took " << hollow_time.count() << std::endl;
    std::cout << "Time difference is " << (addition_time - hollow_time).count() << std::endl;

    return 0;
}