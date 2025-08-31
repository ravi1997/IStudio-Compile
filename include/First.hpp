#pragma once
#include "Types_Compiler.hpp"
#include "Terminal.hpp"
#include "Symbol.hpp"
#include "Grammar.hpp"
#include "Util.hpp"

namespace IStudio::Compiler
{
    using FIRST_TYPE = std::set<Terminal>;
    using FIRST_STACK_TYPE = std::vector<Symbol>;

    FIRST_TYPE FIRST(const Symbol &s, const Grammar &g, FIRST_STACK_TYPE stack = {}, bool verbose = false);
    FIRST_TYPE FIRST(const Rule &s, const Grammar &g, FIRST_STACK_TYPE stack = {}, bool verbose = false);

    FIRST_TYPE FIRST(const Symbol &s, const Grammar &g, [[maybe_unused]] FIRST_STACK_TYPE stack, [[maybe_unused]] bool verbose)
    {
        FIRST_TYPE result;

        // Check if symbol has already been processed in the current path (avoid infinite recursion)
        if (std::find(stack.begin(), stack.end(), s) != stack.end())
        {
            return result;
        }

        // Add the symbol to the processing stack
        stack.push_back(s);

        if (s.isNonterminal())
        {
            for (const auto &rule : g.getRules())
            {
                if (rule.getLeft() == s)
                {
                    // Recursively compute FIRST set for the rule's right-hand side symbols
                    auto temp_first = FIRST(rule, g, stack, verbose);
                    result.insert(temp_first.begin(), temp_first.end());

                    // Check for EPSILON in the FIRST set of the rule
                    if (temp_first.find(EPSILON) == temp_first.end())
                    {
                        // No EPSILON found, stop processing further rules
                        break;
                    }
                }
            }
        }
        else
        {
            // Terminal symbol case: directly insert the terminal into the result set
            result.insert(Terminal(s));
        }

        // Remove the symbol from the processing stack
        stack.pop_back();

        return result;
    }

    FIRST_TYPE FIRST(const Rule &s, const Grammar &g, [[maybe_unused]] FIRST_STACK_TYPE stack, [[maybe_unused]] bool verbose)
    {
        FIRST_TYPE result;

        bool has_epsilon = true;

        for (const auto &rhs : s.getRight())
        {
            // Check if RHS symbol has already been processed in the current path
            if (std::find(stack.begin(), stack.end(), rhs) != stack.end())
            {
                has_epsilon = false;
                break;
            }

            // Add the LHS symbol to the processing stack
            stack.push_back(s.getLeft());

            // Calculate FIRST set for the RHS symbol
            auto temp_first = FIRST(rhs, g, stack);

            // Remove LHS symbol from the processing stack
            stack.pop_back();

            result.insert(temp_first.begin(), temp_first.end());

            // Check if EPSILON is in the FIRST set of the RHS symbol
            if (temp_first.find(EPSILON) == temp_first.end())
            {
                has_epsilon = false;
                break;
            }
        }

        // If EPSILON is in the FIRST set of all RHS symbols, add EPSILON to the result set
        if (has_epsilon)
        {
            result.insert(EPSILON);
        }

        return result;
    }

} // namespace IStudio::Compiler
