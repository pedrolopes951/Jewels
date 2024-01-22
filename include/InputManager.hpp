#pragma once
#include <SDL.h>

class InputManager {
public:
    InputManager();

    // Initialize the input manager
    void initialize();

    // Update input state
    void update();

    // Check if a mouse button is currently pressed
    bool isMouseButtonPressed(Uint8 button);

    // Get the current mouse coordinates
    void getMousePosition(int& x, int& y);

private:
    bool mouseButtonsState[SDL_BUTTON_X2 + 1];
    int mouseX, mouseY;
};
