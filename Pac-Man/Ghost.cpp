#include <iostream>
using namespace std;
#include <cstring>
#include <ctime>
#include <conio.h>
#include "Ghost.h"
#include "Console.h";
using namespace System;

Ghost::Ghost(ConsoleColor color, COORD startingpos)
{
	ghostPos = startingpos;
	ghostColor = color;
	avoidMove = -1;
	incage = true;
	if (color == Red)
		incage = false;
	scared = White;
	pastDirection = rand() % 4;
	iscared = false;
	counter = rand();
	Draw();

}

// AI Al
void Ghost::Move(char maze[MAZE_ROWS][MAZE_COLS], Ghost * ghostPtr[], COORD playerpos)
{
	do
	{
		COORD evilmove[4] = { CreateCoord(-2, 0), CreateCoord(0, -1), CreateCoord(2, 0), CreateCoord(0, 1) };
		enum evilmove { left, up, right, down };
		COORD temp;
		bool AllowRed = true;

		while (true)
		{
			size_t i = rand() % 4;
			int t[8] = { 0, 0, 0, 0, 0, -1, 1, 2 }; // used to add 5/8 probability
			if (iscared && ghostColor == Red)
			{
				i = i + 0;
			}
			while (i == avoidMove)	//keep randomizing a choice until it is NOT avoidMove.
				i = rand() % 4;

			if (incage == true)		// increase probability of moving up by 5/8 so ghosts are more prone to exit cage
			{
				i = 1;
				i += t[rand() % 8];
			}
			if (incage && (ghostColor == Red || ghostColor == DarkYellow))
			{
				do
				{
					if (iscared)
						break;
					do
					{
						i = 1;
						i += t[rand() % 8];
					} while (i == avoidMove);

					temp = CreateCoord(ghostPos.X + evilmove[i].X, ghostPos.Y + evilmove[i].Y);


					if (incage == false && maze[temp.Y][temp.X] == MGD) continue;		//If  theyr out side the cage and go into gate: Continue.

					if ((ghostColor != Red || incage) && maze[temp.Y][temp.X] != MTL && maze[temp.Y][temp.X] != MUD && maze[temp.Y][temp.X] != MTR && maze[temp.Y][temp.X] != MLR &&
						maze[temp.Y][temp.X] != MBL && maze[temp.Y][temp.X] != MBR)
					{
						if (maze[temp.Y][temp.X] == MGD)		//If inside cage and move into gate. set data member to false.
						{
							incage = false; avoidMove = down;
						}
						break;
					}

				} while (true);
			}

			// Rudimentary position tracking algorithm based on quadrants
			//(Only applied this to two ghosts otherwise it gets impossible)
			else if ((!incage && ghostColor == Red)||  (!iscared && ghostColor == DarkYellow))
			{
				COORD secondTemp[4] = { CreateCoord(ghostPos.X + evilmove[left].X, ghostPos.Y + evilmove[left].Y),
					CreateCoord(ghostPos.X + evilmove[up].X, ghostPos.Y + evilmove[up].Y), CreateCoord(ghostPos.X + evilmove[right].X, ghostPos.Y + evilmove[right].Y),
					CreateCoord(ghostPos.X + evilmove[down].X, ghostPos.Y + evilmove[down].Y) };
				do
				{
					if (ghostPos.X > playerpos.X)	/////////////////////////////////////////////////////////////PAC IS TO THE LEFT
					{
						if (ghostPos.Y > playerpos.Y)	// IF PAC IS TO THE TOP LEFT 
						{

							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))
							{
								temp = secondTemp[up]; avoidMove = down;  break;	// MOVE UP OR
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left)) //MOVE LEFT OR
							{
								temp = secondTemp[left]; avoidMove = right; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down)) // MOVE DOWN ELSE
							{
								temp = secondTemp[down]; avoidMove = up; break;
							}
							else
							{
								temp = secondTemp[right]; avoidMove = left;  break;
							}
						}
						else if (ghostPos.Y < playerpos.Y)//////PAC IS TO THE BOTTOM LEFT
						{
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))
							{
								temp = secondTemp[down]; avoidMove = up; break;	// MOVE DOWN OR
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left)) // MOVE LEFT OR
							{
								temp = secondTemp[left]; avoidMove = right; break;
							}
							if (ghostColor == Red)
							{
								if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))
								{
									temp = secondTemp[up];  avoidMove = down; break;
								}
								else
								{
									temp = secondTemp[right]; avoidMove = left;  break;
								}
							}
							if (ghostColor = DarkYellow)
							{
								if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))
								{
									temp = secondTemp[right];  avoidMove = left; break;
								}
								else
								{
									temp = secondTemp[up]; avoidMove = down;  break;
								}
							}
						}
						else if (ghostPos.Y == playerpos.Y)//PAC IS IN OUR ROW TO THE LEFT
						{
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left))	//MOVE LEFT
							{
								temp = secondTemp[left]; avoidMove = right; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
							{
								temp = secondTemp[up];  avoidMove = down; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))		//MOVE DOWN
							{
								temp = secondTemp[down]; avoidMove = up; break;
							}
							else
							{
								temp = secondTemp[right]; avoidMove = left;  break;
							}
						}
					}
					if (ghostPos.X < playerpos.X)	////////////////////////////////////////////////////////PAC IS TO THE RIGHT
					{
						if (ghostPos.Y > playerpos.Y)	// IF PAC IS TO THE TOP RIGHT 
						{
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
							{
								temp = secondTemp[up]; avoidMove = down; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))	//MOVE RIGHT
							{
								temp = secondTemp[right]; avoidMove = left; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))	//MOVE DOWN
							{
								temp = secondTemp[down]; avoidMove = up; break;
							}
							else
							{
								temp = secondTemp[left]; avoidMove = right;  break;
							}
						}
						else if (ghostPos.Y < playerpos.Y)//PAC IS TO THE BOTTOM RIGHT
						{
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))	//MOVE DOWN
							{
								temp = secondTemp[down]; avoidMove = up; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))		//MOVE RIGHT
							{
								temp = secondTemp[right]; avoidMove = left; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
							{
								temp = secondTemp[up];  avoidMove = down; break;
							}
							else
							{
								temp = secondTemp[left]; avoidMove = right;  break;
							}
						}
						else if (ghostPos.Y == playerpos.Y)//PAC IS IN OUR ROW TO THE LEFT
						{
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))	//MOVE RIGHT
							{
								temp = secondTemp[right]; avoidMove = left; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
							{
								temp = secondTemp[up]; avoidMove = down; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))	//MOVE DOWN
							{
								temp = secondTemp[down]; avoidMove = up; break;
							}
							else
							{
								temp = secondTemp[left]; avoidMove = right;  break;
							}
						}
					}
					if (ghostPos.X == playerpos.X)	////////////////////////////////PAC SAME COLUMN AS US
					{
						if (ghostPos.Y > playerpos.Y)							// IF PAC IS TO THE TOP RIGHT 
						{
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
							{
								temp = secondTemp[up]; avoidMove = down; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))	//MOVE RIGHT
							{
								temp = secondTemp[right]; avoidMove = left; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left)) {	//MOVE LEFT
								temp = secondTemp[left]; avoidMove = right; break;}
							else
							{
								temp = secondTemp[down]; avoidMove = up;  break;
							}
						}
						else if (ghostPos.Y < playerpos.Y)								//PAC IS BELOW US
						{
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))	//MOVE DOWN
							{
								temp = secondTemp[down]; avoidMove = up; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))	//MOVE RIGHT
							{
								temp = secondTemp[right]; avoidMove = left; break;
							}
							if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left))	//MOVE LEFT
							{
								temp = secondTemp[left]; avoidMove = right; break;
							}
							else
							{
								temp = secondTemp[up]; avoidMove = down;  break;
							}
						}
					}
				} while (true);
			}
			if ((ghostColor != Red && ghostColor != DarkYellow) || (ghostColor == Red && iscared) || (ghostColor == DarkYellow && iscared))
			{	//If the Ghost is neither Red or Yellow. OR if Red and Yellow are Scared.
				temp = CreateCoord(ghostPos.X + evilmove[i].X, ghostPos.Y + evilmove[i].Y);
				if (incage == false && maze[temp.Y][temp.X] == MGD) continue;	//If  theyr out side the cage and go into gate: Continue.
				if ((incage == true) && (maze[temp.Y][temp.X] == MGD))			//If inside cage and move into gate. set data member to false.
					incage = false;
			}

			if (CoordsEqual(temp, CreateCoord(WARP_RIGHT_X, WARP_Y)))       // comparing position with warp points.
				temp = CreateCoord(WARP_LEFT_X, WARP_Y);
			else if (CoordsEqual(temp, CreateCoord(WARP_LEFT_X, WARP_Y)))
				temp = CreateCoord(WARP_RIGHT_X, WARP_Y);

			if (((ghostColor != DarkYellow && ghostColor != Red) || incage || iscared) && maze[temp.Y][temp.X] != MTL && maze[temp.Y][temp.X] != MUD && 
				maze[temp.Y][temp.X] != MTR && maze[temp.Y][temp.X] != MLR && maze[temp.Y][temp.X] != MBL && maze[temp.Y][temp.X] != MBR)
			{
				if (maze[temp.Y][temp.X] == MGD)		//Enter here if no collisions with boundaries. Then set avoidMove to the opposite of what was chosen.
					incage = false;
				if (i == left)
					avoidMove = right;
				else if (i == up)
					avoidMove = down;
				else if (i == right)
					avoidMove = left;
				else if (i == down)
					avoidMove = up;
				break;
			}
			if ((ghostColor == Red && !iscared) || (ghostColor == DarkYellow && !iscared))
				break;
		}

		TryResetSpot(maze, ghostPtr);
		ghostPos = temp;		//assign temp to be permanent position of ghost.
		Draw();
		break;
	} while (true);
}

bool Ghost::AiSmartMoveCheck(char maze[MAZE_ROWS][MAZE_COLS], COORD secondTemp[], int &avoidMove, int directions)
{
	// TEST IF MOVE direction IS POSSIBLE
	if (maze[secondTemp[directions].Y][secondTemp[directions].X] != MTL && maze[secondTemp[directions].Y][secondTemp[directions].X] != MUD &&
		maze[secondTemp[directions].Y][secondTemp[directions].X] != MTR && maze[secondTemp[directions].Y][secondTemp[directions].X] != MLR &&
		maze[secondTemp[directions].Y][secondTemp[directions].X] != MBL && maze[secondTemp[directions].Y][secondTemp[directions].X] != MBR &&
		maze[secondTemp[directions].Y][secondTemp[directions].X] != MGD && avoidMove != directions)
		return true;
	else
		return false;
	
}

void Ghost::Draw()
{
	if (iscared == false)
	{
		Console::SetCursorPosition(ghostPos.X, ghostPos.Y);
		Console::ForegroundColor(ghostColor);
		cout << MGH;
		Console::ResetColor();
	}
	else if (iscared == true)
	{
		Console::SetCursorPosition(ghostPos.X, ghostPos.Y);
		if (counter % 2 == 1)
		{
			Console::ForegroundColor(Cyan); counter++;
		}
		else if (counter % 2 == 0)
		{
			Console::ForegroundColor(scared); counter++;
		}
		cout << MGH;
		Console::ResetColor();
	}
}
void Ghost::Kill(char maze[MAZE_ROWS][MAZE_COLS])
{
	ClearSpot(maze);
	ghostPos = CreateCoord(31, 14);
	Draw();
	if (ghostColor == Red)
	{
		incage = true;
		avoidMove = 3;
	}
	else
	{
		incage = true;
		avoidMove = -1;
	}

}

// Prevent clearing a ghost if more than one are close together
void Ghost::TryResetSpot(char maze[MAZE_ROWS][MAZE_COLS], Ghost * ghostPtr[])
{
	int check = 0;
	for (size_t i = 0; i < NUM_GHOSTS; i++)
	{

		if (CoordsEqual(ghostPos, ghostPtr[i]->ghostPos))
			check++;
	}
	if (check <= 1)
		ClearSpot(maze);
}

// Reset Ghost to cage
void Ghost::Reset(char maze[MAZE_ROWS][MAZE_COLS], COORD resetTo)
{
	avoidMove = -1;
	incage = true;
	if (ghostColor == Red)
		incage = false;
	ClearSpot(maze);
	ghostPos = resetTo;
}
// Repainting the spot where the ghost was
void Ghost::ClearSpot(char maze[MAZE_ROWS][MAZE_COLS])
{
	Console::SetCursorPosition(ghostPos.X, ghostPos.Y);
	if (maze[ghostPos.Y][ghostPos.X] == MPP)
		Console::ForegroundColor(Green);
	else if (maze[ghostPos.Y][ghostPos.X] == MDOT)
		Console::ForegroundColor(Yellow);
	else if (maze[ghostPos.Y][ghostPos.X] == PAC)
		Console::ForegroundColor(Yellow);
	else if (maze[ghostPos.Y][ghostPos.X] == MGD)
		Console::ForegroundColor(Yellow);
	cout << maze[ghostPos.Y][ghostPos.X];
	Console::ResetColor();
}
