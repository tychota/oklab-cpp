# Oklab Color Interpolation Library

This project provides a library for color interpolation using the Oklab color space. It includes unit tests and benchmarks to ensure correctness and performance. Future optimizations may leverage NEON-specific instructions for improved performance on ARM architectures.

**Note:** the code is not proof. Don't use in production.  
I adapted it from a working (yet unproven) javascript file we wrote earlier on.  
I used GPT4 and GPT4o extensively to ease the conversion (and test conversational programming).  
I repeat, don't use it in production.

## Features

- Interpolation between colors in the Oklab color space
- Gamut mapping to ensure colors stay within valid RGB ranges
- Unit tests for verifying functionality
- Benchmarks for performance profiling

## Getting Started

### Libraries used

You do not need to install them in advanced.

- [Conan](https://conan.io/) - C/C++ package manager
- [CMake](https://cmake.org/) - Build system
- [Google Test](https://github.com/google/googletest) - Unit testing framework
- [Google Benchmark](https://github.com/google/benchmark) - Benchmarking library
- [Poetry](https://python-poetry.org/) - Python dependency management and packaging

### Building the Project

1. **Install Conan:**

   Follow the instructions on the [Conan website](https://conan.io/downloads) to install Conan, then run

   ```bash
   conan profile detect
   ```

2. **Install Poetry:**
   Follow the instructions on the [Poetry website](https://python-poetry.org/docs/#installation) to install Poetry.

3. **Install Cmake:**

   ```bash
   brew install cmake
   ```

4. Clone the repository:

   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

5. **Install Python Dependencies:**

   ```bash
   poetry install
   ```

6. Install dependencies using Conan:

   ```bash
   conan install . --build=missing
   ```

7. Configure and build the project using Conan:

   ```bash
   conan build . -s build_type=Release -b all
   ```

### Running Tests and Benchmarks

- To run the unit tests:

  ```bash
  ./build/Release/tests/oklab_tests
  ```

- To run the benchmarks:
  ```bash
  ./build/Release/benchmarks/oklab_benchmark
  ```

## Example Usage

Here's a simple example of how to use the Oklab interpolation library:

```cpp
#include "Oklab.h"
#include <iostream>

int main() {
    RGB color1 = { 255, 0, 0 }; // Red
    RGB color2 = { 0, 0, 255 }; // Blue

    float t = 0.5;
    RGB result = interpolateColor(color1, color2, t);

    std::cout << "Interpolated Color: ("
              << result[0] << ", "
              << result[1] << ", "
              << result[2] << ")" << std::endl;

    return 0;
}
```

# Future Optimizations with NEON

To enhance the performance of matrix operations, we may introduce NEON-specific optimizations.

Below is an example of how such an optimization might look for matrix-vector multiplication:

```cpp
#include <arm_neon.h>
#include "MatrixOperations.h"

Vector3 neon_multiply(const Matrix3x3& matrix, const Vector3& vec) {
    // Convert the matrix and vector to float
    float32x4_t row0 = {static_cast<float>(matrix[0][0]), static_cast<float>(matrix[0][1]), static_cast<float>(matrix[0][2]), 0};
    float32x4_t row1 = {static_cast<float>(matrix[1][0]), static_cast<float>(matrix[1][1]), static_cast<float>(matrix[1][2]), 0};
    float32x4_t row2 = {static_cast<float>(matrix[2][0]), static_cast<float>(matrix[2][1]), static_cast<float>(matrix[2][2]), 0};

    float32x4_t v = {static_cast<float>(vec[0]), static_cast<float>(vec[1]), static_cast<float>(vec[2]), 0};

    float32x4_t result0 = vmulq_f32(row0, v);
    float32x4_t result1 = vmulq_f32(row1, v);
    float32x4_t result2 = vmulq_f32(row2, v);

    Vector3 result;
    result[0] = vaddvq_f32(result0); // Sum of all elements
    result[1] = vaddvq_f32(result1);
    result[2] = vaddvq_f32(result2);

    return result;

}
```

However, for small matrix operation like this, it doesn't seems to be worth.

- micro benchmark without neon.

```
2024-06-03T17:19:02+02:00
Running ./build/Release/oklabBenchmark
Run on (12 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x12)
Load Average: 5.50, 5.43, 4.95
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
BM_InterpolateColor               162 ns          161 ns      4377325
BM_InterpolateRandomColors        289 ns          288 ns      2434131
```

- micro benchmark with NEON

```
2024-06-03T17:33:53+02:00
Running ./build/Release/oklabBenchmark
Run on (12 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x12)
Load Average: 6.17, 5.63, 5.22
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
BM_InterpolateColor               173 ns          173 ns      4046149
BM_InterpolateRandomColors        303 ns          302 ns      2313293
```

# License

This project is licensed under the MIT License - see the LICENSE file for details.
