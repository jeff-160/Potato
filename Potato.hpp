#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>

#include "src/Engine.hpp"
#include "src/Character.hpp"
#include "src/Effects.hpp"
#include "src/Choices.hpp"

// fucky wucky main signature replace
#define main() main(int argc, char* argv[])