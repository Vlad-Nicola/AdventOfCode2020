#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

unsigned int getDifferenceProduct(const std::vector<unsigned int>& adapters)
{
    int diff1{ 0 }, diff3{ 1 };
    for (auto it = adapters.cbegin() + 1; it != adapters.cend(); ++it)
    {
        int diff = *it - *(it - 1);
        if (1 == diff) ++diff1;
        else if (3 == diff) ++diff3;
    }
    return diff1 * diff3;
}

unsigned long long getNumberOfCombinations(const std::vector<unsigned int>& adapters)
{
    unsigned long long total_combinations{ 1 }, local_combinations{ 1 };
    bool double_remove = false;
    for (auto it = adapters.cbegin() + 1; it < adapters.cend() - 1; ++it)
    {
        if (*(it + 1) - *(it - 1) <= 3)
        {
            if (double_remove)
            {
                local_combinations = 2 * local_combinations - 1;
            }
            else
            {
                local_combinations = 2 * local_combinations;

                if (adapters.cend() != it + 2 && 3 == *(it + 2) - *(it - 1))
                {
                    double_remove = true;
                    local_combinations = 2 * local_combinations;
                    ++it;
                }
            }
        }
        else
        {
            double_remove = false;
            total_combinations *= local_combinations;
            local_combinations = 1;
        }
    }

    return total_combinations * local_combinations;
}

int main()
{
    std::vector<unsigned int> adapters(1, 0);

    std::ifstream file("input.txt");
    for (unsigned int val; file >> val; adapters.push_back(val));

    std::sort(adapters.begin(), adapters.end());

    //Part1
    std::cout << "(Part 1) Difference product: " << getDifferenceProduct(adapters) << std::endl;
    //Part2
    std::cout << "(Part 2) Different combinations: " << getNumberOfCombinations(adapters);

    std::cin.get();
    return 0;
}

//(Part 1) Difference product : 2574
//(Part 2) Different combinations : 2644613988352
