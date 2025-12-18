#pragma once
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
class Window{
public:
    Window();
private:
    SDL_Window* window;
    SDL_Surface* windowSurface;
    bool quit = false;
};