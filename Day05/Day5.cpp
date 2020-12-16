#include <iostream>
#include <fstream>
#include <string>
#include <array>

short binarySearch(const std::string& code)
{
    short left_limit = 0;
    short right_limit = (1 << code.size()) - 1;

    for (const auto c : code)
    {
        if ('F' == c || 'L' == c)
        {
            right_limit -= (right_limit - left_limit + 1) / 2;
        }
        else if ('B' == c || 'R' == c)
        {
            left_limit += (right_limit - left_limit + 1) / 2;
        }
    }

    return left_limit;
}

unsigned int calculateID(const std::string& ticket)
{
    return 8 * binarySearch(ticket.substr(0, 7)) + binarySearch(ticket.substr(7));
}

int main()
{
    constexpr unsigned int PLANE_CAPACITY = 1024;
    std::array<bool, PLANE_CAPACITY> plane_occupation{ false };

    std::ifstream file("input.txt");

    unsigned int max_id = 0;

    std::string line;
    while (file >> line)
    {
        const unsigned int ticket_id = calculateID(line);
        max_id = std::max(max_id, ticket_id);

        plane_occupation[ticket_id] = true;
    }

    unsigned int my_ticket = 0;
    for (size_t i = 16; i < PLANE_CAPACITY - 16; ++i)
    {
        if (!plane_occupation[i] && plane_occupation[i - 1] && plane_occupation[i + 1])
        {
            my_ticket = i;
            break;
        }
    }

    //Part1
    std::cout << "(Part 1) Highest ticket id is: " << max_id << std::endl;
    //Part2
    std::cout << "(Part 2) My ticket id is: " << my_ticket;

    std::cin.get();
    return 0;
}

//(Part 1) Highest ticket id is: 878
//(Part 2) My ticket id is : 504
