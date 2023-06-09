#pragma once

#include "Types_Compiler.hpp"

namespace IStudio::Util
{
    template <typename T>
    concept NotStdArray = !std::is_same_v<T, std::array<typename T::value_type, T::size()>>;

    constexpr auto valid = [](auto e)->bool{return e.isValid();};

    template <typename t, std::size_t N, typename filter_type = decltype(valid)>
    constexpr auto iterate(std::array<t, N> &arr, filter_type fil = valid)
    {
        return arr | std::views::filter(fil);
    }

    template <typename t, std::size_t N, typename filter_type = decltype(valid)>
    constexpr std::size_t length(std::array<t, N> &arr, filter_type fil = valid)
    {
        std::size_t len = 0;
        for ([[maybe_unused]] auto e : iterate(arr,fil))
            len++;
        return len;
    }

    template <typename t, std::size_t N, typename T, typename filter_type = decltype(valid)>
    constexpr std::array<t, N> &insert(std::array<t, N> &arr, T& e, filter_type fil = valid)
    {
        arr[length(arr,fil)] = e;
        return arr;
    }

    template <typename t, std::size_t N, typename T, typename filter_type = decltype(valid)>
    constexpr std::array<t, N> &insert_if_no_present(std::array<t, N> &arr, T &e, filter_type fil = valid)
    {
        auto find = []<typename t2, std::size_t N2, typename T2, typename filter_type2 = decltype(valid)>(std::array<t2, N2> & arr2, T2 e2, filter_type2 fil2 = valid)->std::size_t
        {
            std::size_t index = 1;
            for (auto& a : iterate(arr2, fil2))
                if (a == e2)
                    return index;
                else
                    index++;
            return 0;
        };
        if(find(arr,e,fil)==0){
            arr[length(arr, fil)] = e;
            std::sort(arr.begin(), arr.begin()+length(arr,fil));
        }
        return arr;
    }



    template <typename t, std::size_t N, typename T, std::size_t M, typename filter_type = decltype(valid)>
    constexpr std::array<t, N> &insert(std::array<t, N> &arr, std::array<T, M> arr1, filter_type fil = valid)
    {
        auto index = length(arr,fil);
        for (auto e : iterate(arr1,fil))
            arr[index++] = e;
        return arr;
    }

    template <typename t, std::size_t N, typename T, std::size_t M, typename filter_type = decltype(valid)>
    constexpr std::array<t, N> &insert_if_no_present(std::array<t, N> &arr, std::array<T, M> arr1, filter_type fil = valid)
    {
        for (auto e : iterate(arr1,fil))
            insert_if_no_present(arr, e, fil);
        return arr;
    }


    template <typename t, std::size_t N, typename T, typename filter_type = decltype(valid)>
    constexpr std::size_t find_first(std::array<t, N> &arr, T e, filter_type fil = valid)
    {
        std::size_t index = 1;
        for(auto a : iterate(arr,fil))
            if(a == e)
                return index;
            else
                index++;
        return 0;
    }

    template <typename t, std::size_t N, typename T, typename filter_type = decltype(valid)>
    constexpr std::size_t find_last(std::array<t, N> &arr, T e,filter_type fil = valid)
    {
        std::size_t index = length(arr,fil);
        for (auto a : iterate(arr,fil) | std::views::reverse)
            if (a == e)
                return index;
            else
                index--;
        return 0;
    }

    template <typename t, std::size_t N, typename T, typename filter_type = decltype(valid)>
    constexpr std::array<std::size_t, N> find_all(std::array<t, N> &arr, T e, filter_type fil = valid)
    {
        std::array<std::size_t,N> result = {0};
        std::size_t index = 1;
        for (auto a : iterate(arr,fil))
            if (a == e)
                insert(result,index,[](auto sss){return sss != 0;});
            else
                index++;
        return result;
    }

    template <typename t, std::size_t N, typename T, typename filter_type = decltype(valid)>
    constexpr std::array<t, N> &remove_first(std::array<t, N> &arr, T e, filter_type fil = valid)
    {
        auto index = find_first(arr,e,fil);
        for(auto len = length(arr,fil);index<=len;index++)
            arr[index-1] = arr[index];
        return arr;
    }

    template <typename t, std::size_t N, typename T, typename filter_type = decltype(valid)>
    constexpr std::array<t, N> &remove_last(std::array<t, N> &arr, T e,filter_type fil = valid)
    {
        auto index = find_last(arr, e,fil);
        for (auto len = length(arr,fil); index <= len; index++)
            arr[index - 1] = arr[index];
        return arr;
    }

    template <typename t, std::size_t N, typename T, typename filter_type = decltype(valid)>
    constexpr std::array<t, N> &remove_all(std::array<t, N> &arr, T e,filter_type fil = valid)
    {
        std::size_t index = find(arr, e,fil);
        while(index !=0){
            for (auto len = length(arr,fil); index <= len; index++)
                arr[index - 1] = arr[index];
            index = find(arr,e,fil);
        }
        return arr;
    }

} // namespace IStudio::Util
