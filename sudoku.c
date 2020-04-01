#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define UNASSIGNED 0
#define true 1
#define false 0

int isValid(int **grid, int gridSize, int row, int col, unsigned int n)
{
	for (int i = 0; i < gridSize; ++i)
		if (grid[row][i] == n || grid[i][col] == n)
			return false;

	unsigned int startRow, startCol;
	startRow = (row / 3) * 3;
	startCol = (col / 3) * 3;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (grid[startRow + i][startCol + j] == n)
				return false;
	return true;
}

void display(int **grid, int gridSize)
{
	for (int row = 0; row < gridSize; ++row)
	{
		for (int col = 0; col < gridSize; ++col)
		{
			printf("%d\t", grid[row][col]);
			if ((col + 1) % 3 == 0)
				printf("|");
		}
		printf("\n");
		if ((row + 1) % 3 == 0)
			printf("-------------------------------------------------------------------------\n");
	}
}

int solve(int **grid, int gridSize)
{
	for (int row = 0; row < gridSize; ++row)
		for (int col = 0; col < gridSize; ++col)
			if (grid[row][col] == UNASSIGNED)
			{
				for (int n = 1; n <= 9; ++n)
					if (isValid(grid, gridSize, row, col, n))
					{
						grid[row][col] = n;

						// Return only if another solution is not demanded
						if (solve(grid, gridSize))
							return true;

						grid[row][col] = UNASSIGNED;
					}
				return false;
			}

	printf("A solution is:\n");
	display(grid, gridSize);

	// Asking whether to get multiple solutions
	char tryAgain;
	printf("Try for another solution?(y/N) ");
	tryAgain = getchar();

	// Clearing the input buffer
	char ch;
	while ((ch = getchar()) != EOF && ch != '\n')
		;

	return tolower(tryAgain) != 'y';
}

int main()
{
	char fileName[20];
	printf("The name of file with grid: ");
	scanf("%s", fileName);

	FILE *file = fopen(fileName, "r");
	if (!file)
	{
		printf("Error opening %s", fileName);
		return -1;
	}

	int gridSize = 1;
	char ch;
	while ((ch = getc(file)) != EOF && ch != '\n')
		if (ch == ',')
			++gridSize;

	if (ch == EOF)
	{
		printf("Invalid grid. Only rows are present!");
		return -1;
	}

	int **grid = malloc(gridSize * sizeof(int *));
	for (int i = 0; i < gridSize; ++i)
		grid[i] = malloc(gridSize * sizeof(int));

	// Rewinding file pointer since it has read a line already
	rewind(file);
	int i = 0, j = 0;
	while ((ch = getc(file)) != EOF && i < gridSize)
	{
		if (ch >= '0' && ch <= '9')
			grid[i][j++] = ch - '0';
		else if (ch == '\n')
		{
			// When there is enter before data is complete in a row
			if (j != gridSize)
			{
				printf("Less data in row %d", i + 1);
				return -1;
			}

			// Increasing row and resetting column to zero
			++i, j = 0;
		}
	}

	// WHen there is EOF before data is complete
	if (i != gridSize)
	{
		printf("Less number of rows");
		return -1;
	}

	solve(grid, gridSize);

	// Closing file is a good habit
	fclose(file);
	return 0;
}
