#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

const int MAX_ROWS = 50;
const int MAX_COLS = 50;
const int RANDOM_MIN = -50;
const int RANDOM_MAX = 50;

namespace
{

int readIntInRange(const char* prompt, int minValue, int maxValue)
{
	int value;

	while (true)
	{
		cout << prompt;
		cin >> value;

		if (!cin.fail() && value >= minValue && value <= maxValue)
		{
			return value;
		}

		cout << "Input error. Enter a value in range ["
			<< minValue << "; " << maxValue << "].\n";
		cin.clear();
		cin.ignore(10000, '\n');
	}
}

int randomInRange(int minValue, int maxValue)
{
	return minValue + rand() % (maxValue - minValue + 1);
}

void fillMatrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols)
{
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			matrix[row][col] = randomInRange(RANDOM_MIN, RANDOM_MAX);
		}
	}
}

void printMatrix(const int matrix[MAX_ROWS][MAX_COLS], int rows, int cols)
{
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			cout << setw(5) << matrix[row][col];
		}
		cout << '\n';
	}
}

int countEvenOnSecondaryDiagonal(const int matrix[MAX_ROWS][MAX_COLS], int rows, int cols)
{
	const int diagonalSize = rows < cols ? rows : cols;
	int evenCount = 0;

	for (int row = 0; row < diagonalSize; ++row)
	{
		const int col = cols - 1 - row;
		if (matrix[row][col] % 2 == 0)
		{
			++evenCount;
		}
	}

	return evenCount;
}

}

int task_2_3()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	int matrix[MAX_ROWS][MAX_COLS];

	const int rows = readIntInRange("Enter number of rows (1..50): ", 1, MAX_ROWS);
	const int cols = readIntInRange("Enter number of columns (1..50): ", 1, MAX_COLS);

	fillMatrix(matrix, rows, cols);

	cout << "\nGenerated matrix (range [" << RANDOM_MIN << "; "
		<< RANDOM_MAX << "]):\n";
	printMatrix(matrix, rows, cols);

	const int evenCount = countEvenOnSecondaryDiagonal(matrix, rows, cols);

	cout << "\nNumber of even elements on the secondary diagonal: "
		<< evenCount << "\n";

	return 0;
}
