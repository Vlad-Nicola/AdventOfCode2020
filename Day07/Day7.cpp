#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <memory>

using ContainedMap = std::unordered_map<std::string, std::vector<std::string>>;
using ContainsMap = std::unordered_map<std::string, std::vector<std::pair<std::string, unsigned int>>>;

unsigned int getContainerQuantity(const ContainedMap& map, const std::string& bag, std::vector<std::string>& visited)
{
    unsigned int return_value = 0;

    if (std::find(visited.cbegin(), visited.cend(), bag) != visited.cend())
    {
        return 0;
    }

    const auto& container_vec = map.at(bag);
    for (const auto& container : container_vec)
    {
        return_value += getContainerQuantity(map, container, visited);
    }

    visited.push_back(bag);

    return return_value + 1;
}

unsigned int getContainedQuantity(const ContainsMap& map, const std::string& bag)
{
    unsigned int return_value = 0;

    const auto& container_vec = map.at(bag);
    for (const auto& container : container_vec)
    {
        return_value += container.second * getContainedQuantity(map, container.first);
    }

    return return_value + 1;
}

int main()
{
    ContainedMap contained_map;
    ContainsMap  contains_map;

    std::ifstream file("input.txt");

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string word;

        std::string container = "";
        iss >> container >> word; container += word;
        contained_map[container];
        contains_map[container];

        iss >> word >> word;

        unsigned int quantity;
        while (iss >> quantity)
        {
            std::string contained = "";

            iss >> contained >> word; contained += word; iss >> word;

            contained_map[contained].push_back(container);
            contains_map[container].emplace_back(contained, quantity);
        }
    }

    const std::string my_bag = "shinygold";
    std::vector<std::string> visited;
    const unsigned int container_quantity = getContainerQuantity(contained_map, my_bag, visited) - 1;
    const unsigned int contained_quantity = getContainedQuantity(contains_map, my_bag) - 1;

    //Part1
    std::cout << "(Part 1) Bags that could fit my bag: " << container_quantity << std::endl;
    //Part2
    std::cout << "(Part 2) Bags contained in my bag: " << contained_quantity;

    std::cin.get();
    return 0;
}

//(Part 1) Bags that could fit my bag: 115
//(Part 2) Bags contained in my bag : 1250
