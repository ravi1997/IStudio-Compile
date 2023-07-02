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

        constexpr auto getRight() const
        {
            return IStudio::Util::iterate(right);
        }

        constexpr bool isValid() const
        {
            return valid;
        }

        constexpr bool operator==(const Rule &r) const
        {
            return getLeft() == r.getLeft() && IStudio::Util::compareFilterViews(getRight(), r.getRight());
        }

        constexpr bool operator!=(const Rule &r) const
        {
            return !(*this == r);
        }

        constexpr bool operator<(const Rule &r) const
        {
            auto r1 = getRight();
            auto r2 = r.getRight();

            auto s1 = r1.begin();
            auto e1 = r1.end();
            auto s2 = r2.begin();
            auto e2 = r2.end();

            if (getLeft() == r.getLeft())
                return std::lexicographical_compare(s1,e1,s2,e2);
            return getLeft() < r.getLeft();
        }

        constexpr bool operator>(const Rule &r) const
        {
            auto r1 = getRight();
            auto r2 = r.getRight();

            auto s1 = r1.begin();
            auto e1 = r1.end();
            auto s2 = r2.begin();
            auto e2 = r2.end();

            if (getLeft() == r.getLeft())
                return !std::lexicographical_compare(s1, e1, s2, e2);
            return getLeft() > r.getLeft();
        }

        constexpr bool operator<=(const Rule &r) const
        {
            return (*this==r) || (*this < r);
        }

        constexpr bool operator>=(const Rule &r) const
        {
            return (*this == r) || (*this > r);
        }

        friend std::ostream &operator<<(std::ostream &o, const Rule &r)
        {
            o << r.getLeft() << " <= ";
            auto right = r.getRight();
            for (auto rhs : right)
                o << rhs << " ";
            return o;
        }
    };

    static constexpr Rule DEFAULT_RULE;
} // namespace IStudio::Compiler
