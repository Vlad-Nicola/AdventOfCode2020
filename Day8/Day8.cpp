#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>

using Program = std::vector<std::pair<std::string, int>>;

bool executeProgram(const Program& program, int& accumulator)
{
    std::vector<bool> executed(program.size(), 0);
    for (int pc{}; ; )
    {
        if (pc == program.size()) return true;
        if (pc < 0 || pc > program.size()) return false;
        if (executed[pc]) return false;

        executed[pc] = true;

        if ("nop" == program[pc].first)
        {
            ++pc;
        }
        else if ("acc" == program[pc].first)
        {
            accumulator += program[pc].second;
            ++pc;
        }
        else
        {
            pc += program[pc].second;
        }
    }
    return false;
}

int main()
{
    std::ifstream file("input.txt");

    Program program;
    for(std::pair<std::string, int> instr; file >> instr.first >> instr.second;)
    {
        program.emplace_back(instr);
    }
    
    int accumulator_p1 = 0;
    executeProgram(program, accumulator_p1);

    int accumulator_p2 = 0;
    for (auto& [op, offset] : program)
    {
        if ("nop" == op)
        {
            op = "jmp";
            if (executeProgram(program, accumulator_p2))
            {
                break;
            }
            op = "nop";
        }
        else if ("jmp" == op)
        {
            op = "nop";
            if (executeProgram(program, accumulator_p2))
            {
                break;
            }
            op = "jmp";
        }
        accumulator_p2 = 0;
    }

    //Part1
    std::cout << "(Part 1) At first line executed a second time, accumulator is: " << accumulator_p1 << std::endl;
    //Part2
    std::cout << "(Part 2) Accumulator after successfull execution: " << accumulator_p2 << std::endl;

    std::cin.get();
    return 0;
}

//(Part 1) At first line executed a second time, accumulator is: 1586
//(Part 2) Accumulator after successfull execution : 703
