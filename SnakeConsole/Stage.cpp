#include "stdafx.h"
#include "SnakeObjects.h"
#include <exception>

namespace SnakeObjects
{
    /*
     * Creates an instance of Stage.
     */
    Stage::Stage(int height, int width, IPartGenerator* generator)
    {
        this->height = height;
        this->width = width;
        this->generator = generator;
        generator->Stage = this;
        direction = SNAKE_DIRECTION_DEFAULT;
        
        snake = new Snake(2, Location(height / 2, width / 2));
        snake->SetDirection(direction);
    }

    /*
     * Frees a Stage.
     */
    Stage::~Stage()
    {
        height = 0;
        width = 0;
        delete generator;
        direction = DirectionNone;
        delete snake;
    }

    /*
     * Gets height in the Stage.
     */
    int Stage::GetHeight()
    {
        return height;
    }

    /*
     * Gets width in the Stage.
     */
    int Stage::GetWidth()
    {
        return width;
    }

    /*
     * Gets current direction of the Snake.
     */
    Direction Stage::GetDirection()
    {
        return direction;
    }

    /*
     * Gets snake.
     */
    Snake* Stage::GetSnake()
    {
        return snake;
    }

    /*
     * Gets part-maker generator.
     */
    IPartGenerator* Stage::GetGenerator()
    {
        return generator;
    }

    /*
     * Turns direction by relative position and action (left/right).
     */
    bool Stage::MakeTurnAction(TurnSnake action)
    {
        if (action != TurnSnakeLeft && action != TurnSnakeRight)
        {
            return false;
        }

        switch (direction)
        {
        case DirectionNorth:
            direction = action == TurnSnakeRight ? DirectionEast : DirectionWest;
            break;
        case DirectionEast:
            direction = action == TurnSnakeRight ? DirectionSouth : DirectionNorth;
            break;
        case DirectionSouth:
            direction = action == TurnSnakeRight ? DirectionWest : DirectionEast;
            break;
        case DirectionWest:
            direction = action == TurnSnakeRight ? DirectionNorth : DirectionSouth;
            break;
        }

        snake->SetDirection(direction);

        return true;
    }

    /*
     * Play a turn game by current state.
     */
    void Stage::PlayTurn()
    {
        snake->Advance(generator->Generate());
    }

    /*
     * Indicates if snake is contained inside the stage.
     */
    bool Stage::IsSnakeContained() const
    {
        return snake->IsInside(0, 0, width - 1, height - 1);
    }
}