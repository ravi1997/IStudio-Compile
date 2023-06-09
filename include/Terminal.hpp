#pragma once

#include "Symbol.hpp"

namespace IStudio::Compiler
{
    class Terminal:public Symbol
    {
        public:
        using Symbol::Symbol;

        constexpr Terminal(const Symbol& s):Symbol{s}{}

    };

    constexpr Terminal EPSILON{"EPSILON","",1,Associativity::LEFT,TerminalType::SPECIAL};
    constexpr Terminal DOLLAR{"DOLLAR", "", 0, Associativity::LEFT, TerminalType::SPECIAL};

} // namespace IStudio::Compiler

