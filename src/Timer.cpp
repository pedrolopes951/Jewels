#include "Timer.hpp"
#include <sstream>
#include <iostream>

Timer::Timer() : m_startTicks(0), m_textTexture(nullptr), m_font(nullptr), m_textColor{ 255, 255, 255, 255 } {
    if (!this->init())
        std::cerr << "Error: Timer Init Failed" << std::endl;
}

Timer::~Timer() {
    if (m_textTexture) {
        SDL_DestroyTexture(m_textTexture);
    }
    if (m_font) {
        TTF_CloseFont(m_font);
    }
}

void Timer::start(SDL_Renderer* renderer) {
    m_renderer = renderer;
    m_startTicks = SDL_GetTicks();
}

void Timer::update() {
    updateTimeText();
}

void Timer::render() {
    if (!m_textTexture) {
        return;
    }

    SDL_Rect renderQuad = { WIDTH - m_textWidth - 20, 20, m_textWidth, m_textHeight };
    SDL_RenderCopy(m_renderer, m_textTexture, nullptr, &renderQuad);
}

bool Timer::init()
{
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    m_font = TTF_OpenFont("assets/fonts/RobotoLight.ttf", 34); // Load your font
    if (!m_font) {
        return false;
        std::cerr << "Error opening font: " << TTF_GetError() << std::endl;
    }
    return true;
}

void Timer::updateTimeText() {
    Uint32 time = (SDL_GetTicks() - m_startTicks) / 1000;
    std::stringstream timeStream;
    timeStream << time;
    m_timeText = timeStream.str();

    SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, m_timeText.c_str(), m_textColor);
    if (textSurface) {
        if (m_textTexture) {
            SDL_DestroyTexture(m_textTexture);
        }
        m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
        m_textWidth = textSurface->w;
        m_textHeight = textSurface->h;
        SDL_FreeSurface(textSurface);
    }
}
