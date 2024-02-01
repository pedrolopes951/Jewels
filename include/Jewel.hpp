#pragma once

// Enum for different types of jewels
enum class JewelType {
    EMPTY,
    BLACK,
    SILVER,
    PINK,
    BLUE,
    ORANGE
};

// Struct to encapsulate all jewel-related data
struct Jewel {
    JewelType m_type;   // The type of the jewel
    int m_posX;         // X position on the grid
    int m_posY;         // Y position on the grid

    Jewel(JewelType type = JewelType::EMPTY, int x = 0, int y = 0)
        : m_type(type), m_posX(x), m_posY(y) {}

    // Equality operators to compare two Jewel objects
    bool operator==(const Jewel& other) const {
        return (m_posX == other.m_posX) && (m_posY == other.m_posY);
    }

    bool operator!=(const Jewel& other) const {
        return !(*this == other);
    }
};
