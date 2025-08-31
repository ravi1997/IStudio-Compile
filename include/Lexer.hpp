#pragma once

#include "Types_Compiler.hpp"
#include "Terminal.hpp"
#include "Token.hpp"
#include "Exception.hpp"
#include "Lang.hpp"
#include "Logger.hpp"

namespace IStudio::Compiler
{

    class Lexer
    {
    public:
        using Terminals_Type = std::set<Terminal>;

    private:
        Terminals_Type terminals;
        Terminals_Type skipSymbols;
        mutable Logger logger;  // mutable to allow logging in const methods

    public:
        Lexer(Terminals_Type ts, Terminals_Type ss,
              Logger l = Logger("logfile.txt", LogLevel::DEBUG, 0, 2))
            : terminals{std::move(ts)}, skipSymbols{std::move(ss)}, logger{std::move(l)} {}

        auto getTerminals() const { return terminals; }
        auto getSkipSymbols() const { return skipSymbols; }

        friend std::ostream &operator<<(std::ostream &o, const Lexer &l)
        {
            auto printSet = [](const auto &s, std::ostream &o1) -> std::ostream &
            {
                o1 << "[ ";
                for (auto it = s.begin(); it != s.end(); ++it)
                {
                    if (it != s.begin())
                        o1 << ", ";
                    o1 << *it;
                }
                o1 << " ]";
                return o1;
            };

            o << "{ terminals: ";
            printSet(l.getTerminals(), o);
            o << ", skip-terminals: ";
            printSet(l.getSkipSymbols(), o);
            o << " }";

            return o;
        }

        std::vector<Token> tokenize(Lang::String input) const
        {
            auto match = [](const Terminal &terminal, const Lang::String &input) -> Lang::String
            {
                std::smatch sm;
                std::regex re("^(" + std::string(terminal.getPattern()) + ")");

                if (std::regex_search(input, sm, re))
                {
                    return sm[1].str();
                }
                return "";
            };

            std::vector<Token> result;
            Lang::Integer column = 1, line = 1;

            logger(LogLevel::INFO, 1) << "🔍 Starting tokenization...";

            while (!input.empty())
            {
                Lang::String max = "";
                Terminal max_terminal;

                // Match against defined terminals
                for (const auto &terminal : terminals)
                {
                    Lang::String temp = match(terminal, input);
                    if (temp.length() > max.length())
                    {
                        max = temp;
                        max_terminal = terminal;
                    }
                }

                // Try skipping if no match
                if (max.empty())
                {
                    for (const auto &terminal : skipSymbols)
                    {
                        Lang::String temp = match(terminal, input);
                        if (temp.length() > max.length())
                        {
                            max = temp;
                            max_terminal = terminal;
                        }
                    }

                    if (!max.empty())
                    {
                        logger(LogLevel::TRACE, 2) << "Skipping: '" << max << "'";
                        column += max.length();
                        input = input.substr(max.length());
                        continue;
                    }
                }

                // Still no match: unexpected input
                if (max.empty())
                {
                    Lang::String description = std::format("🛑 Unexpected input at {}:{} → {}", line, column, input.substr(0, 10));
                    logger(LogLevel::ERROR, 1) << description;
                    throw IStudio::Exception::UnexpectedInputException{description};
                }

                // Create token
                logger(LogLevel::DEBUG, 2) << "Token: [" << max_terminal.getName() << "] = '" << max << "'";
                result.push_back(Token{max_terminal, max, column, line});

                // Advance
                column += max.length();
                input = input.substr(max.length());
            }

            logger(LogLevel::INFO, 1) << "✅ Tokenization complete. Total tokens: " << result.size();

            // End of input marker
            result.push_back(Token{DOLLAR, "", column, line});
            return result;
        }

        friend auto operator|(Lang::String input, const Lexer &l)
        {
            return l.tokenize(input);
        }
    };

} // namespace IStudio::Compiler
