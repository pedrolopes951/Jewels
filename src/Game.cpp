#include "Game.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cstdlib>  // Include this header for std::srand
#include <ctime>    // Include this header for std::time

Game::Game() : m_window(nullptr), m_inputManager(), m_jewel{0,0} {
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

        // Check if the left mouse button is pressed
        if (m_inputManager.isMouseButtonPressed(SDL_BUTTON_LEFT)) {
            int mouseX, mouseY;
            m_inputManager.getMousePosition(mouseX, mouseY);

            // Convert mouse coordinates to grid position with offset
            int clickedRow = (mouseY - OFFSETY) / JEWELSIZEX;
            int clickedCol = (mouseX - OFFSETX) / JEWELSIZEX;

            // Check if the clicked position is within the grid bounds
            if (clickedRow >= 0 && clickedRow < GRIDY && clickedCol >= 0 && clickedCol < GRIDX) {
                // Update the selected jewel's position
                m_jewel.posY = clickedRow;
                m_jewel.posX = clickedCol;

                // For testing purposes, print the selected jewel's position
                std::cout << "Selected Jewel: (" << m_jewel.posY << ", " << m_jewel.posX << ")\n";
            }
        }


        // Clear the renderer, prepares for he renderer for drawing the next frame
        SDL_RenderClear(m_renderer);

        // Render background using the Sprite class
        m_background.render(m_renderer, 0, 0, WIDTH, HEIGHT);


        // Render the 8x8 jewel grid
        this->renderJewelGrid();


        // Present the rendered frame to the screen
        SDL_RenderPresent(m_renderer);
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
