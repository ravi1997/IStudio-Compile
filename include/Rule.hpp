#pragma once

#include "Symbol.hpp"
#include "Util.hpp"

namespace IStudio::Compiler
{
    class Rule
    {
    public:
        constexpr static std::size_t array_size = 25;
        using Left_Type = Symbol;
        using Right_Type = std::array<Symbol, array_size>;

    private:
        Left_Type left;
        Right_Type right;
        bool valid = false;

    public:
        constexpr Rule(Left_Type l, Right_Type r)
            : left{l}, right{r}, valid{true}
        {
        }

        constexpr Rule() = default;
        constexpr ~Rule() = default;

        constexpr Left_Type getLeft() const
        {
            return left;
        }

        constexpr Right_Type getRight() const
        {
            return right;
        }

        constexpr bool isValid() const
        {
            return valid;
        }

        constexpr bool operator==(const Rule &r) const
        {
            return getLeft() == r.getLeft() && getRight() == r.getRight();
        }

        constexpr bool operator!=(const Rule &r) const
        {
            return !(*this == r);
        }

        constexpr bool operator<(const Rule &r) const
        {
            if (getLeft() == r.getLeft())
                return getRight() < r.getRight();
            return getLeft() < r.getLeft();
        }

        constexpr bool operator>(const Rule &r) const
        {
            if (getLeft() == r.getLeft())
                return getRight() > r.getRight();
            return getLeft() > r.getLeft();
        }

        constexpr bool operator<=(const Rule &r) const
        {
            if (getLeft() == r.getLeft())
                return getRight() <= r.getRight();
            return getLeft() <= r.getLeft();
        }

        constexpr bool operator>=(const Rule &r) const
        {
            if (getLeft() == r.getLeft())
                return getRight() >= r.getRight();
            return getLeft() >= r.getLeft();
        }

        friend std::ostream &operator<<(std::ostream &o, const Rule &r)
        {
            o << r.getLeft() << " <= ";
            auto right = r.getRight();
            for (auto rhs : Util::iterate(right))
                o << rhs << " ";
            return o;
        }
    };

    constexpr Rule DEFAULT_RULE;
} // namespace IStudio::Compiler
