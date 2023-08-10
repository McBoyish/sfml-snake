#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <array>
#include <fstream>
#include <string>
#include <list>
#include <sstream>
#include <iomanip>

// the tilemap is the area enclosed by the border
class Tilemap
{
public:
    Tilemap(const sf::FloatRect&);
    void draw(sf::RenderWindow&) const;
    float getTileSize() const;
    const sf::FloatRect& getBounds() const;
    const sf::FloatRect& getBorderBounds() const;
    const std::vector<std::vector<sf::Vector2f>>& tiles() const;
    std::vector<std::vector<sf::Vector2f>> getTiles() const;
    const sf::Vector2<unsigned int>& getSize() const;
public:
    std::vector<sf::Vector2f> filledTiles;
    std::vector<sf::Vector2f> emptyTiles;
private:
    sf::Texture borderTexture_;
    sf::Sprite borderSprite_;
    const sf::Vector2<unsigned int> SIZE_; // rows by columns of the tile map
    static constexpr float TILE_SIZE_{32.0f};
    static constexpr float BORDER_THICKNESS_{16.0f};
    const sf::FloatRect BORDER_BOUNDS_;
    const sf::FloatRect BOUNDS_;
    const std::vector<std::vector<sf::Vector2f>> TILES_; // stores the position of all the tiles
};

