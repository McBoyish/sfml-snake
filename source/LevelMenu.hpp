#pragma once

#include "Level.hpp"

class LevelMenu
{
private:
    static const unsigned int NUMBER_OF_LEVELS_{4};
public:
    LevelMenu(const sf::Vector2f&);
    void draw(sf::RenderWindow&) const;
    void updateSelection(unsigned int);
    void detectLeftClick(Level*&, std::array<Level, NUMBER_OF_LEVELS_>&, sf::Vector2f*&, std::array<sf::Vector2f, NUMBER_OF_LEVELS_>&, const sf::Vector2f&);
    void optionsFillColor();
    bool isBackPressed() const;
    void unpressBack();
private:
    sf::SoundBuffer soundBufferSelect_;
    sf::Sound soundSelect_; // sound that plays when an option is selected
    sf::Font font_;
    enum Option
    {
        LEVEL1,
        LEVEL2,
        LEVEL3,
        LEVEL4,
        BACK,
        COUNT
    };
    sf::Text options_[COUNT];
    bool isOptionPressed_[COUNT]
    {
        false,
        false,
        false,
        false,
        false,
    };
    bool isLevelSelected_[NUMBER_OF_LEVELS_]
    {
        true,
        false,
        false,
        false,
    };
    const unsigned int OPTIONS_SIZE_{50}; // character size of the options text
    const float FIRST_OPTION_TOP_{150.0f}; // top coordinates of the first option
    const float OPTIONS_SPACING_{100.0f}; // spacing between each option
};
