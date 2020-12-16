#include <iostream>
#include <unordered_map>

int main()
{
    std::unordered_map<unsigned int, unsigned int> spoken_catalog {{13, 1}, {0, 2}, {10, 3}, {12, 4}, {1, 5}, {5, 6}};

    unsigned int last_spoken{ 8 }, last_spoken_turn_2020{};
    for (unsigned int turn{ spoken_catalog.size() + 1};  turn != 30000000; ++turn)
    {
        if (2020 == turn) last_spoken_turn_2020 = last_spoken;

        const auto& [it, inserted] = spoken_catalog.insert({ last_spoken, turn });
        last_spoken = inserted ? 0 : turn - it->second;
        it->second = turn;
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
