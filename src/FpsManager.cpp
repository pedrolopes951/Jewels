#include "FpsManager.hpp"
#include <iostream>

FpsManager::FpsManager() : m_lastTime(0), m_frameCount(0), m_fps(0.0), m_fpsTexture(nullptr), m_font(nullptr), m_fpsTextColor{ ColorText::WHITE, ColorText::WHITE, ColorText::WHITE, ColorText::WHITE } {
    if (!this->init())
        std::cerr << "Error: FpsManager Init Failed" << std::endl;
}

FpsManager::~FpsManager() {
    if (m_fpsTexture) {
        SDL_DestroyTexture(m_fpsTexture);
    }
    if (m_font) {
        TTF_CloseFont(m_font);
    }
}

void FpsManager::start(SDL_Renderer* renderer) {
    m_renderer = renderer;
    m_lastTime = SDL_GetTicks();
}

void FpsManager::update() {
    updateFPSText();
}

void FpsManager::render() {
    if (!m_fpsTexture) {
        return;
    }

    SDL_Rect renderQuad = { 20, 20, m_fpsTextWidth, m_fpsTextHeight };
    SDL_RenderCopy(m_renderer, m_fpsTexture, nullptr, &renderQuad);
}

bool FpsManager::init() {
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    m_font = TTF_OpenFont("assets/fonts/RobotoLight.ttf", SIZETEXT); 
    if (!m_font) {
        std::cerr << "Error opening font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void FpsManager::updateFPSText() {
    Uint32 currentTime = SDL_GetTicks();
    m_frameCount++;
    if (currentTime - m_lastTime >= 1000) {
        m_fps = m_frameCount / ((currentTime - m_lastTime) / 1000.0);
        m_frameCount = 0;
        m_lastTime = currentTime;

        std::stringstream fpsStream;
        fpsStream.precision(2);
        fpsStream << std::fixed << "FPS: " << m_fps;
        m_fpsText = fpsStream.str();

        SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, m_fpsText.c_str(), m_fpsTextColor);
        if (textSurface) {
            if (m_fpsTexture) {
                SDL_DestroyTexture(m_fpsTexture);
            }
            m_fpsTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
            m_fpsTextWidth = textSurface->w;
            m_fpsTextHeight = textSurface->h;
            SDL_FreeSurface(textSurface);
        }
    }
}
