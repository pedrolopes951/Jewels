#pragma once
#include <SDL.h>
#include <memory>

#include "Sprite.hpp"

class Game
{
public:
    Game();
    ~Game();

    // Initialize the Window
    bool init();

    // run the game loop
    void run();
    // Clear window
    void clear();

private:
   SDL_Window* m_window;
   SDL_Renderer* m_renderer;
   Sprite m_background;

 
};

