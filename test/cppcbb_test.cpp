// Copyright(C) 2020 Henry Bullingham
// This file is subject to the license terms in the LICENSE file
// found in the top - level directory of this distribution.

#include "catch_amalgamated.hpp"

#include "cppcbb/cbb_vector.hpp"
#include "cppcbb/cbb_map.hpp"

#include <algorithm>

#include <limits>

#include <random>

/*

Make sure static storage objects have enough size to test

*/
static constexpr int k_test_max_size = 500;

static std::mt19937 NewRandom()
{
    std::random_device rd;
    std::mt19937 generator{ rd() };
    return generator;
}

static std::mt19937& GetRandom()
{
    static std::mt19937 generator = NewRandom();
    return generator;
}

template< typename Vector>
void TestVector(Vector& v)
{
    SECTION("Push Back")
    {
        v.push_back(5);
        v.push_back(-1);
        v.push_back(3);

        REQUIRE(v.size() == 3);

        REQUIRE(std::find(v.begin(), v.end(), 5) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), -1) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), 3) != v.end());

        REQUIRE(std::find(v.begin(), v.end(), INT_MAX) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), INT_MIN) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), 7) == v.end());
    }

    SECTION("Pop Back")
    {
        v.push_back(5);
        v.push_back(-1);
        v.push_back(3);

        REQUIRE(v.size() == 3);

        REQUIRE(std::find(v.begin(), v.end(), 5) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), -1) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), 3) != v.end());

        REQUIRE(std::find(v.begin(), v.end(), INT_MAX) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), INT_MIN) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), 7) == v.end());

        v.pop_back();
        REQUIRE(std::find(v.begin(), v.end(), 3) == v.end());
        REQUIRE(v.size() == 2);

        v.pop_back();
        REQUIRE(std::find(v.begin(), v.end(), -1) == v.end());
        REQUIRE(v.size() == 1);

        v.pop_back();
        REQUIRE(std::find(v.begin(), v.end(), 5) == v.end());
        REQUIRE(v.size() == 0);

    }

    SECTION("Copying")
    {
        v.push_back(5);
        v.push_back(-1);
        v.push_back(3);

        Vector v2 = v;

        REQUIRE(v2.size() == 3);

        REQUIRE(std::find(v2.begin(), v2.end(), 5) != v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), -1) != v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), 3) != v2.end());

        REQUIRE(std::find(v2.begin(), v2.end(), INT_MAX) == v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), INT_MIN) == v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), 7) == v2.end());
    }

    SECTION("Moving")
    {
        v.push_back(5);
        v.push_back(-1);
        v.push_back(3);

        Vector v2 = std::move(v);

        REQUIRE(v2.size() == 3);

        REQUIRE(std::find(v2.begin(), v2.end(), 5) != v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), -1) != v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), 3) != v2.end());

        REQUIRE(std::find(v2.begin(), v2.end(), INT_MAX) == v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), INT_MIN) == v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), 7) == v2.end());
    }

    SECTION("Copy-Overwrite")
    {
        v.push_back(5);
        v.push_back(-1);
        v.push_back(3);

        REQUIRE(v.size() == 3);

        REQUIRE(std::find(v.begin(), v.end(), 5) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), -1) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), 3) != v.end());

        REQUIRE(std::find(v.begin(), v.end(), INT_MAX) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), INT_MIN) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), 7) == v.end());

        Vector v2;
        v2.push_back(7);
        v2.push_back(INT_MAX);
        v2.push_back(INT_MIN);

        v = v2;

        REQUIRE(std::find(v.begin(), v.end(), 5) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), -1) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), 3) == v.end());

        REQUIRE(std::find(v.begin(), v.end(), INT_MAX) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), INT_MIN) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), 7) != v.end());
    }

    SECTION("Deep-Copy")
    {
        v.push_back(5);
        v.push_back(-1);
        v.push_back(3);

        REQUIRE(v.size() == 3);

        REQUIRE(std::find(v.begin(), v.end(), 5) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), -1) != v.end());
        REQUIRE(std::find(v.begin(), v.end(), 3) != v.end());

        REQUIRE(std::find(v.begin(), v.end(), INT_MAX) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), INT_MIN) == v.end());
        REQUIRE(std::find(v.begin(), v.end(), 7) == v.end());

        Vector v2 = v;
        v.push_back(7);
        v.push_back(INT_MAX);
        v.push_back(INT_MIN);

        REQUIRE(std::find(v2.begin(), v2.end(), 5) != v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), -1) != v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), 3) != v2.end());

        REQUIRE(std::find(v2.begin(), v2.end(), INT_MAX) == v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), INT_MIN) == v2.end());
        REQUIRE(std::find(v2.begin(), v2.end(), 7) == v2.end());
    }

    SECTION("Clear")
    {
        for (int i = 0; i < k_test_max_size; i++)
        {
            v.push_back(i);
        }

        for (int i = 0; i < k_test_max_size; i++)
        {
            REQUIRE(std::find(v.begin(), v.end(), i) != v.end());
        }

        v.clear();

        for (int i = 0; i < k_test_max_size; i++)
        {
            REQUIRE(std::find(v.begin(), v.end(), i) == v.end());
        }
    }

    SECTION("Random insertions and deletions")
    {
        std::uniform_int_distribution<int> dist{ INT_MIN, INT_MAX };

        const int num_runs = 10;
        const int num_values = k_test_max_size;

        for (int j = 0; j < num_runs; j++)
        {
            //Fill with random values
            for (int i = 0; i < num_values; i++)
            {
                int x = dist(GetRandom());
                while (std::find(v.begin(), v.end(), x) != v.end())
                {
                    x = dist(GetRandom());
                }
                v.push_back(x);
            }

            //Copy, shuffle and verify all elements exist
            Vector v2;
            v2 = v;

            std::random_shuffle(v.begin(), v.end());

            for (int x : v2)
            {
                REQUIRE(std::find(v.begin(), v.end(), x) != v.end());
            }

            //Now, remove from random indices
            while (v.size() > 0)
            {
                std::uniform_int_distribution<int> index_dist(0, (int)v.size() - 1);
                int index = index_dist(GetRandom());
                int x = v[index];
                v.erase(v.begin() + index);
                REQUIRE(std::find(v.begin(), v.end(), x) == v.end());
            }
        }
    }
}

TEST_CASE("CPPCBB Vector", "[CPPCBB]")
{
    SECTION("Dynamic, Ordered")
    {
        cppcbb::cbb_vector<int> vec;
        TestVector(vec);
    }

    SECTION("Dynamic, Unordered")
    {
        cppcbb::cbb_unordered_vector<int> vec;
        TestVector(vec);
    }

    SECTION("Static, Ordered")
    {
        cppcbb::cbb_static_vector<int, k_test_max_size> vec;
        TestVector(vec);
    }

    SECTION("Static, Unordered")
    {
        cppcbb::cbb_static_unordered_vector<int, k_test_max_size> vec;
        TestVector(vec);
    }
}

template<typename Map>
void TestMap(Map& map)
{
    SECTION("Insert")
    {
        map[1] = 5;
        map[2] = 7;
        map[-1] = 3;
        map[6] = -32;

        REQUIRE(map.find(1) != map.end());
        REQUIRE(map.find(2) != map.end());
        REQUIRE(map.find(-1) != map.end());
        REQUIRE(map.find(6) != map.end());

        REQUIRE(map.find(4) == map.end());
        REQUIRE(map.find(-11111) == map.end());
        REQUIRE(map.find(0xFFFFFF) == map.end());
        REQUIRE(map.find(INT_MIN) == map.end());

        REQUIRE(map[1] == 5);
        REQUIRE(map[2] == 7);
        REQUIRE(map[-1] == 3);
        REQUIRE(map[6] == -32);
    }

    SECTION("Erase")
    {
        map[1] = 5;
        map[2] = 7;
        map[-1] = 3;
        map[6] = -32;

        REQUIRE(map.find(1) != map.end());
        REQUIRE(map.find(2) != map.end());
        REQUIRE(map.find(-1) != map.end());
        REQUIRE(map.find(6) != map.end());

        REQUIRE(map.find(4) == map.end());
        REQUIRE(map.find(-11111) == map.end());
        REQUIRE(map.find(0xFFFFFF) == map.end());
        REQUIRE(map.find(INT_MIN) == map.end());

        REQUIRE(map[1] == 5);
        REQUIRE(map[2] == 7);
        REQUIRE(map[-1] == 3);
        REQUIRE(map[6] == -32);

        map.erase(map.find(2));
        map.erase(map.find(6));
        map[11] = 4;

        REQUIRE(map.find(2) == map.end());
        REQUIRE(map.find(6) == map.end());


        REQUIRE(map.find(-1) != map.end());
        REQUIRE(map.find(1) != map.end());
        REQUIRE(map.find(11) != map.end());
    }

    SECTION("Random Insertions and Deletions")
    {
        constexpr int num_runs = 10;
        constexpr int num_values = k_test_max_size;

        std::uniform_int_distribution<int> key_value_dist(INT_MIN, INT_MAX);

        for (int i = 0; i < num_runs; i++)
        {
            cppcbb::cbb_vector<int> keys;

            //Fill with random keys
            for (int j = 0; j < num_values; j++)
            {
                int key = key_value_dist(GetRandom());
                while (map.find(key) != map.end())
                {
                    key = key_value_dist(GetRandom());
                }
                map[key] = key_value_dist(GetRandom());
                keys.push_back(key);
            }

            //Shuffle keys
            std::random_shuffle(keys.begin(), keys.end());

            //Remove random elements
            while (keys.size() > 0)
            {
                int key = keys.back();
                keys.pop_back();

                map.erase(map.find(key));
                REQUIRE(map.find(key) == map.end());
            }
        }
    }
}

TEST_CASE("CPPCBB Map", "[CPPCBB]")
{
    SECTION("VectorMap, Dynamic, Ordered")
    {
        cppcbb::cbb_vector_map<int, int> map;
        TestMap(map);
    }

    SECTION("VectorMap, Dynamic, Unordered")
    {
        cppcbb::cbb_unordered_vector_map<int, int> map;
        TestMap(map);
    }

    SECTION("VectorMap, Dynamic, Sorted")
    {
        cppcbb::cbb_sorted_vector_map<int, int> map;
        TestMap(map);
    }

    SECTION("VectorMap, Static, Ordered")
    {
        cppcbb::cbb_static_vector_map<int, int, k_test_max_size> map;
        TestMap(map);
    }

    SECTION("VectorMap, Static, Unordered")
    {
        cppcbb::cbb_static_unordered_vector_map<int, int, k_test_max_size> map;
        TestMap(map);
    }

    SECTION("VectorMap, Static, Sorted")
    {
        cppcbb::cbb_static_sorted_vector_map<int, int, k_test_max_size> map;
        TestMap(map);
    }
}