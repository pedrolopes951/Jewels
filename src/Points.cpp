#include "Points.hpp"
#include <sstream>
#include <iostream>

Points::Points() : m_points(0), m_pointsTexture(nullptr), m_font(nullptr), m_color({ ColorText::WHITE, ColorText::WHITE, ColorText::WHITE, ColorText::WHITE }) {
    if (!this->init())
        std::cerr << "Error: Points Init Failed" << std::endl;
}

Points::~Points() {
    if (m_pointsTexture) {
        SDL_DestroyTexture(m_pointsTexture);
    }
    if (m_font) {
        TTF_CloseFont(m_font);
    }
}

bool Points::init() {
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    m_font = TTF_OpenFont("assets/fonts/RobotoLight.ttf", SIZETEXT); 
    if (!m_font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void Points::start(SDL_Renderer* renderer) {
    m_renderer = renderer;
}

void Points::addPoints(int count) {
    m_points += count; 
    this->updatePointsText(); 
}

void Points::updatePointsText() {
    std::stringstream pointsStream;
    pointsStream << "Points: " << m_points;
    std::string pointsText = pointsStream.str();

    SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, pointsText.c_str(), m_color);
    if (textSurface) {
        if (m_pointsTexture) {
            SDL_DestroyTexture(m_pointsTexture);
        }
        m_pointsTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
        m_pointsWidth = textSurface->w;
        m_pointsHeight = textSurface->h;
        SDL_FreeSurface(textSurface);
    }
}

void Points::render() {
    if (!m_pointsTexture) {
        return;
    }

    int totalGridWidth = GRIDX * JEWELSIZEX;

    // Position the points display centered relative to the grid's position
    int x = OFFSETX + (totalGridWidth - m_pointsWidth) / 2; // Center above the grid
    int y = OFFSETY - m_pointsHeight - 20; // Place above the grid with some vertical offset

    SDL_Rect renderQuad = { x, y, m_pointsWidth, m_pointsHeight };
    SDL_RenderCopy(m_renderer, m_pointsTexture, nullptr, &renderQuad);
}


void Points::reset() {
    m_points = 0;
    updatePointsText(); 
}

const int& Points::getPoints() const
{
    return m_points;
}
