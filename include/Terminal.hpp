#pragma once

#include "Symbol.hpp"

namespace IStudio::Compiler
{
    class Terminal : public Symbol
    {
    public:
        using Symbol::Symbol;

        explicit Terminal(const Symbol &s) : Symbol{s} {}
    };

    inline const Terminal EPSILON{"EPSILON", "", 1, Associativity::LEFT, TerminalType::SPECIAL};
    inline const Terminal DOLLAR{"DOLLAR", "", 0, Associativity::LEFT, TerminalType::SPECIAL};

} // namespace IStudio::Compiler
