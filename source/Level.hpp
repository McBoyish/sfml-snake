#pragma once

#include "Tilemap.hpp"
#include <fstream>

class Level
{
public:
    Level(const std::vector<std::vector<char>>&, const Tilemap&, const std::string&);
    void draw(sf::RenderWindow&) const;
    const std::vector<sf::Sprite>& wallSprites() const;
    void incrementScore();
    void updateScore();
    std::string toString(int) const;
    void resetScore();
    unsigned int getScore() const;
private:
    std::string path_;
    sf::Texture wallTexture_;
    std::vector<sf::Sprite> wallSprites_;
    sf::Font font_;
    sf::Text scoreCounter_;
    sf::Text highScoreCounter_;
    const unsigned int COUNTER_SIZE_{30}; // character size of the score and high score counter
    unsigned int score_{0};
    unsigned int highScore_{0};
};

