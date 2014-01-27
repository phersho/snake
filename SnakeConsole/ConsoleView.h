
#include "stdafx.h"
#include "SnakeObjects.h"

#ifndef _CONSOLE_VIEW_H_
#define _CONSOLE_VIEW_H_

#define SNAKE_CONSOLE_HEIGHT 80
#define SNAKE_CONSOLE_WIDTH 50

using namespace SnakeObjects;

namespace ConsoleView
{
	class SnakeView
	{
	private:
		HANDLE readingHandler
			, writingHandler;
		SMALL_RECT windowSize
			, consoleWriteArea;
		COORD bufferSize
			, characterInitialPosition
			, characterBufferSize;
		CHAR_INFO* consoleBuffer;

		WORD stageColor, snakeColor;
		char stageChar, snakePart
			, snakeBody, snakeHeadNorth, snakeHeadSouth, snakeHeadEast, snakeHeadWest;

		bool isRunning
			, redrawBuffer;

		Stage* stage;

		void HandleKeyboard(KEY_EVENT_RECORD& e);
		void HandleMouse(MOUSE_EVENT_RECORD& e);
		void FillConsoleRandomly();
		void InitializeView(short height, short width);
		void PlayTurn();
        char GetHeadChar();

	public:
		SnakeView(short height, short width);
		~SnakeView();

		void ClearBuffer();
		void Play();
	};
}

#endif