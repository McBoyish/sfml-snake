#pragma once

#include "Tilemap.hpp"
#include "Food.hpp"
#include "Level.hpp"

class Snake
{
public:
    Snake();
    void draw(sf::RenderWindow&) const;
    void update(Food&, Tilemap&, Level* const&);
    void setMovingDirection(const std::string&);
    void setSnakeTexture();
    void checkOutOfBounds(const Tilemap&);
    bool hasCollided(const Tilemap&) const;
    bool hasEaten(const Food&) const;
    void grow();
    void findFilledTiles(Tilemap&, const Level* const&) const;
    void findEmptyTiles(Tilemap&) const;
    bool isDead() const;
    bool hasWon() const;
    void start(Tilemap&, Food&, Level* const&, const sf::Vector2f*);
private:
    enum Sound
    {
        EAT,
        DIE,
        WIN,
        SOUND_COUNT
    };
    sf::SoundBuffer soundBuffers_[SOUND_COUNT];
    sf::Sound sounds_[SOUND_COUNT];
    enum HeadTexture
    {
        HEAD_UP,
        HEAD_LEFT,
        HEAD_DOWN,
        HEAD_RIGHT,
        HEAD_COUNT
    };
    enum BodyTexture
    {
        BODY_HORIZONTAL,
        BODY_VERTICAL,
        BODY_NODE1,
        BODY_NODE2,
        BODY_NODE3,
        BODY_NODE4,
        BODY_COUNT
    };
    enum TailTexture
    {
        TAIL_UP,
        TAIL_LEFT,
        TAIL_DOWN,
        TAIL_RIGHT,
        TAIL_COUNT
    };
    sf::Texture headTextures_[HEAD_COUNT];
    sf::Texture bodyTextures_[BODY_COUNT];
    sf::Texture tailTextures_[TAIL_COUNT];
    std::map<std::string, sf::Texture> bodyTexture_;
    std::vector<sf::Sprite> sprites_; // stores the snake segments
    std::vector<std::string> directions_; // stores the direction of the snake segments
    std::string key_; // stores the input
    const unsigned int INITIAL_LENGTH_{3};
    enum Direction
    {
        UP,
        LEFT,
        DOWN,
        RIGHT,
        DIRECTION_COUNT
    };
    const sf::Vector2f DIRECTION_[DIRECTION_COUNT]
    {
        {0.0f, -1.0f},
        {-1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f}
    };
    sf::Vector2f movingDirection_{DIRECTION_[RIGHT]};
    bool isDead_{false};
    bool hasWon_{false};
};
