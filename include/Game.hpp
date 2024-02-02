#pragma once
#include <SDL.h>
#include <memory>
#include <map>
#include "Sprite.hpp"
#include <vector>
#include "InputManager.hpp"
#include "Timer.hpp"
#include "FpsManager.hpp"
#include "GridManager.hpp"
#include "Points.hpp"



class Game {
public:
    Game();
    ~Game();

    // Initialize the game
    bool init();

    // Run the game loop
    void run();

    // Cleanup resources
    void clear();

private:
    // Load game assets
    bool loadSprites();

    // Handle SDL events
    void handleEvents();
    // Handle mouse events
    void handleMouseEvents(const SDL_Event& e);
    // Handle setup events
    void handleSetupEvents();
    // Handle game over events
    void handleGameOverEvents();

    // Render game objects
    void render();
    // Render setup screen
    void renderSetupScreen();
    // Render game over screen
    void renderGameOverScreen();
    // Render text
    void renderText(const std::string& text, int x, int y, SDL_Color color);

    // Update game logic
    void update();
    // Reset game to initial state
    void resetGame();

    // SDL components
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    // Game components
    Sprite m_background;
    std::map<JewelType, Sprite> m_jewelSprites;
    std::vector<std::vector<JewelType>> m_jewelGrid;
    InputManager m_inputManager;
    GridManager m_grid;
    Timer m_timer;
    FpsManager m_fpsManager;
    Points m_points;

    // Game state management
    GameState m_currentState;
    int m_gameTime; // Game duration in seconds
    bool m_gameOver; // Game over flag
};

