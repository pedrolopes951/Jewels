#pragma once
#include <vector>
#include <map>
#include "Constants.hpp"
#include <SDL.h>
#include "Sprite.hpp"
#include <iostream>
#include <algorithm>
#include "InputManager.hpp"



class GridManager {
public:
    GridManager();
    ~GridManager();
    bool loadSprites(SDL_Window* window, SDL_Renderer* renderer);
    void handleClickedJewel(const JewelPos& jewelpos, InputManager& input_manager);
    void initGridJewels();
    void renderJewelGrid(SDL_Renderer* renderer,  InputManager& input_manager);
    void checkForMatches();
    void resetPoints();
    const int& getPoints() const;

private:

    void checkAndRemoveMatches();
    bool checkPotentialMatchAt(int y, int x);
    void applyGravity();
    JewelPos snapToGrid(int x, int y, const JewelPos& original_pos);
    void swapJewels(const JewelPos& posA, const JewelPos& posB);
    bool willSwapMatch(const JewelPos& posA, const JewelPos& posB);


    bool m_initialcheck;
    int m_pointsPerMatch;

    bool m_swapPerformed;

    // Map of jewel sprites
    std::map<JewelType, Sprite> m_jewelSprites;

    // 2D array to store the types of jewels inthe grid
    std::vector<std::vector<JewelType>> m_jewelGrid;

    bool isMatch(JewelType jewelType, int row, int col);
};
