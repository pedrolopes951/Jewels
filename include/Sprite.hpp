#pragma once
#include <string>
#include <SDL.h>

class Sprite {
public:
    Sprite();
    ~Sprite();

    bool loadFromFile(SDL_Renderer* renderer, const std::string& path);
    void render(SDL_Renderer* renderer, int x, int y, int width, int height);

private:
    SDL_Texture* m_texture;
};
