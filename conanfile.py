from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class OklabRecipe(ConanFile):
    name = "oklab"
    version = "0.1.0"
    package_type = "application"

    # Optional metadata
    license = "MIT"
    author = "Tycho Tatitscheff <tychot@bam.tech>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of foo package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "tests/*", "benchmarks/*"

    def requirements(self):
        self.requires("taywee-args/6.4.6")
        self.requires("gtest/1.14.0")
        # self.requires("benchmark/1.8.4")
        self.requires("cppbenchmark/cci.20201029")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
