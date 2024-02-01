#include "Timer.hpp"
#include <iostream>
#include <sstream>

Timer::Timer() : m_startTicks(0), m_textTexture(nullptr), m_font(nullptr), m_textColor{ 255, 255, 255, 255 }, m_finished(false), m_duration(TIMERSECONDS) {
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
    m_startTicks = SDL_GetTicks(); // Record the start time
    m_finished = false; // Reset finished status
    updateTimeText(); // Initial update to set text correctly
}

void Timer::update() {
    Uint32 ticksElapsed = SDL_GetTicks() - m_startTicks; // How much time has passed
    Uint32 secondsElapsed = ticksElapsed / 1000; // Convert milliseconds to seconds
    Uint32 secondsLeft = (m_duration > secondsElapsed) ? (m_duration - secondsElapsed) : 0; // Calculate remaining time

    if (secondsLeft <= 0) {
        m_finished = true;
        secondsLeft = 0; // Prevents negative display
    }

    std::stringstream timeStream;
    timeStream << secondsLeft;
    m_timeText = "Time: " + timeStream.str(); // Update text to show remaining time

    updateTimeText(); // Update the texture for rendering
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

    m_font = TTF_OpenFont("assets/fonts/RobotoLight.ttf", 24); // Adjust font size as needed
    if (!m_font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void Timer::updateTimeText() {
    if (m_textTexture) {
        SDL_DestroyTexture(m_textTexture); // Avoid memory leak by destroying the old texture
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, m_timeText.c_str(), m_textColor);
    if (!textSurface) {
        std::cerr << "Unable to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    m_textWidth = textSurface->w;
    m_textHeight = textSurface->h;

    SDL_FreeSurface(textSurface); // Clean up the surface
}

bool Timer::isFinished() const {
    return m_finished;
}
