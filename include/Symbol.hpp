#pragma once

#include "Types_Compiler.hpp"
#include "Lang.hpp"
#include "UUID.h"
#include <ostream>
#include <string_view>

namespace IStudio::Compiler
{

    namespace Details
    {
        template <std::size_t N>
        IStudio::Lang::String toStdArray(const char (&str)[N])
        {
            IStudio::Lang::String arr{};
            for (std::size_t i = 0; i < N; ++i)
            {
                arr[i] = str[i];
            }
            return arr;
        }
    }

    enum class SymbolType
    {
        TERMINAL,
        NONTERMINAL,
        NONE
    };

    enum class Associativity
    {
        LEFT,
        RIGHT,
        NONE
    };

    enum class TerminalType
    {
        KEYWORD,
        LITERAL,
        IDENTIFIER,
        OPERATOR,
        SEPARATOR,
        COMMENT,
        SPECIAL,
        NONE
    };

    enum class LiteralType
    {
        CHARACTER,
        STRING,
        INTEGER,
        FLOATING,
        USERDEFINED,
        NONE
    };

    enum class OperatorType
    {
        UNARYPOST,
        UNARYPRE,
        BINARY,
        USERDEFINED,
        NONE
    };

    enum class CommentType
    {
        LINE,
        BLOCK,
        NONE
    };

    class Symbol
    {
    public:
        using NAME_TYPE = std::string_view;
        using PATTERN_TYPE = std::string_view;
        using PRECEDENCE = IStudio::Lang::Integer;

    private:
        NAME_TYPE name{};
        PATTERN_TYPE pattern{};
        SymbolType type = SymbolType::NONE;
        PRECEDENCE precedence = 0;
        Associativity associativity = Associativity::NONE;

        TerminalType terminalType = TerminalType::NONE;
        LiteralType literalType = LiteralType::NONE;
        OperatorType operatorType = OperatorType::NONE;
        CommentType commentType = CommentType::NONE;

        Util::UUID guid;

    public:
        Symbol() = default;

        // Constructor for non-terminal symbols
        explicit Symbol(NAME_TYPE n, PRECEDENCE p, Associativity a)
            : name{n}, pattern{n}, type{SymbolType::NONTERMINAL}, precedence{p}, associativity{a} {}

        // Constructor for literal terminal symbols
        Symbol(NAME_TYPE n, NAME_TYPE p, PRECEDENCE pc, Associativity a, LiteralType l)
            : name{n}, pattern{p}, type{SymbolType::TERMINAL}, precedence{pc}, associativity{a}, terminalType{TerminalType::LITERAL}, literalType{l} {}

        // Constructor for operator terminal symbols
        Symbol(NAME_TYPE n, NAME_TYPE p, PRECEDENCE pc, Associativity a, OperatorType o)
            : name{n}, pattern{p}, type{SymbolType::TERMINAL}, precedence{pc}, associativity{a}, terminalType{TerminalType::OPERATOR}, operatorType{o} {}

        // Constructor for comment terminal symbols
        Symbol(NAME_TYPE n, NAME_TYPE p, PRECEDENCE pc, Associativity a, CommentType c)
            : name{n}, pattern{p}, type{SymbolType::TERMINAL}, precedence{pc}, associativity{a}, terminalType{TerminalType::COMMENT}, commentType{c} {}

        // Constructor for general terminal symbols
        Symbol(NAME_TYPE n, NAME_TYPE p, PRECEDENCE pc, Associativity a, TerminalType t)
            : name{n}, pattern{p}, type{SymbolType::TERMINAL}, precedence{pc}, associativity{a}, terminalType{t} {}

        // Getter methods
        [[nodiscard]] NAME_TYPE getName() const { return name; }
        [[nodiscard]] PATTERN_TYPE getPattern() const { return pattern; }
        [[nodiscard]] SymbolType getType() const { return type; }
        [[nodiscard]] PRECEDENCE getPrecedence() const { return precedence; }
        [[nodiscard]] Associativity getAssociativity() const { return associativity; }
        [[nodiscard]] TerminalType getTerminalType() const { return terminalType; }
        [[nodiscard]] LiteralType getLiteralType() const { return literalType; }
        [[nodiscard]] OperatorType getOperatorType() const { return operatorType; }
        [[nodiscard]] CommentType getCommentType() const { return commentType; }

        // Check methods
        [[nodiscard]] bool isNonterminal() const { return getType() == SymbolType::NONTERMINAL; }
        [[nodiscard]] bool isTerminal() const { return getType() == SymbolType::TERMINAL; }

        // Comparison operators
        bool operator==(const Symbol &s) const { return getName() == s.getName(); }
        bool operator!=(const Symbol &s) const { return getName() != s.getName(); }
        bool operator<(const Symbol &other) const { return getPrecedence() == other.getPrecedence() ? getName() < other.getName() : getPrecedence() < other.getPrecedence(); }
        bool operator<=(const Symbol &other) const { return getPrecedence() == other.getPrecedence() ? getName() <= other.getName() : getPrecedence() < other.getPrecedence(); }
        bool operator>(const Symbol &other) const { return getPrecedence() == other.getPrecedence() ? getName() > other.getName() : getPrecedence() > other.getPrecedence(); }
        bool operator>=(const Symbol &other) const { return getPrecedence() == other.getPrecedence() ? getName() >= other.getName() : getPrecedence() > other.getPrecedence(); }

        // Output operator for printing
        friend std::ostream &operator<<(std::ostream &o, const Symbol &s)
        {
            for (const auto &n : s.getName())
                o << n;
            return o;
        }
    };

} // namespace IStudio::Compiler
