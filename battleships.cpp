#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#include <unistd.h>
#include <stdio.h>
#include <ncurses.h>

using namespace std;

int nFieldWidth = 10;
int nFieldHeight = 21;
int nScreenWidth = 12; // Console screen width
int nScreenHeight = 23; // Console screen height
int nBoxOffsetX = 4;
int nBoxOffsetY = 6;
wstring battleship[14];
unsigned char *pField = nullptr;

int Rotate(int px, int py, int r)
{
    int rot = 0;
    switch(r % 4)
    {
        case 0: 
            rot = py * 5 + px; // 0 degrees
            break;
        case 1: 
            rot = 20 + py - (5 * px); // 90 degrees
            break;
        case 2: 
            rot = 24 - (5 * py) - px; // 180 degrees
            break;
        case 3: 
            rot = 4 - py + (5 * px); // 270 degrees
            break;
    }
    return rot;
}

bool DoesPieceFit(int nbattleship, int nRotation, int nPosX, int nPosY)
{
    for (int px = 0; px < 5; px++)
        for(int py = 0; py < 5; py++)
        {
            // Get index into piece
            int pi = Rotate(px, py, nRotation);

            // Get index into field
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if(nPosX + px >= 0 && nPosX + px < nFieldWidth && nPosY + py >= 0 && nPosY + py < nFieldHeight && battleship[nbattleship][pi] == L'X' && pField[fi] != 0)
            {
                return false; // fail on first hit
            }
        }
    return true;
}


int main()
{
    // Create screen buffer
    wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
    for(int i = 0; i < nScreenWidth*nScreenHeight; i++) 
        screen[i] = L' ';

    // Create screen window
    WINDOW *BattleshipLand;
    int offsetx, offsety, ch;
    initscr();
    noecho();
    cbreak();
    timeout(50);
    keypad(stdscr, TRUE);
    refresh();
    offsetx = (80 - (nFieldWidth + nBoxOffsetX))/2;
    offsety = (24 - (nFieldHeight + nBoxOffsetY))/2;
    BattleshipLand = newwin(nFieldHeight + nBoxOffsetY,nFieldWidth + nBoxOffsetX,offsety,offsetx);
    box(BattleshipLand,0,0);
    refresh();

    // Create assets
    battleship[0].append(L"...X.");
    battleship[0].append(L"...X.");
    battleship[0].append(L"...X.");
    battleship[0].append(L"...X.");
    battleship[0].append(L"...X.");

    battleship[1].append(L"..X.");
    battleship[1].append(L"..X.");
    battleship[1].append(L"..X.");
    battleship[1].append(L"..X.");
    battleship[1].append(L"....");

    battleship[2].append(L"....");
    battleship[2].append(L"..X.");
    battleship[2].append(L"..X.");
    battleship[2].append(L"..X.");
    battleship[2].append(L"....");

    battleship[3].append(L"....");
    battleship[3].append(L"..X.");
    battleship[3].append(L"..X.");
    battleship[3].append(L"..X.");
    battleship[3].append(L"....");

    battleship[4].append(L"....");
    battleship[4].append(L"..X.");
    battleship[4].append(L"..X.");
    battleship[4].append(L"..X.");
    battleship[4].append(L"....");

    battleship[5].append(L"....");
    battleship[5].append(L"..X.");
    battleship[5].append(L"..X.");
    battleship[5].append(L"..X.");
    battleship[5].append(L"....");

    battleship[6].append(L"....");
    battleship[6].append(L"..X.");
    battleship[6].append(L"..X.");
    battleship[6].append(L"....");
    battleship[6].append(L"....");

    battleship[7].append(L"...X.");
    battleship[7].append(L"...X.");
    battleship[7].append(L"...X.");
    battleship[7].append(L"...X.");
    battleship[7].append(L"...X.");

    battleship[8].append(L"..X.");
    battleship[8].append(L"..X.");
    battleship[8].append(L"..X.");
    battleship[8].append(L"..X.");
    battleship[8].append(L"....");

    battleship[9].append(L"....");
    battleship[9].append(L"..X.");
    battleship[9].append(L"..X.");
    battleship[9].append(L"..X.");
    battleship[9].append(L"....");

    battleship[10].append(L"....");
    battleship[10].append(L"..X.");
    battleship[10].append(L"..X.");
    battleship[10].append(L"..X.");
    battleship[10].append(L"....");

    battleship[11].append(L"....");
    battleship[11].append(L"..X.");
    battleship[11].append(L"..X.");
    battleship[11].append(L"..X.");
    battleship[11].append(L"....");

    battleship[12].append(L"....");
    battleship[12].append(L"..X.");
    battleship[12].append(L"..X.");
    battleship[12].append(L"..X.");
    battleship[12].append(L"....");

    battleship[13].append(L"....");
    battleship[13].append(L"..X.");
    battleship[13].append(L"..X.");
    battleship[13].append(L"....");
    battleship[13].append(L"....");

    // Create empty playing field
    pField = new unsigned char[nFieldWidth*nFieldHeight]; 
    for (int x = 0; x < nFieldWidth; x++)
        for (int y = 0; y < nFieldHeight; y++)
            pField[y*nFieldWidth + x] = (y == nFieldHeight/2) ? 9 : 0;

    // Game logic ********************
    bool bGameOver = false;
    srand((unsigned int)time(NULL));
    int nCurrentPiece = rand() % 7;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth / 2;
    int nCurrentY = 0;
    int nPieceCount;
    int nScore = 0;
    string totalScore;
	vector<int> vLines;
 
    while ((ch = getch()) != 'x') {

        if(bGameOver != true) {

            // Display game instructions on initialising ***********

            // Track ship placement and game commencement *************

            // Game Timing 
            this_thread::sleep_for(std::chrono::milliseconds(50)); // Small Step = 1 Game Tick

            // Input
            enum direction { UP, DOWN, RIGHT, LEFT, ROTATE, NONE };
            int cur_dir;
            if(ch != ERR) {
                switch(ch) {
                    case KEY_UP:
                        cur_dir = UP;
                        break;
                    case KEY_DOWN:
                        cur_dir = DOWN;
                        break;
                    case KEY_RIGHT:
                        cur_dir = RIGHT;
                        break;
                    case KEY_LEFT:
                        cur_dir = LEFT;
                        break;
                    case L'z':
                        cur_dir = ROTATE;
                        break;
                    default:
                        break;
                }
            }

            // Game Logic ================

            // Handle player placement of ships and random placement of enemy ships *******
            nCurrentX += ((cur_dir == RIGHT) && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
            nCurrentX -= ((cur_dir == LEFT) && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;		
            nCurrentY += ((cur_dir == DOWN) && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;
            nCurrentRotation += ((cur_dir == ROTATE) && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
            cur_dir = NONE;

            // Test if pieces can be placed as directed by player / enemy *******
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                nCurrentY++; // It can, so do it!
            else
            {
                // It can't! Lock the piece in place
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if (battleship[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

                // Pick New Piece
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;
                nCurrentRotation = 0;
                nCurrentPiece = rand() % 7;

                // If piece does not fit straight away, game over!
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }

            // Player shooting and enemy shooting logic **************

            // Game scoring tracking ******************

            // Render output

            // Draw playing field
            for (int x = 0; x < nFieldWidth; x++)
                for (int y = 0; y < nFieldHeight; y++)
                    screen[(y)*nScreenWidth + (x)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]];

            // Draw Current Piece
            for (int px = 0; px < 4; px++)
                for (int py = 0; py < 4; py++)
                    if (battleship[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                        screen[(nCurrentY + py)*nScreenWidth + (nCurrentX + px)] = nCurrentPiece + 65;

            // Display Frame
            for (int x = 0; x < nFieldWidth; x++)
                for (int y = 0; y < nFieldHeight; y++)
                    mvwaddch(BattleshipLand,(y + nBoxOffsetY/2),(x + nBoxOffsetX/2),screen[(y)*nScreenWidth + (x)]);

        } else {
            for (int x = 0; x < nFieldWidth; x++)
                    for (int y = 0; y < nFieldHeight; y++)
                        mvwaddch(BattleshipLand,(y + nBoxOffsetY/2),(x + nBoxOffsetX/2),L' ');
            mvwprintw(BattleshipLand,(nFieldHeight + nBoxOffsetY)/2 - 1,1,"* GAME  OVER *");
            mvwprintw(BattleshipLand,(nFieldHeight + nBoxOffsetY)/2 + 1,2,"HIT X TO END");
            wrefresh(BattleshipLand);
        }
    }
    // Delete Frame
    delwin(BattleshipLand);
    endwin();

    return 0;
}