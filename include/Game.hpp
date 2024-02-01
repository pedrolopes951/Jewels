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
    void initGridJewels();
   


    /*
    * EVENT METHODS
    */
    // Method to hanbdle all the SDL events, like button and quitting
    bool handleEvents();
    // Mouse clicking and mouse dragging
    void handleMouseEvents(const SDL_Event& e);


    /*
    * Renderer METHODS
    */
    void render();
    void renderJewelGrid();

  


    /*
    * Update Game States METHODS
    */

    void update();

    // Check for jewels that have matching pattern
    void checkAndRemoveMatches();

    // Check if any matches exist in the grid
    void checkForMatches();

    // check temporaly if the swap is valid
    bool willSwapMatch(const JewelPos& posA, const JewelPos& posB);

    bool checkPotentialMatchAt(int x, int y);

    // Apply Gravity to the jewels after matching to fall down;
    void applyGravity();


    // Check if there is a match for more than 3 jewels
    bool isMatch(JewelType jewelType, int row, int col);

    // Convert the visual position of the jewl to grid coordinates
    JewelPos snapToGrid(int x, int y);

    // Swap the Position of the two jewels in the jewel grid
    void swapJewels(const JewelPos& pos1, const JewelPos& pos2);


    /*
    * Variables 
    */

    bool m_swapPerformed{false};

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


};

