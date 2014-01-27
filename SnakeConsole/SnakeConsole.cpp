// SnakeConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SnakeObjects.h"
#include "ConsoleView.h"

using ConsoleView::SnakeView;

int _tmain(int argc, _TCHAR* argv[])
{	
	SnakeView sv = SnakeView(SNAKE_CONSOLE_HEIGHT / 2, SNAKE_CONSOLE_WIDTH / 2);

	sv.Play();

	return 0;
}
