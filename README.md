# microhil [![AppVeyor Build Status][appveyor-build-status-svg]][appveyor-build-status] [![Travis CI Build Status][travis-build-status-svg]][travis-build-status]
Parses a subset of the [Hashicorp Interpolation Language](https://www.github.com/hashicorp/hil).

Only identifiers and function calls are supported.

## Requirements
- C++14-compliant compiler
- CMake >= 2.8.12

## Usage
```c++
std::ifstream ifs("foo.hil");
hil::ParseResult parseResult = hil::parse(ifs);

if (!parseResult.valid()) {
    std::cout << pr.errorReason << std::endl;
    return;
}
```

## Running the tests
```
mkdir out/Debug
cd out/Debug
cmake ../../tests
make
./test_runner
```

<!-- Badges -->
[appveyor-build-status]: https://ci.appveyor.com/project/Ruin0x11/microhil/branch/master
[appveyor-build-status-svg]: https://ci.appveyor.com/api/projects/status/fi2su01yo2eah7wf/branch/master?svg=true
[travis-build-status]: https://travis-ci.org/Ruin0x11/microhil?branch=master
[travis-build-status-svg]: https://travis-ci.org/Ruin0x11/microhil.svg?branch=master
