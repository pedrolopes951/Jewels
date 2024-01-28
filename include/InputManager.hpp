#pragma once
#include <SDL.h>
#include <Constants.hpp>
#include <iostream>
#include <algorithm>

struct JewelPos {
    int posX;
    int posY;
    bool operator==(const JewelPos& other) const {
        return posX == other.posX && posY == other.posY;
    }

    // Overload the '!=' operator
    bool operator!=(const JewelPos& other) const {
        return !(*this == other);
    }
};


class InputManager {
public:
    InputManager();

    

    // Update input state
    void update();


    // Check if a mouse button is currently pressed
    bool isMouseButtonPressed(Uint8 button);

    // Get the current mouse coordinates
    void getMousePosition(int& x, int& y);

    // Dragging functions
    // Start dragging and update the initial position
    void startDragging(const JewelPos& initialPos);

    // Update the position of the dragged jewel during the drag
    void updateDragging();

    // Check if the user has released the mouse button to end the drag
    bool isDraggingEnded();

    // Reset the dragging state
    void resetDragging();

    // Check if dragging is in progress
    bool isDraggingInProgress() const;

    // Get the current position of the dragged jewel
    const JewelPos& getDraggedJewel() const;

    // Additional getters for visual position of dragged jewel
    int getJewelVisualPosX() const;
    int getJewelVisualPosY() const;

    // Add new methods for click-based swapping
    bool isFirstClickActive() const;
    JewelPos getFirstClickPos() const;
    JewelPos getSecondClickPos() const;
    void resetClicks();
    void setFirstClickPos(const JewelPos& pos);

    void setSecondClickPos(const JewelPos& pos);



private:


    // Initialize the input manager
    void initialize();

    bool m_mouseButtonsState[SDL_BUTTON_X2 + 1];
    int m_mouseX, m_mouseY;



    // Position of the selected jewel
    JewelPos m_jewel;

    // Save Dragging status
    bool m_isDragging;
    JewelPos m_jewelDragPos;

    // Visual position of the jewel being dragged
    int m_jewelVisualPosX;
    int m_jewelVisualPosY;

    // Initial click position (used to calculate the visual offset)
    int m_initialClickX;
    int m_initialClickY;

    // Check if the leftbutton from the last frame 
    bool m_wasLeftMouseButtonDown;

    // Add new members for the click-based swapping
    bool m_firstClickActive;
    JewelPos m_firstClickPos;
    JewelPos m_secondClickPos;


};
