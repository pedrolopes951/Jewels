#include "Game.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cstdlib> 
#include <ctime>    

Game::Game() : m_window(nullptr), m_inputManager(), m_timer{}, m_currentState(GameState::Setup) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

}

Game::~Game() {
}

bool Game::init() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    m_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!m_window) {
        SDL_Quit();
        return false;
    }


    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    if (!this->loadSprites())
        return false;

    m_grid.initGridJewels();


    m_fpsManager.start(m_renderer);


    m_points.start(m_renderer);


    return true;
}

bool Game::loadSprites()
{
    if (!m_background.loadFromFile(m_renderer, "assets/images/Backdrop13.jpg")) {
        std::cerr << "Failed to load jewel asset (" << "assets/images/Backdrop13.jpg" << "): " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_grid.loadSprites(m_window, m_renderer);

    return true;
}







void Game::run() {
    m_gameOver = false;

    while (!m_gameOver) {
        switch (m_currentState) {
        case GameState::Setup:
            this->handleSetupEvents();
            this->renderSetupScreen();
            break;
        case GameState::Playing:
            this->handleEvents();

            this->update();

            this->render();
            break;
        case GameState::GameOver:
            this->handleGameOverEvents();
            this->renderGameOverScreen();
            break;
        }
    }
}

void Game::handleEvents(){
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT) {
            this->clear(); 
            m_gameOver = true; 
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            this->handleMouseEvents(e);
        }
    }
    m_gameOver = false;
}




void Game::handleMouseEvents(const SDL_Event& e){
    int mouseX = e.button.x;
    int mouseY = e.button.y;


    // Convert mouse position to grid position
    JewelPos clickPos = { (mouseX - OFFSETX) / JEWELSIZEX, (mouseY - OFFSETY) / JEWELSIZEY };

    m_grid.handleClickedJewel(clickPos, m_inputManager);

}

void Game::handleSetupEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            this->clear();
            m_gameOver = true;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_RETURN:
                m_currentState = GameState::Playing;
                m_timer.start(m_renderer);
                break;

            }
        }
    }
}

void Game::handleGameOverEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            this->clear();
            m_gameOver = true;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_r:
                m_currentState = GameState::Setup;
                resetGame();
                break;
            case SDLK_x:
                m_gameOver = true;

                break;
            }
        }
    }
}

void Game::update(){
    m_inputManager.update();

    m_grid.checkForMatches();

    int pointsThisFrame = m_grid.getPoints();
    m_points.addPoints(pointsThisFrame);
    m_grid.resetPoints();

    m_timer.update();

    m_fpsManager.update();

    if (m_timer.isFinished()) {
        m_currentState = GameState::GameOver;
    }


}

void Game::resetGame() {
    m_grid.initGridJewels();

    m_grid.resetPoints();

    m_timer.start(m_renderer);

    m_points.reset();

    m_currentState = GameState::Playing;
}


void Game::render(){
    SDL_RenderClear(m_renderer);

    m_background.render(m_renderer, 0, 0, WIDTH, HEIGHT);

    m_grid.renderJewelGrid(m_renderer, m_inputManager);

    m_timer.render();

    m_fpsManager.render();

    m_points.render();

    SDL_RenderPresent(m_renderer);

    SDL_Delay(16);

}
void Game::renderSetupScreen() {
    SDL_SetRenderDrawColor(m_renderer, ColorText::BLACK, ColorText::BLACK, ColorText::BLACK, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    SDL_Color whiteColor = { ColorText::WHITE, ColorText::WHITE, ColorText::WHITE, ColorText::WHITE };
    std::string setupText = "Press ENTER to start";
    int x = (WIDTH - 300) / 2;
    int y = HEIGHT / 2;
    this->renderText(setupText, x, y, whiteColor);

    SDL_RenderPresent(m_renderer);
}

void Game::renderGameOverScreen() {
    SDL_SetRenderDrawColor(m_renderer, ColorText::BLACK, ColorText::BLACK, ColorText::BLACK, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    SDL_Color whiteColor = { ColorText::WHITE, ColorText::WHITE, ColorText::WHITE, ColorText::WHITE };
    std::string points = std::string("Points : ") + std::to_string(m_points.getPoints());
    std::string gameOverText = "Game Over! Press R to restart or X to exit";
    int x = (WIDTH - 500) / 2;
    int y = HEIGHT / 2;
    this->renderText(points, x + WIDTH / 4, y - 40, whiteColor);
    this->renderText(gameOverText, x, y, whiteColor);

    SDL_RenderPresent(m_renderer);
}
void Game::renderText(const std::string& text, int x, int y, SDL_Color color) {

    TTF_Font* font = TTF_OpenFont("assets/fonts/RobotoLight.ttf", SIZETEXT);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        TTF_CloseFont(font);
        std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect renderQuad = { x, y, surface->w, surface->h };
    SDL_RenderCopy(m_renderer, texture, nullptr, &renderQuad);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}


void Game::clear() {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
