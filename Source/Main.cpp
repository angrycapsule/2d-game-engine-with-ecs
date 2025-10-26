#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cstdio>
#include <string>
#include "sol/sol.hpp"
#include "lua/src/lua.h"

static void log_sdl(const char* tag, const char* err) 
{
    std::fprintf(stderr, "[%s] %s\n", tag, err);
}

void test_lua() 
{
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

    lua.script(R"(
        function add(a, b) return a + b end
    )");

    int result = lua["add"](40, 2);
    std::printf("Lua says: 40 + 2 = %d\n", result);
}

int main(int, char**)
{
    test_lua();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::printf("SDL_image init error: %s\n", IMG_GetError());
    }

    if (TTF_Init() != 0) {
        std::printf("SDL_ttf init error: %s\n", TTF_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
        std::printf("SDL_mixer init error: %s\n", Mix_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 addons OK",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderClear(renderer);
    SDL_Rect rect{ 200, 150, 400, 300 };
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}