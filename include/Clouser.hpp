#pragma once

#include "State.hpp"
#include "Grammar.hpp"

namespace IStudio::Compiler
{
    STATE_TYPE CLOUSER(const STATE_TYPE &I, [[maybe_unused]] const Grammar &g, bool verbose = false);
    STATE_TYPE CLOUSER(const Symbol &I, const LOOKAHEAD_TYPE &lk, [[maybe_unused]] const Grammar &g, bool verbose = false);

    STATE_TYPE CLOUSER(const STATE_TYPE &I, [[maybe_unused]] const Grammar &g, [[maybe_unused]] bool verbose)
    {
        if (verbose)
        {
            std::cout << "checking point 1 (Clouser Function I)" << std::endl;
            std::cout << "function called for state : " << std::endl
                      << I;
        }

        STATE_TYPE result = I;
        auto oldLength = result.size();
        auto newLength = oldLength;

        do
        {
            oldLength = result.size();

            if (verbose)
            {
                std::cout << "checking point 2" << std::endl;
                std::cout << "oldLength variable : " << oldLength << std::endl;
                std::cout << "newLength variable : " << newLength << std::endl;
                std::cout << "checking point 3" << std::endl;
            }

            STATE_TYPE newStateItems;
            for (StateItem item : result)
            {

                if (verbose)
                {
                    std::cout << "Active item : " << item << std::endl;
                }
                auto [form, lookaheads] = item;
                Symbol nextSymbol = form.getSymbolAfterMarker();
                LOOKAHEAD_TYPE lks = form.getLookAheadForNextSymbol(lookaheads, g);
                if (verbose)
                {
                    std::cout << "nextSymbol variable : " << nextSymbol << std::endl;
                    std::cout << "lks variable : " << lks << std::endl;
                }

                STATE_TYPE temp = CLOUSER(nextSymbol, lks, g, verbose);

                if (verbose)
                {
                    std::cout << "temp variable : " << std::endl
                              << temp;

                    std::cout << "newStateItems variable size before insert : " << newStateItems.size() << std::endl;
                }
                newStateItems.insert(temp.begin(), temp.end());

                if (verbose)
                {

                    std::cout << "newStateItems variable size after insert : " << newStateItems.size() << std::endl;
                }
            }
            result.insert(newStateItems.begin(), newStateItems.end());
            newLength = result.size();

        } while (newLength != oldLength);

        return result;
    }

    STATE_TYPE CLOUSER(const Symbol &I, const LOOKAHEAD_TYPE &lk, [[maybe_unused]] const Grammar &g, [[maybe_unused]] bool verbose)
    {

        if (verbose)
        {
            std::cout << "checking point 1 (Clouser Function II)" << std::endl;
            std::cout << "function called for symbol : " << I << std::endl;
            std::cout << "function called for lookaheads : " << lk << std::endl;
        }

        STATE_TYPE result;

        const auto &rules = g.getRules();

        if (verbose)
        {
            std::cout << "checking point 2" << std::endl;
        }
        for (const auto &rule : rules)
        {

            if (verbose)
            {
                std::cout << "active rule : " << rule << std::endl;
            }

            auto left = rule.getLeft();

            if (left == I)
            {
                SentinalForm myform{rule};
                StateItem item{myform, lk};
                if (verbose)
                {
                    std::cout << "item variable : " << item << std::endl;
                    std::cout << "result variable size before insert : " << result.size() << std::endl;
                }

                result.insert(item);
            }else if (verbose)
            {
                std::cout << "left is not equal right" << std:: endl;
                std::cout << "left variable : " << left << std::endl;
                std::cout << "I variable : " << I << std::endl;
            }

            if (verbose)
            {
                std::cout << "result size after insert : " << result.size() << std::endl;
            }
        }

        if (verbose)
        {
            std::cout << "result size after iteration : " << result.size() << std::endl;
        }

        return CLOUSER(result, g, verbose);
    }

} // namespace IStudio::Compiler
