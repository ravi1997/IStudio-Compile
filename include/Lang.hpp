#pragma once

#include "Types_Compiler.hpp"

namespace IStudio::Lang
{
    constexpr std::size_t string_size = 50;
    using String = std::array<char,string_size>;

    using Integer = int;

} // namespace IStudio::Lang
