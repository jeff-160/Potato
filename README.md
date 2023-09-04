# Potato Visual Novel Framework
#### Big thanks to [zhogrog](https://github.com/GaoZR2008) for christening this

## Requirements
1. GNU g++
2. python 3.11 (idk as long as it has the json module)
3. The blood of an infant
---

## Project setup
A complete Potato project will be structured as such.
```
[Root Directory]/
    |
    ├── .cpp project file
    |
    ├── .exe game file (generated on compilation)
    |
    ├── SDL .dll files
    |
    ├── SDL2/
    |   └── SDL .h files
    |
    ├── lib/
    |   └── SDL libraries
    |
    ├── default/
    |   ├── default framework settings
    |   └── default framework assets
    |
    ├── src/
    |   └── Potato.hpp source files
    |
    ├── Potato.hpp (main framework file, include in project file)
    |
    ├── build.bat (sets up project folder)
    |
    └── compile.ps1 (compiles and runs project)
```
