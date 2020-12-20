#include <iostream>
#include <fstream>
#include <deque>

enum class op_t { none, add, mul };

int64_t doOp(const int64_t left, const op_t op, const int64_t right)
{
    if (op_t::add == op)
    {
        return left + right;
    }
    else if (op_t::mul == op)
    {
        return left * right;
    }
    else
    {
        return right;
    }
}

void resetValues(int64_t& result, op_t& op)
{
    result = -1;
    op = op_t::none;
}

int main()
{ 
    std::ifstream file("input.txt");

    std::deque<std::pair<int64_t, op_t>> stack_p1;
    std::deque<std::tuple<int64_t, op_t, bool>> stack_p2;

    int64_t result_p1{ -1 }, total_sum_p1{};
    int64_t result_p2{ -1 }, total_sum_p2{};
    op_t op_p1{ op_t::none }, op_p2{ op_t::none };
    while (!file.eof())
    {
        for (char c; file.get(c); )
        {
            if (' ' == c)
            {
                continue;
            }
            else if ('\n' == c)
            {
                break;
            }
            else if ('0' <= c && c <= '9')
            {
                result_p1 = doOp(result_p1, op_p1, c - '0');
                result_p2 = doOp(result_p2, op_p2, c - '0');
            }
            else if ('+' == c)
            {
                op_p1 = op_p2 = op_t::add;
            }
            else if ('*' == c)
            {
                op_p1 = op_p2 = op_t::mul;
                stack_p2.emplace_back(result_p2, op_p2, false);
                resetValues(result_p2, op_p2);
            }
            else if ('(' == c)
            {
                stack_p1.emplace_back(result_p1, op_p1);
                stack_p2.emplace_back(result_p2, op_p2, true);
                resetValues(result_p1, op_p1);
                resetValues(result_p2, op_p2);
            }
            else if (')' == c)
            {
                const auto& [stack_result_p1, stack_op_p1] = stack_p1.back();
                result_p1 = doOp(stack_result_p1, stack_op_p1, result_p1);
                stack_p1.pop_back();

                while (true)
                {
                    auto& [stack_result_p2, stack_op_p2, par] = stack_p2.back();
                    if (!par)
                    {
                        result_p2 = doOp(stack_result_p2, stack_op_p2, result_p2);
                        stack_p2.pop_back();
                    }
                    else
                    {
                        par = false;
                        if (stack_op_p2 != op_t::mul)
                        {
                            result_p2 = doOp(stack_result_p2, stack_op_p2, result_p2);
                            stack_p2.pop_back();
                        }
                        break;
                    }
                }
            }
        }

        while (stack_p2.size())
        {
            const auto& [stack_result, stack_op, par] = stack_p2.back();
            result_p2 = doOp(stack_result, stack_op, result_p2);
            stack_p2.pop_back();
        }
        total_sum_p1 += result_p1;
        total_sum_p2 += result_p2;
        resetValues(result_p1, op_p1);
        resetValues(result_p2, op_p2);
    }

    //Part1
    std::cout << "(Part 1) Sum of results: " << total_sum_p1 << std::endl;
    //Part2
    std::cout << "(Part 2) Sum of results: " << total_sum_p2;

    std::cin.get();
    return 0;
}

//(Part 1) Sum of results: 8929569623593
//(Part 2) Sum of results : 231235959382961
