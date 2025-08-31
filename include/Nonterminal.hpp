#pragma once

#include "Terminal.hpp"
#include "Lang.hpp"
#include "Symbol.hpp"
#include "Rule.hpp"

namespace IStudio::Compiler
{

    template <typename T>
    concept SYMBOL = std::is_base_of<Symbol, T>::value;

    template <SYMBOL... S>
     Rule::Right_Type rule(S... s) requires(sizeof...(S)>0)
    {
        return {s...};
    }

     Rule::Right_Type rule()
    {
        return {EPSILON};
    }

    class Nonterminal : public Symbol
    {
    public:
         Nonterminal(std::string_view n) : Symbol{n, 100, Associativity::RIGHT} {}

        using Symbol::Symbol;

         Nonterminal(const Symbol &s) : Symbol{s} {}

         friend Rule operator<=(Nonterminal n,Rule::Right_Type s) {
            return Rule{n,s};
        }
    };

} // namespace IStudio::Compiler
