#include <iostream>
#include <bitset>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

bool validField(const short field, const std::string& value)
{
    switch(field)
    {
    case 0:
    {
        return std::regex_match(value, std::regex("^19[2-9][0-9]|200[0-2]$"));
    }
    case 1:
    {
        return std::regex_match(value, std::regex("^201[0-9]|2020$"));
    }
    case 2:
    {
        return std::regex_match(value, std::regex("^202[0-9]|2030$"));
    }
    case 3:
    {
        return std::regex_match(value, std::regex("^(1([5-8][0-9]|9[0-3])cm)|((59|6[0-9]|7[0-6])in)$"));
    }
    case 4:
    {
        return std::regex_match(value, std::regex("^#[0-9a-f]{6,6}$"));
    }
    case 5:
    {
        return std::regex_match(value, std::regex("^amb|blu|brn|gry|grn|hzl|oth$"));
    }
    case 6:
    {
        return std::regex_match(value, std::regex("^[0-9]{9,9}$"));
    }
    case 7:
    {
        return true;
    }
    default:
        return false;
    }
    return false;
}

int main()
{
    const std::unordered_map<std::string, const short> code_pos{ {"byr", 0},
                                                                 {"iyr", 1},
                                                                 {"eyr", 2},
                                                                 {"hgt", 3},
                                                                 {"hcl", 4},
                                                                 {"ecl", 5},
                                                                 {"pid", 6},
                                                                 {"cid", 7}
                                                               };

    unsigned int valid_passwords             = 0;
    unsigned int valid_passwords_invalidated = 0;

    std::ifstream file("input.txt");

    std::string data;
    std::bitset<8> check_box;
    bool values_invalid = false;

    while (std::getline(file, data))
    {
        //Added INPUT_END to the end of the input file to avoid checking last passport after the loop
        if (data.empty() || 0 == data.compare("INPUT_END"))
        {
            if (check_box == 0x7F || check_box == 0xFF)
            {
                ++valid_passwords;
                if (values_invalid)
                {
                    ++valid_passwords_invalidated;
                }
            }

            values_invalid = false;
            check_box.reset();
            continue;
        }

        std::string portion;
        std::istringstream iss(data);
        while (iss >> portion)
        {
            const std::string value = portion.substr(portion.find(':') + 1);
            const std::string field = portion.substr(0, 3);
            const short field_index = code_pos.at(field);
            
            check_box[field_index] = 1;
            if (!values_invalid && !validField(field_index, value))
            {
                values_invalid = true;
            }
        }
    }

    //Part1:
    std::cout << "(Part1) Number of valid passwords: " << valid_passwords << std::endl;
    //Part2:
    std::cout << "(Part2) Number of valid passwords: " << valid_passwords - valid_passwords_invalidated;

    std::cin.get();
    return 0;
}

//(Part1) Number of valid passwords: 213
//(Part2)Number of valid passwords : 147
