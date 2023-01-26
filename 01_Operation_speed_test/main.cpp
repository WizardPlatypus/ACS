#include <iostream> // input/output
#include <chrono> // timers

/* This is how arguments work
int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }
    return 0;
}
// */

int main() {
    std::cout << "Hallo! Enter two numbers separated by spaces to define testing range." << std::endl;
    int begin, end;
    std::cin >> begin >> end;
    if (begin > end) {
        auto _ = begin;
        begin = end;
        end = _;
    }
    if (begin == end) {
        std::cerr << "Range is of length 0" << std::endl;
        return 1;
    }
    
    std::chrono::duration<double> elapsed_units;
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = start;
    int r = -1;

    start = std::chrono::high_resolution_clock::now();
    for (int i = begin; i <= end; i++) {
        for (int j = begin; j <= end; j++) {
            r = i * j;
        }
    }
    finish = std::chrono::high_resolution_clock::now();
    elapsed_units = finish - start;
    // std::cout << "Last r: " << r << std::endl; // does not matter
    std::cout << "Loop with operations took " << elapsed_units.count() << std::endl;

    int steady_value = 10;
    // int count = 0;

    start = std::chrono::high_resolution_clock::now();
    for (int i = begin; i <= end; i++) {
        for (int j = begin; j <= end; j++) {
            // r = 10; // slower than i * j
            r = steady_value; // even more slow
            // count++; // same
        }
    }
    finish = std::chrono::high_resolution_clock::now();
    elapsed_units = finish - start;
    // std::cout << "Last r: " << r << std::endl; // in case that matters
    std::cout << "Loop without operations took " << elapsed_units.count() << std::endl;

    return 0;
}