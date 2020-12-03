#include <iostream>
#include <fstream>
#include <chrono>
#include <list>

int calculateProductOfN(const std::list<unsigned int>::const_iterator& begin,
                        const std::list<unsigned int>::const_iterator& end,
                        const unsigned int depth,
                        const unsigned int sum_value
                       )
{
    for (auto firstIt = begin; firstIt != end; )
    {
        const unsigned int value = *firstIt;
        if (2 == depth)
        {
            const auto secondIt = std::find(++firstIt , end, sum_value - value);
            if (secondIt != end)
            {
                return value * *secondIt;
            }
        }
        else
        {
            const int product_of_n_minus_1 = calculateProductOfN(++firstIt, end, depth - 1, sum_value - value);
            if (-1 != product_of_n_minus_1)
            {
                return product_of_n_minus_1 * value;
            }
        }
    }
    return -1;
}

int main(int argc, char* argv[])
{
    constexpr unsigned int sum_value = 2020;

    std::list<unsigned int> numbers;
    std::ifstream f("input.txt");

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    unsigned int number;
    while (f >> number)
    {
        numbers.push_back(number);
    }

    //Part1
    std::cout << "(Part1) Product of two: " << calculateProductOfN(numbers.cbegin(), numbers.cend(), 2, sum_value) << std::endl;
    //Part2
    std::cout << "(Part2) Product of three: " << calculateProductOfN(numbers.cbegin(), numbers.cend(), 3, sum_value);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    std::cin.get();
    return 0;
}

//(Part1)Product of two : 987339
//(Part2)Product of three : 259521570
