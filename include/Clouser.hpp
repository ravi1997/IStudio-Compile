#pragma once

#include "State.hpp"


namespace IStudio::Compiler
{
    STATE_TYPE& CLOUSER(STATE_TYPE& I,[[maybe_unused]]Grammer& g){
        bool newItem = false;
        STATE_TYPE newItems;
        do{
            for (StateItem item : Util::iterate(I))
            {
                auto [form,lookaheads,valid] = item;
                
            }

        }while(newItem);


        return I;
    }
} // namespace IStudio::Compiler
