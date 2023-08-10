#include "Food.hpp"

Food::Food()
{
    // set up the food
    texture_.loadFromFile(resourcePath() + "sprites/food.png");
    sprite_.setTexture(texture_);
}

void Food::draw(sf::RenderWindow& window) const
{
    window.draw(sprite_);
}

bool Food::spawn(const Tilemap& tilemap)
{
    // random generator to generate a random position
    static std::default_random_engine randomGenerator((unsigned)time(NULL));
    // spawn food if there are empty tiles
    if (tilemap.emptyTiles.size() > 0)
    {
        std::uniform_int_distribution<int> randomIndex(0, (int)tilemap.emptyTiles.size() - 1);
        sprite_.setPosition(tilemap.emptyTiles[randomIndex(randomGenerator)]);
        return true;
    }
    return false;
}

const sf::Vector2f& Food::getPosition() const
{
    return sprite_.getPosition();
}
