// SnakeConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SnakeObjects.h"
#include "ConsoleView.h"

using ConsoleView::SnakeView;

void Test();

int _tmain(int argc, _TCHAR* argv[])
{	
	SnakeView sv = SnakeView(SNAKE_CONSOLE_HEIGHT, SNAKE_CONSOLE_WIDTH);

	sv.Run();

	return 0;
}

void Test()
{
	SnakeObjects::Snake* snake = new SnakeObjects::Snake(3, SnakeObjects::Location(5, 5));
	delete snake;
}
