#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream file("input.txt");
    
    unsigned int arrival_time;
    file >> arrival_time;

    int min_time = std::numeric_limits<int>::max();
    int fastest_bus_id{};

    uint64_t prev_number = 1, total_number = 0;
    int index = 0;

    std::string bus;
    while (std::getline(file, bus, ','))
    {
        if (bus != "x")
        {
            int bus_id = std::stoi(bus);
            int time_to_come = bus_id - arrival_time % bus_id;
            if (time_to_come < min_time)
            {
                min_time = time_to_come;
                fastest_bus_id = time_to_come * bus_id;
            }

            if (!total_number) total_number = bus_id;

            int req_rem = (bus_id - index) % bus_id;
            if (req_rem < 0) req_rem += bus_id;

            while (total_number % bus_id != req_rem)
            {
                total_number += prev_number;
            }

            prev_number *= bus_id;
        }

        ++index;
    }

    //Part1
    std::cout << "(Part 1) Fastest bus * time to arrival: " << fastest_bus_id << std::endl;
    //Part2
    std::cout << "(Part 2) Lowest timestamp: " << total_number;
    
    std::cin.get();
    return 0;
}

//(Part 1) Fastest bus * time to arrival: 205
//(Part 2) Lowest timestamp: 803025030761664
