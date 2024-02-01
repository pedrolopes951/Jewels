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

    // Initialize Methods
    bool loadSprites();
   



    /*
    * EVENT METHODS
    */
    // Method to hanbdle all the SDL events, like button and quitting
    void handleEvents();
    // Mouse clicking and mouse dragging
    void handleMouseEvents(const SDL_Event& e);
    void handleSetupEvents();
    void handleGameOverEvents();


    /*
    * Renderer METHODS
    */
    void render();
    void renderSetupScreen();
    void renderGameOverScreen();
    void renderText(const std::string& text, int x, int y, SDL_Color color);


  


    /*
    * Update Game States METHODS
    */

    void update();

    void resetGame();

    /*
    * Variables 
    */


   // SDL Functions
   SDL_Window* m_window;
   SDL_Renderer* m_renderer;

   // Sprites
   Sprite m_background;
   // Map of jewel sprites
   std::map<JewelType, Sprite> m_jewelSprites;

   // 2D array to store the types of jewels inthe grid
   std::vector<std::vector<JewelType>> m_jewelGrid;

   // Input from User handler
   InputManager m_inputManager;

   // Grid Manager
   GridManager m_grid;

   // Timer
   Timer m_timer;

   // Fps Manager
   FpsManager m_fpsManager;

   // Points class
   Points m_points;

   // Game State enum 
   GameState m_currentState;

   int m_gameTime; // Total game time in seconds
   bool m_gameOver; // Flag for game over state

 

};

