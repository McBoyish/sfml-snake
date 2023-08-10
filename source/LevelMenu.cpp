#include "LevelMenu.hpp"

LevelMenu::LevelMenu(const sf::Vector2f& windowSize)
{
    // load the sound
    soundBufferSelect_.loadFromFile(resourcePath() + "sounds/select.wav");
    soundSelect_.setBuffer(soundBufferSelect_);
    
    // load the font
    font_.loadFromFile(resourcePath() + "font/retro.ttf");
    
    // initialize the options
    for (int i = 0; i < COUNT; i++)
    {
        if (i == BACK)
        {
            options_[i] = sf::Text("BACK", font_, OPTIONS_SIZE_);
        }
        else
        {
            options_[i] = sf::Text("LEVEL " + std::to_string(i + 1), font_, OPTIONS_SIZE_);
        }
        // black means unselected, red means selected
        options_[i].setFillColor(sf::Color::Black);
        options_[i].setStyle(sf::Text::Bold);
        options_[i].setOrigin(options_[i].getLocalBounds().left, options_[i].getLocalBounds().top);
        options_[i].setPosition((windowSize.x - options_[i].getLocalBounds().width)/2, FIRST_OPTION_TOP_ + i*OPTIONS_SPACING_);
    }
    // level 1 is selected by default
    options_[LEVEL1].setFillColor(sf::Color::Red);
}

void LevelMenu::draw(sf::RenderWindow& window) const
{
    for (int i = 0; i < COUNT; i++)
    {
        window.draw(options_[i]);
    }
}

void LevelMenu::updateSelection(unsigned int selection)
{
    // check which option is selected and change its color to red and change the rest to black
    for (int i = 0; i < COUNT - 1; i++)
    {
        if (i == selection)
        {
            isLevelSelected_[i] = true;
        }
        else
        {
            isLevelSelected_[i] = false;
        }
    }
    optionsFillColor();
}

void LevelMenu::detectLeftClick(Level*& level, std::array<Level, NUMBER_OF_LEVELS_>& levels, sf::Vector2f*& initialPosition, std::array<sf::Vector2f, NUMBER_OF_LEVELS_>& initialPositions, const sf::Vector2f& mousePosition)
{
    for (int i = 0; i < NUMBER_OF_LEVELS_; i++)
    {
        if (options_[i].getGlobalBounds().contains(mousePosition) && !isLevelSelected_[i])
        {
            soundSelect_.play();
            // change the level by pointing it to a different level
            level = &levels[i];
            // change the initial position for the new level
            initialPosition = &initialPositions[i];
            updateSelection(i);
            break;
        }
    }
    if (options_[BACK].getGlobalBounds().contains(mousePosition))
    {
        soundSelect_.play();
        isOptionPressed_[BACK] = true;
    }
}

void LevelMenu::optionsFillColor()
{
    // highlight the selected option and unhighlight the rest
    for (int i = 0; i < COUNT - 1; i++)
    {
        if (isLevelSelected_[i])
        {
            options_[i].setFillColor(sf::Color::Red);
        }
        else
        {
            options_[i].setFillColor(sf::Color::Black);
        }
    }
}

bool LevelMenu::isBackPressed() const
{
    return isOptionPressed_[BACK];
}

void LevelMenu::unpressBack()
{
    isOptionPressed_[BACK] = false;
}
