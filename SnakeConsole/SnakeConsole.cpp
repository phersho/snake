// SnakeConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SnakeObjects.h"
#include "ConsoleView.h"

using ConsoleView::SnakeView;

int _tmain(int argc, _TCHAR* argv[])
{	
	SnakeView sv = SnakeView(SNAKE_CONSOLE_HEIGHT, SNAKE_CONSOLE_WIDTH);

	sv.Play();

	return 0;
}
