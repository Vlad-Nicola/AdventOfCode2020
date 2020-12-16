#include <modif_array.h>
#include <iostream>
#include <chrono>

constexpr const unsigned int* find(const unsigned int* begin,
                                   const unsigned int* end,
                                   const unsigned int value
                                  )
{
    auto it = begin;
    while (it != end)
    {
        if (*it == value)
        {
            return it;
        }
        ++it;
    }

    return end;
}

constexpr int calculateProductOfN(const unsigned int* begin,
                                  const unsigned int* end,
                                  const unsigned int depth,
                                  const unsigned int sum_value
                                 )
{
    for (auto firstIt = begin; firstIt != end;)
    {
        const unsigned int value = *firstIt;
        if (2 == depth)
        {
            const auto secondIt = find(++firstIt, end, sum_value - value);
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

int main()
{
    constexpr unsigned int sum_value = 2020;

    std::chrono::steady_clock::time_point beginClock = std::chrono::steady_clock::now();

    constexpr int product1 = calculateProductOfN(&numbers[0], &numbers[ARRAY_SIZE], 2, sum_value);
    constexpr int product2 = calculateProductOfN(&numbers[0], &numbers[ARRAY_SIZE], 3, sum_value);

    std::chrono::steady_clock::time_point endClock = std::chrono::steady_clock::now();

    //Part1
    std::cout << "(Part1) Product of two: " << product1 << std::endl;
    //Part2
    std::cout << "(Part2) Product of three: " << product2 << std::endl;

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(endClock - beginClock).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (endClock - beginClock).count() << "[ns]" << std::endl;

    std::cin.get();
    return 0;
}

//(Part1)Product of two : 987339
//(Part2)Product of three : 259521570
