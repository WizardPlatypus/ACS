#include <iostream> // input/output
#include <chrono> // timers
#include <functional> // std::function<>
#include <tuple> // coupling return values
#include <string> // string to number conversions

std::tuple<std::chrono::milliseconds, size_t> performance_test(std::function<int(int, int)> operation, int begin, int end, int step = 1) {
    if (begin > end) {
        auto _ = begin;
        begin = end;
        end = _;
    }

    size_t execution_count = 0;
    std::chrono::high_resolution_clock::time_point start, finish;
    int result;

    start = std::chrono::high_resolution_clock::now();
    for (int a = begin; a <= end; a += step) {
        for (int b = begin; b <= end; b += step) {
            result = operation(a, b);
            execution_count++;
        }
    }
    finish = std::chrono::high_resolution_clock::now();

    return std::make_tuple(std::chrono::duration_cast<std::chrono::milliseconds>(finish - start), execution_count);
}

int main(int argc, char* argv[]) {
    int begin = 0, end = 10'000, step = 1;
    // this kind of switch ensures that as much of input data is processed as possible
    switch (argc) {
        case 4:
            step = std::stoi(argv[3]);
        case 3:
            end = std::stoi(argv[2]);
        case 2:
            begin = std::stoi(argv[1]);
            break;
    }
    auto addition = performance_test([](int a, int b) { return a + b; }, begin, end, step);
    auto addition_time = std::get<0>(addition);
    auto addition_count = std::get<1>(addition);

    auto hollow = performance_test([](int a, int b) { return 0; }, begin, end, step);
    auto hollow_time = std::get<0>(hollow);
    auto hollow_count = std::get<1>(hollow);

    std::cout << "Testing addition on " << addition_count << " operations took " << addition_time.count()  << " milliseconds." << std::endl;
    std::cout << "Testing hollow call on " << hollow_count << " operations took " << hollow_time.count()  << " milliseconds." << std::endl;
    std::cout << "Time difference is " << (addition_time - hollow_time).count()
              << " milliseconds, count difference is " << addition_count - hollow_count << "." << std::endl;

    return 0;
}