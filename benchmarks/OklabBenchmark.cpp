#include <benchmark/benchmark.h>
#include "../src/Oklab.h"

static void BM_InterpolateColor(benchmark::State &state)
{
    RGB color1 = {255, 0, 0}; // Red
    RGB color2 = {0, 0, 255}; // Blue
    float t = 0.5;
    for (auto _ : state)
    {
        RGB result = interpolateColor(color1, color2, t);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_InterpolateColor);

static void BM_InterpolateRandomColors(benchmark::State &state)
{
    for (auto _ : state)
    {
        RGB color1 = {rand() % 256, rand() % 256, rand() % 256};
        RGB color2 = {rand() % 256, rand() % 256, rand() % 256};
        float t = static_cast<float>(rand()) / RAND_MAX;
        RGB result = interpolateColor(color1, color2, t);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_InterpolateRandomColors);

BENCHMARK_MAIN();