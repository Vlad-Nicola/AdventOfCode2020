#include <iostream>
#include <fstream>
#include <regex>
#include <list>

bool func(const std::vector<std::vector<bool>>& possibilities, const int index, std::vector<int>& field_to_category)
{
    if (index == field_to_category.size())
    {
        return true;
    }

    const auto& categories = possibilities[index];
    for (unsigned int i = 0; i < categories.size(); ++i)
    {
        if (categories[i] == false) continue;

        auto it = std::find(field_to_category.begin(), field_to_category.end(), i);
        if (it != field_to_category.end())
        {
            continue;
        }

        field_to_category[index] = i;

        bool interm_res = func(possibilities, index + 1, field_to_category);

        if (interm_res) return true;

        field_to_category[index] = -1;
    }

    return false;
}

int main()
{
    using Ranges = std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int>>;

    std::ifstream file("input.txt");

    std::vector<Ranges> categories;
    std::vector<bool> cat_is_departure;
    std::vector<std::vector<unsigned int>> tickets;

    std::regex re_category{ "(.*): (\\d+)-(\\d+) or (\\d+)-(\\d+)" }, re_ticket{ "\\d+" };

    std::string line;
    while (std::getline(file, line))
    {
        if (std::smatch match; std::regex_match(line, match, re_category))
        {
            cat_is_departure.push_back(match[1].str().find("departure") != std::string::npos ? true : false);
            categories.emplace_back(std::make_pair(std::stoi(match[2].str()), std::stoi(match[3].str())),
                                    std::make_pair(std::stoi(match[4].str()), std::stoi(match[5].str())));
        }
        else if(line.size() && line.front() >= '0' && line.front() <= '9')
        {
            tickets.emplace_back(std::vector<unsigned int>{});
            for (std::sregex_token_iterator re_it(line.cbegin(), line.cend(), re_ticket); re_it != std::sregex_token_iterator(); ++re_it)
            {
                tickets.back().emplace_back(std::stoi(*re_it));
            }
        }
    }

    std::vector<std::vector<bool>> possibilities(tickets[0].size(), std::vector<bool>(categories.size(), true));
    unsigned int sum_invalid{};
    for (auto ticket_it = tickets.cbegin() + 1; ticket_it != tickets.cend(); ++ticket_it)
    {
        for (unsigned int field_index = 0; field_index < ticket_it->size(); ++field_index)
        {
            bool valid{ false };
            auto possib_copy = possibilities[field_index];
            for (unsigned int cat_index = 0; cat_index < categories.size(); ++cat_index)
            {
                const auto& [lower, higher] = categories[cat_index];
                const auto& [lower_low, lower_high] = lower;
                const auto& [higher_low, higher_high] = higher;

                if (lower_low <= ticket_it->at(field_index)  && ticket_it->at(field_index) <= lower_high ||
                    higher_low <= ticket_it->at(field_index) && ticket_it->at(field_index) <= higher_high )
                {
                    valid = true;
                }
                else
                {
                    possibilities[field_index][cat_index] = false;
                }

            }
            if (!valid)
            {
                possibilities[field_index] = std::move(possib_copy);
                sum_invalid += ticket_it->at(field_index);
                break;
            }
        }
    }

    uint64_t  departure_prod{1};
    std::vector<int> field_to_category(categories.size(), -1);
    if (func(possibilities, 0, field_to_category))
    {
        for (unsigned int field_index = 0; field_index < field_to_category.size(); ++field_index)
        {
            if (cat_is_departure[field_to_category[field_index]])
            {
                departure_prod *= tickets.front()[field_index];
            }
        }
    }

    //Part1
    std::cout << "(Part 1) Sum of invalid fields: " << sum_invalid << std::endl;
    //Part2
    std::cout << "(Part 2) Prod of departure fields: " << departure_prod << std::endl;

    std::cin.get();
    return 0;
}

//(Part 1) Sum of invalid fields: 21956
//(Part 2) Prod of departure fields : 3709435214239
