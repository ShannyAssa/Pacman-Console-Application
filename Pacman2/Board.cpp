#include "Board.h"

//Constructor
Board::Board()
{
	readFilesNames();
	sortFileNames();
}

//The following function reads files names from the directory and stores them in a string vector.
void Board::readFilesNames()
{
	string fullName, temp;

	for (const auto& entry : filesystem::directory_iterator("."))
	{
		fullName = entry.path().string();
		if (fullName.size() > suffix_len) // in order to use substr
		{
			temp = fullName.substr(fullName.size() - suffix_len, suffix_len); // we want to seperate the ".screen" from the rest of the file's name

			if (temp.compare(".screen") == 0)
			{
				screen_files.push_back(fullName);
			}
		}
	}
}

//The following function sorts the files names.
void Board::sortFileNames()
{
	sort(screen_files.begin(), screen_files.end());
}

/*The following gets a file nameand builds a game's matrix accordingly. 
it the building hasn't succeeded it presents a proper message and returns 0. */
int Board::bulidMaze(string mazeName)
{
	int currRow = 0, currCol = 0, maxCols = 0, flag = 1, pacmanOccur = 0, ghostsFlag = 1, limitsFlag = 1;
	char currChar;
	numOfGhosts = 0;

	ifstream myfile(mazeName);
	if (myfile.is_open())
	{
		myfile >> noskipws; // so we can read spaces as well

		while (myfile.get(currChar) && flag)
		{
			switch (currChar) 
			{
			case PACMAN:
				matrix[currRow][currCol] = MatValue::EMPTY;
				startingPacman.updatePosition(currCol, currRow);
				pacmanOccur = 1;
				break;
			case GHOST:
				matrix[currRow][currCol] = MatValue::EMPTY;
				startingGhosts[numOfGhosts].updatePosition(currCol, currRow);
				numOfGhosts++;
				if (!ghostsValidation())
				{
					flag = 0;
					ghostsFlag = 0;
				}
				break;
			case FOODIES:
				matrix[currRow][currCol] = MatValue::FOOD;
				break;
			case SPACE:
				matrix[currRow][currCol] = MatValue::EMPTY;
				break;
			case LEGEND:
				matrix[currRow][currCol] = MatValue::LEGEND;
				startingLegend.updatePosition(currCol, currRow);
				break;
			case BORDER:
				matrix[currRow][currCol] = MatValue::WALL;
				break;
			case ENDL:
				currRow++;
				if (!checkingScreenLimits(currRow, currCol))
				{
					flag = 0;
					limitsFlag = 0;
				}
				if (maxCols < currCol)
					maxCols = currCol;
				else if (currCol < maxCols)
				{
					fillWithBreadcrumbs(currRow, currCol, maxCols);
				}
				currCol = -1; // when the loop ends we're doing currCol++ 
				break;
			}
			currCol++;
			if (!checkingScreenLimits(currRow, currCol))
			{
				flag = 0;
				limitsFlag = 0;
			}
		}

		myfile.close();

		if (!pacmanOccur)
		{
			errorsMessages(Error::NO_PACMAN);
			return 0;
		}
		if (!ghostsFlag)
		{
			errorsMessages(Error::TOO_MANY_GHOSTS);
			return 0;
		}
		if (!limitsFlag)
		{
			errorsMessages(Error::OUT_OF_LIMITS);
			return 0;
		}

		actualRows = currRow + 1;
		actualCols = maxCols;

		updateLegendOnBoard();
	}
	else
	{
		errorsMessages(Error::NO_FILE);
		return 0;
	}

	return 1;
}

// The following function displays error messages according to a specific error.
void Board::errorsMessages(Error err)
{
	clrscr();

	switch (err)
	{
	case Error::NO_PACMAN:
		cout << "Validation problem : There was no Pacman on the board! such a game is not valid." << endl;
		break;
	case Error::TOO_MANY_GHOSTS:
		cout << "Validation problem : This kind of maze is not compatible with the demand of up to 4 ghosts only!" << endl;
		break;
	case Error::NO_FILE:
		cout << "Validation problem : File opening has not succeeded. Cannot start a new game." << endl;
		break;
	case Error::OUT_OF_LIMITS:
		cout << "Validation problem : The screen which was given is not compatible with the size of 25X80" << endl;
		break;
	default:
		break;
	}

	cout << endl;
	cout << "PRESS ANY KEY TO CONTINUE !" << endl;

	while (_getch())
		return;
}

// The following function fills a line with breadcrumbs in case the line length is lower than the max colunms number.
void Board::fillWithBreadcrumbs(int row, int col, int maxCols)
{
	for (int i = col; i < maxCols; i++)
	{
		matrix[row][i] = MatValue::FOOD;
	}
}

// The following function checks whether the number of ghosts appears on the file in which we build the game's maze is valid or not.
int Board::ghostsValidation() const
{
	if (numOfGhosts > MAX_GHOSTS)
		return 0;

	return 1;
}

// The following function checks that the file's limits are matching the ones we're supporting.
int Board::checkingScreenLimits(int& rows, int& cols) const
{
	if (rows > MAX_ROWS || cols > MAX_COLS)
		return 0;

	return 1;
}

// The following function updates the legend on board according to the starting position which we read from a file.
void Board::updateLegendOnBoard()
{
	for (int i = startingLegend.getY(); i < startingLegend.getY()+HEIGHT; i++)
	{
		for (int j = startingLegend.getX(); j < startingLegend.getX()+ WIDTH; j++)
				matrix[i][j] = MatValue::LEGEND;
	}
}

// The following funcion prints a maze.
void Board::printMaze() const
{
	for (int i = 0; i < actualRows; i++)
	{
		for (int j = 0; j < actualCols; j++)
		{
			switch (matrix[i][j])
			{
			case MatValue::FOOD:
				cout << static_cast<char>(PrintingFigures::FOOD_FIG);
				break;
			case MatValue::WALL:
				cout << static_cast<char>(PrintingFigures::BORDERS);
				break;
			case MatValue::EMPTY:
			case MatValue::LEGEND:
				cout << static_cast<char>(PrintingFigures::NONE);
				break;
			default:
				break;
			}

		}
		cout << endl;
	}

}

// The following funcion prints a colored maze.
void Board::printColoredMaze() const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < actualRows; i++)
	{
		for (int j = 0; j < actualCols; j++)
		{
			if (i == actualRows - 1)
			{
				cout << "";
			}
			switch (matrix[i][j])
			{
			case MatValue::FOOD:
				cout << static_cast<char>(PrintingFigures::FOOD_FIG);
				break;
			case MatValue::WALL:
				SetConsoleTextAttribute(hConsole, static_cast<int>(Color::BLUE));
				cout << static_cast<char>(PrintingFigures::BORDERS);
				SetConsoleTextAttribute(hConsole, static_cast<int>(Color::GREY));
				break;
			case MatValue::EMPTY:
			case MatValue::LEGEND:
				cout << static_cast<char>(PrintingFigures::NONE);
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
}

// The following function returns a comaptible figure according to the matValue which has been sent.
const char Board::getFig(MatValue matValue)const
{
	char c = static_cast<char>(PrintingFigures::NONE);

	switch (matValue)
	{
	case MatValue::EMPTY:
		c = static_cast<char>(PrintingFigures::NONE);
		break;
	case MatValue::WALL:
		c = static_cast<char>(PrintingFigures::BORDERS);
		break;
	case MatValue::FOOD:
		c = static_cast<char>(PrintingFigures::FOOD_FIG);
		break;
	}
	return c;
}

//The following function reads 
const int Board::countFoodies() const
{
	int count = 0;

	for (int i = 0; i < actualRows; i++)
		for (int j = 0; j < actualCols; j++)
		{
			if (matrix[i][j] == MatValue::FOOD)
				count++;
		}

	return count;
}

// The following function gets a point and returns its mat value.
const MatValue Board::getValue(int rowY, int colX) const 
{
	return matrix[rowY][colX]; 
}

// The following function edits a mat value.
void Board::setValue(const int rowY, const int colX, const MatValue val)
{
	matrix[rowY][colX] = val;
}

// The following function returns the value of ROWS in the current maze.
const int Board::getROWS() const
{
	return actualRows;
}

// The following function returns the value of COLS in the current maze.
const int Board::getCOLS() const
{
	return actualCols;
}

// The following function returns the legned's starting position.
const Position Board::getStartingLegend()const
{
	return startingLegend;
}

// The following function returns pacman's starting position.
const Position Board::getStartingPacman()const
{
	return startingPacman;
}

// The following function returns the number of ghosts which has been read to the maze.
const int Board::getNumOfGhosts()const
{
	return numOfGhosts;
}

// The following function returns a ghost's starting position.
const Position Board::getStartingGhost(int index) const
{
	return startingGhosts[index];
}

// The following function returns a file name according to an index.
const string Board::getFileName(int index) const
{
	return screen_files[index];
}

// The following function returns the number of files in the directory.
const int Board::getAmountOfFiles()const
{
	return static_cast<int>(screen_files.size());
}