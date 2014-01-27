#include "stdafx.h"

/*
 * Console implementations from [ http://benryves.com/tutorials/winconsole/ ].
 */

namespace ConsoleView
{
	SnakeView::SnakeView(short height, short width)
	{
		InitializeView(height, width);

		stage = new Stage(height, width, new SinglePartGenerator());

		ClearBuffer();
	}

	SnakeView::~SnakeView()
	{
		delete stage;
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

		stageChar = ' ';
		stageColor = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;

		snakeBody = (char)0xDB;
		snakeHeadNorth = (char)0x1E;
		snakeHeadSouth = (char)0x1F;
		snakeHeadEast = (char)0x10;
		snakeHeadWest = (char)0x11;
		snakeColor = FOREGROUND_GREEN | stageColor;

        partChar = (char)0x09;
        partColor = FOREGROUND_RED | snakeColor;
	}

	void SnakeView::ClearBuffer()
	{
		int countMax = characterBufferSize.X * characterBufferSize.Y;

		for (int i = 0; i < countMax; ++i)
		{
			CHAR_INFO* current = consoleBuffer + i;
			current->Char.AsciiChar = stageChar;
			current->Attributes = stageColor;
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

	void SnakeView::Play()
	{
		DWORD numEvents = 0;
		DWORD numEventsRead = 0;
    
		isRunning = true;

        DrawTurn();
        WriteConsoleOutputA(writingHandler, consoleBuffer, characterBufferSize
			, characterInitialPosition, &consoleWriteArea);

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
						//HandleMouse(eventBuffer[i].Event.MouseEvent);
					}
				}

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
        else if (e.wVirtualKeyCode == VK_LEFT && e.bKeyDown == TRUE)
		{
			stage->MakeTurnAction(TurnSnakeLeft);
            PlayTurn();
            redrawBuffer = true;
		}
        else if (e.wVirtualKeyCode == VK_RIGHT && e.bKeyDown == TRUE)
		{
			stage->MakeTurnAction(TurnSnakeRight);
            PlayTurn();
            redrawBuffer = true;
		}
        else if (e.wVirtualKeyCode == VK_SPACE && e.bKeyDown == TRUE)
        {
            PlayTurn();
            redrawBuffer = true;
        }
	}

	void SnakeView::HandleMouse(MOUSE_EVENT_RECORD& e)
	{
		// Set the index to our buffer of CHAR_INFO
		int offsetPos = e.dwMousePosition.X 
			+ bufferSize.X * e.dwMousePosition.Y;
		CHAR_INFO* position = consoleBuffer 
            + (e.dwMousePosition.X + bufferSize.X * e.dwMousePosition.Y);

		// left
		if (e.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) 
		{
		}
		// right
		else if (e.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
		{
		}
		// middle
		else if (e.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
		{
		}
	}

	void SnakeView::PlayTurn()
	{
        stage->PlayTurn();
        DrawTurn();
	}

    void SnakeView::DrawTurn()
    {
        LocationListPointer body = stage->GetSnake()->GetBody();
        ClearBuffer(); // TODO research how to avoid it

        for (int y = 0; y < bufferSize.Y; ++y)
        {
            for (int x = 0; x < bufferSize.X; ++x)
            {
                CHAR_INFO* current = consoleBuffer + (x + bufferSize.X * (bufferSize.Y - 1 - y));

                Location currentLocation(x, y);

                if (stage->GetSnake()->CanOverlap(currentLocation))
                {
                    current->Char.AsciiChar = stage->GetSnake()->IsHead(currentLocation) 
                        ? GetHeadChar() : snakeBody;
                    current->Attributes = snakeColor;
                }
                else if (LocationIsNewPart(currentLocation))
                {
                    current->Char.AsciiChar = partChar;
                    current->Attributes = partColor;
                }
                else
                {
                    current->Char.AsciiChar = stageChar;
                    current->Attributes = stageColor;
                }
                stage->GetGenerator();
            }
        }

        consoleBuffer[(bufferSize.X) * (bufferSize.Y)].Char.AsciiChar = '*';
    }

    char SnakeView::GetHeadChar()
    {
        switch (stage->GetSnake()->GetDirection())
        {
        case DirectionNorth:
            return snakeHeadNorth;
        case DirectionSouth:
            return snakeHeadSouth;
        case DirectionEast:
            return snakeHeadEast;
        case DirectionWest:
            return snakeHeadWest;
        }
        return snakeBody;
    }

    bool SnakeView::LocationIsNewPart(Location& location)
    {
        for (auto i = stage->GetGenerator()->Generate()->begin();
            i != stage->GetGenerator()->Generate()->end();
            ++i)
        {
            if (*i == location)
            {
                return true;
            }
        }
        return false;
    }
}