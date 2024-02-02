#include "GridManager.hpp"

GridManager::GridManager() : m_initialcheck{ false }, m_pointsPerMatch{ 0 }, m_swapPerformed{ false }
{
}

GridManager::~GridManager()
{
}

bool GridManager::loadSprites(SDL_Window* window, SDL_Renderer* renderer)
{
    // Load each jewel into the map
    // Black jewel
    if (!m_jewelSprites[JewelType::BLACK].loadFromFile(renderer, "assets/images/Color-1.png")) {
        std::cerr << "Failed to load jewel asset (" << "assets/images/Color-1.png" << "): " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    // Silver jewel
    if (!m_jewelSprites[JewelType::SILVER].loadFromFile(renderer, "assets/images/Color-2.png")) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    // Pink jewel
    if (!m_jewelSprites[JewelType::PINK].loadFromFile(renderer, "assets/images/Color-3.png")) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    // Blue jewel
    if (!m_jewelSprites[JewelType::BLUE].loadFromFile(renderer, "assets/images/Color-4.png")) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    // Orange jewel
    if (!m_jewelSprites[JewelType::ORANGE].loadFromFile(renderer, "assets/images/Color-5.png")) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    return false;
}

void GridManager::handleClickedJewel(const JewelPos& jewelpos, InputManager& input_manager)
{
    
    // Check if the click is within the bounds of the grid, do nothing
    if (jewelpos.posX < 0 || jewelpos.posX >= GRIDX || jewelpos.posY < 0 || jewelpos.posY >= GRIDY) {
        return;
    }

    // First click
    if (!input_manager.isFirstClickActive()) {
        input_manager.setFirstClickPos(jewelpos); // Stores where the first click was in the input manager class
    }
    // Second click
    else {
        if (jewelpos != input_manager.getFirstClickPos() && this->willSwapMatch(input_manager.getFirstClickPos(), jewelpos)) {
            this->swapJewels(input_manager.getFirstClickPos(), jewelpos);
            this->checkAndRemoveMatches();
            this->applyGravity();
        }
        else {
        }
        input_manager.resetClicks();
    }
}

void GridManager::initGridJewels()
{
    m_jewelGrid.resize(GRIDY, std::vector<JewelType>(GRIDY, JewelType::EMPTY));

    for (int row = 0; row < GRIDY; row++)
    {
        for (int col = 0; col < GRIDX; col++)
        {
            m_jewelGrid[row][col] = static_cast<JewelType>(rand() % 5 + 1);

        }
    }
    this->checkForMatches();
    if (!m_initialcheck) {
        m_initialcheck = true;
        this->resetPoints();
    }
}

void GridManager::checkForMatches()
{
    bool matchesFound = true;

    while (matchesFound) {
        matchesFound = false;

        for (int row = 0; row < GRIDX; ++row) {
            for (int col = 0; col < GRIDY; ++col) {
                if (this->isMatch(m_jewelGrid[row][col], row, col)) {
                    matchesFound = true;
                    break; 
                }
            }
            if (matchesFound) break;
        }

        if (matchesFound) {
            this->checkAndRemoveMatches();
            this->applyGravity();
        }
    }
}

void GridManager::checkAndRemoveMatches()
{
    // Vector to store the jewels which are matching
    std::vector<std::vector<bool>> matched(GRIDX, std::vector<bool>(GRIDY, false));
    int matchesThisCheck = 0; // Variable to count matches in this check

    // Check for horizontal matches
    for (int row = 0; row < GRIDY; row++) {
        for (int col = 0; col <= GRIDX - 3; ) {
            if (m_jewelGrid[row][col] != JewelType::EMPTY &&
                m_jewelGrid[row][col] == m_jewelGrid[row][col + 1] &&
                m_jewelGrid[row][col] == m_jewelGrid[row][col + 2]) {

                // Start of a match
                int matchStart = col;

                // Find the end of the match
                while (col < GRIDX && m_jewelGrid[row][col] == m_jewelGrid[row][matchStart]) {
                    matched[row][col] = true;
                    col++;
                }

                // Calculate points for this match
                matchesThisCheck += (col - matchStart);
            }
            else {
                col++;
            }
        }
    }

    // Check for vertical matches
    for (int col = 0; col < GRIDY; col++) {
        for (int row = 0; row <= GRIDX - 3; ) {
            if (m_jewelGrid[row][col] != JewelType::EMPTY &&
                m_jewelGrid[row][col] == m_jewelGrid[row + 1][col] &&
                m_jewelGrid[row][col] == m_jewelGrid[row + 2][col]) {

                // Start of a match
                int matchStart = row;

                // Find the end of the match
                while (row < GRIDY && m_jewelGrid[row][col] == m_jewelGrid[matchStart][col]) {
                    matched[row][col] = true;
                    row++;
                }

                // Calculate points for this match
                matchesThisCheck += (row - matchStart);
            }
            else {
                row++;
            }
        }
    }

    // remove matched jewels and update the points
    for (int row = 0; row < GRIDY; row++) {
        for (int col = 0; col < GRIDX; col++) {
            if (matched[row][col]) {
                m_jewelGrid[row][col] = JewelType::EMPTY;
            }
        }
    }

    m_pointsPerMatch += matchesThisCheck;
}


void GridManager::applyGravity()
{
    for (int col = 0; col < GRIDX; col++) {
        int emptyRow = -1; // Start with no empty row found
        for (int row = GRIDY - 1; row >= 0; row--) {
            if (m_jewelGrid[row][col] == JewelType::EMPTY && emptyRow == -1) {
                emptyRow = row; // Found the first empty row from the bottom
            }
            else if (m_jewelGrid[row][col] != JewelType::EMPTY && emptyRow != -1) {
                m_jewelGrid[emptyRow][col] = m_jewelGrid[row][col]; // Fill up that empty jewel with th
                m_jewelGrid[row][col] = JewelType::EMPTY;
                emptyRow--; // Move up to the next empty row
            }
        }
        // Fill now the empty places
        for (int row = 0; row < GRIDY; row++) {
            if (m_jewelGrid[row][col] == JewelType::EMPTY) {
                m_jewelGrid[row][col] = static_cast<JewelType>(rand() % 5 + 1);
            }
        }

    }
}

JewelPos GridManager::snapToGrid(int x, int y, const JewelPos& original_pos)
{
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

    int overlapLeft = std::max(x - JEWELSIZEX / 2, cellStartX);
    int overlapTop = std::max(y - JEWELSIZEY / 2, cellStartY);
    int overlapRight = std::min(x + JEWELSIZEX / 2, cellEndX);
    int overlapBottom = std::min(y + JEWELSIZEY / 2, cellEndY);
    int overlapWidth = std::max(0, overlapRight - overlapLeft);
    int overlapHeight = std::max(0, overlapBottom - overlapTop);

    double overlapArea = overlapWidth * overlapHeight;
    double cellArea = JEWELSIZEX * JEWELSIZEY;
    double overlapPercentage = overlapArea / cellArea;

    // Check if the overlap is at least  (Changeable)
    if (overlapPercentage >= JEWELCOVERAGE) {
        // Clamp grid coordinates to grid dimensions
        gridPosX = std::max(0, std::min(gridPosX, GRIDX - 1));
        gridPosY = std::max(0, std::min(gridPosY, GRIDY - 1));

        return { gridPosX, gridPosY };
    }
    else {
        return original_pos;
    }
}




void GridManager::renderJewelGrid(SDL_Renderer* renderer,InputManager& input_manager)
{
    JewelPos draggedJewelPos = input_manager.getDraggedJewel();

    for (int row = 0; row < GRIDX; row++) {
        for (int col = 0; col < GRIDY; col++) {
            // Skip rendering the jewel at the dragged position
            if (input_manager.isDraggingInProgress() &&
                draggedJewelPos.posX == col && draggedJewelPos.posY == row) {
                continue;
            }

            int x = OFFSETX + col * JEWELSIZEX;
            int y = OFFSETY + row * JEWELSIZEY;
            JewelType jewelType = m_jewelGrid[row][col];

            if (jewelType != JewelType::EMPTY) {
                m_jewelSprites[jewelType].render(renderer, x, y, JEWELSIZEX, JEWELSIZEY);
            }
        }
    }

    // If a jewel is being dragged, render it at the dragged position
    if (input_manager.isDraggingInProgress()) {
        int dragX = input_manager.getJewelVisualPosX();
        int dragY = input_manager.getJewelVisualPosY();
        JewelPos draggedJewelPos = input_manager.getDraggedJewel();
        JewelType draggedJewelType = m_jewelGrid[draggedJewelPos.posY][draggedJewelPos.posX];

        // Render the dragged jewel sprite at the mouse cursor position
        m_jewelSprites[draggedJewelType].render(renderer, dragX, dragY, JEWELSIZEX, JEWELSIZEY);

    }
    if (input_manager.isDraggingEnded()) {
        if (!m_swapPerformed) {
            JewelPos originalPos = input_manager.getDraggedJewel();
            JewelPos finalPos = this->snapToGrid(input_manager.getJewelVisualPosX(), input_manager.getJewelVisualPosY(), input_manager.getDraggedJewel());

            if (finalPos != originalPos && this->willSwapMatch(originalPos, finalPos)) {
                this->swapJewels(originalPos, finalPos);

                m_swapPerformed = true;
                this->checkAndRemoveMatches();
                this->applyGravity();
            }
            else {
            }
            input_manager.resetDragging();
        }
    }
    else if (m_swapPerformed) {
        //std::cout << "Resetting swap flag" << std::endl;
        m_swapPerformed = false;
    }
}

void GridManager::swapJewels(const JewelPos& posA, const JewelPos& posB)
{
    // Check if the positions are within the grid bounds
    if (posA.posX >= 0 && posA.posX < GRIDX && posA.posY >= 0 && posA.posY < GRIDY &&
        posB.posX >= 0 && posB.posX < GRIDX && posB.posY >= 0 && posB.posY < GRIDY) {

        // Check if the jewels are adjacent
        bool isAdjacent = (posA.posX == posB.posX && std::abs(posA.posY - posB.posY) == 1) ||
            (posA.posY == posB.posY && std::abs(posA.posX - posB.posX) == 1);

        if (isAdjacent) {
            std::swap(m_jewelGrid[posA.posY][posA.posX], m_jewelGrid[posB.posY][posB.posX]);
        }
        else {
            // If not adjacent, do nothing 
        }
    }
}

bool GridManager::willSwapMatch(const JewelPos& posA, const JewelPos& posB)
{
    // Temporarily swap jewels
     std::swap(m_jewelGrid[posA.posY][posA.posX], m_jewelGrid[posB.posY][posB.posX]);
 
     // Check if the swap results in a match
     bool matchA = this->checkPotentialMatchAt(posA.posY, posA.posX);
     bool matchB = this->checkPotentialMatchAt(posB.posY, posB.posX);
 
     // Swap back to original positions
     std::swap(m_jewelGrid[posA.posY][posA.posX], m_jewelGrid[posB.posY][posB.posX]);
 
     return matchA || matchB;
}

void GridManager::resetPoints()
{
    m_pointsPerMatch = 0;
}

const int& GridManager::getPoints() const
{
    return m_pointsPerMatch;
}

bool GridManager::checkPotentialMatchAt(int y, int x)
{
    JewelType jewelType = m_jewelGrid[y][x];

    // Check horizontal matches
    // Check for match with one jewel to the left and one to the right
    if (x > 0 && x < GRIDX - 1 &&
        m_jewelGrid[y][x - 1] == jewelType &&
        m_jewelGrid[y][x + 1] == jewelType) {
        return true;
    }
    // Check for match with two jewels to the left
    if (x > 1 &&
        m_jewelGrid[y][x - 1] == jewelType &&
        m_jewelGrid[y][x - 2] == jewelType) {
        return true;
    }
    // Check for match with two jewels to the right
    if (x < GRIDX - 2 &&
        m_jewelGrid[y][x + 1] == jewelType &&
        m_jewelGrid[y][x + 2] == jewelType) {
        return true;
    }

    // Check vertical matches
    // Check for match with one jewel above and one below
    if (y > 0 && y < GRIDY - 1 &&
        m_jewelGrid[y - 1][x] == jewelType &&
        m_jewelGrid[y + 1][x] == jewelType) {
        return true;
    }
    // Check for match with two jewels above
    if (y > 1 &&
        m_jewelGrid[y - 1][x] == jewelType &&
        m_jewelGrid[y - 2][x] == jewelType) {
        return true;
    }
    // Check for match with two jewels below
    if (y < GRIDY - 2 &&
        m_jewelGrid[y + 1][x] == jewelType &&
        m_jewelGrid[y + 2][x] == jewelType) {
        return true;
    }

    return false;
}
bool GridManager::isMatch(JewelType jewelType, int row, int col)
{
    // Compares the actual jewel with the adjacent cells to the left,right,up and down
      // Check at least 2 jewels to the left
    if (col >= 2 &&
        m_jewelGrid[row][col - 1] == jewelType &&
        m_jewelGrid[row][col - 2] == jewelType) {
        return true;
    }

    // Check right k at least 2 jewels to the right
    if (col <= GRIDY - 3 &&
        m_jewelGrid[row][col + 1] == jewelType &&
        m_jewelGrid[row][col + 2] == jewelType) {
        return true;
    }

    // Check up
    if (row >= 2 &&
        m_jewelGrid[row - 1][col] == jewelType &&
        m_jewelGrid[row - 2][col] == jewelType) {
        return true;
    }

    // Check down
    if (row <= GRIDX - 3 &&
        m_jewelGrid[row + 1][col] == jewelType &&
        m_jewelGrid[row + 2][col] == jewelType) {
        return true;
    }

    return false;
}

