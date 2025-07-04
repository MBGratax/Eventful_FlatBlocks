#pragma once

#include <array>
#include <assert.h>
#include "CoreTypes.h"

namespace EventfulEngine{

    template <typename T, uint32 maxLength>
    struct EFStaticArray : private std::array<T, maxLength>{
        typedef std::array<T, maxLength> base;

        enum{ max_elements = maxLength };

        using typename base::value_type;
        using typename base::size_type;
        using typename base::difference_type;
        using typename base::reference;
        using typename base::const_reference;
        using typename base::pointer;
        using typename base::const_pointer;
        using typename base::iterator;
        using typename base::const_iterator;

        EFStaticArray()
            : base()
              , current_size(0){
        }

        EFStaticArray(size_t size)
            : base()
              , current_size(size){
            assert(size <= max_elements);
        }

        EFStaticArray(std::initializer_list<T> il)
            : current_size(0){
            for (auto i : il)
                push_back(i);
        }

        using base::at;

        reference operator[](size_type pos){
            assert(pos < current_size);
            return base::operator[](pos);
        }

        const_reference operator[](size_type pos) const{
            assert(pos < current_size);
            return base::operator[](pos);
        }

        using base::front;

        reference back() noexcept{
            auto tmp = end();
            --tmp;
            return *tmp;
        }

        const_reference back() const noexcept{
            auto tmp = cend();
            --tmp;
            return *tmp;
        }

        using base::data;
        using base::begin;
        using base::cbegin;

        iterator end() noexcept{ return iterator(begin()) + current_size; }
        const_iterator end() const noexcept{ return cend(); }
        const_iterator cend() const noexcept{ return const_iterator(cbegin()) + current_size; }

        bool empty() const noexcept{ return current_size == 0; }
        size_t size() const noexcept{ return current_size; }
        constexpr size_t max_size() const noexcept{ return max_elements; }

        void fill(const T& value) noexcept{
            base::fill(value);
            current_size = max_elements;
        }

        void swap(EFStaticArray& other) noexcept{
            base::swap(*this);
            std::swap(current_size, other.current_size);
        }

        void push_back(const T& value) noexcept{
            assert(current_size < max_elements);
            *(data() + current_size) = value;
            ++current_size;
        }

        void push_back(T&& value) noexcept{
            assert(current_size < max_elements);
            *(data() + current_size) = std::move(value);
            ++current_size;
        }

        void pop_back() noexcept{
            assert(current_size > 0);
            --current_size;
        }

        void resize(size_type new_size) noexcept{
            assert(new_size <= max_elements);

            if (current_size > new_size){
                for (size_type i = new_size; i < current_size; ++i)
                    *(data() + i) = T{};
            }
            else{
                for (size_type i = current_size; i < new_size; ++i)
                    *(data() + i) = T{};
            }

            current_size = new_size;
        }

        reference emplace_back() noexcept{
            assert(current_size < max_elements);
            ++current_size;
            back() = T{};
            return back();
        }

    private:
        size_type current_size = 0;
    };
}
