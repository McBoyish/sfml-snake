#include "MainMenu.hpp"

MainMenu::MainMenu(const sf::Vector2f& windowSize)
{
    // load the sound
    soundBufferSelect_.loadFromFile(resourcePath() + "sounds/select.wav");
    soundSelect_.setBuffer(soundBufferSelect_);
    
    // load the font
    font_.loadFromFile(resourcePath() + "font/retro.ttf");
    
    // set up title text
    title_ = sf::Text("SNAKE", font_, TITLE_SIZE_);
    title_.setFillColor(sf::Color::Black);
    title_.setStyle(sf::Text::Bold);
    title_.setOrigin(title_.getLocalBounds().left, title_.getLocalBounds().top);
    title_.setPosition((windowSize.x - title_.getLocalBounds().width)/2, TITLE_TOP_);
    
    // set up options text
    options_[PLAY] = sf::Text("PLAY", font_, OPTIONS_SIZE_);
    options_[LEVELS] = sf::Text("LEVELS", font_, OPTIONS_SIZE_);
    options_[QUIT] = sf::Text("QUIT", font_, OPTIONS_SIZE_);
    for (int i = 0; i < COUNT; i++)
    {
        options_[i].setFillColor(sf::Color::Black);
        options_[i].setStyle(sf::Text::Bold);
        options_[i].setOrigin(options_[i].getLocalBounds().left, options_[i].getLocalBounds().top);
        options_[i].setPosition((windowSize.x - options_[i].getLocalBounds().width)/2, FIRST_OPTION_TOP_ + i*OPTIONS_SPACING_);
    }
    
    // set up credit text
    credit_ = sf::Text("CREATED BY JAKE", font_, CREDIT_SIZE);
    credit_.setFillColor(sf::Color::Black);
    credit_.setStyle(sf::Text::Bold);
    credit_.setOrigin(credit_.getLocalBounds().left, credit_.getLocalBounds().top);
    credit_.setPosition((windowSize.x - credit_.getLocalBounds().width)/2, windowSize.y - credit_.getLocalBounds().height - CREDIT_SPACING_);
}

void MainMenu::draw(sf::RenderWindow& window) const
{
    window.draw(title_);
    for (int i = 0; i < COUNT; i++)
    {
        window.draw(options_[i]);
    }
    window.draw(credit_);
}

void MainMenu::detectLeftClick(const sf::Vector2f& mousePosition)
{
    for (int i = 0; i < COUNT; i++)
    {
        if (options_[i].getGlobalBounds().contains(mousePosition))
        {
            if (i != QUIT)
            {
                soundSelect_.play();
            }
            isOptionPressed_[i] = true;
        }
    }
}

bool MainMenu::isPlayPressed() const
{
    return isOptionPressed_[PLAY];
}

bool MainMenu::isLevelsPressed() const
{
    return isOptionPressed_[LEVELS];
}

bool MainMenu::isQuitPressed() const
{
    return isOptionPressed_[QUIT];
}

void MainMenu::unpressPlay()
{
    isOptionPressed_[PLAY] = false;
}

void MainMenu::unpressLevels()
{
    isOptionPressed_[LEVELS] = false;
}

void MainMenu::unpressQuit()
{
    isOptionPressed_[QUIT] = false;
}
