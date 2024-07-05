#include "benchmark/cppbenchmark.h"

#include "ColorConversions.h"
#include "../src/OkLxx.h"

#include <random>
#include <chrono>

using namespace oklab;

const auto settings = CppBenchmark::Settings().Latency(1, 1000000000, 5);

// BENCHMARK("RGB to Oklab", settings)
// {
//     RGB rgb = RGB{rand() % 256, rand() % 256, rand() % 256};

//     auto start = std::chrono::high_resolution_clock::now();

//     Oklab oklab = rgbToOklab(rgb);

//     auto stop = std::chrono::high_resolution_clock::now();

//     // Register latency metrics
//     uint64_t latency = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
//     if (latency > 0)
//     {
//         context.metrics().AddLatency(latency);
//     }
// }

BENCHMARK("P3 to RGB", settings)
{
    static auto A = std::chrono::high_resolution_clock::now();
    static auto B = std::chrono::high_resolution_clock::now();
    uint64_t resolution = std::chrono::duration_cast<std::chrono::nanoseconds>(B - A).count();

    // P3 p3 = P3{rand() % 256, rand() % 256, rand() % 256};

    P3 p3 = P3{128, 128, 128};

    auto start = std::chrono::high_resolution_clock::now();

    RGB result = p3ToRgb(p3);

    auto stop = std::chrono::high_resolution_clock::now();

    // Register latency metrics
    uint64_t latency = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start - (B - A)).count();
    if (latency > 0)
    {
        context.metrics().AddLatency(latency);
    }
}

BENCHMARK_MAIN()