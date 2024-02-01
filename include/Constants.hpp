#pragma once

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr int GRIDX = 8;
constexpr int GRIDY = 8;
constexpr int JEWELSIZEX = 50;
constexpr int JEWELSIZEY = 50;
constexpr int OFFSETX = (WIDTH - GRIDX * JEWELSIZEX) / 2;
constexpr int OFFSETY = (HEIGHT - GRIDY * JEWELSIZEY) / 2;
constexpr double JEWELCOVERAGE = 0.8;

enum class JewelType {
    EMPTY,
    BLACK,
    SILVER,
    PINK,
    BLUE,
    ORANGE

};

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
