#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "globals.h"

class Player
{
public:
	
	Player(char maze[MAZE_ROWS][MAZE_COLS], COORD startingpos, char playername[]);
	~Player();
	void Move(char maze[MAZE_ROWS][MAZE_COLS], COORD movement);
	void DisplayHUD();
	void Reset(char maze[MAZE_ROWS][MAZE_COLS], COORD resetTo);
	void Kill() { lives--; };
	COORD GetPos() { return pos; };
	int GetLives() { return lives; };
	bool GetPowerPellet() { return PowerPellet; };
	void GhostKilled() { score += 200; };
	int GetScores(){ return score; }
	char * GetName(){ return name; }



private:
	void Draw(char maze[MAZE_ROWS][MAZE_COLS]);
	void ClearSpot(char maze[MAZE_ROWS][MAZE_COLS]);
	COORD pos;
	char * name;
	int score;
	int lives;
	bool PowerPellet; 
	int frames;
};

#endif
