// TODO Part 1: Method Definitions for Player
#include <iostream>
using namespace std;
#include <cstring>
#include <ctime>
#include <conio.h>
#include "Player.h"
#include "Console.h"
#include <cstring>
#include <fstream>
using namespace System;

   


Player::Player(char maze[MAZE_ROWS][MAZE_COLS], COORD startingpos, char playername[])
{
	pos = startingpos;
	size_t length = strlen(playername) + 1;
	name = new char[length];
	strcpy_s(name, length, playername);
	score = 0;
	lives = 3;
	if (!(strcmpi(name, "Batman")))
	{
		lives = 10;
	}
	PowerPellet = false;
	frames = 0;
	Draw(maze);
}
Player::~Player() { delete[] name; }

// Player Input
void Player::Move(char maze[MAZE_ROWS][MAZE_COLS], COORD movement)
{
	
	if (PowerPellet == true)		// Taking care of frames during PowerPellet Time
		frames--;
	if (frames == 0)			//Ends duration of PowerPellet
		PowerPellet = false;

	COORD test = CreateCoord(pos.X + movement.X, pos.Y + movement.Y);  // Lets test if future position is a boundary
	if (maze[test.Y][test.X] == MTL || maze[test.Y][test.X] == MTR || maze[test.Y][test.X] == MBL 
		|| maze[test.Y][test.X] == MBR || maze[test.Y][test.X] == MUD
		|| maze[test.Y][test.X] == MLR || maze[test.Y][test.X] == MGD)
		return;
	{
		if (CoordsEqual(test, CreateCoord(WARP_RIGHT_X, WARP_Y)))       // comparing position with warp points.
		test = CreateCoord(WARP_LEFT_X, WARP_Y);
		else if (CoordsEqual(test, CreateCoord(WARP_LEFT_X, WARP_Y)))
		test = CreateCoord(WARP_RIGHT_X, WARP_Y); 
	}
	if (maze[test.Y][test.X] == MDOT)    //Increase score if position = dot
		score += 10;
	if (maze[test.Y][test.X] == MPP)	//increase score if position is a power pellet.
	{
		score += 50; PowerPellet = true; frames = 50;
	}
	
	ClearSpot(maze);
	pos = test;
	Draw(maze);
}

void Player::DisplayHUD()
{
	Console::SetCursorPosition(0, 31);
	Console::ForegroundColor(Green);
	cout << "Player: ";
	Console::ForegroundColor(Red); 
	cout <<name;
	Console::SetCursorPosition(34, MAZE_ROWS);
	Console::ForegroundColor(Yellow);
	cout << "Score: ";
	Console::ForegroundColor(Cyan); 
	cout << score;
	Console::SetCursorPosition(MAZE_COLS - 9, MAZE_ROWS);
	Console::ForegroundColor(Yellow);
	if (lives == 3)
		cout << PAC << " " << PAC  << " " << PAC;
	else if (lives == 2)
		cout << PAC << " " << PAC<< "  ";
	else if (lives == 1)
		cout << PAC << "    ";
	else if (lives == 0)
		cout << "      ";
	else if (lives <= 10 && lives > 3)
		cout << PAC << " x" << lives << " ";
	//cout << "Lives: ";
	Console::ForegroundColor(Green);
	//cout << lives << " ";
	Console::ResetColor();

}

void Player::Draw(char maze[MAZE_ROWS][MAZE_COLS])
{
	maze[pos.Y][pos.X] = PAC;
	Console::ForegroundColor(Yellow);
	Console::SetCursorPosition(pos.X, pos.Y);
	cout << maze[pos.Y][pos.X];
	Console::ResetColor();
}
void Player::ClearSpot(char maze[MAZE_ROWS][MAZE_COLS])
{
	maze[pos.Y][pos.X] = ' ';
	Console::SetCursorPosition(pos.X, pos.Y);
	cout << maze[pos.Y][pos.X];

}
void Player::Reset(char maze[MAZE_ROWS][MAZE_COLS], COORD resetTo)
{
	ClearSpot(maze);
	pos = resetTo;
	Draw(maze);
}
