#include <iostream>
#include <fstream>
#include <vector>

using Row = std::vector<bool>;
using Page = std::vector<Row>;
using Grid = std::vector<Page>;
using Hypercube = std::vector<Grid>;

template<typename Beg, typename End, typename Emp>
void expandGrid(Hypercube& hyper_cube, const Beg& my_begin, const End& my_end, const Emp& my_emplace, bool d4 = true)
{
    bool add_page{ false }, add_row{ false }, add_col{ false }, add_cube{ false };
    for (auto cube_it = my_begin(hyper_cube); cube_it != my_end(hyper_cube); ++cube_it)
    {
        for (auto page_it = my_begin(*cube_it); page_it != my_end(*cube_it); ++page_it)
        {
            for (auto row_it = my_begin(*page_it); row_it != my_end(*page_it); ++row_it)
            {
                for (auto col_it = my_begin(*row_it); col_it != my_end(*row_it); ++col_it)
                {
                    if (*col_it)
                    {
                        if (d4 && cube_it == my_begin(hyper_cube)) add_cube = true;
                        if (page_it == my_begin(*cube_it))    add_page = true;
                        if (row_it == my_begin(*page_it)) add_row = true;
                        if (col_it == my_begin(*row_it))  add_col = true;
                    }

                    if (cube_it != my_begin(hyper_cube) && page_it != my_begin(*cube_it) && row_it != my_begin(*page_it))
                    {
                        break;
                    }
                }
            }
        }
    }

    if (add_cube)
    {
        my_emplace(hyper_cube, Grid(hyper_cube.begin()->size(),
            Page(hyper_cube.begin()->begin()->size(),
                Row(hyper_cube.begin()->begin()->begin()->size(), false))));
    }
    for (unsigned int cube_it = 0; cube_it < hyper_cube.size(); ++cube_it)
    {
        if (add_page)
        {
            my_emplace(hyper_cube[cube_it], Page(hyper_cube[cube_it][0].size(),
                Row(hyper_cube[cube_it][0][0].size(), false)));
        }
        for (unsigned int page_it = 0; page_it < hyper_cube[cube_it].size(); ++page_it)
        {
            if (add_row)
            {
                my_emplace(hyper_cube[cube_it][page_it], Row(hyper_cube[cube_it][page_it][0].size(), false));
            }
            for (unsigned int row_it = 0; row_it != hyper_cube[cube_it][page_it].size(); ++row_it)
            {
                if (add_col)
                {
                    my_emplace(hyper_cube[cube_it][page_it][row_it], false);
                }
            }
        }
    }
}

unsigned int advanceCycle(Hypercube& hyper_cube)
{
    unsigned int active_sum{};

    Hypercube hyper_cube_copy = hyper_cube;
    for (int cube_it = 0; cube_it < (int)hyper_cube.size(); ++cube_it)
    {
        for (int page_it = 0; page_it < (int)hyper_cube[cube_it].size(); ++page_it)
        {
            for (int row_it = 0; row_it < (int)hyper_cube[cube_it][page_it].size(); ++row_it)
            {
                for (int col_it = 0; col_it < (int)hyper_cube[cube_it][page_it][row_it].size(); ++col_it)
                {

                    unsigned int active_neigh{};
                    for (int cube_it_n = std::max(0, cube_it - 1);
                        cube_it_n < std::min(cube_it + 2, (int)hyper_cube.size()); ++cube_it_n)
                    {
                        for (int page_it_n = std::max(0, page_it - 1);
                            page_it_n < std::min(page_it + 2, (int)hyper_cube[cube_it].size()); ++page_it_n)
                        {
                            for (int row_it_n = std::max(0, row_it - 1);
                                row_it_n < std::min(row_it + 2, (int)hyper_cube[cube_it][page_it].size()); ++row_it_n)
                            {
                                for (int col_it_n = std::max(0, col_it - 1);
                                    col_it_n < std::min(col_it + 2, (int)hyper_cube[cube_it][page_it][row_it].size()); ++col_it_n)
                                {
                                    if (cube_it == cube_it_n && page_it == page_it_n && row_it == row_it_n && col_it == col_it_n) continue;

                                    if (hyper_cube_copy[cube_it_n][page_it_n][row_it_n][col_it_n]) ++active_neigh;
                                }
                            }
                        }
                    }


                    if (hyper_cube_copy[cube_it][page_it][row_it][col_it])
                    {
                        if (2 == active_neigh || 3 == active_neigh)
                        {
                            ++active_sum;
                        }
                        else
                        {
                            hyper_cube[cube_it][page_it][row_it][col_it] = false;
                        }
                    }
                    else
                    {
                        if (3 == active_neigh)
                        {
                            hyper_cube[cube_it][page_it][row_it][col_it] = true;
                            ++active_sum;
                        }
                    }
                }
            }
        }
    }

    return active_sum;
}

int main()
{
    std::ifstream file("input.txt");

    Grid cube_grid(1, Page(1));
    unsigned int it_page{}, it_row{};
    for (char c; file.get(c); )
    {
        if ('\n' == c)
        {
            cube_grid[it_page].emplace_back(Row{});
            ++it_row;
        }
        else if ('.' == c)
        {
            cube_grid[it_page][it_row].emplace_back(false);
        }
        else if ('#' == c)
        {
            cube_grid[it_page][it_row].emplace_back(true);
        }
    }

    const auto begin = [](auto& container) { return container.begin(); };
    const auto rbegin = [](auto& container) { return container.rbegin(); };
    const auto end = [](auto& container) { return container.end(); };
    const auto rend = [](auto& container) { return container.rend(); };
    const auto emplace_front = [](auto& container, auto&& val) { container.emplace(container.begin(), val); };
    const auto emplace_back = [](auto& container, auto&& val) { container.emplace(container.end(), val); };

    Hypercube hyper_cube[2]{ Hypercube(1, cube_grid), Hypercube(1, cube_grid) };
    unsigned int active_count[2]{ {}, {} };

    for (int part = 0; part < 2; ++part)
    {
        for (int i = 0; i < 6; ++i)
        {
            expandGrid(hyper_cube[part], begin, end, emplace_front, part == 1);
            expandGrid(hyper_cube[part], rbegin, rend, emplace_back, part == 1);

            active_count[part] = advanceCycle(hyper_cube[part]);
        }
    }

    //Part1
    std::cout << "(Part 1) Active cubes after 6 cycles (3D): " << active_count[0] << std::endl;
    //Part2
    std::cout << "(Part 2) Active cubes after 6 cycles (4D): " << active_count[1];

    std::cin.get();
    return 0;
}

//(Part 1) Active cubes after 6 cycles (3D): 359
//(Part 2) Active cubes after 6 cycles(4D) : 2228
