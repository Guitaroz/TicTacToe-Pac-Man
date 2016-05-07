#ifndef GHOST_H
#define GHOST_H
#include "globals.h"
#include "Console.h"
using namespace System;



class Ghost
{
public:
	Ghost(ConsoleColor color, COORD startingpos);
	void Move(char maze[MAZE_ROWS][MAZE_COLS], Ghost * ghostPtr[], const COORD playerpos);
	void Draw();
	void Kill(char maze[MAZE_ROWS][MAZE_COLS]);
	void Reset(char maze[MAZE_ROWS][MAZE_COLS], COORD resetTo);
	COORD GetPos() { return ghostPos; }
	bool iscared; //Ghost is running away from player
	bool incage;
private:
	void TryResetSpot(char maze[MAZE_ROWS][MAZE_COLS], Ghost * ghostPtr[]);
	void ClearSpot(char maze[MAZE_ROWS][MAZE_COLS]);
	bool AiSmartMoveCheck(char maze[MAZE_ROWS][MAZE_COLS], COORD move[], int &avoidmove, int directions);
	COORD ghostPos;

	int avoidMove;
	int pastDirection; // Ghost heading direction
	
	int counter; // counter used to switch colors when ghosts are scared.
	ConsoleColor ghostColor;
	ConsoleColor scared;
};

#endif