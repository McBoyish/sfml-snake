#include "Snake.hpp"

Snake::Snake()
{
    // load the sounds
    soundBuffers_[EAT].loadFromFile(resourcePath() + "sounds/eat.wav");
    soundBuffers_[DIE].loadFromFile(resourcePath() + "sounds/die.wav");
    soundBuffers_[WIN].loadFromFile(resourcePath() + "sounds/win.wav");
    sounds_[EAT].setBuffer(soundBuffers_[EAT]);
    sounds_[DIE].setBuffer(soundBuffers_[DIE]);
    sounds_[WIN].setBuffer(soundBuffers_[WIN]);
    
    // load the head textures
    headTextures_[HEAD_UP].loadFromFile(resourcePath() + "sprites/snake/head_up.png");
    headTextures_[HEAD_LEFT].loadFromFile(resourcePath() + "sprites/snake/head_left.png");
    headTextures_[HEAD_DOWN].loadFromFile(resourcePath() + "sprites/snake/head_down.png");
    headTextures_[HEAD_RIGHT].loadFromFile(resourcePath() + "sprites/snake/head_right.png");
    
    // load the body textures
    bodyTextures_[BODY_HORIZONTAL].loadFromFile(resourcePath() + "sprites/snake/body_horizontal.png");
    bodyTextures_[BODY_VERTICAL].loadFromFile(resourcePath() + "sprites/snake/body_vertical.png");
    bodyTextures_[BODY_NODE3].loadFromFile(resourcePath() + "sprites/snake/body_node3.png");
    bodyTextures_[BODY_NODE4].loadFromFile(resourcePath() + "sprites/snake/body_node4.png");
    bodyTextures_[BODY_NODE1].loadFromFile(resourcePath() + "sprites/snake/body_node1.png");
    bodyTextures_[BODY_NODE2].loadFromFile(resourcePath() + "sprites/snake/body_node2.png");
    
    // load the tail textures
    tailTextures_[TAIL_UP].loadFromFile(resourcePath() + "sprites/snake/tail_up.png");
    tailTextures_[TAIL_LEFT].loadFromFile(resourcePath() + "sprites/snake/tail_left.png");
    tailTextures_[TAIL_DOWN].loadFromFile(resourcePath() + "sprites/snake/tail_down.png");
    tailTextures_[TAIL_RIGHT].loadFromFile(resourcePath() + "sprites/snake/tail_right.png");
    
    // initializing the map
    // each body texture is mapped to a string which contains the direction of a segment and the direction of the segment that follows it
    // "W" means up, "A" means left, "S" means down, "D" means right
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("WW", bodyTextures_[BODY_VERTICAL]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("AA", bodyTextures_[BODY_HORIZONTAL]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("SS", bodyTextures_[BODY_VERTICAL]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("DD", bodyTextures_[BODY_HORIZONTAL]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("WD", bodyTextures_[BODY_NODE4]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("AS", bodyTextures_[BODY_NODE4]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("WA", bodyTextures_[BODY_NODE3]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("DS", bodyTextures_[BODY_NODE3]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("SD", bodyTextures_[BODY_NODE1]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("AW", bodyTextures_[BODY_NODE1]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("SA", bodyTextures_[BODY_NODE2]));
    bodyTexture_.insert(std::pair<std::string, sf::Texture>("DW", bodyTextures_[BODY_NODE2]));
}

void Snake::draw(sf::RenderWindow& window) const
{
    for (int i = 0; i < sprites_.size(); i++)
    {
        window.draw(sprites_[i]);
    }
}

void Snake::update(Food& food, Tilemap& tilemap, Level* const& level)
{
    if (!isDead_ && !hasWon_)
    {
        // previous position of the head is stored to allow the other segments to follow it
        sf::Vector2f previousPosition1{sprites_[0].getPosition()};
        sf::Vector2f previousPosition2;
        
        // find all filled tiles before moving to check for collisions
        findFilledTiles(tilemap, level);
        sprites_[0].move(movingDirection_*tilemap.getTileSize());
        checkOutOfBounds(tilemap);
        if (hasCollided(tilemap))
        {
            sounds_[DIE].play();
            isDead_ = true;
            // undo the head movement so that the head does not move into the obstable
            sprites_[0].setPosition(previousPosition1);
        }
        tilemap.filledTiles.clear();
        if (!isDead_)
        {
            if (hasEaten(food))
            {
                sounds_[EAT].play();
                grow();
                level->incrementScore();
                level->updateScore();
            }
            // make the segments follow the head
            for (int i = 1; i < sprites_.size(); i++)
            {
                previousPosition2 = sprites_[i].getPosition();
                sprites_[i].setPosition(previousPosition1);
                previousPosition1 = previousPosition2;
            }
            // after setting the position, set the texture
            setSnakeTexture();
            if (hasEaten(food))
            {
                findFilledTiles(tilemap, level);
                findEmptyTiles(tilemap);
                if (!food.spawn(tilemap))
                {
                    // food not spawning means there is no empty tiles
                    // so the player has won
                    sounds_[WIN].play();
                    hasWon_ = true;
                }
            }
        }
        tilemap.emptyTiles.clear();
        tilemap.filledTiles.clear();
    }
}

void Snake::setMovingDirection(const std::string& key)
{
    key_ = key;
    if (key_ == "W" && movingDirection_ != DIRECTION_[DOWN])
    {
        movingDirection_ = DIRECTION_[UP];
    }
    else if (key_ == "A" && movingDirection_ != DIRECTION_[RIGHT])
    {
        movingDirection_ = DIRECTION_[LEFT];
    }
    else if (key_ == "S" && movingDirection_ != DIRECTION_[UP])
    {
        movingDirection_ = DIRECTION_[DOWN];
    }
    else if (key_ == "D" && movingDirection_ != DIRECTION_[LEFT])
    {
        movingDirection_ = DIRECTION_[RIGHT];
    }
}

void Snake::setSnakeTexture()
{
    // set the texture of the head
    if (key_ == "W" && movingDirection_ != DIRECTION_[DOWN])
    {
        directions_[0] = key_;
        sprites_[0].setTexture(headTextures_[HEAD_UP]);
    }
    else if (key_ == "A" && movingDirection_ != DIRECTION_[RIGHT])
    {
        directions_[0] = key_;
        sprites_[0].setTexture(headTextures_[HEAD_LEFT]);
    }
    else if (key_ == "S" && movingDirection_ != DIRECTION_[UP])
    {
        directions_[0] = key_;
        sprites_[0].setTexture(headTextures_[HEAD_DOWN]);
    }
    else if (key_ == "D" && movingDirection_ != DIRECTION_[LEFT])
    {
        directions_[0] = key_;
        sprites_[0].setTexture(headTextures_[HEAD_RIGHT]);
    }
    // before setting new textures of the body segments and the tail, save the previous directions of the snake
    std::vector<std::string> previousDirections;
    for (int i = 0; i < sprites_.size(); i++)
    {
        previousDirections.push_back(directions_[i]);
    }
    // iterate through the body segments and the tail
    for (int i = 1; i < sprites_.size(); i++)
    {
        if (i == sprites_.size() - 1)
        {
            // set tail texture based on the previous direction of the segment that follows it and then set its new direction
            if (previousDirections[i - 1] == "W")
            {
                sprites_[i].setTexture(tailTextures_[TAIL_UP]);
                directions_[i] = previousDirections[i - 1];
            }
            else if (previousDirections[i - 1] == "A")
            {
                sprites_[i].setTexture(tailTextures_[TAIL_LEFT]);
                directions_[i] = previousDirections[i - 1];
            }
            else if (previousDirections[i - 1] == "S")
            {
                sprites_[i].setTexture(tailTextures_[TAIL_DOWN]);
                directions_[i] = previousDirections[i - 1];
            }
            else if (previousDirections[i - 1] == "D")
            {
                sprites_[i].setTexture(tailTextures_[TAIL_RIGHT]);
                directions_[i] = previousDirections[i - 1];
            }
        }
        else
        {
            // set the body texture based on its direction and the previous direction of the segment that follows it and then set its new direction
            sprites_[i].setTexture(bodyTexture_.at(directions_[i] + previousDirections[i - 1]));
            directions_[i] = previousDirections[i - 1];
        }
    }
}

void Snake::checkOutOfBounds(const Tilemap& tilemap)
{
    // check if the snake goes out of the tilemap border and make it reappear on the opposite side
    if (sprites_[0].getPosition().y < tilemap.getBounds().top)
    {
        sprites_[0].setPosition(sprites_[0].getPosition().x, (tilemap.getBounds().top + tilemap.getBounds().height - tilemap.getTileSize()));
    }
    else if (sprites_[0].getPosition().x < tilemap.getBounds().left)
    {
        sprites_[0].setPosition(tilemap.getBounds().left + tilemap.getBounds().width - tilemap.getTileSize(), sprites_[0].getPosition().y);
    }
    else if (sprites_[0].getPosition().y > tilemap.getBounds().top + tilemap.getBounds().height - tilemap.getTileSize())
    {
        sprites_[0].setPosition(sprites_[0].getPosition().x, tilemap.getBounds().top);
    }
    else if (sprites_[0].getPosition().x > tilemap.getBounds().left + tilemap.getBounds().width - tilemap.getTileSize())
    {
        sprites_[0].setPosition(tilemap.getBounds().left, sprites_[0].getPosition().y);
    }
}

bool Snake::hasCollided(const Tilemap& tilemap) const
{
    // check collision between the head and all the filled tiles
    for (int i = 0; i < tilemap.filledTiles.size(); i++)
    {
        if (sprites_[0].getPosition() == tilemap.filledTiles[i])
        {
            return true;
        }
    }
    return false;
}

bool Snake::hasEaten(const Food& food) const
{
    if (sprites_[0].getPosition() == food.getPosition())
    {
        return true;
    }
    return false;
}

void Snake::grow()
{
    sprites_.push_back(sf::Sprite(tailTextures_[TAIL_RIGHT]));
    directions_.push_back("D");
}

void Snake::findFilledTiles(Tilemap& tilemap, const Level* const& level) const
{
    // store the position of all the walls and the snake
    for (int i = 0; i < level->wallSprites().size(); i++)
    {
        tilemap.filledTiles.push_back(level->wallSprites()[i].getPosition());
    }
    for (int i = 0; i < sprites_.size(); i++)
    {
        tilemap.filledTiles.push_back(sprites_[i].getPosition());
    }
}

void Snake::findEmptyTiles(Tilemap& tilemap) const
{
    // iterate through all the tiles in the tilemap
    for (int j = 0; j < tilemap.tiles().size(); j++)
    {
        for (int i = 0; i < tilemap.tiles()[0].size(); i++)
        {
            bool isEmpty{true};
            // iterate through all the filled tiles for each tile to check if the tile filled
            for (int k = 0; k < tilemap.filledTiles.size(); k++)
            {
                if (tilemap.tiles()[j][i] == tilemap.filledTiles[k])
                {
                    isEmpty = false;
                    break;
                }
            }
            // if tile is not filled, store the position of the tile
            if (isEmpty)
            {
                tilemap.emptyTiles.push_back(tilemap.tiles()[j][i]);
            }
        }
    }
}

bool Snake::isDead() const
{
    return isDead_;
}

bool Snake::hasWon() const
{
    return hasWon_;
}

void Snake::start(Tilemap& tilemap, Food& food, Level* const& level, const sf::Vector2f* initialPosition)
{
    // set the initial state of the snake
    isDead_ = false;
    hasWon_ = false;
    movingDirection_ = DIRECTION_[RIGHT];
    key_ = "D";
    sprites_.clear();
    directions_.clear();
    
    // set the initial textures
    for (int i = 0; i < INITIAL_LENGTH_; i++)
    {
        if (i == 0)
        {
            sprites_.push_back(sf::Sprite(headTextures_[HEAD_RIGHT]));
            directions_.push_back("D");
        }
        else if (i == INITIAL_LENGTH_ - 1)
        {
            sprites_.push_back(sf::Sprite(tailTextures_[TAIL_RIGHT]));
            directions_.push_back("D");
        }
        else
        {
            sprites_.push_back(sf::Sprite(bodyTextures_[BODY_HORIZONTAL]));
            directions_.push_back("D");
        }
    }
    
    // set the initial position
    for (int i = 0; i < INITIAL_LENGTH_; i++)
    {
        if (i == 0)
        {
            sprites_[0].setPosition(*initialPosition);
        }
        else
        {
            sprites_[i].setPosition(sprites_[0].getPosition().x - (i * tilemap.getTileSize()), sprites_[0].getPosition().y);
        }
    }
    
    // initialize the score and spawn the food on an empty tile
    level->resetScore();
    level->updateScore();
    findFilledTiles(tilemap, level);
    findEmptyTiles(tilemap);
    food.spawn(tilemap);
    tilemap.emptyTiles.clear();
    tilemap.filledTiles.clear();
}
