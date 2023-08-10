#include "Level.hpp"

Level::Level(const std::vector<std::vector<char>>& textLevel, const Tilemap& tilemap,
             const std::string& path) : path_(path)
{
    // reading highscore
    std::ifstream file(resourcePath() + path_);
    file >> highScore_;
    file.close();

    // load the font
    font_.loadFromFile(resourcePath() + "font/retro.ttf");
    
    // set up the score counter
    scoreCounter_ = sf::Text("SCORE: " + toString(score_), font_, COUNTER_SIZE_);
    scoreCounter_.setStyle(sf::Text::Bold);
    scoreCounter_.setFillColor(sf::Color::Black);
    scoreCounter_.setOrigin(scoreCounter_.getLocalBounds().left, scoreCounter_.getLocalBounds().top);
    scoreCounter_.setPosition(tilemap.getBorderBounds().left, (tilemap.getBorderBounds().top - scoreCounter_.getLocalBounds().height)/2);
    
    // set up the high score counter
    highScoreCounter_ = sf::Text("HIGH SCORE: " + toString(highScore_), font_, COUNTER_SIZE_);
    highScoreCounter_.setStyle(sf::Text::Bold);
    highScoreCounter_.setFillColor(sf::Color::Black);
    highScoreCounter_.setOrigin(highScoreCounter_.getLocalBounds().left, highScoreCounter_.getLocalBounds().top);
    highScoreCounter_.setPosition(tilemap.getBorderBounds().left + tilemap.getBorderBounds().width - highScoreCounter_.getLocalBounds().width, (tilemap.getBorderBounds().top - highScoreCounter_.getLocalBounds().height)/2);
    
    // set up the walls
    wallTexture_.loadFromFile(resourcePath() + "sprites/wall.png");
    // iterate through all the characters in the text level
    for (int j = 0; j < textLevel.size(); j++)
    {
        for (int i = 0; i < textLevel[0].size(); i++)
        {
            // '#' represents a wall
            if (textLevel[j][i] == '#')
            {
                // add the wall to the level
                wallSprites_.push_back(sf::Sprite(wallTexture_));
                wallSprites_.back().setPosition(tilemap.tiles()[j][i]);
            }
        }
    }
}

void Level::draw(sf::RenderWindow& window) const
{
    for (int i = 0; i < wallSprites_.size(); i++)
    {
        window.draw(wallSprites_[i]);
    }
    window.draw(scoreCounter_);
    window.draw(highScoreCounter_);
}

const std::vector<sf::Sprite>& Level::wallSprites() const
{
    return wallSprites_;
}

void Level::incrementScore()
{
    score_++;
    if (score_ > highScore_)
    {
        highScore_++;
        std::ofstream file(resourcePath() + path_, std::ios::out | std::ios::trunc);
        file << highScore_;
        file.close();
    }
}

void Level::updateScore()
{
    scoreCounter_.setString("SCORE: " + toString(score_));
    highScoreCounter_.setString("HIGH SCORE: " + toString(highScore_));
}

std::string Level::toString(int score) const
{
    // convert an integer to a string of width 3, for example, 1 becomes "001"
    std::stringstream ss;
    std::string str;
    ss << std::setw(3) << std::setfill('0') << score;
    ss >> str;
    return str;
}

void Level::resetScore()
{
    score_ = 0;
}

unsigned int Level::getScore() const
{
    return score_;
}
