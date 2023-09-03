#pragma once

#ifndef POTATO_HEADERS
#define POTATO_HEADERS

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <optional>
#include <map>
#include <vector>
#include <tuple>
#include <functional>
#include <algorithm>
#include <chrono>
#include <thread>

namespace Potato{
    class Engine;
    Engine* CurrentEngine = nullptr;
}

#include "src/System.hpp"
#include "src/Character.hpp"
#include "src/UI.hpp"
#include "src/Engine.hpp"
#include "src/PostFD.hpp"
#include "src/Choices.hpp"
#include "src/VFX.hpp"

// fucky wucky main signature replace
#define main() main(int argc, char* argv[])

#endif