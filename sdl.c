#include <stdio.h>
#include <SDL3/SDL.h>

int SDL_Fail() {
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return -1;
}

int SDL_AppInit(void **appstate, int argc, char *argv[]) {
    // init the library, here we make a window so we only need the Video
    // capabilities.
    if (SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_Fail();
    }

    // create a window
    SDL_Window *window =
        SDL_CreateWindow("Window", 352, 430, SDL_WINDOW_RESIZABLE);
    if (!window) {
        return SDL_Fail();
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL, 0);
    if (!renderer) {
        return SDL_Fail();
    }

    // print some information about the window
    SDL_ShowWindow(window);
    {
        int width, height, bbwidth, bbheight;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowSizeInPixels(window, &bbwidth, &bbheight);
        SDL_Log("Window size: %ix%i", width, height);
        SDL_Log("Backbuffer size: %ix%i", bbwidth, bbheight);
        if (width != bbwidth) {
            SDL_Log("This is a highdpi environment.");
        }
    }

    SDL_Log("Application started successfully!");

    return 0;
}
