#include "Tilemap.hpp"

Tilemap::Tilemap(const sf::FloatRect& innerBounds):
SIZE_         (innerBounds.width/TILE_SIZE_,
               innerBounds.height/TILE_SIZE_),
BOUNDS_       (innerBounds.left,
               innerBounds.top,
               innerBounds.width,
               innerBounds.height),
BORDER_BOUNDS_(innerBounds.left - BORDER_THICKNESS_,
               innerBounds.top - BORDER_THICKNESS_,
               innerBounds.width + 2*BORDER_THICKNESS_,
               innerBounds.height + 2*BORDER_THICKNESS_),
TILES_        (getTiles())
{
    // set up the border
    borderTexture_.loadFromFile(resourcePath() + "levels/border.png");
    borderSprite_.setTexture(borderTexture_);
}

void Tilemap::draw(sf::RenderWindow& window) const
{
    window.draw(borderSprite_);
}

float Tilemap::getTileSize() const
{
    return TILE_SIZE_;
}

const sf::FloatRect& Tilemap::getBounds() const
{
    return BOUNDS_;
}

const sf::FloatRect& Tilemap::getBorderBounds() const
{
    return BORDER_BOUNDS_;
}

const std::vector<std::vector<sf::Vector2f>>& Tilemap::tiles() const
{
    return TILES_;
}

std::vector<std::vector<sf::Vector2f>> Tilemap::getTiles() const
{
    std::vector<std::vector<sf::Vector2f>> tiles;
    // calculate the position of all the tiles and store them in a 2d vector
    for (int j = 0; j < SIZE_.y; j++)
    {
        tiles.push_back(std::vector<sf::Vector2f>());
        for (int i = 0; i < SIZE_.x; i++)
        {
            tiles[j].push_back(sf::Vector2f(BOUNDS_.left + i*TILE_SIZE_, BOUNDS_.top + j*TILE_SIZE_));
        }
    }
    return tiles;
}

const sf::Vector2<unsigned int>& Tilemap::getSize() const
{
    return SIZE_;
}
