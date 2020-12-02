#include <iostream>
#include <fstream>
#include <vector>

int calculateProductOfN(const std::vector<unsigned int>::const_iterator& begin,
                        const std::vector<unsigned int>::const_iterator& end,
                        const unsigned int depth,
                        const unsigned int sum_value
                       )
{
    for (auto firstIt = begin; firstIt != end; ++firstIt)
    {
        if (2 == depth)
        {
            const auto secondIt = std::find(firstIt + 1, end, sum_value - *firstIt);
            if (secondIt != end)
            {
                return *firstIt * *secondIt;
            }
        }
        else
        {
            const int product_of_n_minus_1 = calculateProductOfN(firstIt + 1, end, depth - 1, sum_value - *firstIt);
            if (-1 != product_of_n_minus_1)
            {
                return product_of_n_minus_1 * *firstIt;
            }
        }
    }
    return -1;
}

int main(int argc, char* argv[])
{
    constexpr unsigned int sum_value = 2020;

    std::vector<unsigned int> numbers;
    std::ifstream f("input.txt");

    unsigned int number;
    while (f >> number)
    {
        numbers.push_back(number);
    }

    //Part1
    std::cout << "(Part1) Product of two: " << calculateProductOfN(numbers.cbegin(), numbers.cend(), 2, sum_value) << std::endl;
    //Part2
    std::cout << "(Part2) Product of three: " << calculateProductOfN(numbers.cbegin(), numbers.cend(), 3, sum_value);

    std::cin.get();
    return 0;
}

//(Part1)Product of two : 987339
//(Part2)Product of three : 259521570
