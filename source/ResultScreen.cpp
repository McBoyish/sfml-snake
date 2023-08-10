#include "ResultScreen.hpp"

ResultScreen::ResultScreen(const sf::Vector2f& windowSize):
WINDOW_SIZE_(windowSize)
{
    // load the sound
    soundBufferSelect_.loadFromFile(resourcePath() + "sounds/select.wav");
    soundSelect_.setBuffer(soundBufferSelect_);
    
    // load the font
    font_.loadFromFile(resourcePath() + "font/retro.ttf");
}

void ResultScreen::setResult(const Snake& snake, const Level* const& level)
{
    // set up the announcement
    if (snake.hasWon())
    {
        // display winning text
        announcement_ = sf::Text("YOU WON!", font_, ANNOUNCEMENT_SIZE_);
    }
    else
    {
        // display losing text
        announcement_ =  sf::Text("YOU DIED!", font_, ANNOUNCEMENT_SIZE_);
    }
    announcement_.setFillColor(sf::Color::Black);
    announcement_.setStyle(sf::Text::Bold);
    announcement_.setOrigin(announcement_.getLocalBounds().left, announcement_.getLocalBounds().top);
    announcement_.setPosition((WINDOW_SIZE_.x - announcement_.getLocalBounds().width)/2, ANNOUNCEMENT_TOP_);
    
    // set up the score
    score_ = sf::Text("SCORE: " + level->toString(level->getScore()), font_, SCORE_SIZE_);
    score_.setFillColor(sf::Color::Black);
    score_.setStyle(sf::Text::Bold);
    score_.setOrigin(score_.getLocalBounds().left, score_.getLocalBounds().top);
    score_.setPosition((WINDOW_SIZE_.x - score_.getLocalBounds().width)/2, SCORE_TOP_);
}

void ResultScreen::draw(sf::RenderWindow& window) const
{
    window.draw(announcement_);
    window.draw(score_);
}

void ResultScreen::playSelectSound()
{
    soundSelect_.play();
}
