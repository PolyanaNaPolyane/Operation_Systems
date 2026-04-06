#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <io.h>
#include <fcntl.h>

int main()
{
    std::cout << "Select encoding:\n";
    std::cout << "1 - ASCII\n";
    std::cout << "2 - Unicode\n";
    std::cout << "Your choice: ";

    int choice = 0;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1)
    {
        std::vector<std::string> lines;
        std::string line;

        std::cout << "\nEnter ASCII lines (empty line to finish):\n";
        while (std::getline(std::cin, line) && !line.empty())
            lines.push_back(line);

        std::sort(lines.begin(), lines.end());

        std::cout << "\nSorted lines (ASCII):\n";
        for (const std::string& l : lines)
            std::cout << l << '\n';
    }
    else
    {
        _setmode(_fileno(stdin),  _O_U16TEXT);
        _setmode(_fileno(stdout), _O_U16TEXT);

        std::vector<std::wstring> lines;
        std::wstring line;

        std::wcout << L"\nEnter Unicode lines (empty line to finish):\n";
        while (std::getline(std::wcin, line) && !line.empty())
            lines.push_back(line);

        std::sort(lines.begin(), lines.end());

        std::wcout << L"\nSorted lines (Unicode):\n";
        for (const std::wstring& l : lines)
            std::wcout << l << L'\n';
    }

    return 0;
}
