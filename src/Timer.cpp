#include "Timer.hpp"
#include <iostream>
#include <sstream>

Timer::Timer() : m_startTicks(0), m_textTexture(nullptr), m_font(nullptr), m_textColor{ ColorText::WHITE, ColorText::WHITE, ColorText::WHITE, ColorText::WHITE }, m_finished(false), m_duration(TIMERSECONDS) {
    if (!this->init()) {
        std::cerr << "Error: Timer Init Failed" << std::endl;
    }
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
    m_finished = false; 
}

void Timer::update() {
    Uint32 ticksElapsed = SDL_GetTicks() - m_startTicks;
    Uint32 secondsElapsed = ticksElapsed / 1000; 
    Uint32 secondsLeft = (m_duration > secondsElapsed) ? (m_duration - secondsElapsed) : 0; 

    if (secondsLeft <= 0) {
        m_finished = true;
        secondsLeft = 0; 
    }

    std::stringstream timeStream;
    timeStream << secondsLeft;
    m_timeText = "Time: " + timeStream.str(); 

    this->updateTimeText(); 
}

void Timer::render() {
    if (!m_textTexture) return;

    SDL_Rect renderQuad = { WIDTH - m_textWidth - 20, 20, m_textWidth, m_textHeight };
    SDL_RenderCopy(m_renderer, m_textTexture, nullptr, &renderQuad);
}

bool Timer::init() {
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

void Timer::updateTimeText() {
    if (m_textTexture) {
        SDL_DestroyTexture(m_textTexture); 
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, m_timeText.c_str(), m_textColor);
    if (!textSurface) {
        std::cerr << "Unable to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    m_textWidth = textSurface->w;
    m_textHeight = textSurface->h;

    SDL_FreeSurface(textSurface); 
}

bool Timer::isFinished() const {
    return m_finished;
}
