/*Einav Yoni*/

///* ********************************************************************************************************
//This program is the game Minesweeper The user has to expose the entire board without clicking on a bomb
//********************************************************************************************************** */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#define ColumnSIZE 22  //columns board size
#define RowSIZE 22   //row board size


//array[][] of struct
struct CellInBoard
{
	char RevealCell;  // cell value  the user clicks
	char CloseCell;  // cell value before the user choose the cell (start value is X)
	bool isReveal;   // tell us if the user click this cell or not
};
typedef struct CellInBoard CellBoard;

bool IsItInTheBoard(int row, int column, int rowSize, int colSize)
{
	if ((column < colSize && column >= 0 && row < rowSize && row >= 0))
	{
		return true;
	}
	else
	{
		return false;
	}

}

//reset the array in X, 0 and updates that none of the cells is open
void ResetTheBoard(CellBoard BoardGame[][ColumnSIZE], int row, int col)
{

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			BoardGame[i][j].CloseCell = 'X';
			BoardGame[i][j].RevealCell = '0';
			BoardGame[i][j].isReveal = false;

		}

	}
}

//Prints a line of numbers from 0 to the size of the board
void PrintLineOfNunbers(int row)
{
	printf("    ");
	for (int i = 0; i < row; i++)
	{
		printf(" %2d ", i);
	}
	printf("\n");
}

//Prints a line base on the size of the board
void DrawLine(int Column)
{
	printf(" ");
	for (int i = 0; i < 4 * Column + 6; i++)
	{
		printf("_");
	}
	printf("\n");
}

//draw the Minesweeper board game
void DrawTheBoard(int row, CellBoard BoardGame[][ColumnSIZE], int col)
{
	//print the a line of numbers
	PrintLineOfNunbers(col);
	DrawLine(col);
	for (int i = 0; i < row; i++)
	{
		printf(" %2d |", i);
		for (int j = 0; j < col; j++)
		{
			//print the bored game vallue in i, j location
			printf(" %c |", BoardGame[i][j].CloseCell);
		}
		printf("\n");
	}
	DrawLine(col);

}

//Checks if the cell exists in the array and if this cell is a bomb
bool ValiedIndex(int Column, int row, int rowSize, CellBoard BoardGame[][ColumnSIZE], int col)
{
	//Check that the cell is inside the array
	if (Column < col && Column >= 0 && row < rowSize && row >= 0)
	{
		//Check if the cell is a bomb 
		if (BoardGame[row][col].RevealCell == '*')
		{
			//this cell is a bomb
			return true;
		}
	}
	return false;
}

//Places bombs in random locations in the array and count the numberss around the bombs
void SetTheBombsAndNumbers(CellBoard BoardGame[][ColumnSIZE], int Row, int col)
{
	int randomIndexcolumn;
	int randomIndexrow;

	for (int i = sqrt((double)Row * col); i > 0; i--)
	{
		//Takes a random number that is in the array
		randomIndexcolumn = rand() % col;
		randomIndexrow = rand() % Row;

		//If the cell value is already a bomb
		if (BoardGame[randomIndexrow][randomIndexcolumn].RevealCell == '*')
		{
			//Choose another cell
			i++;
		}
		else
		{
			//place the boom in this random indexs
			BoardGame[randomIndexrow][randomIndexcolumn].RevealCell = '*';


			/*
			*
		  (row-1, col-1) (row-1,col) (row-1, col+1)
						\     |    /
						 \    |   /
	   (row, col-1)------ B O M B --------(row, col+1)
						 /    |   \
						/     |    \
		(row+1, col-1) (row+1, col)(row+1, col+1)
			*/
			//Checks all the cells around the bomb and adds the right number to the cell
			for (int i = randomIndexrow - 1; i <= randomIndexrow + 1; i++)
			{
				for (int j = randomIndexcolumn - 1; j <= randomIndexcolumn + 1; j++)
				{
					//!(ValiedIndex(i, j, Row, BoardGame, col)
					if ((BoardGame[i][j].RevealCell != '*') && IsItInTheBoard(i, j, Row, col))
					{
						BoardGame[i][j].RevealCell++;
					}


				}

			}





		}
	}
}

//Replaces the zeros in the array to spaces
void SetZeroToSpace(int RowSize, int colSize, CellBoard BoardGame[][ColumnSIZE])
{
	for (int i = 0; i <= RowSize; i++)
	{
		for (int j = 0; j <= colSize; j++)
		{
			//if cell exists in the array and if this cell is a zero
			if (BoardGame[i][j].RevealCell == '0')
			{
				BoardGame[i][j].RevealCell = '\0';
			}
		}
	}

}

//set the numbers around the bombs
void ExposedBoard(CellBoard BoardGame[][ColumnSIZE], int row, int col)
{
	//Places bombs and numbers
	SetTheBombsAndNumbers(BoardGame, row, col);
	SetZeroToSpace(row, col, BoardGame);
}

// Reveals all the cells 
void RevealCells(int Row, CellBoard BoardGame[][ColumnSIZE], int Col)
{
	for (int i = 0; i <= Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			//if the cell is already Reveal go to the next one 
			if (BoardGame[i][j].isReveal == false)
			{
				BoardGame[i][j].CloseCell = BoardGame[i][j].RevealCell;
				BoardGame[i][j].isReveal = true;
			}

		}
	}
}

//Reveals a certain number of cells not including bombs
void Shortcut(int numOfCell, CellBoard BoardGame[][ColumnSIZE], int row, int col)
{
	//counter for the numer of cells that need to be opend 
	int openCell = 0;

	for (int i = 0; i < row && openCell < numOfCell; i++)
	{
		for (int j = 0; j < col && openCell < numOfCell; j++)
		{
			//if that cell is a bomb or already Reveal dont count this cell because it doesnt need to be open 
			if (BoardGame[i][j].isReveal == false && !(BoardGame[i][j].RevealCell == '*'))
			{
				BoardGame[i][j].CloseCell = BoardGame[i][j].RevealCell;
				BoardGame[i][j].isReveal = true;
				openCell++;
			}
		}
	}
}

//Checks whether all cells are open except for the bombs
bool WinBorad(CellBoard BoardGame[][ColumnSIZE], int row, int col)
{
	bool Ff;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			Ff = BoardGame[i][j].CloseCell == 'F' || BoardGame[i][j].CloseCell == 'f';
			// if the cell is not open and not a bomb it is not a winning board yet
			if (BoardGame[i][j].isReveal == false && (BoardGame[i][j].RevealCell != '*'))
			{
				return false;
			}
			if (BoardGame[i][j].isReveal && (BoardGame[i][j].RevealCell != '*') && Ff)
			{
				return false;
			}
		}
	}

	return true;
}

//Opens all the empty cells around
void OpenEmptyCells(CellBoard BoardGame[][ColumnSIZE], int rowsSize, int colsSize, int curRow, int curCol)
{
	//If the cell is empty and has not been opened before
	if ((BoardGame[curRow][curCol].RevealCell == '\0') && BoardGame[curRow][curCol].isReveal == false)
	{
		BoardGame[curRow][curCol].CloseCell = BoardGame[curRow][curCol].RevealCell;
		BoardGame[curRow][curCol].isReveal = true;


		/*
		*
	  (row-1, col-1) (row-1,col) (row-1, col+1)
					\     |    /
					 \    |   /
   (row, col-1)  ------C e l l--------(row, col+1)
					 /    |   \
					/     |    \
	(row+1, col-1) (row+1, col)(row+1, col+1)
		*/
		if ((curRow + 1 < rowsSize - 1) && curCol - 1 > 0)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow + 1, curCol - 1);
		}
		if (curRow < rowsSize - 1)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow + 1, curCol);
		}
		if ((curRow + 1 < rowsSize - 1) && curCol + 1 < colsSize - 1)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow + 1, curCol + 1);
		}

		if (curCol < colsSize - 1)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow, curCol + 1);
		}
		if ((curRow - 1 > 0) && curCol + 1 < colsSize - 1)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow - 1, curCol + 1);
		}
		if ((curRow - 1 > 0) && curCol - 1 > 0)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow - 1, curCol - 1);
		}
		if ((curRow - 1 > 0) && curCol - 1 > 0)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow - 1, curCol - 1);
		}
		if (curCol > 0)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow, curCol - 1);
		}
		if (curRow > 0)
		{
			OpenEmptyCells(BoardGame, rowsSize, colsSize, curRow - 1, curCol);
		}

	}
	//Opens cell that is not empty
	else
	{
		BoardGame[curRow][curCol].CloseCell = BoardGame[curRow][curCol].RevealCell;
		BoardGame[curRow][curCol].isReveal = true;
		return;
	}
}

// Checks whether the user's cell Choice is valid
int ValiedGameStep(int UserRowChoice, int UserColumnChoice, CellBoard BoardGame[][ColumnSIZE], int row, int col, char userCharCho)
{
	bool  shortCut = false;
	//// if KeepPlaying=1 Invalid move . KeepPlaying=0 game end . KeepPlaying=-1 keep play 
	int KeepPlay = -1;
	// The user choice is in the array and o or f 
	if ((UserColumnChoice < col && UserColumnChoice >= 0 && UserRowChoice < row && UserRowChoice >= 0))
	{
		// the user chose a bomb 
		if ((BoardGame[UserRowChoice][UserColumnChoice].RevealCell == '*') && userCharCho != 'f' && userCharCho != 'F')
		{

			printf("\nYou've hit a bomb! Game over!\n");
			//Reveal al the Cells
			RevealCells(row, BoardGame, col);
			DrawTheBoard(row, BoardGame, col);
			printf("\nHope you enjoyed playing !!!!!\n");
			return 0;

		}
		//the user choose that cell before
		else if (BoardGame[UserRowChoice][UserColumnChoice].isReveal)
		{
			printf("Invalid move, please enter valid choice!\n");
			KeepPlay = 1;

		}
		if (userCharCho == 'f' || userCharCho == 'F')
		{
			BoardGame[UserRowChoice][UserColumnChoice].CloseCell = 'F';
			BoardGame[UserRowChoice][UserColumnChoice].isReveal = true;
		}
		else if (userCharCho == 'o' || userCharCho == 'O')
		{
			OpenEmptyCells(BoardGame, row, col, UserRowChoice, UserColumnChoice);
			BoardGame[UserRowChoice][UserColumnChoice].CloseCell = BoardGame[UserRowChoice][UserColumnChoice].RevealCell;
			BoardGame[UserRowChoice][UserColumnChoice].isReveal = true;
		}
		else
		{
			printf("Please enter a valid choice!\n");
			return 1;
		}

	}

	//The user select a shortcut
	shortCut = (UserRowChoice == -1 && UserColumnChoice > 0);
	if (shortCut)
	{
		Shortcut(UserColumnChoice, BoardGame, row, col);
	}
	//Checks if everything is exposed except the bombs
	if (WinBorad(BoardGame, row, col))
	{
		DrawTheBoard(row, BoardGame, col);
		printf("You won!! congratulation! CHAMPION!\n");
		printf("Hope you have enjoyed playing !!!!!\n");
		return 0;
	}
	//The user choice is out the array
	else if (!(shortCut) && (UserColumnChoice > col - 1 || UserColumnChoice < 0 || UserRowChoice > row - 1 || UserRowChoice < 0))
	{

		printf("Please enter a valid choice!\n");
		KeepPlay = 1;

	}

	//the user didnt win or lose or did invalid move 
	if (!((KeepPlay == 1) || (KeepPlay == 0)))
	{
		DrawTheBoard(row, BoardGame, col);
		KeepPlay = -1;
	}
	return KeepPlay;
}

// Checks whether the user's Borad Size Choice is valid
bool validBoradSizeChoice(int UserBordSizeChoice)
{
	//The only option is one right now..
	//
	bool  validBoradSize = UserBordSizeChoice >= 1 && UserBordSizeChoice <= 4;

	//the user choice is not 1 -4  
	if (UserBordSizeChoice < 0 || UserBordSizeChoice>4)
	{
		printf("invalid choice.\nPlease choose again\n\n");
	}

	//the user choice is 1
	if (validBoradSize)
	{
		return !(validBoradSize);
	}
	//if the user choice is not 1 (8*8)
	return !(validBoradSize);
}

//Checks that the numbers entered in the board size are valid
bool validBoradSizeRange(int col, int row)
{
	bool inrang = true;

	if (col <= 0 || col > 22)
	{
		printf("Number of columns is out of range!\n");
		inrang = false;
	}
	if (row <= 0 || row > 22)
	{
		printf("Number of lines is out of range!\n");
		inrang = false;
	}
	return inrang;
}

void main()
{
	srand(time(NULL));
	bool Ok = true;
	int KeepPlaying = -1, UserBordSizeChoice = 9;
	int rowSizeBo = 8, colSizeBo = 8;
	int UserRowChoice, UserColumnChoice;
	bool ValidRang = false  /* Checks whether the board is in range*/, Char_O_F = true  /* Checks whether the char is o or f */;
	bool validBoradSize = true;
	char userCharChoice = 'c';
	CellBoard BoardGame[RowSIZE][ColumnSIZE];
	//set the bord cells to X
	ResetTheBoard(BoardGame, RowSIZE, ColumnSIZE);

	// the board size is valid or the use need to Choose another board size
	while ((validBoradSize))
	{
		printf("Welcome to Minesweeper!\n\n");

		printf("Please choose one of the following optionsand enter it's number:\n\n");

		printf("1 - for size 8X8\n\n");

		printf("2 - for size 12X12\n\n");

		printf("3 - for size 15X15\n\n");

		printf("4 - for custom size\n\n");

		printf("0 - Exit\n\n");

		//read the user board size choice
		scanf("%d", &UserBordSizeChoice);


		if (!(UserBordSizeChoice == 0))
		{
			//Checks whether the user selection is valid
			validBoradSize = validBoradSizeChoice(UserBordSizeChoice);
		}
		//the user choose Exit
		else
		{
			printf("GOOD BYE!");
			return;
		}


	}
	//only if the user chose valid board size 

		//buid the board base on the user choice
	switch (UserBordSizeChoice)
	{
		// board size is 8*8 user choose 1 
	case 1:
		//Places bombs in random locations 
		rowSizeBo = 8;
		colSizeBo = 8;
		break;

	case 2:
		//Places bombs in random locations
		rowSizeBo = 12;
		colSizeBo = 12;
		break;

	case 3:
		//Places bombs in random locations
		rowSizeBo = 15;
		colSizeBo = 15;
		break;

	case 4:

		while (!ValidRang)
		{
			printf("\nPlease enter the size of the board (Lines <= 22 and Cols <= 22  ):");
			scanf("%d %d", &rowSizeBo, &colSizeBo);

			ValidRang = validBoradSizeRange(colSizeBo, rowSizeBo);
		}
		break;

	default:
		break;
	}
	//BuildTheBoard(UserBordSizeChoice, BoardGame);
	ExposedBoard(BoardGame, rowSizeBo, colSizeBo);
	//draw the user choice board
	DrawTheBoard(rowSizeBo, BoardGame, colSizeBo);


	//while the user didnt won or lose 
	while (!(KeepPlaying == 0))
	{
		//the user did valid move 
		if (!(KeepPlaying == 1))
		{
			Char_O_F = true;
			printf("\nPlease enter your move, row and column :");
		}
		scanf("%d %d", &UserRowChoice, &UserColumnChoice);
		if (UserRowChoice != -1)
		{
			scanf(" %c", &userCharChoice);
		}
		Char_O_F = (!(userCharChoice == 'f' || userCharChoice == 'F' || userCharChoice == 'o' || userCharChoice == 'O'));

		// if KeepPlaying=1 Invalid move . KeepPlaying=0 game end . KeepPlaying=-1 keep play 
		KeepPlaying = ValiedGameStep(UserRowChoice, UserColumnChoice, BoardGame, rowSizeBo, colSizeBo, userCharChoice);

	}






}
