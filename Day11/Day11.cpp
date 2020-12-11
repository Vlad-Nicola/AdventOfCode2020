#include <iostream>
#include <string_view>
#include <fstream>
#include <string>
#include <vector>

bool getExtendedNeighboursCount(const std::vector<std::string>& seating_area,
    const unsigned int row,
    const unsigned int col,
    const char state)
{
    unsigned int count = 0;

    const int min_i = row == 0 ? 0 : -1;
    const int max_i = row == seating_area.size() - 1 ? 0 : 1;
    for (int i = min_i; i <= max_i; ++i)
    {
        const int min_j = col == 0 ? 0 : -1;
        const int max_j = col == seating_area[row].size() - 1 ? 0 : 1;
        for (int j = min_j; j <= max_j; ++j)
        {
            if (i == 0 && j == 0) continue;
            
            int ei = i, ej = j;
            while(true)
            {
                if ('#' == seating_area[row + ei][col + ej])
                {
                    ++count;
                    break;
                }
                else if ('L' == seating_area[row + ei][col + ej])
                {
                    break;
                }

                ei += i;
                ej += j;

                if (ei + row < 0 || ei + row >= seating_area.size() ||
                    ej + col < 0 || ej + col >= seating_area[row].size())
                {
                    break;
                }
            }
        }
    }

    if (state == 'L' && count == 0 || state == '#' && count >= 5)
    {
        return true;
    }
    return false;
}

bool getNeighboursCount(const std::vector<std::string>& seating_area,
    const unsigned int row,
    const unsigned int col,
    const char state)
{
    unsigned int count = 0;

    const unsigned int min_i = row == 0 ? row : row - 1;
    const unsigned int max_i = row == seating_area.size() - 1 ? row : row + 1;
    for (unsigned int i = min_i; i <= max_i; ++i) 
    {
        const unsigned int min_j = col == 0 ? col : col - 1;
        const unsigned int max_j = col == seating_area[row].size() - 1 ? col : col + 1;
        for (unsigned int j = min_j; j <= max_j; ++j)
        {
            if (i == row && j == col) continue;
            if (seating_area[i][j] == '#') ++count;
        }
    }

    if (state == 'L' && count == 0 || state == '#' && count >= 4)
    {
        return true;
    }
    return false;
}

unsigned int getOccupiedSeats(std::vector<std::string>& seating_area, decltype(getNeighboursCount) scan_function)
{
    bool change = true;
    while (change)
    {
        change = false;
        std::vector<std::string> seating_area_copy{ seating_area };
        for (size_t row = 0; row < seating_area.size(); ++row)
        {
            const size_t seat_row_size{ seating_area[row].size() };
            for (size_t col = 0; col < seat_row_size; ++col)
            {
                if (seating_area[row][col] == 'L' && scan_function(seating_area_copy, row, col, 'L'))
                {
                    seating_area[row][col] = '#';
                    change = true;
                }
                else if (seating_area[row][col] == '#' && scan_function(seating_area_copy, row, col, '#'))
                {
                    seating_area[row][col] = 'L';
                    change = true;
                }
            }
        }
    }

    unsigned int occupied_seats = 0;
    for (const auto& seat_row : seating_area)
    {
        for (const auto seat : seat_row)
        {
            if ('#' == seat) ++occupied_seats;
        }
    }

    return occupied_seats;
}

int main()
{
    std::ifstream file("input.txt");

    std::vector<std::string> seating_area;
    for (std::string line; file >> line; seating_area.emplace_back(line));
    auto seating_area_copy{ seating_area };
    
    //Part1
    std::cout << "(Part 1) Occupied seats at stabilization: " << getOccupiedSeats(seating_area, &getNeighboursCount) <<std ::endl;
    //Part2
    std::cout << "(Part 2) Occupied seats at stabilization: " << getOccupiedSeats(seating_area_copy, &getExtendedNeighboursCount) << std::endl;

    std::cin.get();
    return 0;
}

//(Part 1) Occupied seats at stabilization: 2261
//(Part 2) Occupied seats at stabilization: 2039
