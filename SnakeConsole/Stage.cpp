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
		direction = SNAKE_DIRECTION_DEFAULT;
		
		snake = new Snake(3, Location(height / 2, width / 2));
		snake->SetDirection(direction);

        newParts = nullptr;
	}

	/*
	 * Frees a Stage.
	 */
	Stage::~Stage()
	{
		height = 0;
		width = 0;
		generator = nullptr;
		direction = DirectionNone;
        delete snake;

        if (newParts != nullptr)
        {
            delete newParts;
        }
        newParts = nullptr;
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

    Snake* Stage::GetSnake()
    {
        return snake;
    }

	bool Stage::MakeTurnAction(TurnSnake action)
	{
		if (action != TurnSnakeLeft && action != TurnSnakeRight)
		{
			return false;
		}

		if (action == TurnSnakeRight)
		{
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
		}

		snake->SetDirection(direction);

		return true;
	}

    void Stage::PlayTurn()
    {
        if (newParts == nullptr)
        {
            newParts = generator->Generate();
        }
        else if (newParts->empty())
        {
            delete newParts;
            newParts = generator->Generate();
        }

        snake->Advance(newParts);
    }
}