#include "stdafx.h"

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

    void Location::Set(Location& other)
    {
        x = other.x;
        y = other.y;
    }

    int Location::GetDistance(const Location &other) const
    {
        return max(std::abs(other.x - x), std::abs(other.y - y));
    }

    Direction Location::GetDirection(const Location &end) const
    {
        int horizontal = end.x - x;
        int vertical = end.y - y;

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

    Location* Location::GetDestiny(Direction direction, int length) const
    {
        switch (direction)
        {
        case DirectionNorth:
            return new Location(x, y + length);
        
        case DirectionSouth:
            return new Location(x, y - length);
        
        case DirectionEast:
            return new Location(x + length, y);
        
        case DirectionWest:
            return new Location(x - length, y);
        
        default:
            throw new std::logic_error("Dirección no reconocida.");
        }
    }

    LocationListPointer Location::BuildList(const Location& start, int length, Direction direction)
    {
        LocationListPointer lstLocations = new LocationList();
        const Location& source = start;

        lstLocations->push_front(*start.GetDestiny(direction, length));
        lstLocations->push_front(Location(start.x, start.y));
        
        return lstLocations;
    }

    bool Location::IsTarget(Direction direction, const Location& other) const
    {
        int onX = other.x - x
            , onY = other.y - y;

        if (onX == 0)
        {
            return direction == DirectionNorth 
                ? onY > 0
                : (direction == DirectionSouth 
                    ? onY < 0
                    : false);
        }
        else if (onY == 0)
        {
            return direction == DirectionEast
                ? onX > 0
                : (direction == DirectionWest
                    ? onX < 0
                    : false);
        }

        return false;
    }

    bool Location::IsBetween(const Location& begin, const Location& end, bool inclusive) const
    {
        if (IsInLineWith(begin) && IsInLineWith(end))
        {
            if (inclusive)
            {
                return y == begin.y
                    ? min(begin.x, end.x) <= x && x <= max(begin.x, end.x)
                    : min(begin.y, end.y) <= y && y <= max(begin.y, end.y);
            }
            else
            {
                return y == begin.y
                    ? min(begin.x, end.x) < x && x < max(begin.x, end.x)
                    : min(begin.y, end.y) < y && y < max(begin.y, end.y);
            }
        }
        return false;
    }

    bool Location::IsInLineWith(const Location& other) const
    {
        return x == other.x || y == other.y;
    }

    bool Location::operator==(const Location& other) const
    {
        return x == other.x && y == other.y;
    }
}