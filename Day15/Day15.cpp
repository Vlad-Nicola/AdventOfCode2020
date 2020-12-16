#include <iostream>
#include <vector>

int main()
{
    std::vector<unsigned int> cat(30000000, 0);
    cat[13] = 1; cat[0] = 2; cat[10] = 3; cat[12] = 4; cat[1] = 5; cat[5] = 6;

    unsigned int last_spoken{ 8 }, last_spoken_turn_2020{};
    for (unsigned int turn{7};  turn != 30000000; ++turn)
    {
        if (2020 == turn) last_spoken_turn_2020 = last_spoken;

        auto& val = cat[last_spoken];
        last_spoken = val ? turn - val : 0;
        val = turn;
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
