#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct slope
{
    slope(const unsigned int horizontal_shift,
          const unsigned int vertical_shift)
        : h_shift(horizontal_shift)
        , v_shift(vertical_shift)
        , tree_count(0)
        , h_index(0)
    {}

    unsigned int h_index;
    unsigned int h_shift;
    unsigned int v_shift;
    unsigned int tree_count;
};

int main()
{
    slope slopes[]{ {1, 1},
                    {3, 1},
                    {5, 1},
                    {7, 1},
                    {1, 2} 
                  };

    std::ifstream file("input.txt");

    std::string row;
    file >> row;

    const size_t row_length = row.length();
    unsigned int currentRow = 0;

    while (file >> row)
    {
        ++currentRow;
        for (auto& slope : slopes)
        {
            if (currentRow % slope.v_shift != 0)
            {
                continue;
            }

            slope.h_index += slope.h_shift;
            slope.h_index %= row_length;

            if ('#' == row[slope.h_index])
            {
                ++slope.tree_count;
            }
        }
    }

    unsigned int product = 1;
    for (const auto& slope : slopes)
    {
        product *= slope.tree_count;
    }

    //Part1
    std::cout << "(Part 1) Trees hit for slope (3,1): " << slopes[1].tree_count << std::endl;
    //Part2
    std::cout << "(Part 2) Product of trees hit using all slopes: " << product;

    std::cin.get();
    return 0;
}

//(Part 1) Trees hit for slope (3,1): 189
//(Part 2) Product of trees hit using all slopes : 1718180100
