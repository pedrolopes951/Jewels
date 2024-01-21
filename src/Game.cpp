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

    // Load background image using the Sprite class and jewels in to the sprite
    if(!this->loadSprites())
        return false;

    // Init the Grid of jewels
    this->initGridJewels();

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

        // Render the 8x8 jewel grid
        renderJewelGrid();


        // Present the rendered frame to the screen
        SDL_RenderPresent(m_renderer);
    }
}

void Game::clear() {
    // Cleanup code (e.g., destroy window, quit SDL)
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Game::loadSprites()
{
    // Background Sprite
    if (!m_background.loadFromFile(m_renderer, "assets/images/Backdrop13.jpg")) {
        std::cerr << "Failed to load jewel asset (" << "assets/images/Backdrop13.jpg" << "): " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // Load each jewel into the map
    // Black jewel
    if (!m_jewelSprites[JewelType::BLACK].loadFromFile(m_renderer, "assets/images/Color-1.png")) {
        std::cerr << "Failed to load jewel asset (" << "assets/images/Color-1.png" << "): " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    // Silver jewel
    if (!m_jewelSprites[JewelType::SILVER].loadFromFile(m_renderer, "assets/images/Color-2.png")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    // Pink jewel
    if (!m_jewelSprites[JewelType::SILVER].loadFromFile(m_renderer, "assets/images/Color-3.png")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    // Blue jewel
    if (!m_jewelSprites[JewelType::PINK].loadFromFile(m_renderer, "assets/images/Color-4.png")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    // Orange jewel
    if (!m_jewelSprites[JewelType::BLUE].loadFromFile(m_renderer, "assets/images/Color-5.png")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    return true;
}

void Game::renderJewelGrid()
{
    //Set the grid in the center of the window

    int offsetx = (WIDTH - GRIDX * JEWELSIZEX) / 2;
    int offsety = (HEIGHT - GRIDY * JEWELSIZEY) / 2;

    for (int row = 0; row < GRIDX; row++) {
        for (int col = 0; col < GRIDY; col++) {
            int x = offsetx + col * JEWELSIZEX;
            int y = offsety + row * JEWELSIZEY;

            // For simplicity, alternate between different jewel types
            JewelType jewelType = m_jewelGrid[row][col];;

            // Render the jewel sprite at the calculated position
            m_jewelSprites[jewelType].render(m_renderer, x, y, JEWELSIZEX, JEWELSIZEY);
        }
    }
}

void Game::initGridJewels()
{
    // Initialize the 2D vector for storing jewel types
    m_jewelGrid.resize(GRIDY, std::vector<JewelType>(GRIDX, JewelType::BLACK));

    // Randomly assign a jewel type to each cell in the grid during initialization
    for (int row = 0; row < GRIDX; row++) {
        for (int col = 0; col < GRIDY; col++) {
            m_jewelGrid[row][col] = static_cast<JewelType>(rand() % 4);
        }
    }
}
