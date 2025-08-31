#pragma once

#include "Types_Compiler.hpp"
#include "Lang.hpp"
#include "Terminal.hpp"
#include "UUID.h"

namespace IStudio::Compiler
{

    class Token
    {
    private:
        Util::UUID uuid;

        Terminal terminal;
        Lang::String code;

        Lang::Integer column;
        Lang::Integer line;


    public:
        auto &getId() const
        {
            return uuid;
        }

        auto &getColumn() const
        {
            return column;
        }

        auto &getLine() const
        {
            return line;
        }

        auto &getCode() const
        {
            return code;
        }

        auto &getTerminal() const
        {
            return terminal;
        }

        Token() = default;

        Token(const Terminal& t, Lang::String c, Lang::Integer col, Lang::Integer l) : 
                                                                                terminal{t},
                                                                                code{c},
                                                                                column{col},
                                                                                line{l}
        {
        }

        Token(const Token &t) : uuid{t.getId()},
                                terminal{t.getTerminal()},
                                code{t.getCode()},
                                column{t.getColumn()},
                                line{t.getLine()}
        {
        }

        Token &operator=(const Token &t)
        {
            uuid = t.getId();
            column = t.getColumn();
            line = t.getLine();
            code = t.getCode();
            terminal = t.getTerminal();

            return *this;
        }

        ~Token() = default;

        bool operator==(const Token &t) const
        {
            return (getId() <=> t.getId()) == 0;
        }

        bool operator!=(const Token &t) const
        {
            return !(*this == t);
        }

        friend std::ostream& operator<<(std::ostream& o, const Token& t){
            o << "{ column : " << t.getColumn() << " , line : " << t.getLine() << " , terminal : " << t.getTerminal() << " , code : " << t.getCode() << "}";

            return o;
        }

    };

} // namespace IStudio::Compiler
