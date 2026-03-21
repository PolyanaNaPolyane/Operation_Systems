#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int MAX_SIZE = 100;

int task_2_2()
{
    int mas[MAX_SIZE];
    int n;
    int c, d;

    srand((unsigned)time(NULL));

    cout << "\nEnter the size of array <" << MAX_SIZE << ": ";
    cin >> n;

    if (n <= 0 || n > MAX_SIZE)
    {
        cout << "Error! Size must be between 1 and " << MAX_SIZE << endl;
        return 1;
    }

    cout << "\nGenerating array with random numbers in range [-50; 49]:" << endl;
    for (int i = 0; i < n; i++)
    {
        mas[i] = rand() % 100 - 50;
    }

    cout << "Array elements: ";
    for (int i = 0; i < n; i++)
    {
        cout << mas[i];
        if (i < n - 1) cout << " ";
    }
    cout << endl;

    cout << "\nEnter start index (c): ";
    cin >> c;
    cout << "Enter end index (d): ";
    cin >> d;

    if (c < 0 || d >= n || c > d)
    {
        cout << "Error! Indices must be valid (0 <= c <= d < " << n << ")" << endl;
        return 1;
    }

    cout << "\nElements from index [" << c << "] to [" << d << "]: ";

    for (int i = c; i <= d; i++)
    {
        if (i > c) cout << " ";
        cout << mas[i];
    }

    cout << endl;

    return 0;
}
