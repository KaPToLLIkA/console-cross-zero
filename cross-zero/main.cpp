#include <iostream>
#include <Windows.h>
#include "game.h"



static Map gameMap = defaultMap3x3;
static Team currentTeam = Team::player;
static bool winningF = false;


void consoleOut(Text &outText);




int main()
{
    SHORT mX = 0, mY = 0;
    Team prevTeam = currentTeam;


    HANDLE   hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleMode(hConsoleIn, ENABLE_MOUSE_INPUT);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    while (true)
    {
        

        Sleep(10);
        

        unsigned long numEvents, numEventsRead;
        GetNumberOfConsoleInputEvents(hConsoleIn, &numEvents);
        if (numEvents != 0)
        {
            INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
            ReadConsoleInput(hConsoleIn, eventBuffer, numEvents, &numEventsRead);
            for (DWORD i = 0; i < numEventsRead; ++i)
            {
                if (eventBuffer[i].EventType == KEY_EVENT)
                {
                    gameMap = defaultMap3x3;
                    winningF = false;
                }
                
                if (eventBuffer[i].EventType == MOUSE_EVENT)
                {
                    
                    if (eventBuffer[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                    {
                        if (!winningF) {
                            mX = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
                            mY = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y % rows;

                            prevTeam = currentTeam;
                            currentTeam = procSelect(currentTeam, mX, mY, defaultPoints3x3, gameMap);

                            winningF = winning(gameMap, prevTeam);

                            if (getEmptyCells(gameMap).size() == 0) {
                                prevTeam = Team::none;
                                winningF = true;
                                continue;
                            }

                            if (currentTeam == Team::ai) {
                                Move bestMove = minimax(gameMap, currentTeam);

                                prevTeam = currentTeam;
                                
                                gameMap[bestMove.y][bestMove.x] = Cell::cross;

                                currentTeam = nextTeam(currentTeam);

                                winningF = winning(gameMap, prevTeam);

                                if (getEmptyCells(gameMap).size() == 0) {
                                    prevTeam = Team::none;
                                    winningF = true;
                                    continue;
                                }
                            }

                        }

                    }
                }
            }

            
            Text out = getTextFromMap(gameMap);
            
            out.push_back(std::to_string(mX) + "   ");
            out.push_back(std::to_string(mY) + "   ");
            out.push_back("Press any key to restart...");

            if (winningF) {
                switch (prevTeam) {
                case Team::ai: out.push_back("AI win!          "); break;
                case Team::player: out.push_back("Player win!     "); break;
                case Team::none: 
                    out.push_back("Draw...       ");
                    currentTeam = Team::player;
                    break;

                }
            }
            else {
                out.push_back("                   ");
            }

            consoleOut(out);
            delete[] eventBuffer;
        }
        
    }

    FreeConsole();

    return 0;
}

void consoleOut(Text &outText)
{
    static bool firstOut = true;
    static bool bufferOne = true;
    static HANDLE firstScreenBuffer, secondScreenBuffer;
    if (firstOut) {
        firstScreenBuffer =
            CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        secondScreenBuffer =
            CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        CONSOLE_SCREEN_BUFFER_INFOEX Info;
        Info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
        GetConsoleScreenBufferInfoEx(firstScreenBuffer, &Info);
        SetConsoleScreenBufferInfoEx(firstScreenBuffer, &Info);
        GetConsoleScreenBufferInfoEx(secondScreenBuffer, &Info);
        SetConsoleScreenBufferInfoEx(secondScreenBuffer, &Info);

        firstOut = false;
    }

    for (size_t i = 0; i < outText.size(); ++i) {

        DWORD count;

        if (bufferOne) {
            WriteConsoleOutputCharacter(firstScreenBuffer, outText[i].c_str(), outText[i].size(), { 0, static_cast<short>(i) }, &count);
        }
        else {
            WriteConsoleOutputCharacter(secondScreenBuffer, outText[i].c_str(), outText[i].size(), { 0, static_cast<short>(i) }, &count);
        }
    }
    
    if (bufferOne) {
        SetConsoleActiveScreenBuffer(firstScreenBuffer);
    }
    else {
        SetConsoleActiveScreenBuffer(secondScreenBuffer);
    }
    bufferOne = ~bufferOne;
}

