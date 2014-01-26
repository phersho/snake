#include "stdafx.h"
#include "SnakeObjects.h"

namespace SnakeObjects
{
	Location::Location(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Location::~Location()
	{
		x = 0;
		y = 0;
	}

	int Location::GetX() const
	{
		return x;
	}

	int Location::GetY() const
	{
		return y;
	}

	int Location::GetDistance(Location &other) const
	{
		return max(std::abs(other.x - x), std::abs(other.y - y));
	}

	Direction Location::GetDirection(Location &start, Location &end) const
	{
		int horizontal = end.x - start.x;
		int vertical = end.y - start.y;

		if (horizontal == 0 && vertical == 0)
		{
			return DirectionNone;
		}

		if (horizontal == 0)
		{
			return vertical > 0
				? DirectionNorth : DirectionSouth;
		}
		else
		{
			return horizontal > 0
				? DirectionEast : DirectionWest;
		}
	}

	Location& Location::GetDestiny(Direction direction, int length) const
	{
		switch (direction)
		{
		case DirectionNorth:
			return *(new Location(x, y + length));
		
		case DirectionSouth:
			return *(new Location(x, y - length));
		
		case DirectionEast:
			return *(new Location(x + length, y));
		
		case DirectionWest:
			return *(new Location(x - length, y));
		
		default:
			throw new std::logic_error("Dirección no reconocida.");
		}
	}

	LocationList& Location::BuildList(Location& start, int length, Direction direction)
	{
		LocationList *lstLocations = new LocationList();
		Location& source = start;

		lstLocations->push_front(start.GetDestiny(direction, length));
		lstLocations->push_front(start);

		return *lstLocations;
	}
}