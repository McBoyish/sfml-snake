#pragma once

#include "Tilemap.hpp"

class Food
{
public:
    Food();
    void draw(sf::RenderWindow&) const;
    bool spawn(const Tilemap&);
    const sf::Vector2f& getPosition() const;
private:
    sf::Texture texture_;
    sf::Sprite sprite_;
};

