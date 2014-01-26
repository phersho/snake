#include <exception>
#include "stdafx.h"
#include "SnakeObjects.h"

namespace SnakeObjects
{
	/*
	 * Creates an instance of Snake.
	 */
	Snake::Snake(int length, Location initial)
	{
		if (length < SNAKE_MIN_LENGTH)
		{
			throw new std::logic_error("Longitud inicial");
		}

		this->length = length;
		direction = SNAKE_DIRECTION_DEFAULT;
		body = Location::BuildList(initial, length, DirectionEast);
	}

	/*
	 * Unloads an instance of Snake.
	 */
	Snake::~Snake()
	{
		body.clear();
		length = 0;
		direction = DirectionNone;
	}

	/*
	 * Gets current direction.
	 */
	Direction Snake::GetDirection()
	{
		return direction;
	}

	/*
	 * Sets a new direction.
	 */
	void Snake::SetDirection(Direction newDirection)
	{
		throw new std::logic_error("Not Implemented.");
		direction = newDirection;
	}

	bool Snake::CanOverlap(Location location)
	{
		throw new std::logic_error("Not Implemented.");
		return false;
	}

	bool Snake::IsCollided()
	{
		throw new std::logic_error("Not Implemented.");
		return false;
	}

	bool Snake::IsCollided(Location location)
	{
		throw new std::logic_error("Not Implemented.");
		return false;
	}
}