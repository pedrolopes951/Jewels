#include "Game.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cstdlib>  // Include this header for std::srand
#include <ctime>    // Include this header for std::time

Game::Game() : m_window(nullptr), m_inputManager(), m_timer{}, m_currentState(GameState::Setup) {
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


    // Start Points for game
    m_points.start(m_renderer);




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

void Game::handleSetupEvents() {
    // Example setup handling
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_RETURN:
                m_currentState = GameState::Playing;
                break;
                // Add more setup handling as needed
            }
        }
    }
}

void Game::handleGameOverEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_r:
                // Restart the game
                m_currentState = GameState::Setup;
                resetGame();
                break;
            case SDLK_x:
                // Exit the game
                m_gameOver = true;
                
                break;
            }
        }
    }
}





void Game::run() {
    m_gameOver = false;


    while (!m_gameOver) {
        switch (m_currentState) {
            case GameState::Setup:
                this->handleSetupEvents();
                this->renderSetupScreen();
                break;
            case GameState::Playing:
                // Handle events
                this->handleEvents();

                // Update events for the inputmanager and the grid of jewels
                this->update();

                // Apply game logic that was updated before and render
                this->render();
                break;
            case GameState::GameOver:
                this->handleGameOverEvents();
                this->renderGameOverScreen();
                break;
        }
    }
}

void Game::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT) {
            this->clear(); // Properly cleanup and close the game
            m_gameOver = true; // Or set a flag to break the main loop
        }
        // Handle mouse click events
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            this->handleMouseEvents(e);
        }
    }
    m_gameOver = false;
}

void Game::update()
{
    // Handle user input using the InputManager
    m_inputManager.update();

    // Updating for checking for matches
    //this->checkForMatches();
    m_grid.checkForMatches();

    // After checking for matches and making all necessary updates
    int pointsThisFrame = m_grid.getPoints();
    m_points.addPoints(pointsThisFrame); // Assuming pointsClass is an instance of Points
    m_grid.resetPoints(); // Reset points in grid manager for the next frame

    // Update timer
    m_timer.update();

    // Update Fps Manage
    m_fpsManager.update();

    if (m_timer.isFinished()) {
        m_currentState = GameState::GameOver;
    }

    
}

void Game::resetGame()
{
    // Reset grid
    m_grid.initGridJewels();

    m_grid.resetPoints();

    // Reset timer with initial duration
    m_timer.start(m_renderer);

    // Reset points
    m_points.reset();

    // Optionally, reset any other game states or components as needed

    // Set the game state back to playing
    m_currentState = GameState::Playing;
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

    // Render points
    m_points.render();


    // Update the screen with any rendering performed since the previous call
    SDL_RenderPresent(m_renderer);

    // Implement a delay to cap the frame rate
    SDL_Delay(16);

}

void Game::renderSetupScreen() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Black background
    SDL_RenderClear(m_renderer);

    SDL_Color whiteColor = { 255, 255, 255, 255 }; // White color
    std::string setupText = "Press ENTER to start";
    int x = (WIDTH - 300) / 2; // Estimate or calculate based on text width
    int y = HEIGHT / 2;
    renderText(setupText, x, y, whiteColor);

    SDL_RenderPresent(m_renderer);
}

void Game::renderGameOverScreen() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Black background
    SDL_RenderClear(m_renderer);

    SDL_Color whiteColor = { 255, 255, 255, 255 }; // White color
    std::string gameOverText = "Game Over! Press R to restart or X to exit";
    int x = (WIDTH - 500) / 2; // Estimate or calculate based on text width
    int y = HEIGHT / 2;
    renderText(gameOverText, x, y, whiteColor);

    SDL_RenderPresent(m_renderer);
}

void Game::renderText(const std::string& text, int x, int y, SDL_Color color) {
    // Ensure the font is loaded
    TTF_Font* font = TTF_OpenFont("assets/fonts/RobotoLight.ttf", 24); // Adjust as needed
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    // Create a surface from the text
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        TTF_CloseFont(font);
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return;
    }

    // Set the rendering space and render to screen
    SDL_Rect renderQuad = { x, y, surface->w, surface->h };
    SDL_RenderCopy(m_renderer, texture, nullptr, &renderQuad);

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
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
