#include <iostream>
#include <bitset>
#include <sstream>
#include <string>

/*
Operation syntax: opcode dst src1 src2
Word length: 13
Operations to implement:
- read data from somewhere to a register
- bit shifts left & right
*/

static constexpr int WORD_LENGTH = 13;
using u64 = uint64_t;

struct CPU {

    CPU() {
        IR = "None";
        R1 = 0;
        R2 = 0;
        R3 = 0;
        PC = 0;
        TC = 0;
        SR = 0;
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

    const char *IR; // Instruction register

    // Registers
    u64 R1 : WORD_LENGTH;
    u64 R2 : WORD_LENGTH;
    u64 R3 : WORD_LENGTH;

    u64 PC : WORD_LENGTH; // Program counter
    u64 TC : WORD_LENGTH; // Tick counter
    u64 SR : 1; // Sign register
};

int main() {
    CPU cpu;
    std::cout << cpu.to_string();
}