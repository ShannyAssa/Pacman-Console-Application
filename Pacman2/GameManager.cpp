#include "GameManager.h"

//Constructor
GameManager::GameManager()
	: theGame(theBoard)
{
	level = Level::NOVICE;
}

// The following function presents the game menu and gets the user's choice regarding the game.
void GameManager::displayMenu()
{
	char userChoice = INIT;

	while (!choiceValidation(userChoice) && userChoice != EXIT) 
	{
		clrscr();

		logo();
		cout << "         ********************************************************" << endl;
		cout << "         *            Press -1- to start a full game            *" << endl;
		cout << "         *          Press -2- to start a specific game          *" << endl;
		cout << "         *    Press -8- to display  the game's instruction      *" << endl;
		cout << "         *                  Press -9- to Exit                   *" << endl;
		cout << "         ********************************************************" << endl;

		userChoice = _getch();
		switch (userChoice)
		{
		case START:
			clrscr();
			userChoice = INIT;
			runAFullGame();
			break;
		case START_SPECIFIC_GAME:
			clrscr();
			userChoice = INIT;
			runASingleGame();
			break;
		case INSTRUCTIONS:
			displayInstructions();
			userChoice = INIT;
			break;
		case EXIT:
			return;
		}
	}
}

// The following function presents the game's instructions and keys.
void GameManager::displayInstructions() const
{
	clrscr();
	cout << "                -------------------------------------------------- " << endl;
	cout << "                            Welcome to Pacman ! " << endl;
	cout << "                        - C - You control Pacman - C - " << endl;
	cout << "- * - Pacman must eat all the breadcrumbs which are spreaded inside the maze - * -" << endl;
	cout << "                  - " << static_cast<char>(PrintingFigures::GHOST) << " - while avoiding the two evil ghosts - " << static_cast<char>(PrintingFigures::GHOST) << " - " << endl;
	cout << "               Each breadcrumb pacman eats entitles you with 1 point :) " << endl;
	cout << "                        Pacman has 3 lives <3 <3 <3" << endl;
	cout << "      :( Each intersection with a ghost makes Pacman lose one of his lives :( " << endl;
	cout << "         X After three instersections Pacman dies and you lose the Game! X " << endl;
	cout << "     ~ If Pacman eats all the breadcrumbs on the screen without dying you win! ~\n" << endl;
	cout << "            PAY ATTENTION : (1) Pacman can cross in two invisible tunnels" << endl;
	cout << "       from the rightmost position to the leftmost position ( and backwards :D )" << endl;
	cout << "       and from the topmost position to the downmost position ( and backwards :D )" << endl;
	cout << "                      IF there are NO WALLS at both sides." << endl;
	cout << "             (2) random FRUITS appearing on the screen as numbers" << endl;
	cout << "        between 5 TO 9 :D Eat them for BONUS SCORE before they disappear :-)\n" << endl;
	cout << "                    Pacman - @ , A breadcrumb - * , A ghost - " << static_cast<char>(PrintingFigures::GHOST) << endl;
	cout << "        You control Pacman's movement in the maze using the following keys:" << endl;
	cout << "                                 a \\ A for LEFT " << endl;
	cout << "                                 d \\ D for RIGHT " << endl;
	cout << "                                 w \\ W for UP " << endl;
	cout << "                                 x \\ X for DOWN " << endl;
	cout << "                                 s \\ S for STAY " << endl;
	cout << "                                   GOOD LUCK !\n" << endl;
	cout << "                    PRESS ANY KEY TO GO BACK TO THE MAIN MENU !" << endl;
	cout << "                -------------------------------------------------- " << endl;

	while (_getch())
		return;

}

// The following function runs a single game (one screen according to the user's input if exists).
void GameManager::runASingleGame()
{
	string nameOfScreen;
	cout << "Please enter name of screen: ";
	cin >> nameOfScreen;

	setLevel();
	setColoredGame();

	if (theBoard.bulidMaze(nameOfScreen)) //if the building succeeded - we can start the game
	{
		startGame();
		if (theGame.getGameStatus() == GameStatus::LOST)
			youLost();
		else
			youWon();
	}
}

// The following function runs a full game (all mazes which exist in the working directory).
void GameManager::runAFullGame()
{
	setLevel();
	setColoredGame();
	int filesAmount = theBoard.getAmountOfFiles();
	int currScreen = 0;
	while (currScreen < filesAmount)
	{
		if (theBoard.bulidMaze(theBoard.getFileName(currScreen)))
		{
			startGame();
		}
		else // if there was an error with the current file - we wouldn't want to print LEVEL UP
		{
			currScreen++;

			if (currScreen != filesAmount - 1)
				continue;
		}
		if (theGame.getGameStatus() == GameStatus::LOST)
		{
			youLost();
			break;
		}
		else
			if (currScreen < filesAmount-1)
				LevelUp();
			else
				youWon();
		currScreen++;
	}
}

// The following function holds a game.
void GameManager::startGame()
{
	clrscr();
	if (isColored)
		theBoard.printColoredMaze();
	else
		theBoard.printMaze();

	theGame.initializeMembers(isColored,level);
	theGame.start();
}

// The following function sets the game's level according to the user's choice.
void GameManager::setLevel()
{
	char userChoice = INIT;

	while (!isLevel(userChoice))
	{
		clrscr();

		cout << "         ********************************************************" << endl;
		cout << "         *            Choose your level of game :)              *" << endl;
		cout << "         *                     (a) BEST                         *" << endl;
		cout << "         *                     (b) GOOD                         *" << endl;
		cout << "         *                     (c) NOVICE                       *" << endl;
		cout << "         ********************************************************" << endl;

		userChoice = _getch();
		switch (userChoice)
		{
		case best1:
		case best2:
			level = Level::BEST;
			break;
		case good1:
		case good2:
			level = Level::GOOD;
			break;
		case novice1:
		case novice2:
			level = Level::NOVICE;
			break;
		}
	}
}

// The following function sets the game's color according to the user's choice.
void GameManager::setColoredGame()
{
	clrscr();
	cout << "\n\n         ********************************************************" << endl;
	cout << "         *       Would you like your game to be colored?        *" << endl;
	cout << "         *                YES PLEASE ! :D - press 1             *" << endl;
	cout << "         *              NO THANK YOU :( press any key           *" << endl;
	cout << "         ********************************************************" << endl;

	char userChoice = 0;
	userChoice = _getch();
	if (userChoice == '1')
	{
		isColored = COLORED;
	}
}

// The following function checks whether the user's choice is valid or not.
int GameManager::choiceValidation(char userChoice) const
{
	return (userChoice == START || userChoice == START_SPECIFIC_GAME || userChoice == INSTRUCTIONS);
}

// The following function checks whether the user's choice regarding the level is valid or not.
int GameManager::isLevel(char userChoice) const
{
	if (userChoice == 'a' || userChoice == 'A' || userChoice == 'b' || userChoice == 'B' || userChoice == 'c' || userChoice == 'C')
		return 1;
	return 0;
}

// The following function prints the game's logo.
void GameManager::logo() const
{
	cout << "             _____          _____  __  __            _   _" << endl;
	cout << "            |  __ \\  /\\    / ____||  \\/  |    /\\    | \\ | |" << endl;
	cout << "            | |__) |/  \\  | |     | \\  / |   /  \\   |  \\| |" << endl;
	cout << "            |  ___// /\\ \\ | |     | |\\/| |  / /\\ \\  | . ` |" << endl;
	cout << "            | |   / ____ \\| |____ | |  | | / ____ \\ | |\\  |" << endl;
	cout << "            |_|  /_/    \\_\\\\_____||_|  |_|/_/    \\_\\|_| \\_|\n" << endl;
	cout << "                    $$$$$$$$$      @@@@@@         " << endl;
	cout << "                    $$$$$$$$$     @@@@@@@@        " << endl;
	cout << "                   $$  $$$  $$   @@@@  @@@@       " << endl;
	cout << "                   $$ O$$$ O$$  @@@@@@@@@         " << endl;
	cout << "                   $$$$$$$$$$$  @@@@@@@     O   O " << endl;
	cout << "                   $$$$$$$$$$$   @@@@@@@@         " << endl;
	cout << "                   $$ $$ $$ $$    @@@@@@@@@       " << endl;
	cout << "                   $$ $$ $$ $$     @@@@@@@        \n" << endl;

}

// The following function is what appears when successfully finishing a level in a full game.
void GameManager::LevelUp() const
{
	clrscr();
	cout << "  _      ________      ________ _        _    _ _____  " << endl;
	cout << " | |    |  ____\\ \\    / /  ____| |      | |  | |  __ \\ " << endl;
	cout << " | |    | |__   \\ \\  / /| |__  | |      | |  | | |__) |" << endl;
	cout << " | |    |  __|   \\ \\/ / |  __| | |      | |  | |  ___/ " << endl;
	cout << " | |____| |____   \\  /  | |____| |____  | |__| | |     " << endl;
	cout << " |______|______|   \\/   |______|______|  \\____/|_|     \n\n" << endl;
	cout << "              $$$$$$$$$      @@@@@@         " << endl;
	cout << "              $$$$$$$$$     @@@@@@@@        " << endl;
	cout << "             $$  $$$  $$   @@@@  @@@@       " << endl;
	cout << "             $$ O$$$ O$$  @@@@@@@@@         " << endl;
	cout << "             $$$$$$$$$$$  @@@@@@@     O   O " << endl;
	cout << "             $$$$$$$$$$$   @@@@@@@@         " << endl;
	cout << "             $$ $$ $$ $$    @@@@@@@@@       " << endl;
	cout << "             $$ $$ $$ $$     @@@@@@@        \n\n" << endl;
	cout << "            PRESS ANY KEY TO CONTINUE !" << endl;

	while (_getch())
		return;
}

// The following function is what appears when winning a game.
void GameManager::youWon()
{
	clrscr();
	cout << "\n\n              CONGRATULATIONS !\n" << endl;
	cout << " __   _____  _   _  __        _____  _   _ _ " << endl;
	cout << " \\ \\ / / _ \\| | | | \\ \\      / / _ \\| \\ | | |" << endl;
	cout << "  \\ V / | | | | | |  \\ \\ /\\ / / | | |  \\| | |" << endl;
	cout << "   | || |_| | |_| |   \\ V  V /| |_| | |\\  |_|" << endl;
	cout << "   |_| \\___/ \\___/     \\_/\\_/  \\___/|_| \\_(_)\n\n" << endl;
	cout << "                    @@@@@@         " << endl;
	cout << "                   @@@@@@@@        " << endl;
	cout << "                  @@@@  @@@@       " << endl;
	cout << "                 @@@@@@@@@         " << endl;
	cout << "                 @@@@@@@     O   O " << endl;
	cout << "                  @@@@@@@@         " << endl;
	cout << "                   @@@@@@@@@       " << endl;
	cout << "                    @@@@@@@        \n" << endl;


	cout << "  PRESS ANY KEY TO GO BACK TO THE MAIN MENU !" << endl;

	theGame.resetGame();
	while (_getch())
		return;
	
}

// The following function is what appears when losing a game.
void GameManager::youLost()
{
	clrscr();

	cout << "   ____    _    __  __ _____    _____     _______ ____  " << endl;
	cout << "  / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\ " << endl;
	cout << " | |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) |" << endl;
	cout << " | |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ < " << endl;
	cout << "  \\____/_/  _\\_\\_|__|_|_____|__\\___/__\\_/_ |_____|_| \\_\\" << endl;
	cout << "           | |   / _ \\/ ___|| ____|  _ \\| |             " << endl;
	cout << "           | |  | | | \\___ \\|  _| | |_) | |             " << endl;
	cout << "           | |__| |_| |___) | |___|  _ <|_|             " << endl;
	cout << "           |_____\\___/|____/|_____|_| \\_(_)             \n\n" << endl;
	cout << "                       $$$$$$$$$   " << endl;
	cout << "                       $$$$$$$$$   " << endl;
	cout << "                      $$  $$$  $$  " << endl;
	cout << "                      $$ O$$$ O$$  " << endl;
	cout << "                      $$$$$$$$$$$  " << endl;
	cout << "                      $$$$$$$$$$$  " << endl;
	cout << "                      $$ $$ $$ $$  " << endl;
	cout << "                      $$ $$ $$ $$  \n" << endl;
	//cout << "                  YOUR SCORE:" << score << endl;
	cout << "\n  PRESS ANY KEY TO GO BACK TO THE MAIN MENU !" << endl;

	theGame.resetGame();
	while (_getch())
		return;

}
