#include "stdafx.h"
#include "ConsoleView.h"

namespace ConsoleView
{
	SnakeView::SnakeView(short height, short width)
	{
		InitializeView(height, width);
		ClearBuffer();
	}

	SnakeView::~SnakeView()
	{
	}

	void SnakeView::InitializeView(short height, short width)
	{
		writingHandler = GetStdHandle(STD_OUTPUT_HANDLE);
		readingHandler = GetStdHandle(STD_INPUT_HANDLE);

		windowSize = SMALL_RECT();
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = height - 1;
		windowSize.Bottom = width - 1;
		SetConsoleWindowInfo(writingHandler, TRUE, &windowSize);

		bufferSize = COORD();
		bufferSize.X = height;
		bufferSize.Y = width;
		SetConsoleScreenBufferSize(writingHandler, bufferSize);

		consoleBuffer = new CHAR_INFO[height * width];

		characterBufferSize = COORD();
		characterBufferSize.X = height;
		characterBufferSize.Y = width;

		characterInitialPosition = COORD();
		characterInitialPosition.X = 0;
		characterInitialPosition.Y = 0;

		consoleWriteArea = SMALL_RECT();
		consoleWriteArea.Left = 0;
		consoleWriteArea.Top = 0;
		consoleWriteArea.Right = height - 1;
		consoleWriteArea.Bottom = width - 1;

		backgroundChar = ' ';
		backgroundColor = BACKGROUND_BLUE | BACKGROUND_GREEN
			| BACKGROUND_RED | BACKGROUND_INTENSITY;
	}

	void SnakeView::ClearBuffer()
	{
		int countMax = characterBufferSize.X * characterBufferSize.Y;

		for (int i = 0; i < countMax; ++i)
		{
			CHAR_INFO* current = consoleBuffer + i;
			current->Char.AsciiChar = backgroundChar;
			current->Attributes = backgroundColor;
		}
	}
	
	void SnakeView::FillConsoleRandomly()
	{
		// We'll fill the console buffer with random data:
		for (int y = 0; y < bufferSize.Y; ++y)
		{
			for (int x = 0; x < bufferSize.X; ++x)
			{
				CHAR_INFO* current = consoleBuffer + (x + bufferSize.X * y);
				current->Char.AsciiChar = rand() % 256;
				current->Attributes = rand() % 256;
			}
		}
	}

	void SnakeView::Run()
	{
		DWORD numEvents = 0;
		DWORD numEventsRead = 0;
    
		isRunning = true;

		while (isRunning)
		{
			redrawBuffer = false;

			GetNumberOfConsoleInputEvents(readingHandler, &numEvents);
			if (numEvents != 0) 
			{
				INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];
				ReadConsoleInput(readingHandler, eventBuffer, numEvents, &numEventsRead);

				for (DWORD i = 0; i < numEventsRead; ++i)
				{
					if (eventBuffer[i].EventType == KEY_EVENT)
					{
						HandleKeyboard(eventBuffer[i].Event.KeyEvent);
					}
					else if (eventBuffer[i].EventType == MOUSE_EVENT)
					{
						HandleMouse(eventBuffer[i].Event.MouseEvent);
					}
				}

				// Clean up our event buffer:
				delete[] eventBuffer;
			}

			if (redrawBuffer)
			{
				WriteConsoleOutputA(writingHandler, consoleBuffer, characterBufferSize
					, characterInitialPosition, &consoleWriteArea);
			}
		}
	}

	void SnakeView::HandleKeyboard(KEY_EVENT_RECORD& e)
	{
		if (e.wVirtualKeyCode == VK_ESCAPE)
		{
			isRunning = false;
		}
		else if (e.uChar.AsciiChar == 'c' || e.uChar.AsciiChar == 'C')
		{
			ClearBuffer();
			redrawBuffer = true;
		}
		else if (e.uChar.AsciiChar == 'r' || e.uChar.AsciiChar == 'R')
		{
			FillConsoleRandomly();
			redrawBuffer = true;
		}
	}

	void SnakeView::HandleMouse(MOUSE_EVENT_RECORD& e)
	{
		// Set the index to our buffer of CHAR_INFO
		int offsetPos = e.dwMousePosition.X 
			+ bufferSize.X * e.dwMousePosition.Y;

		// left
		if (e.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) 
		{
			consoleBuffer[offsetPos].Char.AsciiChar = (char)0xDB;
			redrawBuffer = true;
		}
		// right
		else if (e.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
		{
			consoleBuffer[offsetPos].Char.AsciiChar = (char)0xB1;
			redrawBuffer = true;
		}
		// middle
		else if (e.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
		{
			consoleBuffer[offsetPos].Char.AsciiChar = backgroundChar;
			redrawBuffer = true;
		}
	}
}