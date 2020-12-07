#include <iostream>
#include <fstream>
#include <numeric>
#include <array>
#include <string>

int main()
{
    std::ifstream file("input.txt");

    unsigned int total_amount_anyone = 0, total_amount_everyone = 0;
    
    while (!file.eof())
    {
        std::array<short,26> check_box{ 0 };
        short group_count = 0;

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                break;
            }

            ++group_count;
            for (const auto c : line)
            {
                check_box[c - 'a'] += 1;
            }
        }

        const auto accumulate_anyone = [](const unsigned int acc, const short elem) { return elem ? acc + 1 : acc; };
        const auto accumulate_everyone = [group_count](const unsigned int acc, const short elem) { return elem == group_count ? acc + 1: acc; };

        total_amount_anyone = std::accumulate(check_box.cbegin(), check_box.cend(), total_amount_anyone, accumulate_anyone);
        total_amount_everyone = std::accumulate(check_box.cbegin(), check_box.cend(), total_amount_everyone, accumulate_everyone);
    }

    //Part1
    std::cout << "(Part 1) Total amount of questions answered yes by anyone in group: " << total_amount_anyone << std::endl;
    //Part2
    std::cout << "(Part 2) Total amount of questions answered yes by everyone in group: " << total_amount_everyone << std::endl;

    std::cin.get();
    return 0;
}

//(Part 1) Total amount of questions answered yes by anyone in group: 6351
//(Part 2) Total amount of questions answered yes by everyone in group: 3143
