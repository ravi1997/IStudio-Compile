#pragma once

#include "Types_Compiler.hpp"
#include "Lang.hpp"

namespace IStudio::Compiler
{

    namespace Details
    {
        template <std::size_t N>
        constexpr IStudio::Lang::String toStdArray(const char (&str)[N])
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
        SEPERATOR,
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
        using NAME_TYPE = IStudio::Lang::String;
        using PATTERN_TYPE = IStudio::Lang::String;
        using TYPE = SymbolType;
        using PRECEDENCE = IStudio::Lang::Integer;
        using ASSOCIATIVITY = Associativity;
        using TERMINALTYPE = TerminalType;
        using LITERALTYPE = LiteralType;
        using OPERATORTYPE = OperatorType;
        using COMMENTTYPE = CommentType;

    private:
        NAME_TYPE name = IStudio::Compiler::Details::toStdArray("");
        PATTERN_TYPE pattern = IStudio::Compiler::Details::toStdArray("");
        TYPE type = TYPE::NONE;
        PRECEDENCE precedence = 0;
        ASSOCIATIVITY associativity = ASSOCIATIVITY::NONE;
        bool valid = false;

        TERMINALTYPE terminalType = TERMINALTYPE::NONE;
        LITERALTYPE literalType = LITERALTYPE::NONE;
        OPERATORTYPE operatorType = OPERATORTYPE::NONE;
        COMMENTTYPE commentType = COMMENTTYPE::NONE;

    public:
        constexpr Symbol() = default;
        constexpr Symbol(const Symbol&) = default;
        constexpr Symbol& operator=(const Symbol&) = default;
        constexpr ~Symbol() = default;

        constexpr NAME_TYPE getName() const { return name; }
        constexpr PATTERN_TYPE getPattern() const { return pattern; }
        constexpr TYPE getType() const { return type; }
        constexpr PRECEDENCE getPrecedence() const { return precedence; }
        constexpr ASSOCIATIVITY getAssociativity() const { return associativity; }
        constexpr bool isValid() const { return valid; }
        constexpr TERMINALTYPE getTerminalType() const { return terminalType; }
        constexpr LITERALTYPE getLiteralType() const { return literalType; }
        constexpr OPERATORTYPE getOperatorType() const { return operatorType; }
        constexpr COMMENTTYPE getCommentType() const { return commentType; }

        template <std::size_t N>
        constexpr Symbol(const char (&n)[N], PRECEDENCE p, ASSOCIATIVITY a) : name{Details::toStdArray(n)},
                                                                              pattern{Details::toStdArray(n)},
                                                                              type{TYPE::NONTERMINAL},
                                                                              precedence{p},
                                                                              associativity{a},
                                                                              valid{true}
        {
        }

        template <std::size_t N, std::size_t M>
        constexpr Symbol(const char (&n)[N], const char (&p)[M], PRECEDENCE pc, ASSOCIATIVITY a, LITERALTYPE l) : name{Details::toStdArray(n)},
                                                                                                                  pattern{Details::toStdArray(p)},
                                                                                                                  type{TYPE::TERMINAL},
                                                                                                                  precedence{pc},
                                                                                                                  associativity{a},
                                                                                                                  valid{true},
                                                                                                                  terminalType{TERMINALTYPE::LITERAL},
                                                                                                                  literalType{l}
        {
        }

        template <std::size_t N, std::size_t M>
        constexpr Symbol(const char (&n)[N], const char (&p)[M], PRECEDENCE pc, ASSOCIATIVITY a, OPERATORTYPE o) : name{Details::toStdArray(n)},
                                                                                                                   pattern{Details::toStdArray(p)},
                                                                                                                   type{TYPE::TERMINAL},
                                                                                                                   precedence{pc},
                                                                                                                   associativity{a},
                                                                                                                   valid{true},
                                                                                                                   terminalType{TERMINALTYPE::OPERATOR},
                                                                                                                   operatorType{o}
        {
        }

        template <std::size_t N, std::size_t M>
        constexpr Symbol(const char (&n)[N], const char (&p)[M], PRECEDENCE pc, ASSOCIATIVITY a, CommentType l) : name{Details::toStdArray(n)},
                                                                                                                  pattern{Details::toStdArray(p)},
                                                                                                                  type{TYPE::TERMINAL},
                                                                                                                  precedence{pc},
                                                                                                                  associativity{a},
                                                                                                                  valid{true},
                                                                                                                  terminalType{TERMINALTYPE::COMMENT},
                                                                                                                  commentType{l}
        {
        }
        template <std::size_t N, std::size_t M>
        constexpr Symbol(const char (&n)[N], const char (&p)[M], PRECEDENCE pc, ASSOCIATIVITY a, TERMINALTYPE t) : name{Details::toStdArray(n)},
                                                                                                                   pattern{Details::toStdArray(p)},
                                                                                                                   type{TYPE::TERMINAL},
                                                                                                                   precedence{pc},
                                                                                                                   associativity{a},
                                                                                                                   valid{true},
                                                                                                                   terminalType{t}
        {
        }

        constexpr bool isNonterminal() const
        {
            return getType() == TYPE::NONTERMINAL;
        }

        constexpr bool isTerminal() const
        {
            return getType() == TYPE::TERMINAL;
        }


        constexpr bool operator==(Symbol s)const{
            return getName() == s.getName();
        }

        constexpr bool operator!=(Symbol s)const{
            return getName() != s.getName();
        }

        constexpr bool operator<(const Symbol &other) const
        {
            return getName() < other.getName();
        }

        constexpr bool operator<=(const Symbol &other) const
        {
            return getName() <= other.getName();
        }

        constexpr bool operator>(const Symbol &other) const
        {
            return getName() > other.getName();
        }

        constexpr bool operator>=(const Symbol &other) const
        {
            return getName() >= other.getName();
        }

        friend std::ostream& operator<<(std::ostream& o,Symbol s){
            for(auto n : s.getName() | std::views::filter([](auto c){return c!='\0';}))
                o << n;
            return o;
        }

    };

}