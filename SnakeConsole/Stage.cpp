#include <exception>
#include "stdafx.h"
#include "SnakeObjects.h"

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
	}

	/*
	 * Unload a Stage.
	 */
	Stage::~Stage()
	{
		height = 0;
		width = 0;
		generator = nullptr;
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
	 * Sets new direction.
	 */
	void Stage::SetDirection(Direction newDirection)
	{
		throw new std::logic_error("Not Implemented.");
		direction = newDirection;
		snake->SetDirection(newDirection);

		//TODO must ends implementation
	}
}