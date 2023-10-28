#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <string>
using namespace std;

#include "utils/get_assets_path.h"

int main() {
  string imagePath = getAssetPath("backgrounds/initial_screen.jpg");
  string HeadingFontPath = getAssetPath("fonts/Cabin-Bold.ttf");

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  if (TTF_Init() != 0) {
    SDL_Log("Unable to initialize SDL2_ttf: %s", TTF_GetError());
    SDL_Quit();
    return 1;
  }

  if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG) {
    SDL_Log("Unable to initialize SDL2_image: %s", IMG_GetError());
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Black Jack", 0, 0, 1000, 800, SDL_WINDOW_SHOWN);
  if (!window) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_Log("Unable to create renderer : %s", SDL_GetError());
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  SDL_Texture *imageTexture = IMG_LoadTexture(renderer, imagePath.c_str());
  if (!imageTexture) {
    SDL_Log("Unable to load image: %s", IMG_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  TTF_Font *font = TTF_OpenFont(HeadingFontPath.c_str(), 50);
  if (!font) {
    SDL_Log("Unable to load font: %s", TTF_GetError());
  }

  SDL_Color textColor = {255, 255, 255, 0};
  int fadeDuration = 3000;
  auto startTime = chrono::steady_clock::now();

  SDL_Surface *textSurface =
      TTF_RenderText_Solid_Wrapped(font, "Black Jack", textColor, 0);
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);

  // Main loop
  bool quit = false;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imageTexture, NULL, NULL);
    SDL_Rect textRect = {350, 100, textSurface->w, textSurface->h};

    auto currentTime = chrono::steady_clock::now();
    auto elapsed =
        chrono::duration_cast<chrono::milliseconds>(currentTime - startTime)
            .count();
    if (elapsed <= fadeDuration) {
      int alpha = static_cast<int>(255.0 * elapsed / fadeDuration);
      textColor.a = alpha;
      SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }

    SDL_RenderPresent(renderer);
  }

  // Cleanup and quit
  TTF_CloseFont(font);
  SDL_DestroyTexture(imageTexture);
  SDL_DestroyTexture(textTexture);
  SDL_FreeSurface(textSurface);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
  return 0;
}
