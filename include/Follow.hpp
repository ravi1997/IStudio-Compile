#pragma once
#include "Types_Compiler.hpp"
#include "Terminal.hpp"
#include "Symbol.hpp"
#include "Grammer.hpp"
#include "Util.hpp"
#include "First.hpp"

namespace IStudio::Compiler
{
    constexpr std::size_t follow_size = 50;
    using FOLLOW_TYPE = std::array<Terminal, follow_size>;

    constexpr FOLLOW_TYPE FOLLOW(Symbol s, Grammer g){
        FOLLOW_TYPE result;

        if(s == g.getStartSymbol())
            Util::insert_if_no_present(result,DOLLAR);

        auto rules = g.getRules();

        for(auto rule : Util::iterate(rules)){
            auto left = rule.getLeft();
            auto right = rule.getRight();

            auto indexs = Util::find_all(right,s);
            auto len = Util::length(right);

            for(auto index : Util::iterate(indexs,[](auto e){return e!=0;})){
                if(len == index){
                    if(left!=s){
                        auto temp_follow = FOLLOW(left,g);
                        Util::insert_if_no_present(result,temp_follow);
                    }
                }
                else{
                    bool flag = false;
                    for(;index<=len;index++){
                        auto temp_first = FIRST(right[index],g);
                        if(Util::find_first(temp_first,EPSILON)!=0){
                            Util::remove_first(temp_first,EPSILON);
                            flag = true;
                            Util::insert_if_no_present(result,temp_first);
                        }
                        else{
                            flag = false;
                            Util::insert_if_no_present(result,temp_first);
                            break;
                        }
                    }
                    if(flag){
                        if (left != s)
                        {
                            auto temp_follow = FOLLOW(left, g);
                            Util::insert_if_no_present(result, temp_follow);
                        }
                    }
                }
            }
        }

        return result;
    }
}