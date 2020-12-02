// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

bool checkPolicyPart1(const short min, const short max, const char key, const std::string& password)
{
    unsigned int match_count = 0;
    for (const auto c : password)
    {
        if (c == key)
        {
            ++match_count;
        }
    }

    if (min <= match_count && match_count <= max)
    {
        return true;
    }

    return false;
}

bool checkPolicyPart2(const short leftPos, const short rightPos, const char key, const std::string& password)
{
    return (password[leftPos - 1] == key) ^ (password[rightPos - 1] == key);
}

int main()
{
    std::ifstream inp_file("input.txt");

    unsigned int valid_passwords_p1 = 0;
    unsigned int valid_passwords_p2 = 0;

    std::string line;
    while (std::getline(inp_file, line))
    {
        std::istringstream iss(line);

        short first, second;
        char  key, useless;
        std::string password;

        iss >> first>> useless >> second >> key >> useless >> password;

        //Part1:
        if (checkPolicyPart1(first, second, key, password)) ++valid_passwords_p1;

        //Part2:
        if (checkPolicyPart2(first, second, key, password)) ++valid_passwords_p2;
    }

    std::cout << "(Part1) Valid passwords for policy one: " << valid_passwords_p1 << std::endl;
    std::cout << "(Part2) Valid passwords for policy two: " << valid_passwords_p2 << std::endl;
    std::cin.get();

    return 0;
}

//(Part1)Valid passwords for policy one : 580
//(Part2)Valid passwords for policy two : 611

