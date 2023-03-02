#include <iostream>
#include <bitset>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
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
    CPU(std::vector<std::string> program) {
        PC = 0; TC = 0;
        this->program = std::vector<std::string>(program);
        this->profile = std::vector<std::string>();
        this->profile.reserve(program.size() * 2);
    }

    void execute() {
        while (PC < program.size()) {
            this->next();
            profile.push_back(this->to_string());
            PC += 1;
        }
    }

    void next() {
        auto instruction = program[PC];
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

        TC += 1;

        delete[] tokens;
    }

    std::string to_string() {
        using namespace std;
        stringstream buffer;
        buffer << "Instruction #" << PC << ": " << program[PC] << std::endl;
        for (int i = 1; i < REGISTER_COUNT + 1; i++) {
            buffer << 'R' << i << ": ";
            for (int j = 0; j < WORD_LENGTH; j++) {
                buffer << memory[WORD_LENGTH * i + j];
            }
            buffer << std::endl;
        }
        // program counter
        buffer << "PC: " << Word(PC) << std::endl;
        // tick counter
        buffer << "TC: " << Word(TC) << std::endl;
        // sign bit
        buffer << "SB: " << memory[get_ptr(SB_id)] << std::endl;
        return buffer.str();
    }

    std::vector<std::string> profile;

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
    std::bitset<WORD_LENGTH * (/*register 0*/1 + REGISTER_COUNT) + /*sign bit*/1> memory;

    static const int SB_id = 1 + REGISTER_COUNT;

    int PC, TC; // program and tick counter

    std::vector<std::string> program;
};

int main(int argc, const char* argv[]) {
    // TODO: display cpu state before and after the operation
    //*
    std::cout << "Hallo????" << std::endl;
    for (int i = 1; i < argc; i++) {
        std::string file_path = argv[i];
        std::ifstream file(file_path);
        std::vector<std::string> program;

        while (file.good()) {
            std::string line;
            std::getline(file, line);
            if (line.length() == 0) {
                break;
            }
            program.push_back(line);
        }

        file.close();

        CPU cpu(program);
        cpu.execute();

        for (int i = 0; i < cpu.profile.size() && getch() != NEW_LINE_CODE; i++) {
            std::cout << cpu.profile[i] << std::endl;
        }
    }
    //*/
}