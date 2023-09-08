#pragma once

#ifndef POTATO_HEADERS
#define POTATO_HEADERS

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_audio.h"

#include <windows.h>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <string>
#include <optional>
#include <variant>
#include <map>
#include <vector>
#include <tuple>
#include <functional>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>

namespace Potato{
    class Engine;
    Engine* CurrentEngine = nullptr;
}

#include "src/System.hpp"
#include "src/Threading.hpp"
#include "src/Character.hpp"
#include "src/UI.hpp"
#include "src/Scene.hpp"
#include "src/Engine.hpp"
#include "src/Effects.hpp"
#include "src/Transitions.hpp"
#include "src/PostFD.hpp"
#include "src/Audio.hpp"
#include "src/Choices.hpp"

// fucky wucky main signature replace
#define main() main(int argc, char* argv[])

#endif