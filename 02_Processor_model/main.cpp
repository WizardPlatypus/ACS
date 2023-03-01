#include <iostream>
#include <bitset>
#include <sstream>
#include <string>
#include <fstream>
// windows dependent; an easy way to wait for user input
#include <conio.h> // for getch()

#define WORD_LENGTH 13
#define REGISTER_COUNT 7

#define INSTRUCTION_LENGTH 4
#define NEW_LINE_CODE 13
/*
Operation syntax: opcode dst src1 src2
Word length: 13
Operations to implement:
- read data from somewhere to a register
- bit shifts left & right
*/

using Bits = uint16_t;

// TODO: Support for negative integers
struct CPU {
public:
    CPU() {
        R1 = 0;
        R2 = 0;
        R3 = 0;
        PC = 0;
        TC = 0;
        SR = 0;
    }

    void tick(std::string instruction) {
        int size;
        auto tokens = parse_instruction(instruction, size);

        std::string op = tokens[0];

        Bits **args = new Bits*[size - 1];
        for (int i = 1; i < size; i++) {
            std::string token = tokens[i];
            if (!token.compare("R1")) {
                args[i - 1] = &R1;
                continue;
            }
            if (!token.compare("R2")) {
                args[i - 1] = &R2;
                continue;
            }
            if (!token.compare("R3")) {
                args[i - 1] = &R3;
                continue;
            }
            args[i - 1] = new Bits {(Bits)std::stoi(token)};
        }

        if (!op.compare("mov")) {
            mov(args[0], args[1]);
        } else if (!op.compare("shl")) {
            shl(args[0], args[1], args[2]);
        } else if (!op.compare("shr")) {
            shr(args[0], args[1], args[2]);
        } else {
            std::cerr << "Couldn't do the operation " << op << std::endl;
        }
        TC++;
        PC++;

        delete[] tokens;
    }

    std::string to_string() {
        using namespace std;
        stringstream buffer;
        for (int i = 1; i < REGISTER_COUNT + 1; i++) {
            buffer << 'R' << i << ": ";
            for (int j = 0; j < WORD_LENGTH; j++) {
                buffer << memory[REGISTER_COUNT * i + j];
            }
            buffer << std::endl;
        }
        { // program counter
            buffer << "PC: ";
            for (int i = 0; i < WORD_LENGTH; i++) {
                buffer << memory[RN_pos(PC_id) + i];
            }
            buffer << std::endl;
        }
        { // tick counter
            buffer << "PC: ";
            for (int i = 0; i < WORD_LENGTH; i++) {
                buffer << memory[RN_pos(TC_id) + i];
            }
            buffer << std::endl;
        }
        // sign bit
        buffer << "SB: " << memory[RN_pos(SB_id)] << std::endl;
        return buffer.str();
    }

private:
    std::string* parse_instruction(std::string str, int& size) {
        std::string *instruction = new std::string[4];
        int cursor = 0;
        size = 0;
        while (size < INSTRUCTION_LENGTH && cursor < str.length()) {
            auto length = find_space(str, cursor) - cursor;
            auto token = str.substr(cursor, length);
            instruction[size++] = token;
            cursor += length + 1;
        }
        return instruction;
    }

    int find_space(std::string str, int begin) {
        int cursor = begin;
        while (str[cursor] != ' ' && cursor < str.length()) {
            cursor++;
        }
        return cursor;
    }

    void mov(Bits* dst, const Bits* src) {
        *dst = *src;
        SR = *dst > 0;
    }

    void mov(const int dst_id, const int src_id) {
        int dst_ptr = RN_pos(dst_id);
        int src_ptr = RN_pos(src_id);
        for (int i = 0; i < WORD_LENGTH; i++) {
            memory[dst_ptr + i] = memory[src_ptr + i];
        }
    }

    void shl(Bits* dst, const Bits* src, const Bits* arg) {
        Bits value = *src;
        Bits how_much = *arg;
        value <<= how_much;
        *dst = value;
        SR = value > 0;
    }

    void shr(Bits* dst, const Bits* src, const Bits* arg) {
        Bits value = *src;
        Bits how_much = *arg;
        value >>= how_much;
        *dst = value;
        SR = value > 0;
    }


    // Registers
    // TODO: add more registers
    // TODO: store them as an actual bit array
    std::bitset<WORD_LENGTH * (/*register 0*/1 + REGISTER_COUNT + /*program counter*/1 + /*tick counter*/1) + /*sign bit*/1> memory;
    static const int PC_id = 1 + REGISTER_COUNT + 0;
    static const int TC_id = 1 + REGISTER_COUNT + 1;
    static const int SB_id = 1 + REGISTER_COUNT + 2;
    static constexpr int RN_pos(const int n) {
        return n * WORD_LENGTH;
    }
    Bits R1;
    Bits R2;
    Bits R3;

    Bits PC; // Program counter
    Bits TC; // Tick counter
    Bits SR; // Sign register
};

int main(int argc, const char* argv[]) {
    // TODO: display cpu state before and after the operation
    /*
    for (int i = 1; i < argc; i++) {
        CPU cpu;

        std::string file_path = argv[i];
        std::ifstream file(file_path);

        std::cout << cpu.to_string() << std::endl;
        while (file.good() && getch() != NEW_LINE_CODE) {
            std::string line;
            std::getline(file, line);
            if (line.length() == 0) {
                break;
            }

            cpu.tick(line);
            std::cout << cpu.to_string() << std::endl;
        }
    }
    //*/
    CPU cpu;
    std::cout << cpu.to_string() << std::endl;
    std::cout << "Bye =]" << std::endl;
}