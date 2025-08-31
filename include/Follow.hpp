#pragma once
#include "Types_Compiler.hpp"
#include "Terminal.hpp"
#include "Symbol.hpp"
#include "Grammar.hpp"
#include "Util.hpp"
#include "First.hpp"

namespace IStudio::Compiler
{
    using FOLLOW_TYPE = std::set<Terminal>;

     FOLLOW_TYPE FOLLOW(const Symbol& s, const Grammar& g){
        FOLLOW_TYPE result;

        if(s == g.getStartSymbol())
            result.insert(DOLLAR);


        for (const auto &rule : g.getRules())
        {
            const auto& left = rule.getLeft();
            auto right = rule.getRight();
            bool found = false, EPSILON_flag = false;

            for (auto rhs : right){
                if(found){
                    auto temp_next_first = FIRST(rhs,g);
                    result.insert(temp_next_first.begin(),temp_next_first.end());
                    if(result.find(EPSILON)!=result.end()){
                        result.erase(EPSILON);
                        EPSILON_flag = true;
                    }
                    else{
                        EPSILON_flag = false;
                        found = false;
                    }
                }
                else if( rhs == s){
                    found = true;
                }
            }

            if ((found && left != s) || EPSILON_flag)
            {
                auto temp_follow = FOLLOW(left, g);
                result.insert(temp_follow.begin(),temp_follow.end());
            }

            
        }

        return result;
    }
}