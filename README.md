# Cura Formulae Engine

Cura Formulae Engine is a library designed to parse and evaluate Cura setting-expressions. These (python-like) expressions can be written to express complex logic for 3D printing settings and internal dependencies between settings.

An example expression could be:

```python
min_layer_time if min_layer_time > 0 else 0
```

## Build

Building Cura Formulae Engine requires Conan and CMake. You can use the following commands to build the project:

```shell
conan install . --build=missing --update
source build/Release/generators/conanbuild.sh
cmake --preset conan-release
cmake --build --preset conan-release
```

Some additional CMake presets are available for different build configurations. You can specify these configurations by appending the following options to the `conan install` command:
- tests: `-c tools.build:skip_test=False`
- extensive warnings: `-o enable_extensive_warnings="True"`
- with applications: `-o with_apps="True"`
