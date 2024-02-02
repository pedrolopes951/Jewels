#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include "Constants.hpp"

class FpsManager {
public:
    FpsManager();
    ~FpsManager();

    void start(SDL_Renderer* renderer);
    void update();
    void render();

private:
    bool init();


    Uint32 m_lastTime;
    int m_frameCount;
    double m_fps;
    SDL_Color m_fpsTextColor;
    SDL_Texture* m_fpsTexture;
    int m_fpsTextWidth, m_fpsTextHeight;
    TTF_Font* m_font;
    std::string m_fpsText;

    SDL_Renderer* m_renderer;


    void updateFPSText();
};
