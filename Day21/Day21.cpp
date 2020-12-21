#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

using Food = std::pair<std::vector<std::string>, std::vector<std::string>>;

int main()
{
    std::ifstream file{ "input.txt" };

    std::map<std::string, std::vector<std::string>> allergens;
    std::vector<Food> foods;
    while (!file.eof())
    {
        Food food;
        bool parsing_allergens{ false };
        for (std::string word; file >> word; )
        {
            if (parsing_allergens)
            {
                const auto alg = word.substr(0, word.size() - 1);
                allergens.emplace(alg, std::vector<std::string>{});
                food.second.emplace_back(alg);
                if (word.back() == ')')
                {
                    break;
                }
            }
            else
            {
                if (word == "(contains")
                {
                    parsing_allergens = true;
                }
                else
                {
                    food.first.emplace_back(word);
                }
            }
        }
        foods.emplace_back(food);
    }

    for (auto& [allergen, candidates] : allergens)
    {
        for (const auto& [ingredients_f, allergens_f] : foods)
        {
            if (std::find(allergens_f.cbegin(), allergens_f.cend(), allergen) == allergens_f.cend())
            {
                continue;
            }

            if (candidates.empty())
            {
                candidates.assign(ingredients_f.cbegin(), ingredients_f.cend());
            }
            else
            {
                for (auto it = candidates.begin(); it != candidates.end(); )
                {
                    if (std::find(ingredients_f.cbegin(), ingredients_f.cend(), *it) == ingredients_f.cend())
                    {
                        it = candidates.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    }

    bool no_change{ false };
    while (!no_change)
    {
        no_change = true;
        for (const auto& [allergen, candidates] : allergens)
        {
            if (candidates.size() != 1)
            {
                continue;
            }

            for (auto& [allergen_c, candidates_c] : allergens)
            {
                if (allergen == allergen_c) continue;

                if (auto it = std::find(candidates_c.begin(), candidates_c.end(), candidates.front()); it != candidates_c.end())
                {
                    candidates_c.erase(it);
                    no_change = false;
                }
            }
        }
    }

    unsigned int total_ingredients{};
    for (const auto& [ingredients_c, allergens_c] : foods)
    {
        total_ingredients += ingredients_c.size();
        for (const auto& [allergen_f, ingredients_f] : allergens)
        {
            for (const auto& ingredient : ingredients_f)
            {
                if (std::find(ingredients_c.cbegin(), ingredients_c.cend(), ingredient) != ingredients_c.cend())
                {
                    --total_ingredients;
                }
            }
        }
    }

    //Part 1
    std::cout << "(Part 1) Safe ingredients count: " << total_ingredients << std::endl;

    //Part 2
    std::cout << "(Part 2) Ingredients with allergens: ";
    for (auto it{ allergens.cbegin() }; it != allergens.cend(); ++it)
    {
        std::cout << it->second.front();
        if (it != std::prev(allergens.cend()))
        {
            std::cout << ",";
        }
    }

    std::cin.get();
    return 0;
}

//(Part 1) Safe ingredients count: 2569
//(Part 2) Ingredients with allergens : vmhqr, qxfzc, khpdjv, gnrpml, xrmxxvn, rfmvh, rdfr, jxh
