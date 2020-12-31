//Copyright(C) 2020 Henry Bullingham
//This file is subject to the license terms in the LICENSE file
//found in the top - level directory of this distribution.

#pragma once

#if !defined (CPPCBB_INCLUDE_CBB_VECTOR_H)
#define CPPCBB_INCLUDE_CBB_VECTOR_H

#include "cbb_common.hpp"

#include <algorithm>
#include <memory>

namespace cppcbb
{
    /// <summary>
    /// Data for dynamic storage option
    /// </summary>
    class default_dynamic_storage_params
    {
    public:
        static constexpr size_t initial_capacity = 10;
        static constexpr float  growth_rate = 1.5f;
    };

    /// <summary>
    /// Represents dynamic (heap allocated) vector storage
    /// </summary>
    /// <typeparam name="Elem"></typeparam>
    template<typename Elem, typename Traits = default_traits<Elem>, typename Params = default_dynamic_storage_params>
    class dynamic_vec_storage;

    /// <summary>
    /// Represents static (fixed size) vector storage
    /// </summary>
    /// <typeparam name="Elem"></typeparam>
    template<typename Elem, size_t Capacity = 16, typename Traits = default_traits<Elem>>
    class static_vec_storage;

    /// <summary>
    /// Represents ordered vector management (the elements stay in the order they were inserted)
    /// </summary>
    /// <typeparam name="Elem"></typeparam>
    template<typename Elem, typename Traits = default_traits<Elem>>
    class ordered_vec_management;

    /// <summary>
    /// Represents unordered vector management (the elements stay in no particular order)
    /// </summary>
    /// <typeparam name="Elem"></typeparam>
    template<typename Elem, typename Traits = default_traits<Elem>>
    class unordered_vec_management;

    //TODO: make concepts & template checks for storage & management

    /// <summary>
    /// A vector (resizeable array)
    /// </summary>
    /// <typeparam name="Elem"></typeparam>
    /// <typeparam name="Traits"></typeparam>
    /// <typeparam name="Storage"></typeparam>
    /// <typeparam name="Management"></typeparam>
    template<typename Elem, typename Traits = default_traits<Elem>, typename Storage = dynamic_vec_storage<Elem, Traits>, typename Management = ordered_vec_management<Elem,Traits>>
    class cbb_vector_impl;

    template<typename Elem, typename Traits = default_traits<Elem>>
    using cbb_vector = cbb_vector_impl<Elem, Traits, dynamic_vec_storage<Elem, Traits>, ordered_vec_management<Elem, Traits>>;

    template<typename Elem, size_t Capacity = 16, typename Traits = default_traits<Elem>>
    using cbb_static_vector = cbb_vector_impl<Elem, Traits, static_vec_storage<Elem, Capacity, Traits>, ordered_vec_management<Elem, Traits>>;

    template<typename Elem, typename Traits = default_traits<Elem>>
    using cbb_unordered_vector = cbb_vector_impl<Elem, Traits, dynamic_vec_storage<Elem, Traits>, unordered_vec_management<Elem, Traits>>;

    template<typename Elem, size_t Capacity = 16, typename Traits = default_traits<Elem>>
    using cbb_static_unordered_vector = cbb_vector_impl<Elem, Traits, static_vec_storage<Elem, Capacity, Traits>, unordered_vec_management<Elem, Traits>>;
}

/*

    Implementation details

*/

/// <summary>
/// Dynamic Storage
/// </summary>
namespace cppcbb
{
    template<typename Elem, typename Traits, typename Params>
    class dynamic_vec_storage
    {
    public:
        using iterator = typename Traits::iterator;
        using const_iterator = typename Traits::iterator;

    private:
        std::unique_ptr<Elem[]> m_data;
        size_t m_capacity;

        static constexpr size_t k_initial_capacity = Params::initial_capacity;
        static constexpr float k_growth_rate = Params::growth_rate;
        
    public:
        dynamic_vec_storage() 
            : m_data(std::make_unique<Elem[]>(k_initial_capacity))
            , m_capacity(k_initial_capacity)
        {}

        iterator begin() { return (iterator)m_data.get(); }
        const_iterator begin() const { return (const_iterator)m_data.get(); }
        const_iterator cbegin() const { return (const_iterator)m_data.get(); }

        size_t capacity() const { return m_capacity; }

        bool ensure_capacity(size_t capacity, size_t size);
    };

    template<typename Elem, typename Traits, typename Params>
    inline bool dynamic_vec_storage<Elem, Traits, Params>::ensure_capacity(size_t capacity, size_t size)
    {
        if (capacity <= m_capacity)
        {
            return true;
        }

        //Calculate new capacity by growth rate
        size_t new_capacity = (size_t)( m_capacity * k_growth_rate);
        if (new_capacity < capacity)
        {
            new_capacity = capacity;
        }

        auto new_data = std::make_unique<Elem[]>(new_capacity);
        if (new_data == nullptr)
        {
            return false;
        }

        //Copy over the new elements
        for (size_t i = 0; i < size; i++)
        {
            new_data[i] = std::move(m_data[i]);
        }

        m_data = std::move(new_data);
        m_capacity = new_capacity;

        return true;
    }
}

/// <summary>
/// Static Storage
/// </summary>
namespace cppcbb
{
    template<typename Elem, size_t Capacity, typename Traits>
    class static_vec_storage
    {
    public:
        using iterator = typename Traits::iterator;
        using const_iterator = typename Traits::iterator;

    private:
        Elem m_data[Capacity];

    public:

        iterator begin() { return (iterator)&m_data[0]; }
        const_iterator begin() const { return (const_iterator)& m_data[0]; }
        const_iterator cbegin() const { return (const_iterator)& m_data[0]; }

        size_t capacity() const { return m_capacity; }

        bool ensure_capacity (size_t capacity, size_t size) const { return capacity <= Capacity; }
    };
}

/// <summary>
/// Ordered Management
/// </summary>
namespace cppcbb
{
    template<typename Elem, typename Traits>
    class ordered_vec_management
    {
    public:
        using iterator = typename Traits::iterator;
        using const_iterator = typename Traits::iterator;

        //Inserts at end
        static iterator insert (iterator begin, iterator end, const Elem& elem)
        { 
            return end; 
        }

        // Shifts things over to remove
        // Afterwards, final element should be removable
        static void erase (iterator begin, iterator end, iterator elem)
        {
            std::rotate(elem, elem + 1, end);
        }
    };
}

/// <summary>
/// Unordered Management
/// </summary>
namespace cppcbb
{
    template<typename Elem, typename Traits>
    class unordered_vec_management
    {
    public:
        using iterator = typename Traits::iterator;
        using const_iterator = typename Traits::iterator;

        //Inserts at end
        static iterator insert (iterator begin, iterator end, const Elem& elem)
        {
            return end;
        }

        // Swap element with end
        // Afterwards, final element should be removable
        static void erase (iterator begin, iterator end, iterator elem)
        {
            if ((elem + 1) == end)
            {
                return;
            }

            std::swap(*elem, *(end - 1));
        }
    };
}


///
/// Vector implementation
/// 
namespace cppcbb
{
    template<typename Elem, typename Traits, typename Storage, typename Management>
    class cbb_vector_impl
    {
    public:
        using iterator = typename Traits::iterator;
        using const_iterator = typename Traits::const_iterator;
        using self_type = cbb_vector_impl<Elem, Traits, Storage, Management>;

    private:
        Storage m_storage;
        iterator m_end = m_storage.begin();

        bool ensure_capacity(size_t capacity);

    public:
        cbb_vector_impl(){}

        cbb_vector_impl(const self_type&);
        cbb_vector_impl(self_type&&) noexcept;

        self_type& operator=(const self_type&);
        self_type& operator=(self_type&&);

        iterator begin() { return m_storage.begin(); }
        iterator end() { return m_end; }

        const_iterator begin() const { return m_storage.begin(); }
        const_iterator end() const { return m_end; }

        const_iterator cbegin() const { return m_storage.begin(); }
        const_iterator cend() const { return m_end; }

        size_t size() const { return m_end - begin(); }
        size_t capacity() const { return m_storage.capacity(); }

        void push_back(const Elem& elem);
        void push_back(Elem&& elem);
        template<typename ... Args>
        void emplace_back(Args&& ... args);

        Elem& back();
        const Elem& back() const;

        void pop_back();

        void erase(iterator elem);

        void clear();
        void resize(size_t size);

        Elem& operator[](size_t);
        const Elem& operator[](size_t) const;
    };

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline bool cbb_vector_impl<Elem, Traits, Storage, Management>::ensure_capacity(size_t capacity)
    {
        size_t cur_size = size();
        bool enough_storage = m_storage.ensure_capacity(capacity, cur_size);
        m_end = begin() + cur_size;
        return enough_storage;
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline cbb_vector_impl<Elem, Traits, Storage, Management>::cbb_vector_impl(const self_type& other)
    {
        for (const Elem& e : other)
        {
            push_back(e);
        }
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline cbb_vector_impl<Elem, Traits, Storage, Management>::cbb_vector_impl(self_type&& other) noexcept
    {
        for (Elem& e : other)
        {
            push_back(std::move(e));
        }
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline typename cbb_vector_impl<Elem, Traits, Storage, Management>::self_type& cbb_vector_impl<Elem, Traits, Storage, Management>::operator=(const self_type& other)
    {
        clear();
        for (const Elem& e : other)
        {
            push_back(e);
        }
        return *this;
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline typename cbb_vector_impl<Elem, Traits, Storage, Management>::self_type& cbb_vector_impl<Elem, Traits, Storage, Management>::operator=(self_type&& other)
    {
        clear();
        for (Elem& e : other)
        {
            push_back(std::move(e));
        }
        return *this;
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline void cbb_vector_impl<Elem, Traits, Storage, Management>::push_back(const Elem& elem)
    {
        CPPCBB_ASSERT(ensure_capacity(size() + 1), "Not enough storage!");
        iterator loc = Management::insert(begin(), end(), elem);
        *loc = elem;
        m_end++;
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline void cbb_vector_impl<Elem, Traits, Storage, Management>::push_back(Elem&& elem)
    {
        CPPCBB_ASSERT(ensure_capacity(size() + 1), "Not enough storage!");

        iterator loc = Management::insert(begin(), end(), elem);
        *loc = std::move(elem);
        m_end++;
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    template<typename ...Args>
    inline void cbb_vector_impl<Elem, Traits, Storage, Management>::emplace_back(Args && ...args)
    {
        Elem elem(std::forward<Args>(args)...);
        push_back(std::move(elem));
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline Elem& cbb_vector_impl<Elem, Traits, Storage, Management>::back()
    {
        CPPCBB_ASSERT((size() > 0), "No elements in vector!");

        return *(m_end - 1);
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline const Elem& cbb_vector_impl<Elem, Traits, Storage, Management>::back() const
    {
        CPPCBB_ASSERT((size() > 0), "No elements in vector!");

        return *(m_end - 1);
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline void cbb_vector_impl<Elem, Traits, Storage, Management>::pop_back()
    {
        CPPCBB_ASSERT((size() > 0), "No elements in vector!");
        m_end--;
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline void cbb_vector_impl<Elem, Traits, Storage, Management>::erase(iterator elem)
    {
        Management::erase(begin(), end(), elem);
        pop_back();
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline void cbb_vector_impl<Elem, Traits, Storage, Management>::clear()
    {
        m_end = begin();
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline void cbb_vector_impl<Elem, Traits, Storage, Management>::resize(size_t size)
    {
        CPPCBB_ASSERT(ensure_capacity(size), "Not enough storage!");

        if (size() >= size)
        {
            for (size_t current = size(); current > size; current--)
            {
                pop_back();
            }
        }
        else
        {
            for (size_t current = size(); current < size; current++)
            {
                emplace_back();
            }
        }
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline Elem& cbb_vector_impl<Elem, Traits, Storage, Management>::operator[](size_t idx)
    {
        CPPCBB_ASSERT((idx < size()), "Out of bounds access!");
        return *(begin() + idx);
    }

    template<typename Elem, typename Traits, typename Storage, typename Management>
    inline const Elem& cbb_vector_impl<Elem, Traits, Storage, Management>::operator[](size_t idx) const
    {
        CPPCBB_ASSERT((idx < size()), "Out of bounds access!");
        return *(begin() + idx);
    }

};



#endif //CPPCBB_INCLUDE_CBB_VECTOR_H