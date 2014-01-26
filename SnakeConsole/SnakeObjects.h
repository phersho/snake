
#include <stack>
#include <list>
#include <exception>
#include "stdafx.h"

#ifndef __SNAKEOBJECTS_H__
#define __SNAKEOBJECTS_H__

#define SNAKE_CONSOLE_HEIGHT 80
#define SNAKE_CONSOLE_WIDTH 50
#define SNAKE_MIN_LENGTH 2
#define SNAKE_DIRECTION_DEFAULT DirectionEast

namespace SnakeObjects
{
	enum Direction
	{
		DirectionNone,
		DirectionNorth,
		DirectionSouth,
		DirectionEast,
		DirectionWest
	};

	enum Turn
	{
		TurnNone,
		TurnLeft,
		TurnRight
	};

	class Location;
	class Snake;
	class Stage;
	class IPartGenerator;

	typedef std::list<Location> LocationList;
	typedef std::stack<Location> LocationStack;

	class Location
	{
	private:
		int x, y;

	public:
		Location(int x, int y);
		~Location();

		int GetX() const;
		int GetY() const;

		int GetDistance(Location& other) const;
		Direction GetDirection(Location& start, Location& end) const;
		Location& GetDestiny(Direction direction, int length) const;

		static LocationList& BuildList(Location& start, int length, Direction direction);
	};

	class Snake
	{
	private:
		int length;
		Direction direction;
		LocationList body;

	public:
		Snake(int length, Location initial);
		~Snake();

		Direction GetDirection();
		void SetDirection(Direction newDirection);
		bool CanOverlap(Location location);
		bool IsCollided();
		bool IsCollided(Location location);
	};

	class IPartGenerator
	{
	public:
		virtual LocationList& Generate() = 0;
	};

	class SinglePartGenerator : IPartGenerator
	{
		LocationList& Generate();
	};

	class Stage
	{
	private:
		int height, width;
		Snake* snake;
		Direction direction;
		IPartGenerator* generator;

	public:
		Stage(int height, int width, IPartGenerator* generator);
		~Stage();

		int GetHeight();
		int GetWidth();
		Direction GetDirection();
		void SetDirection(Direction newDirection);
	};
}
#endif