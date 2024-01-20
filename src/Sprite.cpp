#include "Sprite.hpp"
#include <SDL_image.h>

Sprite::Sprite() : m_texture(nullptr) {
    // Constructor implementation
}

Sprite::~Sprite() {
    // Destructor implementation
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

bool Sprite::loadFromFile(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        return false;
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (m_texture != nullptr)
        return true;
}

void Sprite::render(SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_Rect destRect = { x, y, width, height };
    SDL_RenderCopy(renderer, m_texture, nullptr, &destRect);
}
