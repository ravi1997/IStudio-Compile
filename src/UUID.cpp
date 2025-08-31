#include "UUID.h"

namespace IStudio::Util::Details
{

    ID_type unique_id(ID_type seed)
    {
        ID_type id = seed;

        for (int i = 0; i < 12; ++i)
        {
            id = (id << 5) + id + i;
        }

        return id;
    }

    char base36_char(uint8_t digit) { return base36_digits[digit]; }
    BASE36_type to_base36(ID_type id)
    {
        BASE36_type result;

        for (int i = 7; i >= 0; --i)
        {
            result[i] = base36_char(id % 36);
            id /= 36;
        }

        return result;
    }
    ID_type generate_unique_id()
    {
        ID_type myseed = __COUNTER__;
        return unique_id(myseed);
    }
}