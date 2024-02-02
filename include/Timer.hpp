#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Constants.hpp"

class Timer {
public:
    Timer();
    ~Timer();

    void start(SDL_Renderer* renderer);
    void update();
    void render();

    bool isFinished() const; 


private:
    // Init Timer
    bool init();

    Uint32 m_startTicks;
    std::string m_timeText;
    TTF_Font* m_font;
    SDL_Color m_textColor;
    SDL_Texture* m_textTexture;
    int m_textWidth, m_textHeight;


    int m_duration; 
    bool m_finished; 


    SDL_Renderer* m_renderer;

    void updateTimeText();
};
