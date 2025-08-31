#pragma once

#include "Symbol.hpp"
#include "Util.hpp"
#include "UUID.h"
#include <vector>
#include <ostream>
#include <algorithm>
#include <ranges>

namespace IStudio::Compiler
{
    class Rule
    {
    public:
        using Left_Type = Symbol;
        using Right_Type = std::vector<Symbol>;

    private:
        Left_Type left;
        Right_Type right;
        Util::UUID guid;

    public:
        // Constructors and destructor
        Rule(Left_Type l, Right_Type r)
            : left{l}, right{r}
        {
        }

        Rule(const Rule &) = default;
        Rule(Rule &&) = default;
        Rule &operator=(const Rule &) = default;
        Rule &operator=(Rule &&) = default;
        Rule() = default;
        ~Rule() = default;

        // Accessor methods
        const Left_Type &getLeft() const
        {
            return left;
        }

        auto getRight() const
        {
            return right;
        }

        // Comparison operators
        bool operator==(const Rule &r) const
        {
            return getLeft() == r.getLeft() && getRight() == r.getRight();
        }

        bool operator!=(const Rule &r) const
        {
            return !(*this == r);
        }

        bool operator<(const Rule &r) const
        {
            auto r1 = getRight();
            auto r2 = r.getRight();
            return std::lexicographical_compare(r1.begin(), r1.end(), r2.begin(), r2.end());
        }

        bool operator>(const Rule &r) const
        {
            auto r1 = getRight();
            auto r2 = r.getRight();
            return !std::lexicographical_compare(r1.begin(), r1.end(), r2.begin(), r2.end());
        }

        bool operator<=(const Rule &r) const
        {
            return (*this == r) || (*this < r);
        }

        bool operator>=(const Rule &r) const
        {
            return (*this == r) || (*this > r);
        }

        // Output operator for printing
        friend std::ostream &operator<<(std::ostream &o, const Rule &r)
        {
            o << r.getLeft() << " <= ";
            auto right = r.getRight();
            for (const auto &rhs : right)
                o << rhs << " ";
            return o;
        }

        // Get precedence based on the right-hand side symbols
        Lang::Integer getPrecedence() const
        {
            for (auto symbol : std::views::reverse(getRight()))
            {
                if (symbol.isTerminal())
                    return symbol.getPrecedence();
            }
            return DOLLAR.getPrecedence();
        }
    };

    // Default rule definition
    static Rule DEFAULT_RULE;

} // namespace IStudio::Compiler
