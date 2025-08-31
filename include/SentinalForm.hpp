#pragma once

#include "Types_Compiler.hpp"
#include "Rule.hpp"
#include "First.hpp"
#include "UUID.h"
#include <ostream>

namespace IStudio::Compiler
{

    class SentinalForm
    {
    public:
        using RuleType = std::reference_wrapper<const Rule>;
        using MarkerType = std::size_t;

        // Default and parameterized constructors
        SentinalForm() = default;

        SentinalForm(const RuleType &rule, MarkerType marker, MarkerType marker_BEGIN, MarkerType marker_END)
            : rule(rule), marker(marker), marker_BEGIN(marker_BEGIN), marker_END(marker_END)
        {
        }

        SentinalForm(const RuleType &rule)
            : rule(rule), marker(0), marker_BEGIN(0), marker_END(rule.get().getRight().size())
        {
        }

        // Rule-specific member functions
        const Rule &getRule() const
        {
            return rule.get();
        }

        void setRule(const RuleType &newRule)
        {
            rule = newRule;
        }

        // Marker-related member functions
        const MarkerType &getMarker() const
        {
            return marker;
        }

        const MarkerType &getMarker_BEGIN() const
        {
            return marker_BEGIN;
        }

        const MarkerType &getMarker_END() const
        {
            return marker_END;
        }

        SentinalForm getNext() const
        {
            return SentinalForm(rule, marker + 1, marker_BEGIN, marker_END);
        }

        // Symbol manipulation functions
        Symbol getSymbolAfterMarker() const
        {
            auto right = getRule().getRight();
            if (marker < right.size())
            {
                return right[marker];
            }
            return Symbol{};
        }

        // Lookahead calculation
        std::set<Terminal> getLookAheadForNextSymbol(const std::set<Terminal> &lookaheadSet, const Grammar &grammar) const
        {
            std::set<Terminal> result;

            bool epsilon = false;
            auto end = getMarker_END();
            auto marker = getMarker() + 1; // Move to the next marker position
            auto &rule = getRule();
            auto right = rule.getRight();

            while (marker < end)
            {
                if (epsilon)
                {
                    epsilon = false;
                    result.erase(EPSILON);
                }

                auto &symbol = right[marker];
                auto firstSet = FIRST(symbol, grammar);

                result.insert(firstSet.begin(), firstSet.end());

                if (firstSet.find(EPSILON) != firstSet.end())
                {
                    epsilon = true;
                }

                ++marker;
            }

            if (epsilon)
            {
                result.erase(EPSILON);
                result.insert(lookaheadSet.begin(), lookaheadSet.end());
            }

            if (result.empty())
            {
                result = lookaheadSet;
            }

            return result;
        }

        // Comparison operators
        bool operator==(const SentinalForm &other) const
        {
            return (getRule() == other.getRule() && getMarker() == other.getMarker());
        }

        bool operator!=(const SentinalForm &other) const
        {
            return !(*this == other);
        }

        bool operator<(const SentinalForm &other) const
        {
            if (getRule() == other.getRule())
            {
                return getMarker() - getMarker_BEGIN() < other.getMarker() - other.getMarker_BEGIN();
            }
            return getRule() < other.getRule();
        }

        bool operator>(const SentinalForm &other) const
        {
            if (getRule() == other.getRule())
            {
                return getMarker() - getMarker_BEGIN() > other.getMarker() - other.getMarker_BEGIN();
            }
            return getRule() > other.getRule();
        }

        bool operator<=(const SentinalForm &other) const
        {
            return (*this == other) || (*this < other);
        }

        bool operator>=(const SentinalForm &other) const
        {
            return (*this == other) || (*this > other);
        }

        // Output operator for printing
        friend std::ostream &operator<<(std::ostream &o, const SentinalForm &s)
        {
            o << s.getRule().getLeft() << " -> ";

            auto right = s.getRule().getRight();

            std::size_t i = 0;

            for (const auto &sym : right)
            {
                if (i == s.getMarker())
                {
                    o << " . ";
                }
                o << sym << " ";
                ++i;
            }

            if (i == s.getMarker())
            {
                o << " . ";
            }
            return o;
        }

    private:
        RuleType rule = DEFAULT_RULE;
        MarkerType marker = 0;
        MarkerType marker_BEGIN = 0;
        MarkerType marker_END = 0;
        Util::UUID guid;
    };

} // namespace IStudio::Compiler
