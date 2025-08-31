#pragma once

#include "SentinalForm.hpp"
#include "Terminal.hpp"
#include "UUID.h"

namespace IStudio::Compiler
{
    using LOOKAHEAD_TYPE = std::set<Terminal>;

    std::ostream &operator<<(std::ostream &o, const LOOKAHEAD_TYPE &lks)
    {
        bool first = true;
        for (const auto &lk : lks)
        {
            if (!first)
                o << " / ";
            o << lk;
            first = false;
        }
        return o;
    }

    struct StateItem
    {
        SentinalForm form;
        LOOKAHEAD_TYPE lookaheads;

        StateItem(const SentinalForm &f, LOOKAHEAD_TYPE l)
            : form{f}, lookaheads{l} {}

        StateItem() = default;
        ~StateItem() = default;
        StateItem(const StateItem &) = default;
        StateItem &operator=(const StateItem &) = default;
        StateItem(StateItem &&) = default;
        StateItem &operator=(StateItem &&) = default;

        bool checkMerge(const StateItem &other) const
        {
            return form == other.form;
        }

        StateItem operator+(const StateItem &other) const
        {
            StateItem result = *this;
            result.lookaheads.insert(other.lookaheads.begin(), other.lookaheads.end());
            return result;
        }

        const SentinalForm &getForm() const
        {
            return form;
        }

        void setForm(const SentinalForm &newForm)
        {
            form = newForm;
        }

        const LOOKAHEAD_TYPE &getLookaheads() const
        {
            return lookaheads;
        }

        void setLookaheads(const LOOKAHEAD_TYPE &newLookaheads)
        {
            lookaheads = newLookaheads;
        }

        bool operator==(const StateItem &other) const
        {
            return form == other.form && lookaheads == other.lookaheads;
        }

        bool operator!=(const StateItem &other) const
        {
            return !(*this == other);
        }

        bool operator<(const StateItem &other) const
        {
            return form < other.form || (form == other.form && lookaheads < other.lookaheads);
        }

        bool operator<=(const StateItem &other) const
        {
            return *this < other || *this == other;
        }

        bool operator>(const StateItem &other) const
        {
            return !(*this <= other);
        }

        bool operator>=(const StateItem &other) const
        {
            return !(*this < other);
        }

        friend std::ostream &operator<<(std::ostream &o, const StateItem &s)
        {
            o << s.form << " ; " << s.lookaheads;
            return o;
        }
    };

    using STATE_TYPE = std::set<StateItem>;

    STATE_TYPE reduce(const STATE_TYPE &state)
    {
        STATE_TYPE result;
        bool change = false;

        for (const auto &item : state)
        {
            bool inserted = false;
            for (const auto &item1 : state)
            {
                if (item != item1 && item.checkMerge(item1))
                {
                    result.insert(item + item1);
                    change = true;
                    inserted = true;
                }
            }
            if (!inserted)
                result.insert(item);
        }

        return (change) ? reduce(result) : result;
    }

    std::ostream &operator<<(std::ostream &o, const STATE_TYPE &state)
    {
        for (const auto &item : state)
            o << item << std::endl;
        return o;
    }

} // namespace IStudio::Compiler
