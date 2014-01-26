
#include "stdafx.h"
#include "SnakeObjects.h"

#ifndef _CONSOLE_VIEW_H_
#define _CONSOLE_VIEW_H_

#define SNAKE_CONSOLE_HEIGHT 80
#define SNAKE_CONSOLE_WIDTH 50

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

		WORD backgroundColor;
		char backgroundChar;

		bool isRunning
			, redrawBuffer;

		void HandleKeyboard(KEY_EVENT_RECORD& e);
		void HandleMouse(MOUSE_EVENT_RECORD& e);
		void FillConsoleRandomly();
		void InitializeView(short height, short width);

	public:
		SnakeView(short height, short width);
		~SnakeView();

		void ClearBuffer();
		void Run();
	};
}

#endif