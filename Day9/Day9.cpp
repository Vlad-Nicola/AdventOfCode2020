#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>

constexpr unsigned int cache_size = 25;

unsigned long long findEncriptionWeakness(const std::list<unsigned long long>& number_list, const unsigned long long invalid_number)
{
    auto it_first = number_list.crbegin();
    auto it_second = std::next(it_first);

    unsigned long long sum = *it_first + *it_second;
    for ( ; it_first != number_list.crend(); sum -= *it_first, ++it_first)
    {
        bool unchanged_it_second = true;
        if (it_first == it_second)
        {
            ++it_second;
            unchanged_it_second = false;
        }

        for ( ; it_second != number_list.crend(); ++it_second)
        {
            if (!unchanged_it_second)
            {
                sum += *it_second;
            }

            if (sum == invalid_number)
            {
                unsigned long long min = *std::min_element(it_first, std::next(it_second));
                unsigned long long max = *std::max_element(it_first, std::next(it_second));
                return min + max;
            }
            else if (sum > invalid_number)
            {
                break;
            }

            unchanged_it_second = false;
        }
    }
    return 0;
}

bool checkValue(const std::list<unsigned long long>& numbers_list, const unsigned long long val)
{
    unsigned int i{ cache_size };
    for (auto it_first = numbers_list.crbegin(); i != 0; --i)
    {
        unsigned long long val_first = *it_first;
        ++it_first;
        unsigned int j{ i - 1 };
        for (auto it_second = it_first; j != 0; --j, ++it_second)
        {
            if (*it_second == val - val_first)
            {
                return true;
            }
        }
    }

    return false;
}

int main()
{
    std::ifstream file("input.txt");
    
    std::list<unsigned long long> numbers_list;
    unsigned long long invalid_value = 0;

    for (unsigned long long val, i{ cache_size }; file >> val && i != 0; --i)
    {
            numbers_list.push_back(val);
    }

    auto it = numbers_list.cbegin();

    for (unsigned long long val; file >> val; )
    {
        if (!checkValue(numbers_list, val))
        {
            invalid_value = val;
            break;
        }
        numbers_list.push_back(val);
    }

    //Part1
    std::cout << "(Part 1) First invalid value: " << invalid_value << std::endl;
    //Part2
    std::cout << "(Part 2) Encription weakness: " << findEncriptionWeakness(numbers_list, invalid_value);

    std::cin.get();
    return 0;
}

//(Part 1) First invalid value: 1492208709
//(Part 2) Encription weakness: 238243506
