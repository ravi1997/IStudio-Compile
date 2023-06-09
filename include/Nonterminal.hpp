#pragma once

#include "Terminal.hpp"
#include "Symbol.hpp"
#include "Rule.hpp"

namespace IStudio::Compiler
{

    template <typename T>
    concept SYMBOL = std::is_base_of<Symbol, T>::value;

    template <SYMBOL... S>
    constexpr Rule::Right_Type rule(S... s) requires(sizeof...(S)>0)
    {
        return {s...};
    }

    constexpr Rule::Right_Type rule()
    {
        return {EPSILON};
    }

    class Nonterminal : public Symbol
    {
    public:
        template <std::size_t N>
        constexpr Nonterminal(const char (&n)[N]) : Symbol{n, 100, Associativity::RIGHT} {}

        using Symbol::Symbol;

        constexpr Nonterminal(const Symbol &s) : Symbol{s} {}

        constexpr friend Rule operator<=(Nonterminal n,Rule::Right_Type s) {
            return Rule{n,s};
        }
    };

} // namespace IStudio::Compiler
