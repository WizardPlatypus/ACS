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

using Word = std::bitset<WORD_LENGTH>;

// TODO: Support for negative integers
struct CPU {
public:
    CPU() {}

    void tick(std::string instruction) {
        int size;
        auto tokens = parse_instruction(instruction, size);

        std::string op = tokens[0];

        int* ids = new int[size - 1];
        for (int i = 1; i < size; i++) {
            std::string token = tokens[i];
            if (token.at(0) == 'R') {
                try {
                    ids[i - 1] = std::stoi(token.substr(1, token.length() - 1));
                } catch(const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                    exit(1);
                }
                continue;
            }
            int value;
            try {
                value = std::stoi(token);
            } catch(const std::exception& e) {
                std::cerr << e.what() << std::endl;
                exit(1);
            }
            set_value(0, value);
            ids[i - 1] = 0;
        }

        if (!op.compare("mov")) {
            mov(ids[0], ids[1]);
        } else if (!op.compare("shl")) {
            shl(ids[0], ids[1], ids[2]);
        } else if (!op.compare("shr")) {
            shr(ids[0], ids[1], ids[2]);
        } else if (!op.compare("add")) {
            add(ids[0], ids[1], ids[2]);
        } else {
            std::cerr << "Couldn't do the operation " << op << std::endl;
        }

        set_value(0, 1);
        add(TC_id, TC_id, 0); // TC += 1;
        add(PC_id, PC_id, 0); // PC += 1;

        delete[] tokens;
    }

    std::string to_string() {
        using namespace std;
        stringstream buffer;
        for (int i = 1; i < REGISTER_COUNT + 1; i++) {
            buffer << 'R' << i << ": ";
            for (int j = 0; j < WORD_LENGTH; j++) {
                buffer << memory[WORD_LENGTH * i + j];
            }
            buffer << std::endl;
        }
        { // program counter
            buffer << "PC: ";
            for (int i = 0; i < WORD_LENGTH; i++) {
                buffer << memory[get_ptr(PC_id) + i];
            }
            buffer << std::endl;
        }
        { // tick counter
            buffer << "PC: ";
            for (int i = 0; i < WORD_LENGTH; i++) {
                buffer << memory[get_ptr(TC_id) + i];
            }
            buffer << std::endl;
        }
        // sign bit
        buffer << "SB: " << memory[get_ptr(SB_id)] << std::endl;
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

    void mov(const int dst_id, const int src_id) {
        int dst_ptr = get_ptr(dst_id);
        int src_ptr = get_ptr(src_id);
        for (int i = 0; i < WORD_LENGTH; i++) {
            memory[dst_ptr + i] = memory[src_ptr + i];
        }
    }

    void add(const int dst_id, const int a_id, const int b_id) {
        unsigned a = get_value(a_id);
        unsigned b = get_value(b_id);
        set_value(dst_id, a + b);
    }

    void shl(const int dst_id, const int src_id, const int arg_id) {
        auto value = get_value(src_id);
        auto arg= get_value(arg_id);
        value <<= arg;
        set_value(dst_id, value);
    }

    void shr(const int dst_id, const int src_id, const int arg_id) {
        auto value = get_value(src_id);
        auto arg= get_value(arg_id);
        value >>= arg;
        set_value(dst_id, value);
    }

    unsigned get_value(const int id) {
        int ptr = get_ptr(id);
        Word bits;
        for (int i = 0; i < WORD_LENGTH; i++) {
            bits[i] = memory[ptr + i];
        }
        return (unsigned)bits.to_ulong();
    }

    void set_value(const int id, const unsigned value) {
        Word bits(value);

        int ptr = get_ptr(id);
        for (int i = 0; i < WORD_LENGTH; i++) {
            memory[ptr + i] = bits[i];
        }
    }

    static constexpr int get_ptr(const int n) {
        return n * WORD_LENGTH;
    }

    // Registers
    std::bitset<WORD_LENGTH * (/*register 0*/1 + REGISTER_COUNT + /*program counter*/1 + /*tick counter*/1) + /*sign bit*/1> memory;

    static const int PC_id = 1 + REGISTER_COUNT + 0;
    static const int TC_id = 1 + REGISTER_COUNT + 1;
    static const int SB_id = 1 + REGISTER_COUNT + 2;
};

int main(int argc, const char* argv[]) {
    // TODO: display cpu state before and after the operation
    //*
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
            std::cout << line << std::endl;
            std::cout << cpu.to_string() << std::endl;
        }
    }
    //*/
}