
#include "stdafx.h"
#include "SnakeObjects.h"

#ifndef _CONSOLE_VIEW_H_
#define _CONSOLE_VIEW_H_

#define CONSOLE_GET(x, y, lx, ly) ((x) + ((lx) * ((ly) - (y) - 1)))

#define SNAKE_CONSOLE_HEIGHT 20
#define SNAKE_CONSOLE_WIDTH 20
#define GAME_SLEEP 1000
#define GAME_MODE_ENABLED false

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

        WORD lastKey;

        WORD stageColor, snakeColor, partColor, errorColor;
        char stageChar, partChar, errorChar
            , snakeBody, snakeHeadNorth, snakeHeadSouth, snakeHeadEast, snakeHeadWest;

        bool isRunning, hasToPlayTurn, snakeCanContinue
            , redrawBuffer;

        Stage* stage;

        void HandleKeyboard(const KEY_EVENT_RECORD& e);
        void HandleMouse(MOUSE_EVENT_RECORD& e);
        void FillConsoleRandomly();
        void InitializeView(short height, short width);
        void PlayTurn();
        char GetHeadChar();
        void DrawTurn();
        bool LocationIsNewPart(const Location& location);
        void ResetControl();

    public:
        SnakeView(short height, short width);
        ~SnakeView();

        void ClearBuffer();
        void Play();
        void UpdateState();
    };
}

#endif