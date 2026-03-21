#include <ctime>
#include <cstdlib>
#include <iostream>

const int MAX_SIZE = 100;
const int RANDOM_MIN = -50;
const int RANDOM_RANGE = 100;

namespace
{

int readIntInRange(const char* prompt, int minValue, int maxValue)
{
    int value;

    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (!std::cin.fail() && value >= minValue && value <= maxValue)
        {
            return value;
        }

        std::cout << "Incorrect input. Enter a value in range ["
            << minValue << "; " << maxValue << "].\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

int generateRandomValue()
{
    return std::rand() % RANDOM_RANGE + RANDOM_MIN;
}

void fillArray(int array[], int size)
{
    for (int index = 0; index < size; ++index)
    {
        array[index] = generateRandomValue();
    }
}

void printArray(const int array[], int size)
{
    if (size == 0)
    {
        std::cout << "Array is empty.\n";
        return;
    }

    for (int index = 0; index < size; ++index)
    {
        std::cout << array[index] << ' ';
    }

    std::cout << "\n";
}

int findMinIndex(const int array[], int size)
{
    int minIndex = 0;

    for (int index = 1; index < size; ++index)
    {
        if (array[index] < array[minIndex])
        {
            minIndex = index;
        }
    }

    return minIndex;
}

void deleteElementAt(int array[], int& size, int position)
{
    for (int index = position; index < size - 1; ++index)
    {
        array[index] = array[index + 1];
    }

    --size;
}

void appendRandomElements(int array[], int& size, int count)
{
    for (int index = 0; index < count; ++index)
    {
        array[size] = generateRandomValue();
        ++size;
    }
}

void rotateRight(int array[], int size, int shift)
{
    if (size == 0)
    {
        return;
    }

    shift %= size;
    if (shift == 0)
    {
        return;
    }

    int temp[MAX_SIZE];

    for (int index = 0; index < size; ++index)
    {
        temp[(index + shift) % size] = array[index];
    }

    for (int index = 0; index < size; ++index)
    {
        array[index] = temp[index];
    }
}

int findFirstNegativeLinear(const int array[], int size, int& comparisons)
{
    comparisons = 0;

    for (int index = 0; index < size; ++index)
    {
        ++comparisons;
        if (array[index] < 0)
        {
            return index;
        }
    }

    return -1;
}

void selectionSort(int array[], int size)
{
    for (int left = 0; left < size - 1; ++left)
    {
        int minIndex = left;

        for (int index = left + 1; index < size; ++index)
        {
            if (array[index] < array[minIndex])
            {
                minIndex = index;
            }
        }

        if (minIndex != left)
        {
            const int temp = array[left];
            array[left] = array[minIndex];
            array[minIndex] = temp;
        }
    }
}

int binarySearchFirstOccurrence(const int array[], int size, int target, int& comparisons)
{
    int left = 0;
    int right = size - 1;
    int result = -1;
    comparisons = 0;

    while (left <= right)
    {
        const int middle = left + (right - left) / 2;

        ++comparisons;
        if (array[middle] == target)
        {
            result = middle;
            right = middle - 1;
        }
        else
        {
            ++comparisons;
            if (array[middle] < target)
            {
                left = middle + 1;
            }
            else
            {
                right = middle - 1;
            }
        }
    }

    return result;
}

}

int task_2_1()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int array[MAX_SIZE];

    int size = readIntInRange("Enter array size (1..100): ", 1, MAX_SIZE);

    fillArray(array, size);

    std::cout << "\n1. Generated array:\n";
    printArray(array, size);

    const int minIndex = findMinIndex(array, size);
    const int minValue = array[minIndex];
    deleteElementAt(array, size, minIndex);

    std::cout << "\n2. Array after deleting the first minimum element ("
        << minValue << "):\n";
    printArray(array, size);

    const int elementsToAdd = readIntInRange(
        "\nEnter number of elements K to append to the end: ",
        0,
        MAX_SIZE - size);
    appendRandomElements(array, size, elementsToAdd);

    std::cout << "\n3. Array after appending " << elementsToAdd << " element(s):\n";
    printArray(array, size);

    const int shift = readIntInRange(
        "\nEnter cyclic right shift M: ",
        0,
        size == 0 ? 0 : 1000000);
    rotateRight(array, size, shift);

    std::cout << "\n4. Array after cyclic right shift by " << shift << ":\n";
    printArray(array, size);

    int linearComparisons = 0;
    const int firstNegativeIndex = findFirstNegativeLinear(array, size, linearComparisons);
    bool negativeWasFound = firstNegativeIndex != -1;
    int searchValue = 0;

    std::cout << "\n5. Linear search for the first negative element:\n";
    if (negativeWasFound)
    {
        searchValue = array[firstNegativeIndex];
        std::cout << "Found value: " << searchValue
            << ", index: " << firstNegativeIndex
            << ", comparisons: " << linearComparisons << "\n";
    }
    else
    {
        std::cout << "No negative elements found. Comparisons: "
            << linearComparisons << "\n";
    }

    selectionSort(array, size);

    std::cout << "\n6. Array after selection sort in ascending order:\n";
    printArray(array, size);

    std::cout << "\n7. Binary search in the sorted array:\n";
    if (negativeWasFound)
    {
        int binaryComparisons = 0;
        const int sortedIndex = binarySearchFirstOccurrence(
            array,
            size,
            searchValue,
            binaryComparisons);

        if (sortedIndex != -1)
        {
            std::cout << "Found value: " << searchValue
                << ", index: " << sortedIndex
                << ", comparisons: " << binaryComparisons << "\n";
        }
        else
        {
            std::cout << "Value " << searchValue
                << " was not found. Comparisons: " << binaryComparisons << "\n";
        }
    }
    else
    {
        std::cout << "Binary search skipped because there is no negative element to search for.\n";
    }

    return 0;
}
