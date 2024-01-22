#include "InputManager.hpp"



InputManager::InputManager() {
    initialize();
}

void InputManager::initialize() {
    // Initialize members
    for (int i = 0; i <= SDL_BUTTON_X2; ++i) {
        mouseButtonsState[i] = false;
    }
    mouseX = 0;
    mouseY = 0;
}

void InputManager::update() {
    // Update mouse state
    SDL_PumpEvents(); // This updated the event queue 
    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    mouseX = x;
    mouseY = y;

    for (int i = 0; i <= SDL_BUTTON_X2; ++i) {
        mouseButtonsState[i] = (buttons & SDL_BUTTON(i)) != 0; // SDL_BUTTON(1) is left mouse , SDL_BUTTON(2) middle button and SDL_BUTTON(3) if the right button  
    }


}

bool InputManager::isMouseButtonPressed(Uint8 button) {
    if (button <= SDL_BUTTON_X2) {
        return mouseButtonsState[button];
    }
    return false;
}

void InputManager::getMousePosition(int& x, int& y) {
    x = mouseX;
    y = mouseY;
}
