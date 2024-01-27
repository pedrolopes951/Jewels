#pragma once
#include <SDL.h>
#include <memory>
#include <map>
#include "Sprite.hpp"
#include <vector>
#include "InputManager.hpp"

enum class JewelType {
    EMPTY,
    BLACK,
    SILVER,
    PINK,
    BLUE,
    ORANGE
    
};



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

    // Load Textures and create Sprites into the sprites
    bool loadSprites();

    // Function to render the Grid of Jewels
    void renderJewelGrid();

    //Initialize the jewel Grid
    void initGridJewels();

    // Check for jewels that have matching pattern
    void checkAndRemoveMatches();

    // Check if any matches exist in the grid
    void checkForMatches();

    // check temporaly if the swap is valid
    bool willSwapMatch(const JewelPos& posA, const JewelPos& posB);

    bool checkPotentialMatchAt(int x, int y);

    // Apply Gravity to the jewels after matching to fall down;

    void applyGravity();

    // After Matching fill the empty spaces
    void fillEmptySpaces();

    // Check if there is a match for more than 3 jewels
    bool isMatch(JewelType jewelType, int row, int col);

    // Convert the visual position of the jewl to grid coordinates
    JewelPos snapToGrid(int x, int y);

    // Swap the Position of the two jewels in the jewel grid
    void swapJewels(const JewelPos& pos1, const JewelPos& pos2);

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

  


};

