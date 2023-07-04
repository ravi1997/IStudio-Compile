#pragma once
#include "Types_Compiler.hpp"
#include "Terminal.hpp"
#include "Symbol.hpp"
#include "Grammer.hpp"
#include "Util.hpp"

namespace IStudio::Compiler
{
    constexpr std::size_t first_size = 50;
    using FIRST_TYPE = std::array<Terminal,first_size>;

    using FIRST_STACK_TYPE = std::array<Symbol,Grammer::rule_size>;



    constexpr FIRST_TYPE FIRST(Symbol s,Grammer g,FIRST_STACK_TYPE stack = {});
    constexpr FIRST_TYPE FIRST(Rule s,Grammer g,FIRST_STACK_TYPE stack = {});

    constexpr FIRST_TYPE FIRST(Symbol s, Grammer g, [[maybe_unused]] FIRST_STACK_TYPE stack)
    {
        // static_assert(s.isValid(),"Symbol must be valid.");     
        FIRST_TYPE result;
        std::size_t inserted = 0;
        if (Util::find_first(stack, s) != 0)
        {
            return result;
        }
        if(s.isNonterminal()){
            for(auto& rule : g.getRules()){
                if(rule.getLeft() == s){

                    Util::insert(stack,s);
                    auto temp_first = FIRST(rule,g,stack);
                    Util::insert_if_no_present(result,temp_first);
                    Util::remove_first(stack, s);
                }
            }
        }
        else{
            Terminal me = s; 
            result[inserted++] = me;
        }

        return result;
    }

    constexpr FIRST_TYPE FIRST(Rule s,[[maybe_unused]]Grammer g,[[maybe_unused]]FIRST_STACK_TYPE stack){
        FIRST_TYPE result;
        auto right = s.getRight();
        auto left = s.getLeft();
        bool flag = false;
        for (auto rhs : right ){
            if(Util::find_first(stack,rhs)!=0){
                break;
            }
            Util::insert(stack,left);
            auto temp_first = FIRST(rhs,g,stack);
            if(Util::find_first(temp_first,EPSILON)!=0){
                Util::remove_first(temp_first,EPSILON);
                Util::insert_if_no_present(result,temp_first);
                flag = true;
            }
            else{
                Util::insert_if_no_present(result, temp_first);
                flag = false;
                break;
            }
            Util::remove_first(stack,left);
        }

        if(flag)
            Util::insert_if_no_present(result,EPSILON);

        return result;
    }

} // namespace IStudio::Compiler
