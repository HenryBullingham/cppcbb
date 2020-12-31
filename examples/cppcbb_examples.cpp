//Copyright(C) 2020 Henry Bullingham
//This file is subject to the license terms in the LICENSE file
//found in the top - level directory of this distribution.

#include "cppcbb/cbb_vector.hpp"

#include "cppcbb/cbb_map.hpp"

#include <chrono>
#include <cstdio>
#include <cmath>
#include <numeric>
#include <utility>

template<typename Func>
float measure_time(Func&& func)
{
    auto start = std::chrono::high_resolution_clock::now();

    func();

    auto end = std::chrono::high_resolution_clock::now();

    return (float)(end - start).count() / 1000000000.0f;
}

template<typename Vector>
void run_sample(int seed, int num_values = 1000)
{
    Vector v;
    srand(seed);

    // Add a bunch of random elements
    for (int i = 0; i < num_values; i++)
    {
        v.push_back(rand());
    }

    //Remove them from random indices
    while (v.size() > 0)
    {
        int index = (int)((rand() / (float)RAND_MAX) * v.size());
        v.erase(v.begin() + index);
    }
}

/// <summary>
/// 
/// </summary>
/// <typeparam name="Vector"></typeparam>
/// <param name="seed"></param>
/// <param name="num_values"></param>
/// <param name="num_iterations"></param>
/// <returns>average time and std dev of times</returns>
template<typename Vector>
std::pair<float,float> average_runtime(int seed, int num_values = 1000, int num_iterations = 10)
{
    cppcbb::cbb_vector<float> runtimes;

    for (int i = 0; i < num_iterations; i++)
    {
        runtimes.push_back(measure_time([=]()
        {
            run_sample<Vector>(seed, num_values);
        }));
    }
    float average = std::accumulate(runtimes.begin(), runtimes.end(), 0.0f) / (float)runtimes.size();
    float std_dev = sqrtf(
        (float)std::transform_reduce(runtimes.begin(), runtimes.end(), 0.0f, std::plus<float>(), [=](float x) { return (x - average) * (x - average); })
    );

    return std::make_pair(average, std_dev);
}

int main(int argc, char** argv)
{
    int seed = (int) std::chrono::high_resolution_clock::now().time_since_epoch().count();
    const int num_values = 1000;
    int num_runs = 10;

    printf("CPPCBB Example Program: \n\n\n");

    auto regular_vector = average_runtime<cppcbb::cbb_vector<int>>(seed, num_values, num_runs);
    printf("Regular Vector:              Time %10f s StdDev %10f s\n\n", regular_vector.first, regular_vector.second);

    auto static_vector = average_runtime<cppcbb::cbb_static_vector<int, num_values>>(seed, num_values, num_runs);
    printf("Static Vector:               Time %10f s StdDev %10f s\n\n", static_vector.first, static_vector.second);

    auto unordered_vector = average_runtime<cppcbb::cbb_unordered_vector<int>>(seed, num_values, num_runs);
    printf("Unordered Vector:            Time %10f s StdDev %10f s\n\n", unordered_vector.first, unordered_vector.second);

    auto static_u_vector = average_runtime<cppcbb::cbb_static_unordered_vector<int, num_values>>(seed, num_values, num_runs);
    printf("Static Unordered Vector:     Time %10f s StdDev %10f s\n\n", static_u_vector.first, static_u_vector.second);

    return 0;
}