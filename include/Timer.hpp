#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Constants.hpp"

class Timer {
public:
    Timer();
    ~Timer();

    void start();
    void update(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);

private:
    // Init Timer
    bool init();

    Uint32 m_startTicks;
    std::string m_timeText;
    TTF_Font* m_font;
    SDL_Color m_textColor;
    SDL_Texture* m_textTexture;
    int m_textWidth, m_textHeight;

    void updateTimeText(SDL_Renderer* renderer);
};
