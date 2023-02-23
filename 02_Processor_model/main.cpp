#include <iostream>
#include <bitset>
#include <sstream>
#include <string>
#include <vector>

/*
Operation syntax: opcode dst src1 src2
Word length: 13
Operations to implement:
- read data from somewhere to a register
- bit shifts left & right
*/

static constexpr int WORD_LENGTH = 13;
using Bits = uint16_t;
static constexpr int INSTRUCTION_LENGTH = 4;

struct CPU {
public:
    CPU() {
        IR = "reset";
        R1 = 0;
        R2 = 0;
        R3 = 0;
        PC = 0;
        TC = 0;
        SR = 0;
    }

    void tick(std::string instruction) {
        IR = instruction;
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
        } else {
            std::cerr << "Couldn't do the operation " << op << std::endl;
        }
        TC++;
        PC++;
    }

    std::string to_string() {
        using namespace std;
        stringstream buffer;
        buffer << "IR: " << IR << endl;
        buffer << "R1: " << bitset<WORD_LENGTH>(R1) << endl;
        buffer << "R2: " << bitset<WORD_LENGTH>(R2) << endl;
        buffer << "R3: " << bitset<WORD_LENGTH>(R3) << endl;
        buffer << "PC: " << bitset<WORD_LENGTH>(PC) << endl;
        buffer << "TC: " << bitset<WORD_LENGTH>(TC) << endl;
        buffer << "SR: " << bitset<1>(SR) << endl;
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

    std::string IR; // Instruction register

    // Registers
    Bits R1;
    Bits R2;
    Bits R3;

    Bits PC; // Program counter
    Bits TC; // Tick counter
    Bits SR; // Sign register
};

int main() {
    CPU cpu;
    std::cout << cpu.to_string() << std::endl;
    cpu.tick("mov R1 5");
    std::cout << cpu.to_string() << std::endl;
    cpu.tick("mov R2 8");
    std::cout << cpu.to_string() << std::endl;
}