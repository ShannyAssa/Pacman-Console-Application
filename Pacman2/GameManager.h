#pragma once
#include "gotoxy.h"
#include "Board.h"
#include "Game.h"
#include "Level.h"
#include "GameStatus.h"

class GameManager
{
	//consts
	enum choice { START = '1', START_SPECIFIC_GAME = '2', INSTRUCTIONS = '8', EXIT = '9' };
	enum levelChoice { best1 = 'a', best2 = 'A', good1 = 'b', good2 = 'B', novice1 = 'c', novice2 = 'C' };
	static const int INIT = 0;
	static const int COLORED = 1;

	Board theBoard;
	Game theGame;
	Level level;
	int isColored = 0;

public:
	
	GameManager();

	// displaying menu and messages
	void displayMenu();
	void displayInstructions() const;
	void logo() const;
	void LevelUp() const;
	void youWon();
	void youLost();
	void setLevel();
	void setColoredGame();

	// checking the user's choices
	int choiceValidation(char userChoice) const;
	int isLevel(char userChoice) const;

	// handling the game
	void runASingleGame();
	void runAFullGame();
	void startGame();
};

