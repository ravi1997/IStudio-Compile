#pragma once

#include "Types_Compiler.hpp"
#include "Rule.hpp"

namespace IStudio::Compiler
{

    class SentinalForm
    {
    public:
        using RuleType = std::reference_wrapper<const Rule>;
        using MarkerType = decltype(DEFAULT_RULE.getRight().begin());

        constexpr SentinalForm() = default;
        constexpr ~SentinalForm() = default;

        constexpr SentinalForm(const RuleType &rule, const MarkerType &marker, bool valid)
            : rule(rule), marker(marker), valid(valid)
        {
        }

        constexpr SentinalForm(const SentinalForm &other)
            : rule(other.rule), marker(other.marker), valid(other.valid)
        {
        }

        constexpr SentinalForm &operator=(const SentinalForm &other)
        {
            if (this != &other)
            {
                rule = other.rule;
                marker = other.marker;
                valid = other.valid;
            }
            return *this;
        }

        constexpr const Rule &getRule() const
        {
            return rule;
        }

        constexpr void setRule(const RuleType &newRule)
        {
            rule = newRule;
        }

        constexpr const MarkerType &getMarker() const
        {
            return marker;
        }

        constexpr void setMarker(const MarkerType &newMarker)
        {
            marker = newMarker;
        }

        constexpr bool isValid() const
        {
            return valid;
        }

        constexpr void setValid(bool newValid)
        {
            valid = newValid;
        }

        constexpr SentinalForm getNext() const
        {
            MarkerType nextMarker = marker;
            ++nextMarker;
            return SentinalForm(rule, nextMarker, valid);
        }

        constexpr Symbol getSymbolAfterMarker() const{
            auto rule = getRule();
            auto right = rule.getRight();
            MarkerType end = right.end();
            if( end != getMarker())
            return Symbol{};
        }

        constexpr bool operator==(const SentinalForm &other) const
        {
            if (!isValid() || !other.isValid())
                return false;
            return getRule() == other.getRule() && getMarker() == other.getMarker();
        }

        constexpr bool operator!=(const SentinalForm &other) const
        {
            return !(*this == other);
        }

        constexpr bool operator<(const SentinalForm &other) const
        {
            if (!isValid() || !other.isValid())
                return false;
            if (getRule() == other.getRule()){
                auto r1 = getRule();
                auto r2 = other.getRule();
                
                auto rr1 = r1.getRight();
                auto rr2 = r2.getRight();

                auto rrb1 = rr1.begin();
                auto rrb2 = rr2.begin();

                auto d1 = std::distance(rrb1,getMarker());
                auto d2 = std::distance(rrb2,other.getMarker());

                return d1 < d2;
            }
            return getRule() < other.getRule();
        }

        constexpr bool operator>(const SentinalForm &other) const
        {
            if (!isValid() || !other.isValid())
                return false;
            if (getRule() == other.getRule())
            {
                auto r1 = getRule();
                auto r2 = other.getRule();

                auto rr1 = r1.getRight();
                auto rr2 = r2.getRight();

                auto rrb1 = rr1.begin();
                auto rrb2 = rr2.begin();

                auto d1 = std::distance(rrb1, getMarker());
                auto d2 = std::distance(rrb2, other.getMarker());

                return d1 > d2;
            }
            return getRule() > other.getRule();
        }

        constexpr bool operator<=(const SentinalForm &other) const
        {
            return (*this == other) || (*this < other);
        }

        constexpr bool operator>=(const SentinalForm &other) const
        {
            return (*this == other) || (*this > other);
        }

    private:
        RuleType rule = DEFAULT_RULE;
        MarkerType marker = DEFAULT_RULE.getRight().begin();
        bool valid = false;
    };

} // namespace IStudio::Compiler
