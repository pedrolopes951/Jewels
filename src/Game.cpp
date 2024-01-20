#include "Game.hpp"
#include "Constants.hpp"
#include <iostream>

Game::Game() : m_window(nullptr) {
}

Game::~Game() {
}

bool Game::init() {

    // SDL initialization, window creation, etc.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Handle initialization failure
        return false;
    }

    m_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!m_window) {
        // Handle window creation failure
        SDL_Quit();
        return false;
    }


    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // Load background image using the Sprite class
    if (!m_background.loadFromFile(m_renderer, "assets/images/Backdrop13.jpg")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // Other initialization code

    return true;
}

void Game::run() {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the renderer, prepares for he renderer for drawing the next frame
        SDL_RenderClear(m_renderer);

        // Render background using the Sprite class
        m_background.render(m_renderer, 0, 0, WIDTH, HEIGHT);

        // Other rendering code

        // Present the rendered frame to the screen
        SDL_RenderPresent(m_renderer);
    }
}

void Game::clear() {
    // Cleanup code (e.g., destroy window, quit SDL)
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
