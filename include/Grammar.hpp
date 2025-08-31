#pragma once

#include "Nonterminal.hpp"
#include "Terminal.hpp"
#include "Rule.hpp"
#include "UUID.h"
#include "Logger.hpp"

namespace IStudio::Compiler
{
    using namespace IStudio::Log;

    class Grammar {
    public:
        using Rules_Type = std::set<Rule>;
        using StartSymbol_Type = IStudio::Compiler::Nonterminal;
        using NonTerminals_Type = std::set<Nonterminal>;
        using Terminals_Type = std::set<Terminal>;

    private:
        StartSymbol_Type start;
        Terminals_Type terminals;
        Terminals_Type skipTerminals;
        NonTerminals_Type nonterminals;
        Rule firstRule;
        Rules_Type rules;
        Util::UUID guid;
        mutable Logger logger;

    public:
        Grammar(
            StartSymbol_Type s,
            Terminals_Type ts,
            Terminals_Type sts,
            NonTerminals_Type ns,
            Rule fr,
            Rules_Type rs,
            Logger l = Logger("logfile.txt", LogLevel::DEBUG, /*depth=*/0, /*defaultDepth=*/2)
        ) :
            start{s},
            terminals{ts},
            skipTerminals{sts},
            nonterminals{ns},
            firstRule{fr},
            rules{rs},
            logger{std::move(l)}
        {
            logger.setDefaultLogLevel({LogLevel::DEBUG});
            logger.setDefaultDepth(2);

            logger(LogLevel::DEBUG, 1) << "✅ Grammar constructed.";
            logger(LogLevel::DEBUG, 1) << "Start symbol: " << start;
            logger(LogLevel::DEBUG, 1) << "Total terminals: " << terminals.size();
            logger(LogLevel::DEBUG, 1) << "Skip terminals: " << skipTerminals.size();
            logger(LogLevel::DEBUG, 1) << "Nonterminals: " << nonterminals.size();
            logger(LogLevel::DEBUG, 1) << "First rule: " << fr;
            logger(LogLevel::DEBUG, 1) << "Total rules: " << rules.size();
        }

        ~Grammar() {
            logger(LogLevel::DEBUG, 1) << "🧹 Grammar destructed.";
        }

        Grammar(const Grammar& other)
            : start{other.start}, terminals{other.terminals}, skipTerminals{other.skipTerminals},
              nonterminals{other.nonterminals}, firstRule{other.firstRule},
              rules{other.rules}, guid{other.guid}, logger{other.logger}
        {
            logger(LogLevel::DEBUG, 1) << "📋 Grammar copied.";
        }

        Grammar& operator=(const Grammar& other) {
            if (this != &other) {
                start = other.start;
                terminals = other.terminals;
                skipTerminals = other.skipTerminals;
                nonterminals = other.nonterminals;
                firstRule = other.firstRule;
                rules = other.rules;
                guid = other.guid;
                logger = other.logger;
                logger(LogLevel::DEBUG, 1) << "📝 Grammar assigned.";
            }
            return *this;
        }

        const StartSymbol_Type& getStartSymbol() const {
            logger(LogLevel::TRACE, 2) << "🔍 getStartSymbol() called.";
            return start;
        }

        const Terminals_Type& getTerminals() const {
            logger(LogLevel::TRACE, 2) << "🔍 getTerminals() called.";
            return terminals;
        }

        const Terminals_Type& getSkipTerminals() const {
            logger(LogLevel::TRACE, 2) << "🔍 getSkipTerminals() called.";
            return skipTerminals;
        }

        const NonTerminals_Type& getNonterminals() const {
            logger(LogLevel::TRACE, 2) << "🔍 getNonterminals() called.";
            return nonterminals;
        }

        const Rule& getFirstRule() const {
            logger(LogLevel::TRACE, 2) << "🔍 getFirstRule() called.";
            return firstRule;
        }

        const Rules_Type& getRules() const {
            logger(LogLevel::TRACE, 2) << "🔍 getRules() called.";
            return rules;
        }

        auto getRulesForSymbol(const Symbol& s) const {
            logger(LogLevel::TRACE, 2) << "🔍 getRulesForSymbol(" << s << ") called.";
            return rules | std::views::filter([&](const auto& r) { return r.getLeft() == s; });
        }

        bool operator==(const Grammar& g) const noexcept {
            logger(LogLevel::TRACE, 2) << "🤝 operator== called.";
            return (guid <=> g.guid) == 0;
        }

        bool operator!=(const Grammar& g) const noexcept {
            logger(LogLevel::TRACE, 2) << "🚫 operator!= called.";
            return (guid <=> g.guid) != 0;
        }

        friend auto& operator<<(std::ostream& o, const Grammar& g) {
            const_cast<Logger&>(g.logger)(LogLevel::DEBUG, 1) << "🖨️ operator<< called for Grammar.";

            const auto& terminals = g.getTerminals();
            const auto& skipterminals = g.getSkipTerminals();
            const auto& nonterminals = g.getNonterminals();
            const auto& rules = g.getRules();

            o << "Grammar output:\n";
            o << "\tStart Symbol : " << g.getStartSymbol() << "\n";
            
            o << "\tList of Terminals:\n";
            for (const auto& f : terminals)
                o << "\t\t" << f << "\n";

            o << "\tList of Skip Terminals:\n";
            for (const auto& f : skipterminals)
                o << "\t\t" << f << "\n";

            o << "\tList of Nonterminals:\n";
            for (const auto& f : nonterminals)
                o << "\t\t" << f << "\n";

            o << "\tList of Rules:\n";
            for (const auto& r : rules)
                o << "\t\t" << r << "\n";

            return o;
        }
    };
}
