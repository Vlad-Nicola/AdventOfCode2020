#include <iostream>
#include <unordered_map>

int main()
{
    std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>> spoken_catalog
    {
        {13, {0, 1}},
        {0 , {0, 2}},
        {10, {0, 3}},
        {12, {0, 4}},
        {1 , {0, 5}},
        {5 , {0, 6}}    //Last input is last_spoken
    };

    unsigned int last_spoken{ 8 }, last_spoken_turn_2020{};

    for (unsigned int turn{ spoken_catalog.size() + 1};  turn != 30000000; ++turn)
    {
        if (2020 == turn) last_spoken_turn_2020 = last_spoken;

        const auto& [it, inserted] = spoken_catalog.insert({ last_spoken, { 0, turn } });
        if (inserted)
        {
            last_spoken = 0;
        }
        else
        {
            auto& [far, close] = it->second;
            far = close;
            close = turn;
            last_spoken = close - far;
        }
    }
    
    //Part1
    std::cout << "(Part 1) Spoken in turn 2020: " << last_spoken_turn_2020 << std::endl;
    //Part2
    std::cout << "(Part 2) Spoken in turn 30000000: " << last_spoken << std::endl;

    std::cin.get();
    return 0;
}

//(Part 1) Spoken in turn 2020: 260
//(Part 2) Spoken in turn 30000000: 950
