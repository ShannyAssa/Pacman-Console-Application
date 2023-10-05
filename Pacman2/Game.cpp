#include "Game.h"

// Constructor
Game::Game(Board& b)
	: theBoard(b), isColored(0), numOfGhosts(0), score(0)
{
}

// The following function sets the game according to the maze which was built and perpares it to play.
void Game::initializeMembers(int _isColored, Level _level)
{
	status = GameStatus::KEEP_PLAYING;
	updateMaxScore(theBoard.countFoodies());
	pacman.setStartingPosition(theBoard.getStartingPacman());
	numOfGhosts = theBoard.getNumOfGhosts();

	isColored = _isColored;
	if (isColored)
	{
		pacman.setObjColor(Color::YELLOW);
		fruit.setObjColor(Color::RED);
	}

	for (int i = 0; i < numOfGhosts; i++)
	{
		ghost[i].setStartingPosition(theBoard.getStartingGhost(i));

		if (isColored)
			ghost[i].setObjColor(ghoCo[i]);

		ghost[i].setLevel(_level);
		
	}
}

// The following function prints objects and details which should be printed when starting a game.
void Game::initialPrint()
{
	printScore();
	printLives();
	gotoxy(theBoard.getStartingLegend().getX(), theBoard.getStartingLegend().getY() + 2);
	cout << "press ESC to stop" << endl;

	//objects:
	pacman.draw();
	for (int i = 0; i < numOfGhosts; i++)
		ghost[i].draw();
}

// The following function gets the first move.
char Game::getFirstMove()
{
	char key = _getch();

	while (!isDirection(key))
	{
		if (_kbhit())
		{
			key = _getch();
		}
	}

	return key;
}

// The following function is the game itself.
void Game::start()
{	
	//variables
	initialPrint();
	int pacmansSteps = 0;
	int intersection = 0;
	randFruitPos();
	char key = getFirstMove();
	char formerKey = key;

	while (!pacman.isDead() && !ifWon())
	{
		if (key == ESC)
		{
			pauseGame();
			key = formerKey;
		}

		changePacmansDirection(key);

		//is intersecting
		if (checkIntersection())
		{
			if (!pacman.isDead())
				key = getFirstMove();
			continue;
		}

		updatePacmanPosition();

		//is intersecting
		if (checkIntersection())
		{
			if (!pacman.isDead())
				key = getFirstMove();
			continue;
		}

		//this allows us to slow the ghosts' & fruit's steps
		if (pacmansSteps % TWICE_SLOWER_PACE == 0)
			updateGhostsPosition();
		if (pacmansSteps % FOUR_TIMES_SLOWER == 0)
			updateFruitPosition();
		pacmansSteps++; //keeping track of the pacman's steps

		Sleep(200);

		if (_kbhit())
		{
			key = _getch();
			if (isDirection(key))
				formerKey = key;
		}
	}

	if (pacman.isDead())
		status = GameStatus::LOST;
}

/* The following function deals with the ghosts' intersections with pacman and with the fruit.
   If an instersection between pacman and a ghost occured the function returns 1. Otherwise, returns 0. */
int Game::checkIntersection()
{
	int pacmanGhostIntersection = 0;
	MatValue matValue;

	if (pacman.isIntersecting(fruit)) //pacman-fruit intersection
	{
		updateMaxScore(fruit.getValue());
		updateScore(fruit.getValue());
		printScore();
		matValue = getMatValue(Direction::STAY, fruit.getPosition().getX(), fruit.getPosition().getY());
		fruit.erase(getCompatibleFig(matValue));
		fruit.setShowFruit(0);
		randFruitPos();
		pacman.draw();
	}

	for (int i = 0; i < numOfGhosts; i++)
	{
		if (ghost[i].isIntersecting(pacman)) //pacman-ghost intersection
		{
			pacmanGhostIntersection = 1;

			pacman.updateLives();
			printLives();
			pacman.erase(static_cast<char>(PrintingFigures::NONE));
			pacman.reinitializePos();
			pacman.draw();

			for (int j = 0; j < numOfGhosts; j++)
			{
				matValue = getMatValue(Direction::STAY, ghost[j].getPosition().getX(), ghost[j].getPosition().getY());
				ghost[j].erase(getCompatibleFig(matValue));
				ghost[j].reinitializePos();
				ghost[j].draw();
			}
		}

		if (ghost[i].isIntersecting(fruit)) //ghost-fruit intersection
		{
			fruit.setShowFruit(0);
			matValue = getMatValue(Direction::STAY, fruit.getPosition().getX(), fruit.getPosition().getY());
			fruit.erase(getCompatibleFig(matValue));
			randFruitPos();
			ghost[i].draw();
		}
	}

	return pacmanGhostIntersection;
}

// The following function updates Pacman's position and deals with the board updates accordingly.
void Game::updatePacmanPosition()
{
	int x = pacman.getPosition().getX();
	int y = pacman.getPosition().getY();

	pacman.erase(static_cast<char>(PrintingFigures::NONE)); //erasing the current position of pacman from the screen

	MatValue matValue = theBoard.getValue(y, x);
	if (matValue == MatValue::FOOD)
	{
		updateScore(FOOD_ADDITION);
		theBoard.setValue(y, x, MatValue::EMPTY); //updating the board
	}

	if (!isAboutToCrossInvisibleTunnel(x, y))
	{
		pacman.updatePosByDirection();
	}

	pacman.draw();
}

// The following function checks whether pacman is about to enter an hidden tunnel.
int Game::isAboutToCrossInvisibleTunnel(int x, int y)
{
	//note: if the matVal is WALL there is nowhere to go to - in that case we do not change Pacman's position
	// side tunnels
	// left
	if (x == 0 && pacman.getDirection() == Direction::LEFT)
	{
		if(theBoard.getValue(y, theBoard.getCOLS() - 1) != MatValue::WALL)
			pacman.updatePos(theBoard.getCOLS() - 1, y);
		return 1;
	}
	//right
	else if (x == theBoard.getCOLS() - 1 && pacman.getDirection() == Direction::RIGHT)
	{
		if (theBoard.getValue(y, 0) != MatValue::WALL)
			pacman.updatePos(0, y);
		return 1;
	}

	// vertical tunnels
	//up
	else if (y == 0 && pacman.getDirection() == Direction::UP)
	{
		if (theBoard.getValue(theBoard.getROWS() - 1, x) != MatValue::WALL)
			pacman.updatePos(x, theBoard.getROWS() - 1);
		return 1;
	}
	//down
	else if (y == theBoard.getROWS() - 1 && pacman.getDirection() == Direction::DOWN)
	{
		if (theBoard.getValue(0, x) != MatValue::WALL)
			pacman.updatePos(x, 0);
		return 1;
	}

	return 0;
}

// The following function ajusts the direction the pacman should go to according to kbhit and the maze (board).
// It returns a position's status so we'll know how to operate regarding the pacman's move.
void Game::changePacmansDirection(char key)
{
	Direction currDirection=pacman.getDirection();

	switch (key)
	{
	case 'a':
		currDirection = Direction::LEFT;
		break;
	case 'A':
		currDirection = Direction::LEFT;
		break;
	case 'w':
		currDirection = Direction::UP;
		break;
	case 'W':
		currDirection = Direction::UP;
		break;
	case 'd':
		currDirection = Direction::RIGHT;
		break;
	case 'D':
		currDirection = Direction::RIGHT;
		break;
	case 'x':
		currDirection = Direction::DOWN;
		break;
	case 'X':
		currDirection = Direction::DOWN;
		break;
	case 's':
		currDirection = Direction::STAY;
		break;
	case 'S':
		currDirection = Direction::STAY;
		break;
	default:
		break;
	}

	checkNextMove(currDirection);

	pacman.setDirection(currDirection);
}

// The following function changes Pacman's direction to STAY if its about to hit a wall
void Game::checkNextMove(Direction& direction)
{
	MatValue matValue = MatValue::EMPTY;
	int x = pacman.getPosition().getX();
	int y = pacman.getPosition().getY();

	matValue = getMatValue(direction, x, y);

	if (matValue == MatValue::WALL)
	{
		direction = Direction::STAY;
	}
}

// The following function updates the ghosts' direction.
void Game::updateGhostsDirection()
{

	for (int i = 0; i < numOfGhosts; i++)
	{
		ghost[i].changeDirectionRandomly();
	}
}

// The following function updates the ghosts' position.
void Game::updateGhostsPosition()
{
	for (int i = 0; i < numOfGhosts; i++)
	{
		updateGameObjectPosition(ghost[i]);
		ghost[i].move(theBoard,pacman.getPosition());
		ghost[i].draw();
	}
}

// The following function updates the fruit's position.
void Game::updateFruitPosition()
{
	updateGameObjectPosition(fruit);
	fruit.move();

	if (fruit.getShowFruit())
		fruit.draw();
	else
		randFruitPos();
}

// The following function chooses a random valid position for the fruit. 
void Game::randFruitPos()
{
	int x, y,legendY;
	
	MatValue val =MatValue::WALL;
	legendY = theBoard.getStartingLegend().getY();

	while (val != MatValue::FOOD && val != MatValue::EMPTY)
	{
		x = rand() % theBoard.getCOLS();
		y = rand() % theBoard.getROWS();

		if (y >= legendY && y < legendY + LEGEND_HEIGHT) // prevents cases which food appears outside of the board
			val = MatValue::WALL;
		else
			val = getMatValue(Direction::STAY, x, y);

	}

	fruit.updatePos(x, y);
	fruit.setValue();
}

// The following function updates game obj position.
void Game::updateGameObjectPosition(GameObject& obj)
{
	int x, y;
	MatValue matValue;

	x = obj.getPosition().getX();
	y = obj.getPosition().getY();

	matValue = getMatValue(Direction::STAY, x, y);
	obj.erase(getCompatibleFig(matValue));

	matValue = getMatValue(obj.getDirection(), x, y); //value of the next move

	if (matValue == MatValue::WALL || isGameObjAboutToGoBeyondEdge(obj))
		obj.setDirection(Direction::STAY);
}

// The following function checks whether a game object is about to go beyond the scrren limits.
int Game:: isGameObjAboutToGoBeyondEdge(GameObject& obj)
{
	int x = obj.getPosition().getX(), y = obj.getPosition().getY();
	Direction dir = obj.getDirection();

	if (dir == Direction::UP && y == 0)
		return 1;
	if (dir == Direction::DOWN && y == theBoard.getROWS() - 1)
		return 1;
	if (dir == Direction::LEFT && x == 0)
		return 1;
	if (dir == Direction::RIGHT && x == theBoard.getCOLS() - 1)
		return 1;

	return 0;
}

// The following function allows us to get the value of a square in the board.
MatValue Game::getMatValue(Direction direction, int x, int y)
{
	MatValue matValue = MatValue::EMPTY;
	switch (direction)
	{
	case Direction::UP:
		matValue = theBoard.getValue(y - 1, x);
		break;
	case Direction::DOWN:
		matValue = theBoard.getValue(y + 1, x);
		break;
	case Direction::LEFT:
		matValue = theBoard.getValue(y, x - 1);
		break;
	case Direction::RIGHT:
		matValue = theBoard.getValue(y, x + 1);
		break;
	case Direction::STAY:
		matValue = theBoard.getValue(y, x);
		break;
	}

	return matValue;
}

//The following function returns whether a char entered is a direction.
int Game::isDirection(char key)
{
	return (key == 'a' || key == 'A' || key == 'w' || key == 'W' || key == 'd' || key == 'D' ||
		key == 'x' || key == 'X' || key == 's' || key == 'S');
}

// The following function returns the game's current status.
const GameStatus Game::getGameStatus() const
{
	return status;
}

//The following function returns a compatible figure according to a matrix value.
char Game:: getCompatibleFig(MatValue val)  const
{
	char figs = 0;
	
	switch (val)
	{
	case MatValue::FOOD:
		figs = static_cast<char>(PrintingFigures::FOOD_FIG);
		break;
	case MatValue::EMPTY:
		figs = static_cast<char>(PrintingFigures::NONE);
		break;
	}

	return figs;
}

// The following function resets members preparing it for the next one.
void Game:: resetGame()
{
	pacman.resetLives();
	score = 0;
	MAX_SCORE = 0;
}

// This function checks whether the player won.
int Game::ifWon()
{
	if (score == MAX_SCORE)
		return 1;
	else
		return 0;
}

// The following function handles a pause.
void Game::pauseGame()
{
	int x = theBoard.getStartingLegend().getX();
	int y = theBoard.getStartingLegend().getY()+2;

	gotoxy(x, y);
	cout << "                    " << endl;
	gotoxy(x, y);
	cout << "press ESC to proceed" << endl;

	char key = _getch();

	while (key != ESC)
	{
		if (_kbhit())
		{
			key = _getch();
		}
	}

	gotoxy(x, y);
	cout << "                    " << endl;
	gotoxy(x, y);
	cout << "press ESC to stop" << endl;

	return;

}

// The following function updates the max score of the game according to a certain addition.
void Game::updateMaxScore(int addition)
{
	MAX_SCORE = MAX_SCORE + addition;
}

// The following function increases the score by addition.
void Game::updateScore(int addition)
{
	score=score+ addition;
	printScore();
}

// The following function prints the score on the screen.
void Game::printScore()
{
	int x = theBoard.getStartingLegend().getX();
	int y = theBoard.getStartingLegend().getY() + 1;

	gotoxy(x, y);
	cout << "                    " << endl;
	gotoxy(x, y);
	cout << "SCORE: " << score << endl;
}

// The following function prints the score on the screen.
void Game::printLives()
{
	int x = theBoard.getStartingLegend().getX();
	int y = theBoard.getStartingLegend().getY();

	gotoxy(x, y);
	cout << "                    " << endl;
	gotoxy(x, y);
	cout << "LIVES:";

	switch (pacman.getLives())
	{
	case 1:
		cout << "<3" << endl;
		break;
	case 2:
		cout << "<3 <3" << endl;
		break;
	case 3:
		cout << "<3 <3 <3" << endl;
		break;

	}
}