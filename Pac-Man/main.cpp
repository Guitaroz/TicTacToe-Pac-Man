#include <iostream>
using namespace std;
#include <conio.h>
#include "Player.h"
#include"Ghost.h"
#include "Console.h"
using namespace System;
#include <fstream>
#include <ctime>
#include "globals.h"

void ResetCursor()
{
	Console::SetCursorPosition(0, Console::WindowHeight() - 1);
}    // Reset the console's cursor to bottom of screen
bool PlayerMove(Player * player1, char maze[MAZE_ROWS][MAZE_COLS]);
bool CheckCollision(char maze[MAZE_ROWS][MAZE_COLS], Player * playerP, Ghost * ghostA[NUM_GHOSTS]);
bool CheckWIN(char maze[MAZE_ROWS][MAZE_COLS]);
void DisplayCheat(char name[]);
void DisplayWin();
void Intro();
void PickSpeed(int &speed);
void DisplayTopScores(Player * player1);
void DisplayTopScores();
void UpdateTopScores(Player * player1);
bool AskToPlayAgain();



int main()
{
#pragma region Look Nice
	// Get the handle of the console window.
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set font size/properties so the maze fits.
	CONSOLE_FONT_INFOEX font;
	font.nFont = 0;
	font.dwFontSize.X = 10;
	font.dwFontSize.Y = 20;
	font.FontFamily = 48;
	font.FontWeight = 400;
	font.cbSize = sizeof(font);
	SetCurrentConsoleFontEx(consoleHandle, false, &font);

	// What are our current properties?
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);
	GetConsoleScreenBufferInfoEx(consoleHandle, &info);

	// Correctly colored critters.
	info.ColorTable[DarkYellow] = RGB(246, 130, 31);
	info.ColorTable[Cyan] = RGB(27, 177, 230);
	info.ColorTable[Red] = RGB(237, 27, 36);
	info.ColorTable[Magenta] = RGB(245, 153, 178);
	info.ColorTable[Yellow] = RGB(254, 242, 0);

	// Set proper dimensions with no scroll bars.
	COORD winSize = CreateCoord(MAZE_COLS, MAZE_ROWS + 2);
	info.dwSize.X = info.dwMaximumWindowSize.X = info.srWindow.Right = winSize.X;
	info.dwSize.Y = info.dwMaximumWindowSize.Y = info.srWindow.Bottom = winSize.Y;
	SetConsoleScreenBufferInfoEx(consoleHandle, &info);
	SetConsoleScreenBufferSize(consoleHandle, winSize);

	// How large are we and the entire screen?
	HWND hwnd = GetConsoleWindow();
	RECT rect;
	GetWindowRect(hwnd, &rect);
	int	w = rect.right - rect.left,
		h = rect.bottom - rect.top,
		sw = GetSystemMetrics(SM_CXSCREEN),
		sh = GetSystemMetrics(SM_CYSCREEN);

	// Center us.
	MoveWindow(hwnd, (sw - w) >> 1, (sh - h) >> 1, w, h, TRUE);

	SetConsoleTitle(L"Pac-Man"); // Unicode string!
#pragma endregion
	srand(static_cast<unsigned int>(time(nullptr)));
	bool PlayAgain = false;
	// TODO Part 2: Memory Leak Detection

#pragma endregion
	COORD startPos[NUM_GHOSTS] = { { 27, 11 }, { 23, 14 }, { 27, 14 }, { 31, 14 } };	//ghost starting positions
	ConsoleColor startColor[NUM_GHOSTS] = { Red, Cyan, Magenta, DarkYellow };			//ghost starting colors
	COORD playerStart = CreateCoord(27, 23);
	Player * player1;
	bool checkCollision;
	int frameSpeed = 120;
	bool escape = false;
	do
	{
#pragma region board

		char maze[MAZE_ROWS][MAZE_COLS] =
		{
			// 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50   51   52   53   54
			{ MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', ' ', ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, },
			{ MUD, MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, MUD, },
			{ MUD, MPP, ' ', MUD, ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', MUD, ' ', MPP, MUD, },
			{ MUD, MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', ' ', ' ', MTL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, MUD, },
			{ MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MUD, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', ' ', ' ', MUD, ' ', ' ', ' ', MUD, ' ', ' ', ' ', MTL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MUD, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, },
			{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', ' ', ' ', MBL, MLR, MLR, MLR, MBR, ' ', ' ', ' ', MBL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
			{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
			{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', ' ', ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MGD, MGD, MGD, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', ' ', ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
			{ MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MBR, ' ', ' ', ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', ' ', ' ', MBL, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, },
			{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MDOT, ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', MDOT, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
			{ MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MTR, ' ', ' ', ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', ' ', ' ', MTL, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, },
			{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', ' ', ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', ' ', ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
			{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
			{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', ' ', ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', ' ', ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', },
			{ MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MBR, ' ', ' ', ' ', MBL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', ' ', ' ', MTL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', ' ', ' ', MBL, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, },
			{ MUD, MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MBL, MLR, MLR, MLR, MTR, ' ', MUD, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MUD, ' ', MTL, MLR, MLR, MLR, MBR, ' ', MDOT, MUD, },
			{ MUD, MPP, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', ' ', ' ', ' ', ' ', ' ', ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MPP, MUD, },
			{ MBL, MLR, MLR, MTR, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MTL, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MTL, MLR, MTR, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MTL, MLR, MLR, MBR, },
			{ MTL, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MBR, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', ' ', ' ', MTL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MBL, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MTR, },
			{ MUD, MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MUD, ' ', MUD, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, ' ', MUD, ' ', ' ', ' ', MUD, ' ', MDOT, ' ', MTL, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MTR, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MBR, ' ', MDOT, ' ', MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, ' ', MDOT, MUD, },
			{ MUD, MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, ' ', MDOT, MUD, },
			{ MBL, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MLR, MBR, },
		};

#pragma endregion
		do
		{
			char playername[20];			// INFINITE LOOP ASKING FOR PLAYER NAME
			Intro();
			DisplayTopScores();
			Console::SetCursorPosition(Console::WindowHeight() / 2, 8);
			Console::ForegroundColor(DarkGreen);
			cout << "Enter your name: ";
			Console::ForegroundColor(Red);
			Console::CursorVisible(false);
			if (cin.get(playername, 20))
			{
				if (strlen(playername) > 15)
				{
					cin.clear();
					cin.ignore(LLONG_MAX, '\n');
					Console::ResetColor();
					Console::Clear();
					continue;
				}
				DisplayCheat(playername);
				PickSpeed(frameSpeed);
				player1 = new Player(maze, playerStart, playername);
				break;
			}Console::ResetColor();
			cin.clear();
			cin.ignore(LLONG_MAX, '\n');
		} while (true);
		Console::Clear();							//CLEAR CONSOLE AFTER NAME INPUT
		for (int i = 0; i < MAZE_ROWS; i++)		// Draw the maze.
		{
			for (int j = 0; j < MAZE_COLS; j++)
			{
				if (MPP == maze[i][j])
					Console::ForegroundColor(Green);
				else if (MDOT == maze[i][j] || MGD == maze[i][j])
					Console::ForegroundColor(Yellow);
				else if (PAC == maze[i][j])
					Console::ForegroundColor(Yellow);
				else
					Console::ForegroundColor(Blue);
				cout << maze[i][j];
			}
		};
		player1->DisplayHUD();	//Display BOTTOM HUD
		ResetCursor();
		Ghost * ghostplayer[NUM_GHOSTS] = {};	//Declare a pointer to an ARRAY of Ghost Objects.
		for (size_t i = 0; i < NUM_GHOSTS; i++)
		{
			ghostplayer[i] = new Ghost(startColor[i], startPos[i]); /*ghostplayer[i]->Draw();*/	//Assign each of the pointers to hold a newly created Ghost object.
		}
		ResetCursor();
		while (!_getch()){ ResetCursor(); };
		cin.clear();
		cin.ignore(LLONG_MAX, '\n');

		while (true)
		{
			if (!(PlayerMove(player1, maze))) // PLAYER MOVE with ASYNCKEY
			{
				escape = true;  break;
			}

			if (player1->GetPowerPellet())//CHECK FOR POWER PELLET to properly assign variable 'iscared'
			{
				for (size_t i = 0; i < NUM_GHOSTS; i++)
				{
					ghostplayer[i]->iscared = true;
				}
			}
			else if (!(player1->GetPowerPellet()))
			{
				for (size_t i = 0; i < NUM_GHOSTS; i++)
				{
					ghostplayer[i]->iscared = false;
				}
			}
			checkCollision = CheckCollision(maze, player1, ghostplayer); //CheckCollision
			if (!checkCollision)//IF WE DIDNT COLLIDE: MOVE GHOSTS & CheckAgain.
			{
				for (size_t i = 0; i < NUM_GHOSTS; i++)
				{//LOOP TO MOVE AND CHECK ALL GHOSTS
					ghostplayer[i]->Move(maze, ghostplayer, player1->GetPos());
				}
				checkCollision = CheckCollision(maze, player1, ghostplayer);
			}
			if (checkCollision)	// IF WE COLLIDED WITH A GHOST::
			{
				if (player1->GetLives() != 0)	// IF WE HAVE LIVES:
				{									//Reset Player
					player1->Reset(maze, playerStart);
					player1->DisplayHUD();
					for (size_t i = 0; i < NUM_GHOSTS; i++)	//Loop to Reset Ghosts.
					{
						ghostplayer[i]->Reset(maze, startPos[i]);
						ghostplayer[i]->Draw();
					}
					Sleep(500);
				}
				if (player1->GetLives() == 0)		//IF WE DONT HAVE LIVES, GAME OVER.
				{
					player1->DisplayHUD();
					ResetCursor();

					for (size_t i = 0; i < 20; i++)
					{
						Console::SetCursorPosition(Console::WindowWidth() / 2 - 6, Console::WindowHeight() - 1);
						Console::ForegroundColor(rand() % 5);
						cout << "GAME OVER!";
						Sleep(200);
					}
					break;
				}
			}
			if (player1->GetScores() >= 2500)//CHECK IF ALL SPOTS ARE EMPTY AFTER (SCORE > 2600)
			{
				bool checkwin = CheckWIN(maze);
				if (checkwin)
				{
					DisplayWin();
					break;
				}
			}
			//else if (GetAsyncKeyState('E') & 0x8000)
			//	break;
			Sleep(frameSpeed);
		} // end of the infinite game loop

		for (size_t i = 0; i < NUM_GHOSTS; i++)
		{
			delete ghostplayer[i];
			ghostplayer[i] = nullptr;
		}
		if (escape)
			break;
		UpdateTopScores(player1);
		delete player1;
		player1 = nullptr;
		Console::ResetColor();
		ResetCursor();
		PlayAgain = AskToPlayAgain();
		if (PlayAgain)
		{
			cin.clear();
			cin.ignore(LLONG_MAX, '\n');
			Console::Clear();
		}
	} while (PlayAgain);
		
		return 0;
}

bool AskToPlayAgain()
{
	bool temp = true;
	int width = Console::WindowWidth() / 2 - 10;
	//char[3][4] = { '' }
	Console::SetCursorPosition(width -5, 3);
	Console::ForegroundColor(Magenta);
	cout << "Would you like to play again? ";
	Console::SetCursorPosition(width + 3, 5);
	cout << "Yes";
	Console::SetCursorPosition(width + 14, 5);
	cout << "No";
	Console::SetCursorPosition(width + 3 , 6);
	cout << "---";
	while (true)
	{
		Console::SetCursorPosition(width - 3, 9);
		
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			Console::SetCursorPosition(width + 3, 6);
			cout << "   ";
			Console::SetCursorPosition(width + 14, 6);
			cout << "--";
			temp = false;
			
		}
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			Console::SetCursorPosition(width + 14, 6);
			cout << "  ";
			Console::SetCursorPosition(width + 3, 6);
			cout << "---";
			temp = true;
		}

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			break;
	}
	return temp;


}
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool PlayerMove(Player * player1, char maze[MAZE_ROWS][MAZE_COLS])
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000)
		{
			COORD offset = CreateCoord(-2, 0);
			player1->Move(maze, offset);
			player1->DisplayHUD();
			ResetCursor();
			return true;

		}
		else if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000)
		{
			COORD offset = CreateCoord(0, -1);
			player1->Move(maze, offset);
			player1->DisplayHUD();
			ResetCursor();
			return true;
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000)
		{
			COORD offset = CreateCoord(2, 0);
			player1->Move(maze, offset);
			player1->DisplayHUD();
			ResetCursor();
			return true;
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000)
		{
			COORD offset = CreateCoord(0, 1);
			player1->Move(maze, offset);
			player1->DisplayHUD();
			ResetCursor();
			return true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			return false;
		else return true;

	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CheckCollision(char maze[MAZE_ROWS][MAZE_COLS], Player * playerP, Ghost * ghostA[NUM_GHOSTS])
	{
		for (size_t i = 0; i < NUM_GHOSTS; i++)
		{
			if (CoordsEqual(ghostA[i]->GetPos(), playerP->GetPos()))
			{
				if (playerP->GetPowerPellet())
				{
					ghostA[i]->Kill(maze);
					playerP->GhostKilled();
				}
				else if (!(playerP->GetPowerPellet()))
				{
					playerP->Kill(); return true;
				}
				else
					return false;
			}
		}
		return false;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CheckWIN(char maze[MAZE_ROWS][MAZE_COLS])
	{
		for (int i = 0; i < MAZE_ROWS; i++)
		{
			for (int j = 0; j < MAZE_COLS; j++)
			{
				if (maze[i][j] == MDOT)
				{
					return false;
				}
			}

		}
		return true;
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayCheat(char name[])
	{
		if (!strcmpi(name, "Batman"))
		{
			Console::ForegroundColor(DarkYellow);
			Console::SetCursorPosition(11, 15);
			cout << "You're Batman, You have 10 lives.";
			for (size_t i = 0; i < 40; i++)
			{
				Console::SetCursorPosition(18, 5);
				Console::ForegroundColor(i % 16);
				cout << "Cheat Code Enabled";
				Sleep(50);
			}
		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void Intro()
	{
		int width = Console::WindowWidth() / 2 - 10;
		int height = 0;
		Console::SetCursorPosition(width, height);
		Console::ForegroundColor(Green);
		cout << "Welcome to Pacman!";
		Console::SetCursorPosition(4, 4);
		Console::ForegroundColor(DarkCyan);
		cout << "Use and Hold the ";
		Console::ForegroundColor(Yellow);
		cout << "arrows ";
		Console::ForegroundColor(DarkCyan);
		cout << "or the ";
		Console::ForegroundColor(Yellow);
		cout << "'awsd'";
		Console::ForegroundColor(DarkCyan);
		cout << "keys to move";
		Console::SetCursorPosition(14, 7);
		cout << "Press ESCAPE key to Exit";
		
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void PickSpeed(int &speed)
{
	int width = Console::WindowWidth() / 2 - 10 + 2;
	int height = 0;
	int choice;
	Console::ForegroundColor(DarkCyan);
		Console::SetCursorPosition(width, 9 + 1); //got lazy.
		cout << "Pick Game Speed: _";
		Console::SetCursorPosition(width + 1, 10 + 1);
		cout << "(1) Slow";
		Console::SetCursorPosition(width + 1, 11 + 1);
		cout << "(2) Normal";
		Console::SetCursorPosition(width + 1, 12 + 1);
		cout << "(3) Fast";
		Console::SetCursorPosition(width + 1, 13 + 1);
		cout << "(4) Insane!";

		do
		{
			cin.clear();
			cin.ignore(LLONG_MAX, '\n');
			Console::SetCursorPosition(width + 17, 9 + 1);
			Console::ForegroundColor(White);
			cout << "_";
			Console::SetCursorPosition(width + 18, 9 + 1);
			cout << "                             ";
			Console::SetCursorPosition(width + 17, 9 + 1);
			cin >> choice;
			if (choice == 1)
				speed = 190;
			else if (choice == 2)
				speed = 120;
			else if (choice == 3)
				speed = 90;
			else if (choice == 4)
				speed = 60;
		} while (choice > 4 || choice < 1 || cin.fail());

		return;


}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayWin()
	{
		Console::EOLWrap(false);
		int x = Console::WindowWidth() - 25;
		int y = Console::WindowHeight() - 1;

		for (size_t o = 0; o < 2; o++)
		{

			for (size_t i = 0; i < Console::WindowWidth(); i++)
			{
				Console::SetCursorPosition(x - i, y);
				int test = x - i;
				if (test < 0)
				{
					for (size_t t = 0; t < 28; t++)
					{
						Console::SetCursorPosition(0, y);
						cout << "                               ";
						Console::SetCursorPosition(t, y);
						Console::ForegroundColor(t % 16);
						cout << "You've WON!   CONGRATS!!!";
						Sleep(85);
					}
					i = Console::WindowWidth();
					continue;
				}

				Console::SetCursorPosition(x - i, y);
				Console::ForegroundColor(i % 16);
				cout << "You've WON!   CONGRATS!!!   ";
				Sleep(85);
			}

		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayTopScores(Player * player1)
{
	ifstream GetTopScores("TopScores.txt", std::ios_base::in);
	GetTopScores.clear();
	GetTopScores.seekg(0, ios::beg);
	char Names[10][20];
	char HoldMeString[20];
	int Scores[10], width = Console::WindowWidth() / 2 - 10, x = -1;

	for (size_t i = 0; i < 10; i++)
	{
		GetTopScores.getline(Names[i], 20);
		GetTopScores >> Scores[i];
		GetTopScores.getline(HoldMeString, 20);
	}
	for (size_t i = 0; i < 10; i++)
	{
		if (!(strcmp(player1->GetName(), Names[i])))
		{
			if (player1->GetScores() == Scores[i])
			{
				x = i; break; //to break loop
			}
		}
	}
	Console::SetCursorPosition(width + 5, 18);
	Console::ForegroundColor(Green);
	cout << "Top Scores:";
	for (size_t i = 0; i < 10; i++)
	{
		if (x == i)
		{
			Console::BackgroundColor(Yellow);
			Console::SetCursorPosition(width - 4, i + 20);
			Console::ForegroundColor(Red);
			cout << Names[i];
			Console::SetCursorPosition(width + 16, i + 20);
			Console::ForegroundColor(Black);
			cout << Scores[i];
			Console::ResetColor();
		}
		else
		{
			Console::SetCursorPosition(width - 4, i + 20);
			Console::ForegroundColor(Red);
			cout << Names[i];
			Console::SetCursorPosition(width + 16, i + 20);
			Console::ForegroundColor(Yellow);
			cout << Scores[i];
		}
	}
	Console::ResetColor();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void UpdateTopScores(Player *player1)
{
	int score = player1->GetScores();
	char * name = player1->GetName();
	
	ofstream WriteScores("TopScores.txt", std::ios_base::app);
	ifstream GetTopScores("TopScores.txt", std::ios_base::in);


	WriteScores << name << endl;
	WriteScores << (int)score;
	WriteScores.close();
	
	char HoldMeString[50]; int HoldMeInt, z, x = 0;
	
	do
	{
		GetTopScores.getline(HoldMeString, 50);
		GetTopScores >> HoldMeInt;
		GetTopScores.getline(HoldMeString, 50);
		x++;
	} while (!GetTopScores.eof());
	
	GetTopScores.clear();
	GetTopScores.seekg(0, ios::beg);

	char FullNamesList[100][25];
	int Scores[100];
	for (size_t i = 0; i < x; i++)
	{
		GetTopScores.getline(FullNamesList[i], 25);
		GetTopScores >> Scores[i];
		GetTopScores.getline(HoldMeString, 20);
	}

	for (int q = 0; q < x; q++)
	{
		for (int r = 1; r < x; r++)
		{
			if (Scores[r] > Scores[r - 1])
			{
				HoldMeInt = Scores[r]; strcpy(HoldMeString, FullNamesList[r]);
				Scores[r] = Scores[r - 1];  strcpy(FullNamesList[r], FullNamesList[r - 1]);
				Scores[r - 1] = HoldMeInt; strcpy(FullNamesList[r - 1], HoldMeString);
			} 
		}
	}
	ofstream OverWriteScores("TopScores.txt", std::ios_base::out);
	for (size_t i = 0; i < x ; i++)
	{
		OverWriteScores << FullNamesList[i] << '\n' << Scores[i] << '\n';
	}
	Console::Clear();
	OverWriteScores.close();
	
	DisplayTopScores(player1);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void DisplayTopScores()
{
	ifstream GetTopScores("TopScores.txt", std::ios_base::in);
	GetTopScores.clear();
	GetTopScores.seekg(0, ios::beg);
	char Names[10][20];
	char HoldMeString[20];
	int Scores[10], width = Console::WindowWidth() / 2 - 10, x = 0;

	for (size_t i = 0; i < 10; i++)
	{
		GetTopScores.getline(Names[i], 20);
		GetTopScores >> Scores[i];
		GetTopScores.getline(HoldMeString, 20);
	}
	
	Console::SetCursorPosition(width + 5, 18);
	Console::ForegroundColor(Green);
	cout << "Top Scores:";
	for (size_t i = 0; i < 10; i++)
	{
		Console::SetCursorPosition(width - 4, i + 20);
		Console::ForegroundColor(Red);
		cout << Names[i];
		Console::SetCursorPosition(width + 16, i + 20);
		Console::ForegroundColor(Yellow);
		cout << Scores[i];
	}
	Console::ResetColor();
}