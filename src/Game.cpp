#include "Game.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cstdlib>  // Include this header for std::srand
#include <ctime>    // Include this header for std::time

Game::Game() : m_window(nullptr), m_inputManager(), m_timer{} {
    // Ensure that I have seed different for every rand()
    std::srand(static_cast<unsigned>(std::time(nullptr)));

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
    if (!this->loadSprites())
        return false;

    // Init the Grid of jewels
    m_grid.initGridJewels();
   
    // Start timer of game
    m_timer.start(m_renderer);

    // Start Fps for game
    m_fpsManager.start(m_renderer);


    // Other initialization code

    return true;
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

    m_grid.loadSprites(m_window, m_renderer);
   
    return true;
}




void Game::run() {
    bool quit = false;

    while (!quit) {
        // Handle events
        quit = this->handleEvents();

        // Update events for the inputmanager and the grid of jewels
        this->update();

        // Apply game logic that was updated before and render
        this->render();

    }
}

bool Game::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            return  true;
        }
        // Handle mouse click events
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            this->handleMouseEvents(e);
        }
    }
    return false;
}

void Game::update()
{
    // Handle user input using the InputManager
    m_inputManager.update();

    // Updating for checking for matches
    //this->checkForMatches();
    m_grid.checkForMatches();

    // Update timer
    m_timer.update();

    // Update Fps Manage
    m_fpsManager.update();

    
}

void Game::render()
{
    // Clear the renderer, prepares it for drawing the next frame
    SDL_RenderClear(m_renderer);

    // Render background
    m_background.render(m_renderer, 0, 0, WIDTH, HEIGHT);

    // Render the jewel grid
    m_grid.renderJewelGrid(m_renderer, m_inputManager);

    // Render the timer
    m_timer.render();

    m_fpsManager.render();


    // Update the screen with any rendering performed since the previous call
    SDL_RenderPresent(m_renderer);

    // Implement a delay to cap the frame rate
    SDL_Delay(16);

}



void Game::handleMouseEvents(const SDL_Event& e)
{
    int mouseX = e.button.x;
    int mouseY = e.button.y;


    // Convert mouse position to grid position
    JewelPos clickPos = { (mouseX - OFFSETX) / JEWELSIZEX, (mouseY - OFFSETY) / JEWELSIZEY };

    // Handle Clicked position in the grid
    m_grid.handleClickedJewel(clickPos, m_inputManager);

}


void Game::clear() {
    // Cleanup code (e.g., destroy window, quit SDL)
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
