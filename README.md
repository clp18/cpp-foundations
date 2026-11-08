# cpp-foundations
Modern C++20 fundamentals, built test-first: CMake presets, GoogleTest,
ASan/UBSan, and clang-tidy, following the Google C++ Style Guide.

[![CI](https://github.com/clp18/cpp-foundations/actions/workflows/ci.yaml/badge.svg)](https://github.com/clp18/cpp-foundations/actions/workflows/ci.yaml)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
![License: MIT](https://img.shields.io/badge/license-MIT-green)


## Features
- Header-only library (`include/cpp_foundations/`)
- CMake presets: `debug`, `release`, `asan` (ASan + UBSan), `coverage`
- GoogleTest via CMake `FetchContent`
- CI on Linux (GCC) and macOS (Apple Clang)
- Google style enforced with `.clang-format` and `.clang-tidy`

## Requirements
- CMake 3.25+
- Ninja
- A C++20 compiler (GCC or Clang)
- Optional: LLVM (`brew install llvm`) for `clang-format`, `clang-tidy`, `clangd`

## Getting Started
```sh
git clone https://github.com/clp18/cpp-foundations.git
cd cpp-foundations
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Replace `debug` with `asan` to build and test with sanitizers.

## Project Structure
```
include/cpp_foundations/   Public headers
tests/                     GoogleTest unit tests
```

## Code Style
This project follows the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

```sh
# Format
git ls-files -co --exclude-standard '*.cpp' '*.hpp' | xargs clang-format -i
# Lint (needs a configured build/debug)
run-clang-tidy -p build/debug -quiet "$PWD/(tests|src|include)/"
```
## Documentation
- [ADR-001: Message Codec Design](docs/ADR/ADR-001.md)
- [Pitch Tracker Specification](docs/Specs/Pitch-Tracker.md)

## License
MIT. See [LICENSE](LICENSE).