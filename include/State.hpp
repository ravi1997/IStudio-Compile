#pragma once

#include "SentinalForm.hpp"
#include "Terminal.hpp"

namespace IStudio::Compiler
{
    constexpr std::size_t lookahead_size = 50;
    using LOOKAHEAD_TYPE = std::array<Terminal,lookahead_size>;

    struct StateItem
    {
        SentinalForm form;
        LOOKAHEAD_TYPE lookaheads;
        bool valid = false;

        constexpr StateItem(SentinalForm f, LOOKAHEAD_TYPE l):
            form{f},
            lookaheads{l},
            valid{true}
            {}

        constexpr StateItem() = default;
        constexpr ~StateItem() = default;
        constexpr StateItem(const StateItem &) = default;
        constexpr StateItem &operator=(const StateItem &) = default;
        constexpr StateItem(StateItem &&) = default;
        constexpr StateItem &operator=(StateItem &&) = default;


        constexpr bool isValid() const{
            return valid;
        }

        constexpr const SentinalForm &getForm() const
        {
            return form;
        }

        constexpr void setForm(const SentinalForm &newForm)
        {
            form = newForm;
        }

        constexpr const LOOKAHEAD_TYPE &getLookaheads() const
        {
            return lookaheads;
        }

        constexpr void setLookaheads(const LOOKAHEAD_TYPE &newLookaheads)
        {
            lookaheads = newLookaheads;
        }

        constexpr bool operator==(const StateItem &other) const
        {
            return getForm() == other.getForm() && getLookaheads() == other.getLookaheads();
        }

        constexpr bool operator!=(const StateItem &other) const
        {
            return !(*this == other);
        }

        constexpr bool operator<(const StateItem &other) const
        {
            return getForm() < other.getForm() || (getForm() == other.getForm() && getLookaheads() < other.getLookaheads());
        }

        constexpr bool operator<=(const StateItem &other) const
        {
            return *this < other || *this == other;
        }

        constexpr bool operator>(const StateItem &other) const
        {
            return !(*this <= other);
        }

        constexpr bool operator>=(const StateItem &other) const
        {
            return !(*this < other);
        }
    };

    constexpr std::size_t state_size = 25;
    using STATE_TYPE = std::array<StateItem, state_size>;

} // namespace IStudio::Compiler
