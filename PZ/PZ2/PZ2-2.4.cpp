#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

int* form_mas(int n)
{
    return new int[n];
}

void init_mas(int* mas, int n)
{
    for (int i = 0; i < n; i++)
        mas[i] = rand() % 101 - 50;
}

void print_mas(const int* mas, int n)
{
    if (n == 0 || !mas)
    {
        cout << "Array is empty.\n";
        return;
    }
    cout << "Array [" << n << "]: ";
    for (int i = 0; i < n; i++)
        cout << mas[i] << " ";
    cout << "\n";
}

int* del_mas(int* mas, int& n)
{
    if (n == 0 || !mas)
    {
        cout << "Error: array is empty, cannot delete.\n";
        return mas;
    }

    int idx = -1;
    for (int i = 0; i < n; i++)
        if (mas[i] < 0) { idx = i; break; }

    if (idx == -1)
    {
        cout << "No negative elements found.\n";
        return mas;
    }

    int newN = n - 1;
    int* newMas = (newN > 0) ? form_mas(newN) : nullptr;

    for (int i = 0, j = 0; i < n; i++)
        if (i != idx) newMas[j++] = mas[i];

    cout << "Deleted element at index " << idx
        << " (value = " << mas[idx] << ").\n";

    delete[] mas;
    n = newN;
    return newMas;
}

int* add_mas(int* mas, int& n)
{
    int* newMas = form_mas(n + 1);
    for (int i = 0; i < n; i++)
        newMas[i] = mas[i];
    newMas[n] = rand() % 101 - 50;
    cout << "Added element: " << newMas[n] << "\n";
    delete[] mas;
    n++;
    return newMas;
}

int** form_mat(int rows, int cols)
{
    int** mat = new int* [rows];
    for (int i = 0; i < rows; i++)
        mat[i] = new int[cols];
    return mat;
}

void free_mat(int** mat, int rows)
{
    for (int i = 0; i < rows; i++)
        delete[] mat[i];
    delete[] mat;
}

void init_mat(int** mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            mat[i][j] = rand() % 101 - 50;
}

void print_mat(int** mat, int rows, int cols)
{
    if (!mat || rows == 0 || cols == 0)
    {
        cout << "Matrix is empty.\n";
        return;
    }
    cout << "Matrix [" << rows << "x" << cols << "]:\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            cout << setw(5) << mat[i][j];
        cout << "\n";
    }
}

int** del_col(int** mat, int rows, int& cols, int colIdx)
{
    if (cols == 0)
    {
        cout << "Error: matrix has no columns to delete.\n";
        return mat;
    }
    if (colIdx < 0 || colIdx >= cols)
    {
        cout << "Error: column index " << colIdx
            << " out of range [0.." << cols - 1 << "].\n";
        return mat;
    }

    int** newMat = form_mat(rows, cols - 1);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0, k = 0; j < cols; j++)
            if (j != colIdx) newMat[i][k++] = mat[i][j];
    }

    free_mat(mat, rows);
    cols--;
    return newMat;
}

int** add_col(int** mat, int rows, int& cols, int colIdx)
{
    if (colIdx < 0 || colIdx > cols)
    {
        cout << "Error: column index " << colIdx
            << " out of range [0.." << cols << "].\n";
        return mat;
    }

    int** newMat = form_mat(rows, cols + 1);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0, k = 0; j < cols + 1; j++)
        {
            if (j == colIdx)
                newMat[i][j] = rand() % 101 - 50;
            else
                newMat[i][j] = mat[i][k++];
        }
    }

    free_mat(mat, rows);
    cols++;
    return newMat;
}

int task_2_4()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "========================================\n"
        << "        1D Dynamic Array\n"
        << "  Variant 2: delete first negative element\n"
        << "========================================\n";

    int n = 0;
    int* mas = nullptr;
    int k;

    do
    {
        cout << "\n1. Form array\n"
            "2. Print array\n"
            "3. Delete first negative element\n"
            "4. Add random element\n"
            "5. Exit\n"
            "Choice: ";
        cin >> k;

        switch (k)
        {
        case 1:
            if (mas) { delete[] mas; mas = nullptr; n = 0; }
            cout << "Enter size: ";
            cin >> n;
            if (n <= 0)
            {
                cout << "Invalid size.\n";
                n = 0;
                break;
            }
            mas = form_mas(n);
            init_mas(mas, n);
            cout << "Array formed.\n";
            print_mas(mas, n);
            break;

        case 2:
            print_mas(mas, n);
            break;

        case 3:
            if (!mas || n == 0)
                cout << "Error: array is empty, cannot delete.\n";
            else
                mas = del_mas(mas, n);
            break;

        case 4:
            mas = add_mas(mas, n);
            break;

        case 5:
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (k != 5);

    delete[] mas;
    mas = nullptr;

    cout << "\n========================================\n"
        "        2D Dynamic Array\n"
        "  Variant 2: add column at given position\n"
        "========================================\n";

    int rows = 0, cols = 0;
    int** mat = nullptr;
    int k2;

    do
    {
        cout << "\n1. Form matrix\n"
            "2. Print matrix\n"
            "3. Delete column\n"
            "4. Add column at given position\n"
            "5. Exit\n"
            "Choice: ";
        cin >> k2;

        switch (k2)
        {
        case 1:
            if (mat) { free_mat(mat, rows); mat = nullptr; rows = 0; cols = 0; }
            cout << "Enter rows: ";
            cin >> rows;
            cout << "Enter cols: ";
            cin >> cols;
            if (rows <= 0 || cols <= 0)
            {
                cout << "Invalid dimensions.\n";
                rows = 0; cols = 0;
                break;
            }
            mat = form_mat(rows, cols);
            init_mat(mat, rows, cols);
            cout << "Matrix formed.\n";
            print_mat(mat, rows, cols);
            break;

        case 2:
            if (!mat) { cout << "Matrix is not formed yet.\n"; break; }
            print_mat(mat, rows, cols);
            break;

        case 3:
        {
            if (!mat || cols == 0)
            {
                cout << "Error: matrix has no columns to delete.\n";
                break;
            }
            cout << "Enter column index to delete (0.." << cols - 1 << "): ";
            int colIdx;
            cin >> colIdx;
            mat = del_col(mat, rows, cols, colIdx);
            break;
        }

        case 4:
        {
            if (!mat)
            {
                cout << "Matrix is not formed yet.\n";
                break;
            }
            cout << "Enter column position to insert (0.." << cols << "): ";
            int colIdx;
            cin >> colIdx;
            mat = add_col(mat, rows, cols, colIdx);
            print_mat(mat, rows, cols);
            break;
        }

        case 5:
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (k2 != 5);

    if (mat) free_mat(mat, rows);

    return 0;
}
