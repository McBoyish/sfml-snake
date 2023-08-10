#include "MainMenu.hpp"
#include "LevelMenu.hpp"
#include "ResultScreen.hpp"
#include "Snake.hpp"
#include "Food.hpp"
#include "Level.hpp"
#include "Tilemap.hpp"

namespace windowstate
{
    bool isMainMenuOpen{true};
    bool isGameOpen{false};
    bool isLevelMenuOpen{false};
    bool hasLostFocus{true};
    void openMainMenu()
    {
        isMainMenuOpen = true;
        isGameOpen = false;
        isLevelMenuOpen = false;
    }
    void openGame()
    {
        isMainMenuOpen = false;
        isGameOpen = true;
        isLevelMenuOpen = false;
    }
    void openLevelMenu()
    {
        isMainMenuOpen = false;
        isGameOpen = false;
        isLevelMenuOpen = true;
    }
    void closeAll()
    {
        isMainMenuOpen = false;
        isGameOpen = false;
        isLevelMenuOpen = false;
    }
}

void fillTextLevelFromTextFile(const std::string& path, std::vector<std::vector<char>>& textLevel, const Tilemap& tilemap)
{
    std::ifstream textFile(path);
    char c;
    // iterate through all the characters in the text file and store them a 2d vector
    for (int j = 0; j < tilemap.getSize().y; j++)
    {
        textLevel.push_back(std::vector<char>());
        for (int i = 0; i < tilemap.getSize().x; i++)
        {
            // read a character from the text tile
            textFile >> c;
            textLevel[j].push_back(c);
        }
    }
    textFile.close();
}

void sleep(unsigned int time)
{
    sf::Clock clock;
    while (clock.getElapsedTime().asMilliseconds() < time) continue;
}

void showResultScreen(sf::RenderWindow& window, ResultScreen& resultScreen, const Snake& snake, const Level*const& level)
{
    // set up the text to show on the result screen
    resultScreen.setResult(snake, level);
    sf::Event event;
    bool isResultScreenOpen{true};
    while (isResultScreenOpen)
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            // close everything
            case sf::Event::Closed:
                isResultScreenOpen = false;
                windowstate::closeAll();
                window.close();
                break;
            case sf::Event::LostFocus:
                if (!windowstate::hasLostFocus)
                {
                    windowstate::hasLostFocus = true;
                }
                break;
            case sf::Event::GainedFocus:
                if (windowstate::hasLostFocus)
                {
                    windowstate::hasLostFocus = false;
                }
                break;
            // return to main menu when left click is pressed
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    resultScreen.playSelectSound();
                    isResultScreenOpen = false;
                }
                break;
            default:
                break;
            }
        }
        window.clear(sf::Color::White);
        resultScreen.draw(window);
        window.display();
    }
}

int main()
{
    const float WINDOW_WIDTH{960.0f}, WINDOW_HEIGHT{800.0f};
    const sf::FloatRect TILEMAP_INNER_BOUNDS{80.0f, 116.0f, 800.0f, 640.0f};
    const float DELAY{135};
    const int NUMBER_OF_LEVELS{4};
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake", sf::Style::Close);
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(true);
    
    // construct various objects
    Snake snake;
    Food food;
    Tilemap tilemap(TILEMAP_INNER_BOUNDS);
    MainMenu mainMenu(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    LevelMenu levelMenu(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    ResultScreen resultScreen(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // vector storing the level in the form of characters where '#' represents a wall
    std::array<std::vector<std::vector<char>>, NUMBER_OF_LEVELS> textLevels;
    fillTextLevelFromTextFile(resourcePath() + "levels/level1.txt", textLevels[0], tilemap);
    fillTextLevelFromTextFile(resourcePath() + "levels/level2.txt", textLevels[1], tilemap);
    fillTextLevelFromTextFile(resourcePath() + "levels/level3.txt", textLevels[2], tilemap);
    fillTextLevelFromTextFile(resourcePath() + "levels/level4.txt", textLevels[3], tilemap);
    
    // pointers are used to select a level and its initial position for the snake
    std::array<Level, NUMBER_OF_LEVELS> levels
    {
        Level(textLevels[0], tilemap, "highscores/highscore1.txt"),
        Level(textLevels[1], tilemap, "highscores/highscore2.txt"),
        Level(textLevels[2], tilemap, "highscores/highscore3.txt"),
        Level(textLevels[3], tilemap, "highscores/highscore4.txt"),
    };
    std::array<sf::Vector2f, NUMBER_OF_LEVELS> initialPositions
    {
        tilemap.tiles()[13][8],
        tilemap.tiles()[9][7],
        tilemap.tiles()[7][13],
        tilemap.tiles()[10][12]
    };
    Level* level{&levels[0]};
    sf::Vector2f* initialPosition{&initialPositions[0]};

    // event handling
    sf::Clock clock;
    sf::Event mainMenuEvent;
    sf::Event gameEvent;
    sf::Event levelMenuEvent;
    std::list<std::string> inputBuffer;
    while (window.isOpen())
    {
        // main menu loop
        while (windowstate::isMainMenuOpen)
        {
            while (window.pollEvent(mainMenuEvent))
            {
                switch (mainMenuEvent.type)
                {
                case sf::Event::Closed:
                    windowstate::closeAll();
                    window.close();
                    break;
                case sf::Event::LostFocus:
                    if (!windowstate::hasLostFocus)
                    {
                        windowstate::hasLostFocus = true;
                    }
                    break;
                case sf::Event::GainedFocus:
                    if (windowstate::hasLostFocus)
                    {
                        windowstate::hasLostFocus = false;
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (mainMenuEvent.mouseButton.button == sf::Mouse::Left)
                    {
                        // check if any option in the main menu is pressed
                        const float x = mainMenuEvent.mouseButton.x;
                        const float y = mainMenuEvent.mouseButton.y;
                        mainMenu.detectLeftClick(sf::Vector2f(x, y));
                        if (mainMenu.isQuitPressed())
                        {
                            windowstate::closeAll();
                            window.close();
                            mainMenu.unpressQuit();
                        }
                        else if (mainMenu.isPlayPressed())
                        {
                            // initialise the game
                            snake.start(tilemap, food, level, initialPosition);
                            inputBuffer.clear();
                            windowstate::openGame();
                            mainMenu.unpressPlay();
                        }
                        else if (mainMenu.isLevelsPressed())
                        {
                            windowstate::openLevelMenu();
                            mainMenu.unpressLevels();
                        }
                    }
                default:
                    break;
                }
            }
            window.clear(sf::Color::White);
            mainMenu.draw(window);
            window.display();
        }
        
        // level menu loop
        while (windowstate::isLevelMenuOpen)
        {
            while (window.pollEvent(levelMenuEvent))
            {
                switch (levelMenuEvent.type)
                {
                case sf::Event::Closed:
                    windowstate::closeAll();
                    window.close();
                    break;
                case sf::Event::LostFocus:
                    if (!windowstate::hasLostFocus)
                    {
                        windowstate::hasLostFocus = true;
                    }
                    break;
                case sf::Event::GainedFocus:
                    if (windowstate::hasLostFocus)
                    {
                        windowstate::hasLostFocus = false;
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    if (levelMenuEvent.mouseButton.button == sf::Mouse::Left)
                    {
                        // check if any option in the level menu is pressed
                        const float x = levelMenuEvent.mouseButton.x;
                        const float y = levelMenuEvent.mouseButton.y;
                        levelMenu.detectLeftClick(level, levels, initialPosition, initialPositions, sf::Vector2f(x, y));
                        if (levelMenu.isBackPressed())
                        {
                            windowstate::openMainMenu();
                            levelMenu.unpressBack();
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            window.clear(sf::Color::White);
            levelMenu.draw(window);
            window.display();
        }
        
        // game loop
        while (windowstate::isGameOpen)
        {
            while (window.pollEvent(gameEvent))
            {
                switch (gameEvent.type)
                {
                case sf::Event::Closed:
                    windowstate::closeAll();
                    window.close();
                    break;
                case sf::Event::LostFocus:
                    if (!windowstate::hasLostFocus)
                    {
                        windowstate::hasLostFocus = true;
                    }
                    break;
                case sf::Event::GainedFocus:
                    if (windowstate::hasLostFocus)
                    {
                        windowstate::hasLostFocus = false;
                    }
                    break;
                case sf::Event::KeyPressed:
                    if (gameEvent.key.code == sf::Keyboard::W || gameEvent.key.code == sf::Keyboard::Up)
                    {
                        inputBuffer.push_back("W");
                    }
                    else if (gameEvent.key.code == sf::Keyboard::A || gameEvent.key.code == sf::Keyboard::Left)
                    {
                        inputBuffer.push_back("A");
                    }
                    else if (gameEvent.key.code == sf::Keyboard::S || gameEvent.key.code == sf::Keyboard::Down)
                    {
                        inputBuffer.push_back("S");
                    }
                    else if (gameEvent.key.code == sf::Keyboard::D || gameEvent.key.code == sf::Keyboard::Right)
                    {
                        inputBuffer.push_back("D");
                    }
                    break;
                default:
                    break;
                }
            }
            if (clock.getElapsedTime().asMilliseconds() > DELAY && !windowstate::hasLostFocus)
            {
                // update direction if there are input in the buffer
                if (inputBuffer.size() != 0)
                {
                    snake.setMovingDirection(inputBuffer.front());
                    inputBuffer.pop_front();
                }
                snake.update(food, tilemap, level);
                clock.restart();
            }
            window.clear(sf::Color::White);
            tilemap.draw(window);
            snake.draw(window);
            food.draw(window);
            level->draw(window);
            // small delay after each call to display() to ensure that the current frame lasted long enough to match the framerate limit
            window.display();
            if (snake.isDead() || snake.hasWon())
            {
                sleep(500);
                showResultScreen(window, resultScreen, snake, level);
                windowstate::openMainMenu();
            }
        }
    }
}
