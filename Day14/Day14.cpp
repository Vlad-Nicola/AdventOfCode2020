#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <string>

void getMasks(std::unordered_map<uint64_t, std::pair<uint64_t, uint64_t>>& address_table,
              uint64_t& sum,
              uint64_t address,
              const uint64_t value,
              const std::vector<size_t> positions_x,
              const short& index)
{
    if (index == positions_x.size())
    {
        auto& [old_value_p1, old_value_p2] = address_table[address];
        sum = sum - old_value_p2 + value;
        old_value_p2 = value;

        return;
    }

    getMasks(address_table, sum, address, value, positions_x, index + 1);
    getMasks(address_table, sum, address ^ (1ULL << positions_x[index]), value, positions_x, index + 1);
}

int main()
{
    std::ifstream file("input.txt");

    std::unordered_map<uint64_t, std::pair<uint64_t, uint64_t>> address_table;

    uint64_t mask_z{}, mask_o{};
    uint64_t sum_p1{}, sum_p2{};

    std::regex re_mask{ "mask = ([X01]{36,36})" };
    std::regex re_asign{ "mem\\[(\\d+)\\] = (\\d+)" };

    std::vector<size_t> positions_x;

    std::string line;
    while (std::getline(file, line))
    {
        std::smatch match;
        if (std::regex_match(line, match, re_mask))
        {
            positions_x.clear();
            std::string mask_z_str{ match[1] }, mask_o_str{ match[1] };
            for(size_t i = 0; i < mask_z_str.size(); ++i)
            { 
                if (mask_z_str[i] == 'X')
                {
                    positions_x.push_back(mask_z_str.size() - i - 1);
                    mask_z_str[i] = '0';
                    mask_o_str[i] = '1';
                }
            }
            mask_z = std::stoull(mask_z_str, nullptr, 2);
            mask_o = std::stoull(mask_o_str, nullptr, 2);
        }
        else if (std::regex_match(line, match, re_asign))
        {
            uint64_t address = std::stoull(match[1].str());
            uint64_t value = std::stoull(match[2].str());

            const uint64_t value_p1 = value & mask_o | mask_z;

            auto& [old_value_p1, old_value_p2] = address_table[address];
            sum_p1 = sum_p1 - old_value_p1 + value_p1;
            old_value_p1 = value_p1;

            address |= mask_z;

            getMasks(address_table, sum_p2, address, value, positions_x, 0);
        }
    }

    //Part1
    std::cout << "(Part 1) Sum: " << sum_p1 << std::endl;
    //Part2
    std::cout << "(Part 2) Sum: " <<sum_p2;

    std::cin.get();
    return 0;
}

//(Part 1) Sum: 4297467072083
//(Part 2) Sum: 5030603328768
