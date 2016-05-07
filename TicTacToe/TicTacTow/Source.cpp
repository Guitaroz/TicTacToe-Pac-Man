#include <iostream>
using namespace std;
#include <cstring>
#include <ctime>
#include "Console.h"
using namespace System;

void DrawBoard(char matrix[][3], short * playerscore, short * aiscore);	//DrawsBoard
void PlayerInput(short PlayerChoice, char player, char matrix[][3], short * playerscore, short * aiscore);//Gets input from Player
void AiInput(short aiChoice, char ai, char matrix[][3], char player);		//Gets input from Ai (strategy included in function)
bool Tiecheck(char matrix[][3]);											//Checks if there are still empty spaces in the board
short TestWin(char matrix[][3], char player, char ai);						// Checks if Win/Lose
void ClearBoard(char matrix[][3]);											//Resets the board if you want to play again.
void DisplayResult(short result, short * playerscore, short * aiscore);	// Displays whether you won, lost, or tie.
void ChooseSymbol(char * playerptr, char * aiptr);							// Which symbol do you want to use? X/O?
#define LOSE 0
#define WIN 1
#define TIE 2

int main()
{ 
	srand(static_cast<unsigned int>(time(nullptr)));
	Console::SetWindowSize(48, 17);
	Console::SetBufferSize(48, 17);
#pragma region variables	

	char player = 'X';
	char * playerptr = &player;	// pointer to player symbol so we can change it.
	char ai = 'O';
	char * aiptr = &ai;			//pointer to ai symbol.
	short playerChoice = 0;
	short aiChoice = 0;
	char toQuit = ' ';	        // used at the end of the program to determine if they want to play again
	short result = 3;		    //going to be used to determine if WIN = 1, if Lose = 0, if Tie = 2
	short playerscoreCount = 0;	//score thruoghout games
	short aiscoreCount = 0;
	short * playerscore = &playerscoreCount;	// pointer to score so we can change it.
	short * aiscore = &aiscoreCount;
	char matrix[3][3] =		    //Declaring the matrix. assigning all spots to '_' to represent they're empty.
	{ '_', '_', '_',
	  '_', '_', '_',
	  '_', '_', '_'
	};

#pragma endregion
	do
	{
		ChooseSymbol(playerptr, aiptr);			//What symbol do you want to use?
		bool whogoesfirst = rand() % 2;			//who goes first variable?
		do										
		{	
			//infinite loop that breaks when Tie/Win/Lose
			DrawBoard(matrix, playerscore, aiscore);				// Draw Board	
			
			if (whogoesfirst == 1)
				PlayerInput(playerChoice, player, matrix, playerscore, aiscore);//Assigning first turn. either Ai or Player takes turn.
			else if (whogoesfirst == 0)
				AiInput(aiChoice, ai, matrix, player);
			DrawBoard(matrix, playerscore, aiscore);							//Draw Board
			
			result = TestWin(matrix, player, ai);				// Test Win/Lose, catch it in 'result' variable.
			if (result == 1 || result == 0)						// If win/lose, break out loop.
				break;
			if (Tiecheck(matrix))		     					//Test for Tie
			{
				result = 2;  break;
			}								
			
			if (whogoesfirst == 1)								// Assigning second turn based on who went first.
				AiInput(aiChoice, ai, matrix, player);					// Then we repeat the code.
			else if (whogoesfirst == 0)
				PlayerInput(playerChoice, player, matrix, playerscore, aiscore);			
			DrawBoard(matrix, playerscore, aiscore);		
			result = TestWin(matrix, player, ai);			    
			if (result == 1 || result == 0)
				break;
			if (Tiecheck(matrix))
			{
				result = 2;  break;
			}						
		} while (true);
		DisplayResult(result, playerscore, aiscore);			//After win/tie/lose, Display said result.
		
		do
		{													// Prompt to play again? Put it in infinite loop for input validation.
			Console::SetCursorPosition(8, 9);
			Console::ForegroundColor(DarkCyan);
			cout << "Would you like to play again? y/n: _\b";
			Console::ForegroundColor(Yellow);
			if (cin >> toQuit && (toQuit == 'n' || toQuit == 'N' || toQuit == 'y' || toQuit == 'Y'))
				break;										// if player entered one of these characters, break out of loop.
			cin.clear();																			
			cin.ignore(LLONG_MAX, '\n');
			Console::Clear();
			DrawBoard(matrix, playerscore, aiscore);
			DisplayResult(result, playerscore, aiscore);
		} while (true);

		if (toQuit != 'n' && toQuit != 'N')				
			ClearBoard(matrix);									//Clear board if player chose to play again.
	} while (toQuit != 'n' && toQuit != 'N');
	Console::ForegroundColor(DarkCyan); 
	Console::SetCursorPosition(18, 11);
	cout << "Goodbye!";

		_sleep(600);
	return 0;
}
void DrawBoard(char matrix[][3], short * playerscore, short * aiscore)							
{
	Console::Clear();
	Console::SetCursorPosition(14, 0);
	Console::ForegroundColor(Yellow);
	cout << "Let's Play Tic Tac Toe!\n\n";
	
	for (size_t row = 0; row < 3; row++)
	{	
		Console::ForegroundColor(Yellow);
		Console::SetCursorPosition(20, row + 3);
		for (size_t col = 0; col < 3; col++)					//2 nested for loops to draw the board. Extra code is for color coding.
		{
			if (matrix[row][col] == 'X')
			{
				cout << '['; Console::ForegroundColor(Magenta);
				cout << matrix[row][col]; Console::ForegroundColor(Yellow);						
				cout << ']';
			}
			else if (matrix[row][col] == 'O')
			{
				cout << '['; Console::ForegroundColor(Cyan);
				cout << matrix[row][col]; Console::ForegroundColor(Yellow);
				cout << ']';
			}
			else 
				cout << '[' << matrix[row][col] << ']';
		}
		cout << "\n";
		Console::SetCursorPosition(0, 16);
		Console::ForegroundColor(Green);
		cout << "Your score: " << *playerscore;
		Console::ForegroundColor(Red);
		cout<< "\t\tComputer score: " << *aiscore;
	}
	Console::CursorVisible(false);
}
void PlayerInput(short playerChoice, char player, char matrix[][3], short * playerscore, short * aiscore)
{
	do
	{
		Console::SetCursorPosition(14, 8);
		Console::ForegroundColor(Green);
		cout << "Pick a spot (1 -> 9): _\b";
		if (!(cin >> playerChoice) || playerChoice > 9)
		{
			cin.clear();
			cin.ignore(LLONG_MAX, '\n');
			Console::Clear();
			DrawBoard(matrix, playerscore, aiscore);
			continue;
		}
		playerChoice--;
		short rowChosen = playerChoice / 3;
		short colChosen = playerChoice % 3;
		
		if (matrix[rowChosen][colChosen] == '_')
		{
			matrix[rowChosen][colChosen] = player;
			break;
		}
		else
			cout << "That spot is already taken!";
	} while (true);
}
void AiInput(short aiChoice, char ai, char matrix[][3], char player)
{


	do
	{
		short airowChosen;
		short aicolChosen;

#pragma region Offensive Horz and Vert
		for (size_t row = 0; row < 3; row++)									//horizontal
		{
			size_t col = 0;
			if (matrix[row][col] == ai && matrix[row][col + 1] == ai)
			{
				if (matrix[row][col + 2] == '_')
				{
					matrix[row][col + 2] = ai; return;
				}
			}
			if (matrix[row][col + 1] == ai && matrix[row][col + 2] == ai)
			{
				if (matrix[row][col] == '_')
				{
					matrix[row][col] = ai; return;
				}
			}
			if (matrix[row][col] == ai && matrix[row][col + 2] == ai)
			{
				if (matrix[row][col + 1] == '_')
				{
					matrix[row][col + 1] = ai; return;
				}
			}
		}
		for (size_t col = 0; col < 3; col++)									//vertical 
		{
			size_t row = 0;
			if (matrix[row][col] == ai && matrix[row + 1][col] == ai)
			{
				if (matrix[row + 2][col] == '_')
				{
					matrix[row + 2][col] = ai; return;
				}
			}
			if (matrix[row + 1][col] == ai && matrix[row + 2][col] == ai)
			{
				if (matrix[row][col] == '_')
				{
					matrix[row][col] = ai; return;
				}
			}
			if (matrix[row][col] == ai && matrix[row + 2][col] == ai)
			{
				if (matrix[row + 1][col] == '_')
				{
					matrix[row + 1][col] = ai; return;
				}
			}
		}
#pragma endregion
#pragma region Offensive Diagonal
		if (matrix[0][0] == ai && matrix[1][1] == ai)
		{
			if (matrix[2][2] == '_')
			{
				matrix[2][2] = ai; return;
			}
		}
		if (matrix[1][1] == ai && matrix[2][2] == ai)
		{
			if (matrix[0][0] == '_')
			{
				matrix[0][0] = ai; return;
			}
		}
		if (matrix[2][0] == ai && matrix[1][1] == ai)
		{
			if (matrix[0][2] == '_')
			{
				matrix[0][2] = ai; return;
			}
		}
		if (matrix[1][1] == ai && matrix[0][2] == ai)
		{
			if (matrix[2][0] == '_')
			{
				matrix[2][0] = ai; return;
			}
		}
		if ((matrix[0][0] == ai && matrix[2][2] == ai) || (matrix[2][0] == ai && matrix[0][2] == ai))
		{
			if (matrix[1][1] == '_')
			{
				matrix[1][1] = ai; return;
			}
		}
#pragma endregion
#pragma region Defensive horizontal vertical
		for (size_t row = 0; row < 3; row++)									//horizontal 
		{
			size_t col = 0;
			if (matrix[row][col] == player && matrix[row][col + 1] == player)
			{
				if (matrix[row][col + 2] == '_')
				{
					matrix[row][col + 2] = ai; return;
				}
			}
			if (matrix[row][col + 1] == player && matrix[row][col + 2] == player)
			{
				if (matrix[row][col] == '_')
				{
					matrix[row][col] = ai; return;
				}
			}
			if (matrix[row][col] == player && matrix[row][col + 2] == player)
			{
				if (matrix[row][col + 1] == '_')
				{
					matrix[row][col + 1] = ai; return;
				}
			}
		}
		for (size_t col = 0; col < 3; col++)									//vertical 
		{
			size_t row = 0;
			if (matrix[row][col] == player && matrix[row + 1][col] == player)
			{
				if (matrix[row + 2][col] == '_')
				{
					matrix[row + 2][col] = ai; return;
				}
			}
			if (matrix[row + 1][col] == player && matrix[row + 2][col] == player)
			{
				if (matrix[row][col] == '_')
				{
					matrix[row][col] = ai; return;
				}
			}
			if (matrix[row][col] == player && matrix[row + 2][col] == player)
			{
				if (matrix[row + 1][col] == '_')
				{
					matrix[row + 1][col] = ai; return;
				}
			}
		}
#pragma endregion 
#pragma region Defensive Diagonal ai
		if (matrix[0][0] == player && matrix[1][1] == player)
		{
			if (matrix[2][2] == '_')
			{
				matrix[2][2] = ai; return;
			}
		}
		if (matrix[1][1] == player && matrix[2][2] == player)
		{
			if (matrix[0][0] == '_')
			{
				matrix[0][0] = ai; return;
			}
		}
		if (matrix[2][0] == player && matrix[1][1] == player)
		{
			if (matrix[0][2] == '_')
			{
				matrix[0][2] = ai; return;
			}
		}
		if (matrix[1][1] == player && matrix[0][2] == player)
		{
			if (matrix[2][0] == '_')
			{
				matrix[2][0] = ai; return;
			}
		}
		if ((matrix[0][0] == player && matrix[2][2] == player) || (matrix[2][0] == player && matrix[0][2] == player))
		{
			if (matrix[1][1] == '_')
			{
				matrix[1][1] = ai; return;
			}
		}
#pragma endregion

		if (matrix[1][1] == player)
		{
			if (matrix[0][0] == '_' || matrix[2][0] == '_' || matrix[0][2] == '_' || matrix[2][2] == '_')
			{
				do
				{
					size_t i = rand() % 4 + 1;
					switch (i)
					{
					case 1:
						if (matrix[0][0] = '_')
							matrix[0][0] = ai; return;
					case 2:
						if (matrix[0][2] = '_')
							matrix[0][2] = ai; return;
					case 3:
						if (matrix[2][0] = '_')
							matrix[2][0] = ai; return;
					case 4:
						if (matrix[2][2] = '_')
							matrix[2][2] = ai; return;
					}
				} while (true);
			}
		}
		aiChoice = rand() % 9;
		airowChosen = aiChoice / 3;
		aicolChosen = aiChoice % 3;
		if (matrix[airowChosen][aicolChosen] == '_'/* || matrix[airowChosen][aicolChosen] == ai*/)
		{
			matrix[airowChosen][aicolChosen] = ai;
			break;
		}
	} while(true);
}
bool Tiecheck(char matrix[][3])
{
	for (size_t row = 0; row < 3; row++)
	{
		for (size_t col = 0; col < 3; col++)
		{
			if (matrix[row][col] == '_')
				return false;
		}
	}
	return true;
}
short TestWin(char matrix[][3], char player, char ai)
{
	for (size_t row = 0; row < 3; row++)									//horizontal wins
	{	size_t col = 0;
		if (matrix[row][col] == player && matrix[row][col + 1] == player && matrix[row][col + 2] == player)
			return WIN;
	}
	for (size_t col = 0; col < 3; col++)																	
	{
		size_t row = 0;																//checking vertical wins
		{
			if (matrix[row][col] == player && matrix[row + 1][col] == player && matrix[row + 2][col] == player)
				return WIN;
		}
	}
	if (matrix[0][0] == player && matrix[1][1] == player && matrix[2][2] == player)
		return WIN;																							//diagonal wins
	if (matrix[2][0] == player && matrix[1][1] == player && matrix[0][2] == player)
		return WIN;
	
	///////////////////////////////////////////////////////////////////////CHECK FOR LOSES NOW////////////////////////////////////
	for (size_t row = 0; row < 3; row++)
	{
		size_t col = 0;
		if (matrix[row][col] == ai && matrix[row][col + 1] == ai && matrix[row][col + 2] == ai)				// checking horizontal loses
			return LOSE;
	}
	for (size_t col = 0; col < 3; col++)																	//checking vertical LOSEs
		{	size_t row = 0;
			if (matrix[row][col] == ai && matrix[row + 1][col] == ai && matrix[row + 2][col] == ai)
				return LOSE;
		}
	if (matrix[0][0] == ai && matrix[1][1] == ai && matrix[2][2] == ai)
		return LOSE;																							//diagonal LOSEs
	if (matrix[2][0] == ai && matrix[1][1] == ai && matrix[0][2] == ai)
		return LOSE;
	else
		return 3; // means nothing. returning a random number.
	
}
void ClearBoard(char matrix[][3])
{
	for (size_t row = 0; row < 3; row++)
	{
		Console::SetCursorPosition(19, row + 3);
		for (size_t col = 0; col < 3; col++)
		{
			matrix[row][col] = '_';
		}
	}
}
void DisplayResult(short result, short * playerscore, short * aiscore)
{
	switch (result)
	{
	case LOSE:
		Console::SetCursorPosition(13, 7);
		Console::ForegroundColor(Red);
		cout << "The Computer Broke You!"; *aiscore = *aiscore + 1; break; 
	case WIN:
		Console::SetCursorPosition(16, 7);
		Console::ForegroundColor(Green);
		cout << "You Win! Keep it up!"; *playerscore = *playerscore + 1; break;
	case TIE:
		Console::SetCursorPosition(13, 7);
		Console::ForegroundColor(Gray);
		cout << "Stalemate! Let The War Continue!"; break;
	}
}
void ChooseSymbol(char * playerptr, char * aiptr)
{
	char reply;
	do
	{
		Console::Clear();
		Console::SetCursorPosition(6, 0);
		Console::ForegroundColor(Yellow);
		cout << "What symbol would you like to use?";
		Console::SetCursorPosition(10, 2);
		cout << "1) "; Console::ForegroundColor(Magenta); cout << "X\t\t\t";
		Console::ForegroundColor(Yellow); cout << "2) "; Console::ForegroundColor(Cyan); cout << 'O'; Console::CursorVisible(false);
		Console::SetCursorPosition(22, 6);
		Console::ForegroundColor(Yellow);
		cout << "_\b";
		cin >> reply;
		if (cin.good() && (reply == '2' || reply == 'o' || reply == 'O' || reply == '1' || reply == 'x' || reply == 'X'))
			break;
		cin.clear();
		cin.ignore(LLONG_MAX, '\n');
	} while (true);
	if (reply == '2' || reply == 'o' || reply == 'O')
	{
		*playerptr = 'O'; *aiptr = 'X';
	}
	if (reply == '1' || reply == 'x' || reply == 'X')
	{
		*playerptr = 'X'; *aiptr = 'O';
	}
}