#pragma once
#include "Board.h"
#include "GameObject.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Direction.h"
#include "Fruit.h"
#include "gotoxy.h"
#include "Level.h"
#include "GameStatus.h"
#include <ctime> 
#include <cstdlib>

class Game
{
	//consts
	static const int MAX_GHOSTS = 4;
	static const int ESC = 27;
	static const int FOOD_ADDITION = 1;
	static const int TWICE_SLOWER_PACE = 2;
	static const int FOUR_TIMES_SLOWER = 4;
	static const int LEGEND_HEIGHT = 3;

	//game objects and their features
	Pacman pacman;
	Ghost ghost[MAX_GHOSTS];
	Fruit fruit;
	Color ghoCo[4] = { Color::PINK, Color::LIGHTBLUE, Color::PURPULE, Color::GREEN };
	int isColored;
	int numOfGhosts;

	//others
	Board& theBoard;
	int score=0,MAX_SCORE=0;
	GameStatus status = GameStatus::KEEP_PLAYING;

public:

	Game(Board& b);

	//updating and printing data
	void initialPrint();
	void updateScore(int addition);
	void printLives();
	void printScore();
	void updateMaxScore(int addition);

	//handling game objects
	void updatePacmanPosition();
	void changePacmansDirection(char key);
	void updateGhostsDirection();
	void updateGhostsPosition();
	void updateGameObjectPosition(GameObject& obj);
	void updateFruitPosition();
	void randFruitPos();

	//game-conctrol functions
	char getFirstMove();
	void start();
	int checkIntersection();
	int isAboutToCrossInvisibleTunnel(int x, int y);
	void checkNextMove(Direction& direction);
	int isGameObjAboutToGoBeyondEdge(GameObject & obj);
	void pauseGame();
	int ifWon();

	//checking and updating data
	void initializeMembers(int _isColored,Level _level);
	MatValue getMatValue(Direction direction, int x, int y);
	int isDirection(char key);
	const GameStatus getGameStatus() const;
	void resetGame();
	char getCompatibleFig(MatValue val) const;
};

