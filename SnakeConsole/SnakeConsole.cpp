// SnakeConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SnakeObjects.h"

HANDLE wHnd;
HANDLE rHnd;

void Test();
void SetUpConsole();
void ClearConsoleBuffer(CHAR_INFO* consoleBuffer);
void FillConsoleRandomly(CHAR_INFO* consoleBuffer);

int _tmain(int argc, _TCHAR* argv[])
{	
	SetUpConsole();
	//Test();

	return 0;
}

/*
 * Set up console environment for the game.
 * from: http://benryves.com/tutorials/winconsole/
 */
void SetUpConsole()
{
    // Set up the handles for reading/writing:
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);

	// Change the window title:
	SetConsoleTitle(TEXT("Snake by Fernando Camacho Garza"));

	// Set up the required window size:
    SMALL_RECT windowSize = {0, 0, SNAKE_CONSOLE_HEIGHT - 1, SNAKE_CONSOLE_WIDTH - 1};
    
    // Change the console window size:
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	// Create a COORD to hold the buffer size:
    COORD bufferSize = {SNAKE_CONSOLE_HEIGHT, SNAKE_CONSOLE_WIDTH};
	
	// Change the internal buffer size:
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	// Set up the character:
	CHAR_INFO letterA;
	letterA.Char.AsciiChar = 'A';

	letterA.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY
		| BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;

	// Set up the character buffer:
    CHAR_INFO consoleBuffer[SNAKE_CONSOLE_HEIGHT * SNAKE_CONSOLE_WIDTH];

    // fill console
	ClearConsoleBuffer(consoleBuffer);

	// Set up the positions:
	COORD charBufSize = {SNAKE_CONSOLE_HEIGHT, SNAKE_CONSOLE_WIDTH};
	COORD characterPos = {0, 0};
	SMALL_RECT writeArea = {0, 0, SNAKE_CONSOLE_HEIGHT - 1, SNAKE_CONSOLE_WIDTH - 1};
	
	// Write the character:
	WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

	// How many events have happened?
    DWORD numEvents = 0;

    // How many events have we read from the console?
    DWORD numEventsRead = 0;
    
    // Boolean flag to state whether app is running or not.
    bool appIsRunning = true;

	bool redrawBuffer;

    // If we set appIsRunning to false, the program will end!
    while (appIsRunning)
	{
        // Find out how many console events have happened:
        GetNumberOfConsoleInputEvents(rHnd, &numEvents);

		redrawBuffer = false;

        // If it's not zero (something happened...)
        if (numEvents != 0) 
		{
            // Create a buffer of that size to store the events
            INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];

            // Read the console events into that buffer, and save how
            // many events have been read into numEventsRead.
            ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

            // Now, cycle through all the events that have happened:
            for (DWORD i = 0; i < numEventsRead; ++i)
			{
                // Check the event type: was it a key?
                if (eventBuffer[i].EventType == KEY_EVENT)
				{
                    // Yes! Was the key code the escape key?
                    if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
					{
                        // Yes, it was, so set the appIsRunning to false.
                        appIsRunning = false;
                    }
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c')
					{
						ClearConsoleBuffer(consoleBuffer);
						redrawBuffer = true;
					}
                }
				else if (eventBuffer[i].EventType == MOUSE_EVENT)
				{
					// Set the index to our buffer of CHAR_INFO
					int offsetPos = eventBuffer[i].Event.MouseEvent.dwMousePosition.X 
						+  SNAKE_CONSOLE_HEIGHT * eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;

					// Is it a left click?
					if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) 
					{
						// Yep, so set with character 0xDB.
						consoleBuffer[offsetPos].Char.AsciiChar = (char)0xDB;
						consoleBuffer[offsetPos].Char.UnicodeChar= (wchar_t)0xDB;
						redrawBuffer = true;
					}
					// Is it a right click?6
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
					{
						// Yep, so set with character 0xB1 (50% block)
                        consoleBuffer[offsetPos].Char.AsciiChar = (char)0xB1;
						consoleBuffer[offsetPos].Char.UnicodeChar= (wchar_t)0xB1;
						redrawBuffer = true;
					}
					// Is is a middle click?
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
					{
						// clear position
						consoleBuffer[offsetPos].Char.AsciiChar = ' ';
						consoleBuffer[offsetPos].Char.UnicodeChar = L' ';
						redrawBuffer = true;
					}
				}
            }

            // Clean up our event buffer:
            delete[] eventBuffer;
        }

		if (redrawBuffer)
		{
			//WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
			//WriteConsoleOutput(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
			WriteConsoleOutputW(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
		}
	}
}

/*
 * Clear the CHAR_INFO buffer:
 */
void ClearConsoleBuffer(CHAR_INFO* consoleBuffer)
{
	for (int i = 0; i < SNAKE_CONSOLE_HEIGHT * SNAKE_CONSOLE_WIDTH; ++i)
	{
		// Fill it with white-backgrounded spaces
		consoleBuffer[i].Char.AsciiChar = ' ';
		consoleBuffer[i].Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN
			| BACKGROUND_RED | BACKGROUND_INTENSITY;
	}
}
void FillConsoleRandomly(CHAR_INFO* consoleBuffer)
{
    // We'll fill the console buffer with random data:
	for (int y = 0; y < SNAKE_CONSOLE_WIDTH; ++y)
	{
        for (int x = 0; x < SNAKE_CONSOLE_HEIGHT; ++x)
		{
            // An ANSI character is in the range 0-255,
            // so use % to keep it in this range.
            consoleBuffer[x + SNAKE_CONSOLE_HEIGHT * y].Char.AsciiChar = rand() % 256;

            // The colour is also in the range 0-255,
            // as it is a pair of 4-bit colours.
            consoleBuffer[x + SNAKE_CONSOLE_HEIGHT * y].Attributes = rand() % 256;
        }
    }
}
void Test()
{
	SnakeObjects::Snake* snake = new SnakeObjects::Snake(3, SnakeObjects::Location(5, 5));



	delete snake;
}
