#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#define NOMINMAX
#include <windows.h>

using namespace std;

const size_t TEXT_SIZE = 64;

struct Employee {
    char surname[TEXT_SIZE];
    char name[TEXT_SIZE];
    char patronymic[TEXT_SIZE];
    char position[TEXT_SIZE];
    int birthYear;
    double salary;
};

void ClearInputBuffer()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void CopyToBuffer(char* destination, const string& source)
{
    strncpy_s(destination, TEXT_SIZE, source.c_str(), _TRUNCATE);
}

string ReadNonEmptyLine(const string& prompt)
{
    string value;
    do {
        cout << prompt;
        getline(cin, value);
        if (value.empty()) {
            cout << "The line cannot be empty. Please try again.\n";
        }
    } while (value.empty());

    return value;
}

int ReadInt(const string& prompt)
{
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            ClearInputBuffer();
            return value;
        }

        cout << "Invalid integer. Please try again.\n";
        cin.clear();
        ClearInputBuffer();
    }
}

double ReadDouble(const string& prompt)
{
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            ClearInputBuffer();
            return value;
        }

        cout << "Invalid number. Please try again.\n";
        cin.clear();
        ClearInputBuffer();
    }
}

int ReadNonNegativeInt(const string& prompt)
{
    while (true) {
        int value = ReadInt(prompt);
        if (value >= 0) {
            return value;
        }

        cout << "The number cannot be negative.\n";
    }
}

Employee InputEmployee(int index)
{
    cout << "\nEntering data for employee #" << index << ":\n";

    Employee employee{};
    CopyToBuffer(employee.surname, ReadNonEmptyLine("Surname: "));
    CopyToBuffer(employee.name, ReadNonEmptyLine("Name: "));
    CopyToBuffer(employee.patronymic, ReadNonEmptyLine("Patronymic: "));
    CopyToBuffer(employee.position, ReadNonEmptyLine("Position: "));
    employee.birthYear = ReadInt("Year of birth: ");
    employee.salary = ReadDouble("Salary: ");

    return employee;
}

void PrintEmployee(const Employee& employee, int index)
{
    cout << left
        << setw(4) << index
        << setw(18) << employee.surname
        << setw(18) << employee.name
        << setw(18) << employee.patronymic
        << setw(20) << employee.position
        << setw(14) << employee.birthYear
        << fixed << setprecision(2) << employee.salary << '\n';
}

void PrintHeader()
{
    cout << left
        << setw(4) << "No"
        << setw(18) << "Surname"
        << setw(18) << "Name"
        << setw(18) << "Patronymic"
        << setw(20) << "Position"
        << setw(14) << "Birth year"
        << "Salary\n";
    cout << string(100, '-') << '\n';
}

void PrintEmployees(const vector<Employee>& employees, const string& title)
{
    cout << "\n" << title << '\n';
    if (employees.empty()) {
        cout << "The file contains no records.\n";
        return;
    }

    PrintHeader();
    for (size_t i = 0; i < employees.size(); ++i) {
        PrintEmployee(employees[i], static_cast<int>(i + 1));
    }
}

vector<Employee> FindBySurname(const vector<Employee>& employees, const string& surname)
{
    vector<Employee> matches;
    for (const auto& employee : employees) {
        if (surname == employee.surname) {
            matches.push_back(employee);
        }
    }

    return matches;
}

bool WriteEmployeesC(const string& fileName, const vector<Employee>& employees)
{
    FILE* file = nullptr;
    fopen_s(&file, fileName.c_str(), "wb");
    if (file == nullptr) {
        return false;
    }

    size_t written = fwrite(employees.data(), sizeof(Employee), employees.size(), file);
    fclose(file);
    return written == employees.size();
}

vector<Employee> ReadEmployeesC(const string& fileName)
{
    vector<Employee> employees;
    FILE* file = nullptr;
    fopen_s(&file, fileName.c_str(), "rb");
    if (file == nullptr) {
        return employees;
    }

    Employee employee{};
    while (fread(&employee, sizeof(Employee), 1, file) == 1) {
        employees.push_back(employee);
    }

    fclose(file);
    return employees;
}

bool AppendEmployeesC(const string& fileName, const vector<Employee>& employees)
{
    FILE* file = nullptr;
    fopen_s(&file, fileName.c_str(), "ab");
    if (file == nullptr) {
        return false;
    }

    size_t written = fwrite(employees.data(), sizeof(Employee), employees.size(), file);
    fclose(file);
    return written == employees.size();
}

bool WriteEmployeesCpp(const string& fileName, const vector<Employee>& employees)
{
    ofstream file(fileName, ios::binary | ios::trunc);
    if (!file) {
        return false;
    }

    if (!employees.empty()) {
        file.write(reinterpret_cast<const char*>(employees.data()), static_cast<streamsize>(employees.size() * sizeof(Employee)));
    }

    return file.good();
}

vector<Employee> ReadEmployeesCpp(const string& fileName)
{
    vector<Employee> employees;
    ifstream file(fileName, ios::binary);
    if (!file) {
        return employees;
    }

    Employee employee{};
    while (file.read(reinterpret_cast<char*>(&employee), sizeof(Employee))) {
        employees.push_back(employee);
    }

    return employees;
}

bool AppendEmployeesCpp(const string& fileName, const vector<Employee>& employees)
{
    ofstream file(fileName, ios::binary | ios::app);
    if (!file) {
        return false;
    }

    if (!employees.empty()) {
        file.write(reinterpret_cast<const char*>(employees.data()), static_cast<streamsize>(employees.size() * sizeof(Employee)));
    }

    return file.good();
}

vector<Employee> InputEmployees(int count)
{
    vector<Employee> employees;
    employees.reserve(count);

    for (int i = 0; i < count; ++i) {
        employees.push_back(InputEmployee(i + 1));
    }

    return employees;
}

vector<Employee> RemoveBySurname(const vector<Employee>& employees, const string& surname, int& removedCount)
{
    vector<Employee> filtered;
    filtered.reserve(employees.size());
    removedCount = 0;

    for (const auto& employee : employees) {
        if (surname == employee.surname) {
            ++removedCount;
        }
        else {
            filtered.push_back(employee);
        }
    }

    return filtered;
}

void SearchInterface(const vector<Employee>& employees)
{
    cout << "\nSearch records by surname.\n";
    string surname = ReadNonEmptyLine("Enter surname to search: ");
    vector<Employee> matches = FindBySurname(employees, surname);
    PrintEmployees(matches, "Search results");
}

void RunCScenario()
{
    cout << "\n========== Working with a file using C-style I/O ==========" << '\n';
    string fileName = ReadNonEmptyLine("Enter file name for C-style mode: ");

    int initialCount = ReadNonNegativeInt("How many initial employees should be written to the file? ");
    vector<Employee> employees = InputEmployees(initialCount);

    if (!WriteEmployeesC(fileName, employees)) {
        cout << "Failed to create or write the file.\n";
        return;
    }

    employees = ReadEmployeesC(fileName);
    PrintEmployees(employees, "Created file (C-style)");
    SearchInterface(employees);

    string surnameToDelete = ReadNonEmptyLine("\nEnter the employee surname to delete: ");
    int removedCount = 0;
    vector<Employee> filtered = RemoveBySurname(employees, surnameToDelete, removedCount);

    if (!WriteEmployeesC(fileName, filtered)) {
        cout << "Failed to rewrite the file after deletion.\n";
        return;
    }

    cout << "Removed records: " << removedCount << '\n';

    int appendCount = ReadNonNegativeInt("How many K records should be appended to the end of the file? ");
    vector<Employee> newEmployees = InputEmployees(appendCount);
    if (!AppendEmployeesC(fileName, newEmployees)) {
        cout << "Failed to append records to the end of the file.\n";
        return;
    }

    employees = ReadEmployeesC(fileName);
    PrintEmployees(employees, "Modified file (C-style)");
    SearchInterface(employees);
}

void RunCppScenario()
{
    cout << "\n========== Working with a file using C++-style I/O ==========" << '\n';
    string fileName = ReadNonEmptyLine("Enter file name for C++-style mode: ");

    int initialCount = ReadNonNegativeInt("How many initial employees should be written to the file? ");
    vector<Employee> employees = InputEmployees(initialCount);

    if (!WriteEmployeesCpp(fileName, employees)) {
        cout << "Failed to create or write the file.\n";
        return;
    }

    employees = ReadEmployeesCpp(fileName);
    PrintEmployees(employees, "Created file (C++-style)");
    SearchInterface(employees);

    string surnameToDelete = ReadNonEmptyLine("\nEnter the employee surname to delete: ");
    int removedCount = 0;
    vector<Employee> filtered = RemoveBySurname(employees, surnameToDelete, removedCount);

    if (!WriteEmployeesCpp(fileName, filtered)) {
        cout << "Failed to rewrite the file after deletion.\n";
        return;
    }

    cout << "Removed records: " << removedCount << '\n';

    int appendCount = ReadNonNegativeInt("How many K records should be appended to the end of the file? ");
    vector<Employee> newEmployees = InputEmployees(appendCount);
    if (!AppendEmployeesCpp(fileName, newEmployees)) {
        cout << "Failed to append records to the end of the file.\n";
        return;
    }

    employees = ReadEmployeesCpp(fileName);
    PrintEmployees(employees, "Modified file (C++-style)");
    SearchInterface(employees);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    while (true) {
        cout << "Menu:\n";
        cout << "1 - Run the task using C-style I/O\n";
        cout << "2 - Run the task using C++-style I/O\n";
        cout << "3 - Run both scenarios in sequence\n";
        cout << "0 - Exit\n";

        int choice = ReadInt("Your choice: ");
        switch (choice) {
        case 1:
            RunCScenario();
            break;
        case 2:
            RunCppScenario();
            break;
        case 3:
            RunCScenario();
            RunCppScenario();
            break;
        case 0:
            return 0;
        default:
            cout << "Unknown menu item. Please try again.\n";
            break;
        }
    }
}
