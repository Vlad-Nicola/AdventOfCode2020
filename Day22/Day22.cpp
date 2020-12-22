#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <unordered_set>

uint64_t computeChecksum(const std::list<unsigned int>& deck0, const std::list<unsigned int>& deck1)
{
    uint64_t res{};
    for (const auto& card : deck0) res = (res << 1) ^ card;
    res = (res << 1) ^ 51;
    for (const auto& card : deck1) res = (res << 1) ^ card;
    return res;
}

bool playRecursiveCombat(std::list<unsigned int>& deck0, std::list<unsigned int>& deck1)
{
    std::unordered_set<uint64_t> prev_games;
    while (deck0.size() && deck1.size())
    {
        uint64_t hash = computeChecksum(deck0, deck1);
        if (const auto& [it, inserted] = prev_games.insert(hash); !inserted)
        {
            return true;
        }

        unsigned int p0{ deck0.front() }, p1{ deck1.front() };
        deck0.pop_front(); deck1.pop_front();

        bool winner_player_one{ false };
        if (deck0.size() >= p0 && deck1.size() >= p1)
        {
            auto end0 = deck0.cbegin(); std::advance(end0, p0);
            std::list<unsigned int> deck0_copy(deck0.cbegin(), end0);

            auto end1 = deck1.cbegin(); std::advance(end1, p1);
            std::list<unsigned int> deck1_copy(deck1.cbegin(), end1);

            winner_player_one = playRecursiveCombat(deck0_copy, deck1_copy);
        }
        else
        {
            winner_player_one = p0 > p1;
        }

        if(winner_player_one)
        {
            deck0.push_back(p0);
            deck0.push_back(p1);
        }
        else
        {
            deck1.push_back(p1);
            deck1.push_back(p0);
        }
    }

    return deck0.size();
}

void playCombat(std::list<unsigned int>& deck0, std::list<unsigned int>& deck1)
{
    while (deck0.size() && deck1.size())
    {
        unsigned int p0{ deck0.front() }, p1{ deck1.front() };
        deck0.pop_front(); deck1.pop_front();

        if (p0 > p1)
        {
            deck0.push_back(p0);
            deck0.push_back(p1);
        }
        else
        {
            deck1.push_back(p1);
            deck1.push_back(p0);
        }
    }
}

unsigned int getWinnerScore(const std::list<unsigned int>& winner_deck)
{
    unsigned int score{}, count{ 1 };
    for (auto it = winner_deck.crbegin(); it != winner_deck.crend(); ++it)
    {
        score += count++ * *it;
    }
    return score;
}

int main()
{
    std::ifstream file("input.txt");

    std::list<unsigned int> decks_p1[2]{};
    unsigned int current_deck = 0;
    for (std::string line; std::getline(file, line); )
    {
        if (line == "Player 2:")
        {
            current_deck = 1;
        }
        else if (line.empty() || line == "Player 1:") continue;
        else
        {
            decks_p1[current_deck].push_back(std::stoul(line));
        }
    }
    std::list<unsigned int> decks_p2[2]{ decks_p1[0], decks_p1[1] };

    playCombat(decks_p1[0], decks_p1[1]);
    bool player_zero_won{ playRecursiveCombat(decks_p2[0], decks_p2[1]) };

    //Part1
    std::cout << "(Part 1) Score of winner: " << getWinnerScore(decks_p1[0].size() ? decks_p1[0] : decks_p1[1]) << std::endl;
    //Part2
    std::cout << "(Part 2) Score of winner: " << getWinnerScore(player_zero_won ? decks_p2[0] : decks_p2[1]);


    std::cin.get();
    return 0;
}

