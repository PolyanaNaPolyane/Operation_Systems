#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>

std::string NormalizeDirectoryPath(std::string path)
{
    std::replace(path.begin(), path.end(), '/', '\\');

    std::string compactPath;
    compactPath.reserve(path.size());

    for (char ch : path)
    {
        if (ch == '\\' && !compactPath.empty() && compactPath.back() == '\\')
        {
            continue;
        }

        compactPath.push_back(ch);
    }

    path = compactPath;

    while (!path.empty() && path.back() == '\\')
    {
        path.pop_back();
    }

    std::transform(path.begin(), path.end(), path.begin(), [](unsigned char ch)
    {
        return static_cast<char>(std::tolower(ch));
    });

    return path;
}

bool AreDirectoriesEqual(const std::string& left, const std::string& right)
{
    return NormalizeDirectoryPath(left) == NormalizeDirectoryPath(right);
}

int main()
{
    std::string firstDirectory;
    std::string secondDirectory;

    std::cout << "Enter first directory: ";
    std::getline(std::cin, firstDirectory);

    std::cout << "Enter second directory: ";
    std::getline(std::cin, secondDirectory);

    std::cout << (AreDirectoriesEqual(firstDirectory, secondDirectory) ? "true" : "false") << '\n';
}
