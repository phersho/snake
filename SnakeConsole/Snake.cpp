#include "stdafx.h"
#include "SnakeObjects.h"

#include <exception>

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
		body = Location::BuildList(initial, length, DirectionWest);
	}

	/*
	 * Unloads an instance of Snake.
	 */
	Snake::~Snake()
	{
        body->clear();
        delete body;

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
		direction = newDirection;
	}

	/*
	 * Gets access to body inside.
	 */
	LocationListPointer Snake::GetBody()
	{
		return body;
	}

	bool Snake::CanOverlap(Location& location)
	{
        Location previous = body->front();
        for (auto current = body->begin(); current != body->end(); ++current)
        {
            if (previous == *current)
            {
                continue;
            }

            if (location.IsBetween(previous, *current))
            {
                return true;
            }
        }
		return false;
	}

	bool Snake::IsTarget(Location& location)
	{
        return body->front().IsTarget(direction, location);
	}

	bool Snake::IsHead(Location& other)
	{
        return other == body->front();
	}

	bool Snake::IsCollided(Location location)
	{
        return IsTarget(location) && body->front().GetDistance(location) == 0;
	}

    void Snake::Advance(LocationListPointer list)
    {
        Location head = body->front();
        bool withoutNews = true;

        // evaluates if snake will have new parts
        for (auto i = list->begin(); i != list->end(); ++i)
        {
            if (head.IsTarget(direction, *i)
                && head.GetDistance(*i) != 0)
            {
                head.Set(*i);
                withoutNews = false;
                break;
            }
        }

        if (withoutNews)
        {
            Location lastChecked = head;
            
            for (auto i = list->begin(); i != list->end(); ++i)
            {
                if (lastChecked == *i)
                {
                    continue;
                }


            }
        }
    }
}