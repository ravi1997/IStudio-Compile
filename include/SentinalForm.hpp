#pragma once

#include "Types_Compiler.hpp"
#include "Rule.hpp"

namespace IStudio::Compiler
{

    class SentinalForm
    {
    public:
        using RuleType = std::reference_wrapper<const Rule>;
        using MarkerType = std::ranges::filter_view<std::ranges::ref_view<const IStudio::Compiler::Rule::Right_Type>, decltype(Util::valid)>::Iterator;

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
            if (getRule() == other.getRule())
                return getMarker() < other.getMarker();
            return getRule() < other.getRule();
        }

        constexpr bool operator>(const SentinalForm &other) const
        {
            if (!isValid() || !other.isValid())
                return false;
            if (getRule() == other.getRule())
                return getMarker() > other.getMarker();
            return getRule() > other.getRule();
        }

        constexpr bool operator<=(const SentinalForm &other) const
        {
            if (!isValid() || !other.isValid())
                return false;
            if (getRule() == other.getRule())
                return getMarker() <= other.getMarker();
            return getRule() <= other.getRule();
        }

        constexpr bool operator>=(const SentinalForm &other) const
        {
            if (!isValid() || !other.isValid())
                return false;
            if (getRule() == other.getRule())
                return getMarker() >= other.getMarker();
            return getRule() >= other.getRule();
        }

    private:
        RuleType rule = DEFAULT_RULE;
        MarkerType marker = DEFAULT_RULE.getRight().begin();
        bool valid = false;
    };

} // namespace IStudio::Compiler
