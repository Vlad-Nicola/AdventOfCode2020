#include <iostream>
#include <fstream>

class Boat
{
public:
    virtual void moveNorth(const int amount) { y += amount; }
    virtual void moveSouth(const int amount) { y -= amount; }
    virtual void moveEast(const int amount) { x += amount; }
    virtual void moveWest(const int amount) { x -= amount; }
    void shift(const unsigned int direction)
    {
        if (1 == direction)
        {
            std::swap(x_offset, y_offset);
            y_offset *= -1;
        }
        else if (2 == direction)
        {
            x_offset *= -1;
            y_offset *= -1;
        }
        else if (3 == direction)
        {
            std::swap(x_offset, y_offset);
            x_offset *= -1;
        }
    }
    virtual void forward(const int amount)
    {
        x += x_offset * amount;
        y += y_offset * amount;
    }
    unsigned int getManhattan() const
    {
        return std::abs(x) + std::abs(y);
    }

protected:
    int x = 0, y = 0;
    int x_offset = 1, y_offset = 0;
};

class WPBoat : public Boat
{
public:
    WPBoat(const int x_offset_, const int y_offset_) { x_offset = x_offset_; y_offset = y_offset_; }
    void moveNorth(const int amount) override { y_offset += amount; }
    void moveSouth(const int amount) override { y_offset -= amount; }
    void moveEast(const int amount) override { x_offset += amount; }
    void moveWest(const int amount) override { x_offset -= amount; }
};

void navigate(Boat& boat, const char command, const int amount)
{
    switch (command)
    {
    case 'N':
    {
        boat.moveNorth(amount); break;
    }
    case 'S':
    {
        boat.moveSouth(amount); break;
    }
    case 'E':
    {
        boat.moveEast(amount); break;
    }
    case 'W':
    {
        boat.moveWest(amount); break;
    }
    case 'R':
    {
        boat.shift(amount / 90 % 4); break;
    }
    case 'L':
    {
        boat.shift(4 - amount / 90 % 4); break;
    }
    case 'F':
    {
        boat.forward(amount);
        break;
    }
    default: std::cout << "Invalid command!!!" << std::endl;
    }
}

int main()
{
    std::ifstream file("input.txt");

    int wp_offset_x = 10, wp_offset_y = 1;

    char command; int amount;
    Boat boat_p1;
    WPBoat boat_p2{ 10, 1 };
    while (file >> command >> amount)
    {
        navigate(boat_p1, command, amount);
        navigate(boat_p2, command, amount);
    }

    //Part1
    std::cout << "(Part 1) Manhattan distance traveled (without waypoint): " << boat_p1.getManhattan() << std::endl;
    //Part2
    std::cout << "(Part 2) Manhattan distance traveled (with waypoint):" << boat_p2.getManhattan() << std::endl;

    std::cin.get();
    return 0;
}

//(Part 1) Manhattan distance traveled (without waypoint): 445
//(Part 2) Manhattan distance traveled(with waypoint): 42495
