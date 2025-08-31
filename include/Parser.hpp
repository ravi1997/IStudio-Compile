#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "Types_Compiler.hpp"
#include "State.hpp"
#include "Lang.hpp"
#include "Clouser.hpp"
#include "Goto.hpp"
#include "Grammar.hpp"
#include "Lexer.hpp"
#include "token.hpp"
#include "ast.hpp"
#include "Logger.hpp"

namespace IStudio::Compiler
{
    class Parser
    {
    private:
        const Grammar grammer;
        std::set<STATE_TYPE> states;
        IStudio::Log::Logger logger;

        enum class GOTO_COMMAND
        {
            SHIFT,
            REDUCE,
            ACCEPT
        };

        using GOTO_TABLE_VALUE = std::vector<std::pair<GOTO_COMMAND, std::variant<STATE_TYPE, Rule>>>;
        std::map<STATE_TYPE, std::map<Terminal, GOTO_TABLE_VALUE>> gotoTable;
        std::map<STATE_TYPE, std::map<Nonterminal, STATE_TYPE>> actionTable;

        void handleTerminal(const STATE_TYPE &state, const Terminal &terminal)
        {
            auto new_state = reduce(GOTO(state, grammer, terminal));
            if (!new_state.empty())
            {
                logger(IStudio::Log::LogLevel::DEBUG, 2) << "New state created with terminal: " << terminal;
                states.insert(new_state);
                gotoTable[state][terminal].push_back({GOTO_COMMAND::SHIFT, new_state});
                for (const auto &item : new_state)
                {
                    const auto &form = item.getForm();
                    const auto &lks = item.getLookaheads();
                    if (form.getMarker() == form.getMarker_END())
                    {
                        for (const auto &lk : lks)
                        {
                            if (form.getRule() == grammer.getFirstRule() && lk == DOLLAR)
                            {
                                gotoTable[new_state][lk].push_back({GOTO_COMMAND::ACCEPT, form.getRule()});
                            }
                            else
                            {
                                gotoTable[new_state][lk].push_back({GOTO_COMMAND::REDUCE, form.getRule()});
                            }
                        }
                    }
                }
            }
        }

        void handleNonterminal(const STATE_TYPE &state, const Nonterminal &nonterminal)
        {
            auto new_state = reduce(GOTO(state, grammer, nonterminal));
            if (!new_state.empty())
            {
                logger(IStudio::Log::LogLevel::DEBUG, 2) << "New state created with nonterminal: " << nonterminal;
                states.insert(new_state);
                actionTable[state][nonterminal] = new_state;
                for (const auto &item : new_state)
                {
                    const auto &form = item.getForm();
                    const auto &lks = item.getLookaheads();
                    if (form.getMarker() == form.getMarker_END())
                    {
                        for (const auto &lk : lks)
                        {
                            if (form.getRule() == grammer.getFirstRule() && lk == DOLLAR)
                            {
                                gotoTable[new_state][lk].push_back({GOTO_COMMAND::ACCEPT, form.getRule()});
                            }
                            else
                            {
                                gotoTable[new_state][lk].push_back({GOTO_COMMAND::REDUCE, form.getRule()});
                            }
                        }
                    }
                }
            }
        }

        std::optional<STATE_TYPE> getNextState(const STATE_TYPE &currentState, const Terminal &terminal)
        {
            auto it = gotoTable.find(currentState);
            if (it != gotoTable.end())
            {
                auto symbolIt = it->second.find(terminal);
                if (symbolIt != it->second.end())
                {
                    for (const auto &[command, value] : symbolIt->second)
                    {
                        if (command == GOTO_COMMAND::SHIFT)
                        {
                            return std::get<STATE_TYPE>(value);
                        }
                    }
                }
            }
            return std::nullopt;
        }

    public:
        explicit Parser(const Grammar &grammer, IStudio::Log::Logger logger = IStudio::Log::Logger("parser.log", IStudio::Log::LogLevel::DEBUG))
            : grammer(grammer), logger(std::move(logger))
        {
            logger(IStudio::Log::LogLevel::INFO, 1) << "Initializing Parser...";

            auto &start = grammer.getStartSymbol();
            auto terminals = grammer.getTerminals();
            terminals.insert(DOLLAR);

            const auto &nonterminals = grammer.getNonterminals();
            auto I0 = reduce(CLOUSER(start, {DOLLAR}, grammer));

            std::size_t old_size = 0, new_size = 0;

            states.insert(I0);
            do
            {
                old_size = states.size();
                auto test_states = states;
                for (const auto &state : test_states)
                {
                    for (const auto &terminal : terminals)
                    {
                        handleTerminal(state, terminal);
                    }
                    for (const auto &nonterminal : nonterminals)
                    {
                        handleNonterminal(state, nonterminal);
                    }
                }
                new_size = states.size();
            } while (old_size != new_size);

            logger(IStudio::Log::LogLevel::INFO, 1) << "Parser initialized with " << states.size() << " states.";
        }

        std::shared_ptr<ASTNode> parse(auto tokens) const
        {
            std::stack<STATE_TYPE> stateStack;
            std::stack<Symbol> symbolStack;
            std::stack<std::shared_ptr<ASTNode>> astStack;

            stateStack.push(*states.begin());

            for (const Token &currentToken : tokens)
            {
            re_iterate:
                const Terminal &terminal = currentToken.getTerminal();

                logger(IStudio::Log::LogLevel::DEBUG, 2) << "Parsing token: " << terminal;

                auto currentState = stateStack.top();
                auto actionIt = gotoTable.find(currentState);

                if (actionIt != gotoTable.end())
                {
                    auto &terminalActions = actionIt->second;
                    auto terminalIt = terminalActions.find(terminal);

                    if (terminalIt != terminalActions.end())
                    {
                        const auto &actions = terminalIt->second;
                        for (const auto &[command, value] : actions)
                        {
                            if (command == GOTO_COMMAND::SHIFT)
                            {
                                logger(IStudio::Log::LogLevel::DEBUG, 2) << "SHIFT";
                                stateStack.push(std::get<STATE_TYPE>(value));
                                symbolStack.push(terminal);
                                astStack.push(std::make_shared<ASTNode>(terminal));
                                break;
                            }
                            else if (command == GOTO_COMMAND::REDUCE)
                            {
                                const Rule &rule = std::get<Rule>(value);
                                logger(IStudio::Log::LogLevel::DEBUG, 2) << "REDUCE: " << rule;
 
                                auto newNode = std::make_shared<ASTNode>(rule.getLeft());
                                std::vector<std::shared_ptr<ASTNode>> children;

                                if (astStack.size() < rule.getRight().size())
                                {
                                    logger(IStudio::Log::LogLevel::ERROR, 1) << "Not enough AST nodes to reduce.";
                                    throw IStudio::Exception::ParserException{"Not enough AST nodes to reduce."};
                                }

                                for (std::size_t i = 0; i < rule.getRight().size(); ++i)
                                {
                                    stateStack.pop();
                                    symbolStack.pop();


                                    auto child = astStack.top();
                                    astStack.pop();
                                    children.push_back(child);
                                }

                                std::reverse(children.begin(), children.end());
                                for (const auto &child : children)
                                    newNode->addChild(child);
                                astStack.push(newNode);

                                auto state_temp = stateStack.top();
                                auto nextState = actionTable.at(state_temp).find(rule.getLeft());
                                if (nextState != actionTable.at(state_temp).end())
                                {
                                    stateStack.push(nextState->second);
                                    symbolStack.push(rule.getLeft());
                                }
                                else
                                {
                                    logger(IStudio::Log::LogLevel::ERROR, 1) << "No valid state transition after reduce.";
                                    throw IStudio::Exception::ParserException{"No valid state transition after reduce."};
                                }
                                goto re_iterate;
                            }
                            else if (command == GOTO_COMMAND::ACCEPT)
                            {
                                logger(IStudio::Log::LogLevel::INFO, 1) << "ACCEPT";
                                return astStack.top(); // root of AST;
                            }
                        }
                    }
                    else
                    {
                        logger(IStudio::Log::LogLevel::ERROR, 1) << "No valid action for terminal: " << terminal;
                        throw IStudio::Exception::ParserException{
                            "No valid action for terminal: " + std::string{terminal.getName()}};
                    }
                }
                else
                {
                    logger(IStudio::Log::LogLevel::ERROR, 1) << "No valid state transition.";
                    throw IStudio::Exception::ParserException{"No valid state transition."};
                }
            }

            logger(IStudio::Log::LogLevel::ERROR, 1) << "Input not fully parsed.";
            throw IStudio::Exception::ParserException{"Input not fully parsed."};
        }

        friend std::shared_ptr<ASTNode> operator|(auto tokens, const Parser &p)
        {
            return p.parse(tokens);
        }
        void summary(std::ostream &out) const noexcept
        {
            out << "Parser Summary:\n";
            out << "States: " << states.size() << "\n";

            auto terminals = grammer.getTerminals();
            terminals.insert(DOLLAR);

            const auto &rules = grammer.getRules();
            const auto &nonterminals = grammer.getNonterminals();

            out << std::left << std::setw(10) << "State" << " | ";
            for (const auto &terminal : terminals)
                out << std::left << std::setw(10) << terminal << " | ";
            for (const auto &nonterminal : nonterminals)
                out << std::left << std::setw(10) << nonterminal << " | ";
            out << "\n";

            out << std::string(10 + (terminals.size() + nonterminals.size()) * 13, '-') << "\n";

            int state_index = 0;
            for (const auto &state : states)
            {
                out << std::left << std::setw(10) << ("I" + std::to_string(state_index)) << " | ";

                for (const auto &terminal : terminals)
                {
                    try {
                        const auto &actions = gotoTable.at(state).at(terminal);
                        std::string value_str;
                        for (const auto &[command, value] : actions)
                        {
                            if (command == GOTO_COMMAND::SHIFT)
                                value_str += "S ";
                            else if (command == GOTO_COMMAND::REDUCE)
                                value_str += "R ";
                            else if (command == GOTO_COMMAND::ACCEPT)
                                value_str += "A ";
                            break;
                        }
                        out << std::left << std::setw(10) << value_str << " | ";
                    } catch (...) {
                        out << std::left << std::setw(10) << " " << " | ";
                    }
                }

                for (const auto &nonterminal : nonterminals)
                {
                    try {
                        const auto result_state = actionTable.at(state).at(nonterminal);
                        const auto index = std::distance(states.begin(), std::find(states.begin(), states.end(), result_state));
                        out << std::left << std::setw(10) << index << " | ";
                    } catch (...) {
                        out << std::left << std::setw(10) << " " << " | ";
                    }
                }

                out << "\n";
                state_index++;
            }
        }

    };
} // namespace IStudio::Compiler

#endif // PARSER_HPP_
