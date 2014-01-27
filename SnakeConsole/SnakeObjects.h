

#include "stdafx.h"

#ifndef __SNAKEOBJECTS_H__
#define __SNAKEOBJECTS_H__

#define SNAKE_MIN_LENGTH 2
#define SNAKE_DIRECTION_DEFAULT DirectionEast
#define SNAKE_MOV 1

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

	enum TurnSnake
	{
		TurnSnakeNone,
		TurnSnakeLeft,
		TurnSnakeRight
	};

	class Location;
	class Snake;
	class Stage;
	class IPartGenerator;

	typedef std::list<Location> LocationList;
    typedef LocationList* LocationListPointer;
    typedef Location* LocationPointer;

	class Location
	{
	private:
		int x, y;

	public:
		Location(int x, int y);
		~Location();

		int GetX() const;
		int GetY() const;
        void Set(Location& other);

		int GetDistance(const Location& other) const;
		LocationPointer GetDestiny(Direction direction, int length) const;
		bool IsTarget(Direction direction, const Location& other) const;
        bool IsBetween(const Location& begin, const Location& end) const;
        bool IsInLineWith(const Location& other) const;

		Direction GetDirection(const Location& end) const;
		static LocationListPointer BuildList(const Location& start, int length, Direction direction);

        bool operator==(const Location& other) const;
	};

	class Snake
	{
	private:
		int length;
		Direction direction;
		LocationListPointer body;
        int minHeight, minWidth, maxHeight, maxWidht;

        void RefreshLimits();

	public:
		Snake(int length, Location initial);
		~Snake();

		Direction GetDirection();
		void SetDirection(Direction newDirection);
		LocationListPointer GetBody();
		bool CanOverlap(Location& location);
		bool IsHead(Location& other);
		bool IsCollided(Location location);
		bool IsTarget(Location& location);
		void Advance(LocationListPointer list);
	};

	class IPartGenerator
	{
	public:
		Stage* Stage;
		virtual LocationListPointer Generate() = 0;
	};

	class SinglePartGenerator : public IPartGenerator
	{
    private:
        LocationList generated;
	public:
        SinglePartGenerator();
        ~SinglePartGenerator();
		LocationListPointer Generate();
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
        Snake* GetSnake();
        IPartGenerator* GetGenerator();
		bool MakeTurnAction(TurnSnake action);
        void PlayTurn();
	};
}

#endif