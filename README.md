# Totally Accurate Snake-Game

**It's Snake!** 🐍

> Actually this is just some test for myself to code something with
> [Raylib](https://github.com/raysan5/raylib) in raw C++.
>
> Also this Game was to cover the 30-day trial period of JetBrains
> CLion as a possible IDE. In that time I wrote this Game from scratch in
> my limited spare time to test out CLion (this is no ad or whatsoever).
> 
> At least on Linux, I can say that this is quite a powerful dev-environment 
> although I personally don't like subscriptions 🙃

## Snake
Well, it's a rather simple Snake-like clone.

So move around with "WASD", eat some food and grow along 📈.

Maybe there are some things also different...

## Dev
**Prerequisites**
- C++-17 toolchain (tested with gcc & clang)
- CMake (>= 3.6)

Build like any other CMake project
```bash
mkdir build
cmake -S . -B build
cmake --build build
```

Currently no real build targets are defined but building without specifically
setting `CMAKE_BUILD_TYPE` it won't include debug symbols.
