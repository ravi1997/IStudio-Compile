#ifndef _UUID_H_
#define _UUID_H_

#include "Types_Compiler.hpp"
#include <array>
#include <cstdint>
#include <stdexcept>

namespace IStudio::Util
{

    namespace Details
    {
        using ID_type = std::uint64_t;
        using BASE36_type = std::array<char, 8>;
        inline constexpr std::array<char, 37> base36_digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                                            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                                            'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                                                            'u', 'v', 'w', 'x', 'y', 'z'};

        ID_type unique_id(ID_type seed);

        char base36_char(std::uint8_t digit);
        BASE36_type to_base36(ID_type id);

        ID_type generate_unique_id();
    }

    class UUID
    {
        using ID_type = std::uint64_t;
        ID_type id;

    public:
        UUID() noexcept : id{Details::generate_unique_id()} {}
        UUID(const UUID &other) noexcept : id{other.id} {}
        UUID(UUID &&other) noexcept : id{other.id} { other.id = 0; }
        UUID &operator=(const UUID &other) noexcept
        {
            if (this != &other)
                id = other.id;
            return *this;
        }
        UUID &operator=(UUID &&other) noexcept
        {
            if (this != &other)
            {
                id = other.id;
                other.id = 0;
            }
            return *this;
        }
        ~UUID() noexcept = default;

        auto operator<=>(const UUID &b) const noexcept
        {
            return id <=> b.id;
        }

        auto getID() const noexcept { return Details::to_base36(id); }
    };

} // namespace IStudio::Util

#endif // _UUID_H_
