#pragma once

#include "Tilemap.hpp"

class MainMenu
{
public:
    MainMenu(const sf::Vector2f&);
    void draw(sf::RenderWindow&) const;
    void detectLeftClick(const sf::Vector2f&);
    bool isPlayPressed() const;
    bool isLevelsPressed() const;
    bool isQuitPressed() const;
    void unpressPlay();
    void unpressLevels();
    void unpressQuit();
private:
    sf::SoundBuffer soundBufferSelect_;
    sf::Sound soundSelect_; // sound that plays when an option is selected
    sf::Font font_;
    sf::Text credit_;
    sf::Text title_;
    enum Option
    {
        PLAY,
        LEVELS,
        QUIT,
        COUNT
    };
    sf::Text options_[COUNT];
    bool isOptionPressed_[COUNT]
    {
        false,
        false,
        false
    };
    const unsigned int TITLE_SIZE_{100}; // character size of the title
    const unsigned int OPTIONS_SIZE_{50}; // character size of the options
    const unsigned int CREDIT_SIZE{20}; // character size of the credit
    const float TITLE_TOP_{100.0f}; // top coordinates of the title
    const float FIRST_OPTION_TOP_{350.0f}; // top coordinates of the first option
    const float OPTIONS_SPACING_{100.0f}; // spacing between each option
    const float CREDIT_SPACING_{10.0f}; // spacing between the bottom of the screen and the credit
};

