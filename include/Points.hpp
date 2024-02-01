#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "Constants.hpp"

class Points {
public:
    Points();
    ~Points();

    // Initialize the Points system
    bool init();

    // Start the points system with the SDL_Renderer
    void start(SDL_Renderer* renderer);

    // Add points and update the points display
    void addPoints(int count);

    // Render the points display
    void render();

    // Reset the points to 0
    void reset();

    const int& getPoints() const;

private:
    // Update the texture used for displaying points
    void updatePointsText();

    int m_points; // Current points
    SDL_Color m_color; // Color of the points text
    TTF_Font* m_font; // Font used for the points text
    SDL_Texture* m_pointsTexture; // Texture for the points display
    SDL_Renderer* m_renderer; // Renderer to draw the points
    int m_pointsWidth; // Width of the points text texture
    int m_pointsHeight; // Height of the points text texture
};
