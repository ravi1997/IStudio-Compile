#pragma once

#include "Nonterminal.hpp"
#include "Terminal.hpp"
#include "Rule.hpp"

namespace IStudio::Compiler
{
    class Grammer{
        public:
            constexpr static std::size_t rule_size = 250;
            using Rules_Type = std::array<Rule,rule_size>;
            using StartSymbol_Type = IStudio::Compiler::Nonterminal;
            constexpr static std::size_t nonterminal_size = 250;
            using NonTerminals_Type = std::array<Nonterminal, nonterminal_size>;

            constexpr static std::size_t terminal_size = 250;
            using Terminals_Type = std::array<Terminal, terminal_size>;
        private:
            StartSymbol_Type start;
            Terminals_Type terminals;
            NonTerminals_Type nonterminals;
            Rules_Type rules;
        public:
            constexpr Grammer(
                StartSymbol_Type s,
                Terminals_Type ts,
                NonTerminals_Type ns,
                Rules_Type rs):
                start{s},
                terminals{ts},
                nonterminals{ns},
                rules{rs}
                {}

            constexpr ~Grammer() = default;
            constexpr Grammer(const Grammer &) = default;
            constexpr Grammer& operator=(const Grammer &) = default;

            constexpr StartSymbol_Type getStartSymbol() const{
                return start;
            }

            constexpr Terminals_Type getTerminals() const{
                return terminals;
            }

            constexpr NonTerminals_Type getNonterminals() const{
                return nonterminals;
            }

            constexpr Rules_Type getRules() const{
                return rules;
            }

            friend std::ostream& operator<<(std::ostream& o,const Grammer& g){
                auto terminals = g.getTerminals();
                auto nonterminals = g.getNonterminals();
                auto rules = g.getRules();

                o << "Grammer is outputing" << std::endl;
                o << "\tStart Symbol : " << g.getStartSymbol() << std::endl;
                
                o << "\tList of Terminals :" << std::endl;
                for (auto f : Util::iterate(terminals))
                    std::cout << "\t\t" << f << std::endl;


                o << "\tList of Nonterminals :" << std::endl;
                for (auto f : Util::iterate(nonterminals))
                    std::cout << "\t\t" << f << std::endl;

                o << "\tList of rules :" << std::endl;
                for (auto r : Util::iterate(rules))
                    std::cout << "\t\t" << r << std::endl;

                return o;
            }

    };
} // namespace IStudio::Compiler
