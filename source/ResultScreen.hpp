#pragma once

#include "Snake.hpp"

class ResultScreen
{
public:
    ResultScreen(const sf::Vector2f&);
    void setResult(const Snake& snake, const Level* const&);
    void draw(sf::RenderWindow&) const;
    void playSelectSound();
private:
    sf::SoundBuffer soundBufferSelect_;
    sf::Sound soundSelect_; // sound that plays when exiting the result screen
    sf::Font font_;
    const sf::Vector2f WINDOW_SIZE_;
    sf::Text announcement_;
    sf::Text score_;
    const unsigned int ANNOUNCEMENT_SIZE_{90}; // character size of the announcement
    const unsigned int SCORE_SIZE_{30}; // character size of the score
    const float ANNOUNCEMENT_TOP_{100.0f}; // top coordinates of announcement
    const float SCORE_TOP_{200.0f}; // top coordinates of score
};

