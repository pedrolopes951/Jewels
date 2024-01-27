#include "Game.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cstdlib>  // Include this header for std::srand
#include <ctime>    // Include this header for std::time

Game::Game() : m_window(nullptr), m_inputManager(){
    // Ensure that I have seed different for every rand()
    std::srand(static_cast<unsigned>(std::time(nullptr)));

}

Game::~Game() {
}

bool Game::init() {

    // SDL initialization, window creation, etc.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Handle initialization failure
        return false;
    }

    m_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!m_window) {
        // Handle window creation failure
        SDL_Quit();
        return false;
    }


    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // Load background image using the Sprite class and jewels in to the sprite
    if(!this->loadSprites())
        return false;

    // Init the Grid of jewels
    this->initGridJewels();

    // Other initialization code

    return true;
}
void Game::run() {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Handle user input using the InputManager
        m_inputManager.update();

        // Clear the renderer, prepares it for drawing the next frame
        SDL_RenderClear(m_renderer);

        // Render background
        m_background.render(m_renderer, 0, 0, WIDTH, HEIGHT);

        // Render the jewel grid
        this->renderJewelGrid();

        // If a jewel is being dragged, render it at the dragged position
        if (m_inputManager.isDraggingInProgress()) {
            // Get the position of the jewel being dragged
            int dragX = m_inputManager.getJewelVisualPosX();
            int dragY = m_inputManager.getJewelVisualPosY();
            JewelPos draggedJewelPos = m_inputManager.getDraggedJewel();
            JewelType draggedJewelType = m_jewelGrid[draggedJewelPos.posY][draggedJewelPos.posX];

            // Render the dragged jewel sprite at the mouse cursor position
            m_jewelSprites[draggedJewelType].render(m_renderer, dragX, dragY, JEWELSIZEX, JEWELSIZEY);
           std::cout << "Swapping jewels at mouse cursor: " << draggedJewelPos.posX << "," << draggedJewelPos.posY << std::endl;

        }
        if (m_inputManager.isDraggingEnded()) {
            if (!m_swapPerformed) {
                JewelPos finalPos = snapToGrid(m_inputManager.getJewelVisualPosX(), m_inputManager.getJewelVisualPosY());
                JewelPos originalPos = m_inputManager.getDraggedJewel();

                if (finalPos != originalPos) {
                    std::cout << "Performing Swap: " << originalPos.posX << "," << originalPos.posY << " with " << finalPos.posX << "," << finalPos.posY << std::endl;
                    swapJewels(originalPos, finalPos);
                    m_swapPerformed = true;
                }
                m_inputManager.resetDragging();
            }
        }
        else if (m_swapPerformed) {
            //std::cout << "Resetting swap flag" << std::endl;
            m_swapPerformed = false;
        }

        //std::cout << "Dragging Ended: " << m_inputManager.isDraggingEnded() << ", Swap Performed: " << m_swapPerformed << std::endl;


        // Update the screen with any rendering performed since the previous call
        SDL_RenderPresent(m_renderer);

        // Implement a delay to cap the frame rate
        SDL_Delay(16);
    }
}

void Game::clear() {
    // Cleanup code (e.g., destroy window, quit SDL)
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Game::loadSprites()
{
    // Background Sprite
    if (!m_background.loadFromFile(m_renderer, "assets/images/Backdrop13.jpg")) {
        std::cerr << "Failed to load jewel asset (" << "assets/images/Backdrop13.jpg" << "): " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // Load each jewel into the map
    // Black jewel
    if (!m_jewelSprites[JewelType::BLACK].loadFromFile(m_renderer, "assets/images/Color-1.png")) {
        std::cerr << "Failed to load jewel asset (" << "assets/images/Color-1.png" << "): " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    // Silver jewel
    if (!m_jewelSprites[JewelType::SILVER].loadFromFile(m_renderer, "assets/images/Color-2.png")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    // Pink jewel
    if (!m_jewelSprites[JewelType::PINK].loadFromFile(m_renderer, "assets/images/Color-3.png")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    // Blue jewel
    if (!m_jewelSprites[JewelType::BLUE].loadFromFile(m_renderer, "assets/images/Color-4.png")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    // Orange jewel
    if (!m_jewelSprites[JewelType::ORANGE].loadFromFile(m_renderer, "assets/images/Color-5.png")) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    return true;
}

void Game::renderJewelGrid()
{
    //Set the grid in the center of the window


    for (int row = 0; row < GRIDX; row++) {
        for (int col = 0; col < GRIDY; col++) {
            int x = OFFSETX + col * JEWELSIZEX;
            int y = OFFSETY + row * JEWELSIZEY;

            // For simplicity, alternate between different jewel types
            JewelType jewelType = m_jewelGrid[row][col];

            if (jewelType != JewelType::EMPTY) {
                // Render the jewel sprite at the calculated position
                m_jewelSprites[jewelType].render(m_renderer, x, y, JEWELSIZEX, JEWELSIZEY);
            }
        }
    }
}

void Game::initGridJewels()
{
    // Initialize the 2D vector for storing jewel types
    m_jewelGrid.resize(GRIDY, std::vector<JewelType>(GRIDX, JewelType::EMPTY));


    // Randomly assign a jewel type to each cell in the grid during initialization
    for (int row = 0; row < GRIDX; row++) {
        for (int col = 0; col < GRIDY; col++) {
            m_jewelGrid[row][col] = static_cast<JewelType>(rand() % 5 + 1);
        }
    }
    this->checkAndRemoveMatches();
}

void Game::checkAndRemoveMatches()
{

    // Function to generate a random jewel that doesn't form an immediate match
    auto generateRandomNonMatchingJewel = [this](int row, int col) -> JewelType {
        JewelType randomType;
        do {
            randomType = static_cast<JewelType>(rand() % 5 + 1);
        } while (isMatch(randomType, row, col));
        return randomType;
        };

    // Check for Horizontal Matches 3 or more
    auto processHorizontalMatch = [&](int row, int col) {
        JewelType currentType = m_jewelGrid[row][col];
        if (currentType != JewelType::EMPTY &&
            currentType == m_jewelGrid[row][col + 1] &&
            currentType == m_jewelGrid[row][col + 2]) {
            m_jewelGrid[row][col] = generateRandomNonMatchingJewel(row, col);
            m_jewelGrid[row][col + 1] = generateRandomNonMatchingJewel(row, col + 1);
            m_jewelGrid[row][col + 2] = generateRandomNonMatchingJewel(row, col + 2);
        }
        };

    // Check for Vertical Matches 3 or more
    auto processVerticalMatch = [&](int row, int col) {
        JewelType currentType = m_jewelGrid[row][col];
        if (currentType != JewelType::EMPTY &&
            currentType == m_jewelGrid[row + 1][col] &&
            currentType == m_jewelGrid[row + 2][col]) {
            m_jewelGrid[row][col] = generateRandomNonMatchingJewel(row, col);
            m_jewelGrid[row + 1][col] = generateRandomNonMatchingJewel(row + 1, col);
            m_jewelGrid[row + 2][col] = generateRandomNonMatchingJewel(row + 2, col);
        }
        };

    // Process matches
    for (int row = 0; row < GRIDY; ++row) {
        for (int col = 0; col < GRIDX - 2; ++col) {
            processHorizontalMatch(row, col);
        }
    }

    for (int row = 0; row < GRIDY - 2; ++row) {
        for (int col = 0; col < GRIDX; ++col) {
            processVerticalMatch(row, col);
        }
    }

}

bool Game::isMatch(JewelType jewelType, int row, int col)
{
    // Check for horizontal match
    if ((col >= 2) &&
        (m_jewelGrid[row][col - 1] == jewelType) &&
        (m_jewelGrid[row][col - 2] == jewelType)) {
        return true;
    }

    // Check for vertical match
    if ((row >= 2) &&
        (m_jewelGrid[row - 1][col] == jewelType) &&
        (m_jewelGrid[row - 2][col] == jewelType)) {
        return true;
    }

    return false;
}

JewelPos Game::snapToGrid(int x, int y) {
    // Adjust to get the center of the jewel sprite
    x += JEWELSIZEX / 2 - OFFSETX;
    y += JEWELSIZEY / 2 - OFFSETY;

    // Convert from pixel coordinates to the nearest grid coordinates
    int gridPosX = x / JEWELSIZEX;
    int gridPosY = y / JEWELSIZEY;

    // Calculate the overlapping area with the nearest cell
    int cellStartX = gridPosX * JEWELSIZEX;
    int cellStartY = gridPosY * JEWELSIZEY;
    int cellEndX = cellStartX + JEWELSIZEX;
    int cellEndY = cellStartY + JEWELSIZEY;

    int overlapLeft = std::max(x - (int)JEWELSIZEX / 2, cellStartX);
    int overlapTop = std::max(y - (int)JEWELSIZEY / 2, cellStartY);
    int overlapRight = std::min(x + (int)JEWELSIZEX / 2, cellEndX);
    int overlapBottom = std::min(y + (int)JEWELSIZEY / 2, cellEndY);
    int overlapWidth = std::max(0, overlapRight - overlapLeft);
    int overlapHeight = std::max(0, overlapBottom - overlapTop);

    double overlapArea = overlapWidth * overlapHeight;
    double cellArea = JEWELSIZEX * JEWELSIZEY;
    double overlapPercentage = overlapArea / cellArea;

    // Check if the overlap is at least 80%
    if (overlapPercentage >= 0.8) {
        // Clamp grid coordinates to grid dimensions
        gridPosX = std::max(0, std::min(gridPosX, (int) GRIDX - 1));
        gridPosY = std::max(0, std::min(gridPosY, (int)GRIDY - 1));

        return { gridPosX, gridPosY };
    }
    else {
        // If the overlap is less than 80%, return the original position
        return m_inputManager.getDraggedJewel();
    }
}



void Game::swapJewels(const JewelPos& posA, const JewelPos& posB)
{
    // Check if the positions are within the grid bounds
    if (posA.posX >= 0 && posA.posX < GRIDX && posA.posY >= 0 && posA.posY < GRIDY &&
        posB.posX >= 0 && posB.posX < GRIDX && posB.posY >= 0 && posB.posY < GRIDY) {

        // Check if the jewels are adjacent
        bool isAdjacent = (posA.posX == posB.posX && std::abs(posA.posY - posB.posY) == 1) ||
            (posA.posY == posB.posY && std::abs(posA.posX - posB.posX) == 1);

        if (isAdjacent) {
            // Swap the jewels in the grid
            std::swap(m_jewelGrid[posA.posY][posA.posX], m_jewelGrid[posB.posY][posB.posX]);
        }
        else {
            // If not adjacent, do nothing or handle the situation as needed
            // Make a sound maybe if there is no match 
        }
    }
}


