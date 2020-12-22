#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <unordered_map>

using Rule = std::pair < std::tuple<int, int, int>, std::tuple<int, int, int>>;

std::string generateRule(const std::unordered_map<int, Rule>& mp, const int ab[2], const int rule)
{
    if (rule == ab[0]) return "a";
    if (rule == ab[1]) return "b";

    const auto& [left, right] = mp.at(rule);
    const auto& [l_left, l_middle, l_right] = left;
    const auto& [r_left, r_middle, r_right] = right;
    return "(?:" + generateRule(mp, ab, l_left) +
        (l_middle != -1 ? generateRule(mp, ab, l_middle) : "") +
        (l_right != -1 ? generateRule(mp, ab, l_right) : "") +
        (r_left != -1 ? "|" + generateRule(mp, ab, r_left) : "") +
        (r_middle != -1 ? generateRule(mp, ab, r_middle) : "") +
        (r_right != -1 ? generateRule(mp, ab, r_right) : "") +
        ")";
}

int main()
{
    std::ifstream file("input.txt");

    std::unordered_map<int, Rule> mp;
    int ab[2]{ 0, 0 };

    std::regex re_rule1("(\\d+): (\\d+)( \\d+)?( \\d+)?( [|] (\\d+)( \\d+)?( \\d+)?)?");
    std::regex re_rule2("(\\d+): \"([ab])\"");

    std::regex rule_p1, rule_p2[2];
    unsigned int match_count_p1{}, match_count_p2{};
    for (std::string line; std::getline(file, line);)
    {
        if (std::smatch match; std::regex_match(line, match, re_rule1))
        {
            int rule_no = std::stoi(match[1].str());
            Rule elements{ {-1, -1, -1}, {-1, -1, -1} };
            auto& [l_left, l_middle, l_right] = elements.first;
            auto& [r_left, r_middle, r_right] = elements.second;

            l_left = std::stoi(match[2].str());
            if (match[3].matched)
            {
                l_middle = std::stoi(match[3].str());
            }
            if (match[4].matched)
            {
                l_right = std::stoi(match[4].str());
            }
            if (match[5].matched)
            {
                r_left = std::stoi(match[6].str());
                if (match[7].matched)
                {
                    r_middle = std::stoi(match[7].str());
                }
                if (match[8].matched)
                {
                    r_right = std::stoi(match[8].str());
                }
            }
            mp[rule_no] = elements;
        }
        else if (std::smatch match; std::regex_match(line, match, re_rule2))
        {
            if (match[2].str() == "a")
            {
                ab[0] = std::stoi(match[1].str());
            }
            else
            {
                ab[1] = std::stoi(match[1].str());
            }
        }
        else if (line.empty())
        {
            rule_p1 = generateRule(mp, ab, 0);
            rule_p2[0] = "^" + generateRule(mp, ab, 42);
            rule_p2[1] = "^" + generateRule(mp, ab, 31);
        }
        else
        {
            if (regex_match(line, rule_p1))
            {
                ++match_count_p1;
            }
            unsigned int count_42{}, count_31{};
            std::smatch match;
            while (std::regex_search(line, match, rule_p2[0]))
            {
                line = match.suffix();
                ++count_42;
            }
            if (count_42 < 2) continue;
            while (std::regex_search(line, match, rule_p2[1]))
            {
                line = match.suffix();
                ++count_31;
            }
            if (line.empty() && count_31 > 0 && count_31 < count_42)
            {
                ++match_count_p2;
            }
        }
    }

    //Part1
    std::cout << "(Part 1) Lines matching rule 0: " << match_count_p1 << std::endl;
    //Part2
    std::cout << "(Part 2) Lines matching rule 0 (loops): " << match_count_p2;

    std::cin.get();
    return 0;
}

//(Part 1) Lines matching rule 0: 203
//(Part 2) Lines matching rule 0 (loops) : 304
