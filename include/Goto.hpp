#pragma once

#include "State.hpp"
#include "Grammar.hpp"
#include "Clouser.hpp"

namespace IStudio::Compiler
{
    STATE_TYPE GOTO(const STATE_TYPE& I,const Grammar& g,const Symbol& s){
        STATE_TYPE result;
        for (StateItem item:I){
            auto [form, lookaheads] = item;
            Symbol nextSymbol = form.getSymbolAfterMarker();
            if (nextSymbol == s)
            {
                auto newForm = form.getNext();
                // auto newLk = form.getLookAheadForNextSymbol(lookaheads, g);
                StateItem newItem = StateItem{newForm, lookaheads};
                result.insert(newItem);
            }
        }
        return CLOUSER(result,g);
    }
}