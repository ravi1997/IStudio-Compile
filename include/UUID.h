#pragma once

#include "Types_Compiler.hpp"

namespace IStudio::Util
{

    namespace Details{
        using ID_type = uint64_t;
        using BASE36_type = std::array<char, 8>;
        constexpr std::array<char, 37> base36_digits = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b',
            'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
            'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

        constexpr ID_type unique_id(ID_type seed)
        {
            ID_type id = seed;

            for (int i = 0; i < 12; ++i)
            {
                id = (id << 5) + id + i;
            }

            return id;
        }

        constexpr char base36_char(uint8_t digit) { return base36_digits[digit]; }

        constexpr BASE36_type to_base36(ID_type id)
        {
            BASE36_type result;

            for (int i = 7; i >= 0; --i)
            {
                result[i] = base36_char(id % 36);
                id /= 36;
            }

            return result;
        }

        constexpr ID_type generate_unique_id()
        {
            ID_type myseed = __COUNTER__;
            return unique_id(myseed);
        }
    }


    class UUID
    {
        using ID_type = uint64_t;

    public:
        constexpr UUID() noexcept : id_{Details::generate_unique_id()} {}
        constexpr UUID(const UUID &other) noexcept : id_{other.id_} {}
        constexpr UUID(UUID &&other) noexcept : id_{other.id_} { other.id_ = -1; }
        constexpr UUID &operator=(const UUID &other) noexcept
        {
            id_ = other.id_;
            return *this;
        }
        constexpr UUID &operator=(UUID &&other) noexcept
        {
            id_ = other.id_;
            other.id_ = -1;
            return *this;
        }
        ~UUID() noexcept = default;
        constexpr auto operator<=>(const UUID &b) const noexcept
        {
            return id_ <=> b.id_;
        }

        constexpr auto getID() const noexcept { return Details::to_base36(id_); }

    private:
        ID_type id_;
    };
}
