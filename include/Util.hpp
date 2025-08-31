#pragma once

#include "Types_Compiler.hpp"

namespace IStudio::Util
{
    template <typename T>
    concept NotStdArray = !std::is_same_v<T, std::array<typename T::value_type, T::size()>>;

     auto valid = [](auto e)->bool{return e.isValid();};

     auto count = [](auto begin,auto end) -> std::size_t {
        auto iter = begin;
        std::size_t c = 0;
        while(iter!=end){
            c++;
            iter++;
        }

        return c;
    };

     auto countOne = [](auto container) -> std::size_t {
        return count(container.begin(),container.end());
    };



    namespace rng = std::ranges;

    template <rng::range R>
     auto to_vector(R &&r)
    {
        using elem_t = std::decay_t<rng::range_value_t<R>>;
        return std::vector<elem_t>{r.begin(), r.end()};
    }

    bool compareFilterViews(auto filter1, auto filter2)
    {
        return to_vector(filter1) == to_vector(filter2);
    }

    template <typename T>
    class generator
    {
    public:
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type
        {
            T current_value;

            auto initial_suspend() noexcept
            {
                return std::suspend_always{};
            }

            auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }

            auto yield_value(auto &&value) noexcept
            {
                current_value = std::forward<decltype(value)>(value);
                return std::suspend_always{};
            }

            void return_void() noexcept {}

            void unhandled_exception()
            {
                std::terminate();
            }

            auto get_return_object() noexcept
            {
                return generator{handle_type::from_promise(*this)};
            }

            void reset() noexcept
            {
                current_value = T{};
            }
        };

    private:
        handle_type coro_handle;

    public:
        explicit generator(handle_type h) : coro_handle(h) {}

        generator(const generator &) = delete;
        generator &operator=(const generator &) = delete;

        generator(generator &&other) noexcept : coro_handle(other.coro_handle)
        {
            other.coro_handle = nullptr;
        }

        generator &operator=(generator &&other) noexcept
        {
            if (this != &other)
            {
                coro_handle = other.coro_handle;
                other.coro_handle = nullptr;
            }
            return *this;
        }

        ~generator()
        {
            if (coro_handle)
                coro_handle.destroy();
        }

        // Iterator support
        class iterator
        {
        public:
            void operator++()
            {
                coro_handle.resume();
            }

            const T &operator*() const
            {
                return coro_handle.promise().current_value;
            }

            bool operator!=(const iterator &rhs) const
            {
                return coro_handle != rhs.coro_handle;
            }

        private:
            iterator(handle_type h) : coro_handle(h) {}

            friend class generator;
            handle_type coro_handle;
        };

        iterator begin() const
        {
            if (coro_handle)
                coro_handle.resume();
            return iterator{coro_handle};
        }

        iterator end() const
        {
            return iterator{nullptr};
        }

        void reset()
        {
            if (coro_handle)
            {
                coro_handle.promise().reset();
                coro_handle.resume();
            }
        }
    };

} // namespace IStudio::Util
