#include "InputManager.hpp"



InputManager::InputManager() {
    initialize();
}

void InputManager::initialize() {
    // Initialize members
    for (int i = 0; i <= SDL_BUTTON_X2; ++i) {
        m_mouseButtonsState[i] = false;
    }
    m_mouseX = 0;
    m_mouseY = 0;

    m_isDragging = false;
    m_jewelDragPos.posX = 0;
    m_jewelDragPos.posY = 0;
    m_jewelVisualPosX = 0;
    m_jewelVisualPosY = 0;
    m_initialClickX = 0;
    m_initialClickY = 0;

    m_wasLeftMouseButtonDown = false; // Initialize this to false


}

void InputManager::update() {
    // Update mouse state
    SDL_PumpEvents(); // This updates the event queue
    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    m_mouseX = x;
    m_mouseY = y;

    bool leftMouseButtonPressed = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

    if (leftMouseButtonPressed && !m_isDragging) {
        // Mouse button is being pressed and we are not already dragging
        int mouseX, mouseY;
        this->getMousePosition(mouseX, mouseY);

        // Convert mouse coordinates to grid position with offset
        int clickedRow = (mouseY - OFFSETY) / JEWELSIZEX;
        int clickedCol = (mouseX - OFFSETX) / JEWELSIZEX;

        // Check if the clicked position is within the grid bounds
        if (clickedRow >= 0 && clickedRow < GRIDY && clickedCol >= 0 && clickedCol < GRIDX) {
            // Start dragging if a jewel was clicked
            startDragging({ clickedCol, clickedRow });
        }
    }
    else if (!leftMouseButtonPressed && m_isDragging) {
        // Mouse button was released, stop dragging
        resetDragging();
    }
    else if (leftMouseButtonPressed && m_isDragging) {
        // Mouse button is being pressed and we are already dragging
        updateDragging();
    }

    // Keep track of the left mouse button state
    m_mouseButtonsState[SDL_BUTTON_LEFT] = leftMouseButtonPressed;
}

bool InputManager::isMouseButtonPressed(Uint8 button) {
    if (button <= SDL_BUTTON_X2) {
        //std::cout << "Button clicked" << std::endl;
        return m_mouseButtonsState[button];
    }
    return false;
}

void InputManager::getMousePosition(int& x, int& y) {
    x = m_mouseX;
    y = m_mouseY;
}

void InputManager::startDragging(const JewelPos& initialPos) {
    m_isDragging = true;
    m_jewelDragPos = initialPos; // Grid position where the drag started
    m_jewel = initialPos;        // This should be the correct position of the dragged jewel

    m_initialClickX = m_mouseX;
    m_initialClickY = m_mouseY;

    // Set the visual position to match the grid position initially
    m_jewelVisualPosX = OFFSETX + initialPos.posX * JEWELSIZEX;
    m_jewelVisualPosY = OFFSETY + initialPos.posY * JEWELSIZEY;
}


void InputManager::updateDragging() {
    // Calculate the visual position based on the mouse movement
    int deltaX = m_mouseX - m_initialClickX;
    int deltaY = m_mouseY - m_initialClickY;
    m_jewelVisualPosX += deltaX;
    m_jewelVisualPosY += deltaY;

    // Update the initial click position for the next frame
    m_initialClickX = m_mouseX;
    m_initialClickY = m_mouseY;

}

bool InputManager::isDraggingEnded() {
    // Check if the left mouse button is released
    return !isMouseButtonPressed(SDL_BUTTON_LEFT);
}

void InputManager::resetDragging() {
    // Reset the dragging state
    m_isDragging = false;
}

bool InputManager::isDraggingInProgress() const {
    return m_isDragging;
}
const JewelPos& InputManager::getDraggedJewel() const {
    return m_jewel;
}
int InputManager::getJewelVisualPosX() const {
    return m_jewelVisualPosX;
}

int InputManager::getJewelVisualPosY() const {
    return m_jewelVisualPosY;
}

void InputManager::handleFirstClick(int x, int y) {
    firstClickPos = { x / (int)JEWELSIZEX, y / (int)JEWELSIZEY };
    firstClickActive = true;
}

void InputManager::handleSecondClick(int x, int y) {
    secondClickPos = { x / (int)JEWELSIZEX, y / (int)JEWELSIZEY };
}

bool InputManager::isFirstClickActive() const {
    return firstClickActive;
}

JewelPos InputManager::getFirstClickPos() const {
    return firstClickPos;
}

JewelPos InputManager::getSecondClickPos() const {
    return secondClickPos;
}

void InputManager::resetClicks() {
    firstClickActive = false;
}
void InputManager::setFirstClickPos(const JewelPos& pos) {
    firstClickPos = pos;
    firstClickActive = true;
}

void InputManager::setSecondClickPos(const JewelPos& pos) {
    secondClickPos = pos;
}

