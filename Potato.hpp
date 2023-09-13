// O------------------------------------------------------------------------------O
// | if you steal any of this code without crediting youre a nigger               |
// O------------------------------------------------------------------------------O

#pragma once

#ifndef POTATO_HEADERS
#define POTATO_HEADERS

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

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

    void Init(){
        TTF_Init();
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Init(SDL_INIT_AUDIO);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    }
}

#include "src/System.hpp"
#include "src/Threading.hpp"
#include "src/Character.hpp"
#include "src/UI.hpp"
#include "src/Choice.hpp"
#include "src/Scene.hpp"
#include "src/Engine.hpp"
#include "src/Effects.hpp"
#include "src/Transitions.hpp"
#include "src/PostFD.hpp"
#include "src/Audio.hpp"

// fucky wucky main signature replace
#define main() main(int argc, char* argv[])

#endif