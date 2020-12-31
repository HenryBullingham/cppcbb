//Copyright(C) 2020 Henry Bullingham
//This file is subject to the license terms in the LICENSE file
//found in the top - level directory of this distribution.

#pragma once

#if !defined (CPPCBB_INCLUDE_CBB_MAP_H)
#define CPPCBB_INCLUDE_CBB_MAP_H

#include "cbb_common.hpp"
#include "cbb_vector.hpp"

#include <utility>

namespace cppcbb
{
    /// <summary>
    /// Info about vector storage for key & value
    /// </summary>
    /// <typeparam name="Key"></typeparam>
    /// <typeparam name="Value"></typeparam>
    template<typename Key, typename Value>
    class default_pair_storage_traits
    {
    public:
        using Elem = std::pair<Key, Value>;
        using iterator = Elem*;
        using const_iterator = Elem const*;
    };

    /// <summary>
    /// Keeps entries in order
    /// O(1) insert
    /// O(n) delete
    /// O(n) search
    /// </summary>
    /// <typeparam name="Key"></typeparam>
    /// <typeparam name="Value"></typeparam>
    /// <typeparam name="Traits"></typeparam>
    template<typename Key, typename Value, typename Traits = default_pair_storage_traits<Key,Value>>
    class ordered_map_pair_management;

    /// <summary>
    /// Maintains no order between entries
    /// O(1) insert
    /// O(1) delete
    /// O(n) search
    /// </summary>
    /// <typeparam name="Key"></typeparam>
    /// <typeparam name="Value"></typeparam>
    /// <typeparam name="Traits"></typeparam>
    template<typename Key, typename Value, typename Traits = default_pair_storage_traits<Key, Value>>
    class unordered_map_pair_management;

    /// <summary>
    /// Keeps elements in a sorted order using binary search
    /// O(n) insert
    /// O(n) delete
    /// O(log n) search
    /// </summary>
    /// <typeparam name="Key"></typeparam>
    /// <typeparam name="Value"></typeparam>
    /// <typeparam name="Traits"></typeparam>
    template<typename Key, typename Value, typename Traits = default_pair_storage_traits<Key, Value>>
    class sorted_pair_management;

    /// <summary>
    /// Stores elements as a vector of pairs
    /// </summary>
    /// <typeparam name="Key"></typeparam>
    /// <typeparam name="Value"></typeparam>
    /// <typeparam name="Traits"></typeparam>
    /// <typeparam name="Vector"></typeparam>
    /// <typeparam name="Management"></typeparam>
    template<typename Key, typename Value
        , typename Traits = default_pair_storage_traits<Key, Value>
        , typename Vector = cbb_vector<typename Traits::Elem>
        , typename Management = ordered_map_pair_management<Key, Value>
    >
    class pair_storage;

    /// <summary>
    /// Implementation of a map
    /// </summary>
    /// <typeparam name="Key"></typeparam>
    /// <typeparam name="Value"></typeparam>
    /// <typeparam name="Storage"></typeparam>
    template<typename Key, typename Value, typename Storage = pair_storage<Key,Value>>
    class cbb_map_impl;

    /// <summary>
    /// Map with dynamic vector storage
    /// </summary>
    template<typename Key, typename Value, typename Traits = default_pair_storage_traits<Key, Value>>
    using cbb_vector_map =
        cbb_map_impl < Key, Value, pair_storage<Key, Value, Traits, cbb_vector<typename Traits::Elem>>>;

    /// <summary>
    /// Map with static vector storage
    /// </summary>
    template<typename Key, typename Value, size_t Capacity = 16, typename Traits = default_pair_storage_traits<Key, Value>>
    using cbb_static_vector_map =
        cbb_map_impl < Key, Value, pair_storage<Key, Value, Traits, cbb_static_vector<typename Traits::Elem, Capacity>>>;

    /// <summary>
    /// Map with dynamic vector storage
    /// </summary>
    template<typename Key, typename Value, typename Traits = default_pair_storage_traits<Key, Value>>
    using cbb_unordered_vector_map =
        cbb_map_impl < 
            Key, Value, 
                pair_storage<Key, Value, Traits
                    , cbb_vector<typename Traits::Elem>
                    , unordered_map_pair_management<Key, Value, Traits>
            >
        >;

    /// <summary>
    /// Map with static vector storage
    /// </summary>
    template<typename Key, typename Value, size_t Capacity = 16, typename Traits = default_pair_storage_traits<Key, Value>>
    using cbb_static_unordered_vector_map =
        cbb_map_impl < 
            Key, Value, 
            pair_storage<Key, Value, Traits
                , cbb_static_vector<typename Traits::Elem, Capacity>
                , unordered_map_pair_management<Key, Value, Traits>
            >
        >;

    /// <summary>
    /// Map with dynamic vector storage
    /// </summary>
    template<typename Key, typename Value, typename Traits = default_pair_storage_traits<Key, Value>>
    using cbb_sorted_vector_map =
        cbb_map_impl < 
            Key, Value, 
            pair_storage<Key, Value, Traits
                , cbb_vector<typename Traits::Elem>
                , unordered_map_pair_management<Key, Value, Traits>
            >
        >;

    /// <summary>
    /// Map with static vector storage
    /// </summary>
    template<typename Key, typename Value, size_t Capacity = 16, typename Traits = default_pair_storage_traits<Key, Value>>
    using cbb_static_sorted_vector_map =
        cbb_map_impl < 
            Key, Value, 
            pair_storage<Key, Value, Traits
                , cbb_static_vector<typename Traits::Elem, Capacity>
                , unordered_map_pair_management<Key, Value, Traits>
            >
        >;
}

/// <summary>
/// Ordered management for vector maps
/// </summary>
namespace cppcbb
{
    template<typename Key, typename Value, typename Traits>
    class ordered_map_pair_management
    {
    public:
        using Elem = typename Traits::Elem;
        using iterator = typename Traits::iterator;
        using const_iterator = typename Traits::const_iterator;

        //elm passed in is at end of range
        static iterator insert(iterator begin, iterator end, iterator elem)
        {
            CPPCBB_ASSERT((elem + 1) == end, "Elem not passed at end of range!");
            return elem;
        }

        //Need to move elem to end of range
        static void erase(iterator begin, iterator end, iterator elem)
        {
            //Rotate elem to the end
            std::rotate(elem, elem + 1, end);
        }

        static const_iterator find(const_iterator begin, const_iterator end, const Key& key)
        {
            return std::find_if(begin, end, [&](const Elem& entry) { return entry.first == key; });
        }
    };
}

/// <summary>
/// Unordered management for vector maps
/// </summary>
namespace cppcbb
{
    template<typename Key, typename Value, typename Traits >
    class unordered_map_pair_management
    {
    public:
        using Elem = typename Traits::Elem;
        using iterator = typename Traits::iterator;
        using const_iterator = typename Traits::const_iterator;

        //elm passed in is at end of range
        static iterator insert(iterator begin, iterator end, iterator elem)
        {
            CPPCBB_ASSERT((elem + 1) == end, "Elem not passed at end of range!");
            return elem;
        }

        //Need to move elem to end of range
        static void erase(iterator begin, iterator end, iterator elem)
        {
            std::swap(*elem, *(end - 1));
        }

        static const_iterator find(const_iterator begin, const_iterator end, const Key& key)
        {
            return std::find_if(begin, end, [&](const Elem& entry) { return entry.first == key; });
        }
    };
}

/// <summary>
/// Sorted management for vector maps
/// </summary>
namespace cppcbb
{
    template<typename Key, typename Value, typename Traits>
    class sorted_pair_management
    {
    public:
        using Elem = typename Traits::Elem;
        using iterator = typename Traits::iterator;
        using const_iterator = typename Traits::const_iterator;

        //elem passed in is at end of range
        static iterator insert(iterator begin, iterator end, iterator elem)
        {
            CPPCBB_ASSERT((elem + 1) == end, "Elem not passed at end of range!");
            auto loc = std::lower_bound(begin, end, elem, [](const Elem& left, const Elem& right) { return left.first < right.first); };
            std::rotate(loc, elem, end);
            return loc;
        }

        //Need to move elem to end of range
        static void erase(iterator begin, iterator end, iterator elem)
        {
            std::rotate(elem, elem + 1, end);
        }

        static const_iterator find(const_iterator begin, const_iterator end, const Key& key)
        {
            auto loc = std::lower_bound(begin, end, key, [](const Elem& left, const Key& right) { return left.first < key); };
            if (loc->first == key)
            {
                return loc;
            }
            return end;
        }
    };
}

/// <summary>
/// Pair Storage
/// </summary>
namespace cppcbb
{
    template<typename Key, typename Value
        , typename Traits
        , typename Vector
        , typename Management 
    >
    class pair_storage
    {
    public:
        using Elem = typename Traits::Elem;
        using iterator = typename Vector::iterator;
        using const_iterator = typename Vector::const_iterator;

    private:
        Vector m_elements;

    public:
        iterator begin() { return m_elements.begin(); }
        iterator end() { return m_elements.end(); }

        const_iterator cbegin() const { return m_elements.cbegin(); }
        const_iterator cend() const { return m_elements.cend(); }

        //Finds the iterator for the key
        const_iterator find(const Key& key) const
        { 
            return Management::find(cbegin(), cend(), key);
        }
       
        //Inserts a new iterator for the key
        iterator insert(Key key, Value value)
        {
            auto it = find(key);
            if (it != end())
            {
                return (iterator)it;
            }
            m_elements.push_back(Elem(std::move(key), std::move(value)));
            return Management::insert(begin(), end(), end() - 1);
        }

        //Erases the item from the list
        void erase(const_iterator elem)
        {
            Management::erase(begin(), end(), (iterator)elem);
            m_elements.pop_back();
        }

        void clear()
        {
            m_elements.clear();
        }

        size_t size() const
        {
            return m_elements.size();
        }
    };
}

namespace cppcbb
{
    template<typename Key, typename Value, typename Storage>
    class cbb_map_impl
    {
    public:
        using Elem = typename Storage::Elem;
        using iterator = typename Storage::iterator;
        using const_iterator = typename Storage::const_iterator;

    private:
        Storage m_storage;

    public:

        iterator begin() { return m_storage.begin(); }
        iterator end() { return m_storage.end(); }

        const_iterator begin() const { return m_storage.cbegin(); }
        const_iterator end() const { return m_storage.cend(); }

        const_iterator cbegin() const { return m_storage.cbegin(); }
        const_iterator cend() const { return m_storage.cend(); }

        const_iterator find(const Key& key) const { return m_storage.find(key); }

        Value& operator[] (Key key) 
        { 
            auto constIt = find(key); 
            if (constIt != end())
            {
                auto it = (iterator)constIt;
                return it->second;
            }
            return m_storage.insert(std::move(key), Value())->second;
        }

        void erase(const_iterator elem)
        {
            m_storage.erase(elem);
        }

        void clear()
        {
            m_storage.clear();
        }

        size_t size() const { return m_storage.size(); }
    };
}

#endif //CPPCBB_INCLUDE_CBB_MAP_H