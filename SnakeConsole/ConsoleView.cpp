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

        CONSOLE_CURSOR_INFO cursor = { 1, FALSE };
        SetConsoleCursorInfo(writingHandler, &cursor);

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

        errorChar = (char)0x0F;
        errorColor = BACKGROUND_RED | BACKGROUND_INTENSITY
            | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
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
        snakeCanContinue = true;

        DrawTurn();
        WriteConsoleOutputA(writingHandler, consoleBuffer, characterBufferSize
            , characterInitialPosition, &consoleWriteArea);

        while (isRunning)
        {
            ResetControl();

            if (GAME_MODE_ENABLED)
            {
                Sleep(GAME_SLEEP);
            }

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
                }

                delete[] eventBuffer;
            }

            if (GAME_MODE_ENABLED)
            {
                hasToPlayTurn = true;
            }

            if (hasToPlayTurn)
            {
                PlayTurn();
            }

            if (redrawBuffer)
            {
                WriteConsoleOutputA(writingHandler, consoleBuffer, characterBufferSize
                    , characterInitialPosition, &consoleWriteArea);
            }
        }
    }

    void SnakeView::HandleKeyboard(const KEY_EVENT_RECORD& e)
    {
        if (e.wVirtualKeyCode == VK_ESCAPE)
        {
            isRunning = false;
        }
        else if (e.wVirtualKeyCode == VK_LEFT && e.bKeyDown == TRUE)
        {
            lastKey = VK_LEFT;
            hasToPlayTurn = true;
        }
        else if (e.wVirtualKeyCode == VK_RIGHT && e.bKeyDown == TRUE)
        {
            lastKey = VK_RIGHT;
            hasToPlayTurn = true;
        }
        else if (!GAME_MODE_ENABLED && e.wVirtualKeyCode == VK_SPACE && e.bKeyDown == TRUE)
        {
            hasToPlayTurn = true;
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
        if (snakeCanContinue)
        {
            UpdateState();
            DrawTurn();
        }
    }

    void SnakeView::DrawTurn()
    {
        LocationListPointer body = stage->GetSnake()->GetBody();
        LocationPointer currentLocation;
        CHAR_INFO* current;

        for (int y = 0; y < bufferSize.Y; ++y)
        {
            for (int x = 0; x < bufferSize.X; ++x)
            {
                current = consoleBuffer + CONSOLE_GET(x, y, bufferSize.X, bufferSize.Y);

                currentLocation = new Location(x, y);

                if (stage->GetSnake()->CanOverlap(*currentLocation))
                {
                    current->Char.AsciiChar = snakeBody;
                    current->Attributes = snakeColor;
                }
                else if (LocationIsNewPart(*currentLocation))
                {
                    current->Char.AsciiChar = partChar;
                    current->Attributes = partColor;
                }
                else
                {
                    current->Char.AsciiChar = stageChar;
                    current->Attributes = stageColor;
                }
                
                delete currentLocation;
            }
        }

        if (snakeCanContinue)
        {
            current = consoleBuffer 
                + CONSOLE_GET(body->front().GetX(), body->front().GetY(), stage->GetWidth(), stage->GetHeight());
            current->Char.AsciiChar = GetHeadChar();
            current->Attributes = snakeColor;
        }
        else
        {
            if (stage->GetSnake()->IsCollidedInside())
            {
                current = consoleBuffer 
                    + CONSOLE_GET(body->front().GetX(), body->front().GetY(), stage->GetWidth(), stage->GetHeight());
                current->Char.AsciiChar = errorChar;
                current->Attributes = errorColor;
            }

            consoleBuffer[0].Char.AsciiChar = 'F';
            consoleBuffer[1].Char.AsciiChar = 'A';
            consoleBuffer[2].Char.AsciiChar = 'I';
            consoleBuffer[3].Char.AsciiChar = 'L';
            consoleBuffer[0].Attributes = errorColor;
            consoleBuffer[1].Attributes = errorColor;
            consoleBuffer[2].Attributes = errorColor;
            consoleBuffer[3].Attributes = errorColor;
        }

        redrawBuffer = true;
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

    bool SnakeView::LocationIsNewPart(const Location& location)
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

    void SnakeView::UpdateState()
    {
        if (lastKey == VK_LEFT)
        {
            stage->MakeTurnAction(TurnSnakeLeft);
        }
        else if (lastKey == VK_RIGHT)
        {
            stage->MakeTurnAction(TurnSnakeRight);
        }

        stage->PlayTurn();

        snakeCanContinue = stage->IsSnakeContained()
            && !stage->GetSnake()->IsCollidedInside();
    }

    void SnakeView::ResetControl()
    {
        lastKey = 0;
        redrawBuffer = false;
        hasToPlayTurn = false;
    }
}