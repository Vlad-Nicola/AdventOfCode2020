#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <regex>

struct Tile
{
    int id{};
    std::vector<std::string> piece;
    std::shared_ptr<Tile> neigh[4]{ nullptr };
    bool is_fit{ false };

    Tile() {}
    Tile(const std::shared_ptr<Tile>& top_left_corner)
    {
        unsigned int piece_count{};
        auto top_down = top_left_corner;
        while (top_down)
        {
            auto left_right = top_down;
            for (unsigned int i{ 1 }; i < left_right->piece.size() - 1; ++i)
            {
                piece.push_back("");
                while (left_right)
                {
                    piece.back() += left_right->piece[i].substr(1, left_right->piece[i].size() - 2);
                    left_right = left_right->neigh[0];
                }
                left_right = top_down;
            }
            top_down = top_down->neigh[1];
            ++piece_count;
        }
    }

    void flip()
    {
        for (unsigned int i = 0; i < piece.size(); ++i)
        {
            std::reverse(piece[i].begin(), piece[i].end());
        }
    }

    void rotate()
    {
        auto piece_copy = piece;
        for (unsigned int i{ 0 }; i < piece.size(); ++i)
        {
            for (unsigned int j{ 0 }; j < piece[i].size(); ++j)
            {
                piece[j][piece[j].size() - 1 - i] = piece_copy[i][j];
            }
        }
    }
};

bool isMatching(const Tile& first, const Tile& second, const unsigned int dir)
{
    switch (dir)
    {
    case 0:
    {
        for (size_t i{}; i != first.piece.size(); ++i)
        {
            if (first.piece[i].back() != second.piece[i].front())
            {
                return false;
            }
        }
        break;
    }
    case 1:
    {
        if (first.piece.back() != second.piece.front())
        {
            return false;
        }
        break;
    }
    case 2:
    {
        for (size_t i{}; i != first.piece.size(); ++i)
        {
            if (first.piece[i].front() != second.piece[i].back())
            {
                return false;
            }
        }
        break;
    }
    case 3:
    {
        if (first.piece.front() != second.piece.back())
        {
            return false;
        }
        break;
    }
    }
    
    return true;
}

int main()
{
    std::ifstream file("input.txt");

    std::vector<std::shared_ptr<Tile>> tiles;

    Tile current_tile;
    while (!file.eof())
    {
        for (std::string line; std::getline(file, line); )
        {
            if (size_t pos{ line.find(":") }; pos != std::string::npos)
            {
                current_tile.id = std::stoi(line.substr(pos - 4, 4));
            }
            else if (line.size())
            {
                current_tile.piece.emplace_back(line);
            }
            else
            {
                break;
            }
        }
        tiles.emplace_back(std::make_shared<Tile>(std::move(current_tile)));
    }

    const std::function<void(Tile&)> transformations[]{
        [](auto& tile) {},
        [](auto& tile) {tile.rotate(); },
        [](auto& tile) {tile.rotate(); },
        [](auto& tile) {tile.rotate(); },
        [](auto& tile) {tile.flip(); },
        [](auto& tile) {tile.rotate(); },
        [](auto& tile) {tile.rotate(); },
        [](auto& tile) {tile.rotate(); }
    };

    std::queue<std::shared_ptr<Tile>> queue;
    tiles.front()->is_fit = true;
    queue.push(tiles.front());
    while (!queue.empty())
    {
        auto target = queue.front();
        queue.pop();
        for (unsigned int candidate{ 0 }; candidate < tiles.size(); ++candidate)
        {
            if (target->id == tiles[candidate]->id) continue;

            for (const auto& transformation : transformations)
            {
                transformation(*tiles[candidate]);
                for (unsigned int dir{}; dir < 4; ++dir)
                {
                    if (!target->neigh[dir] &&
                        !tiles[candidate]->neigh[(dir + 2) % 4] &&
                        isMatching(*target, *tiles[candidate], dir))
                    {
                        target->neigh[dir] = tiles[candidate];
                        tiles[candidate]->neigh[(dir + 2) % 4] = target;
                        tiles[candidate]->is_fit = true;

                        queue.push(tiles[candidate]);
                    }
                }

                if (tiles[candidate]->is_fit) break;
            }
        }
    }

    uint64_t corners_product{ 1 };
    std::shared_ptr<Tile> top_left;
    for (const auto& tile : tiles)
    {
        if (!tile->neigh[0] && !tile->neigh[1])
        {
            corners_product *= tile->id;
        }
        else if (!tile->neigh[1] && !tile->neigh[2])
        {
            corners_product *= tile->id;
        }
        else if (!tile->neigh[2] && !tile->neigh[3])
        {
            corners_product *= tile->id;
            top_left = tile;
        }
        else if (!tile->neigh[3] && !tile->neigh[0])
        {
            corners_product *= tile->id;
        }
    }

    const std::regex re_body{ "#.{4,4}##.{4,4}##.{4,4}###" };
    const std::regex re_anti_body{"#(.{4,4})##(.{4,4})##(.{4,4})###"};
    const std::regex re_legs{ "#..#..#..#..#..#" };
    const std::regex re_anti_legs{ "#(..)#(..)#(..)#(..)#(..)#" };

    Tile puzzle(top_left);
    for (const auto& transformation : transformations)
    {
        transformation(puzzle);
        for (unsigned int row{ 1 }; row < puzzle.piece.size() - 1; ++row)
        {
            for (unsigned int col{ 19 }; col < puzzle.piece[row].size(); ++col)
            {
                if ('#' == puzzle.piece[row][col] && '#' == puzzle.piece[row - 1][col - 1])
                {
                    const std::string body_substr{ puzzle.piece[row].substr(col - 19, 20) };
                    const std::string legs_substr{ puzzle.piece[row + 1].substr(col - 18, 16) };
                    if (std::regex_match(body_substr, re_body) &&
                        std::regex_match(legs_substr, re_legs))
                    {
                        auto body = std::regex_replace(puzzle.piece[row].substr(col - 19, 20), re_anti_body, "O$1OO$2OO$3OOO");
                        puzzle.piece[row].replace(col - 19, 20, body);
                        auto legs = std::regex_replace(puzzle.piece[row + 1].substr(col - 18, 16), re_anti_legs, "O$1O$2O$3O$4O$5O");
                        puzzle.piece[row + 1].replace(col - 18, 16, legs);
                        puzzle.piece[row - 1][col - 1] = 'O';
                    }
                }
            }
        }
    }

    unsigned int total_hashes{};
    for (const auto& line : puzzle.piece)
    {
        total_hashes += std::count(line.cbegin(), line.cend(), '#');
    }

    //Part1
    std::cout << "(Part 1) Product of corner IDs: " << corners_product << std::endl;
    //Part2
    std::cout << "(Part 2) Non-Monster hashes: " << total_hashes;

    std::cin.get();
    return 0;
}

//(Part 1) Product of corner IDs : 54755174472007
//(Part 2) Non - Monster hashes : 1692
