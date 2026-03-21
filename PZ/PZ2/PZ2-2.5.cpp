#include <cctype>
#include <iostream>
#include <string>

int countWords(const std::string& text)
{
	int wordCount = 0;
	bool insideWord = false;

	for (char ch : text)
	{
		if (std::isspace(static_cast<unsigned char>(ch)))
		{
			insideWord = false;
		}
		else if (!insideWord)
		{
			insideWord = true;
			++wordCount;
		}
	}

	return wordCount;
}

int countOccurrences(const std::string& text, const std::string& pattern)
{
	if (pattern.empty() || text.size() < pattern.size())
	{
		return 0;
	}

	int count = 0;

	for (std::size_t i = 0; i + pattern.size() <= text.size(); ++i)
	{
		if (text.compare(i, pattern.size(), pattern) == 0)
		{
			++count;
		}
	}

	return count;
}

int readNatural(const char* prompt)
{
	int value;

	while (true)
	{
		std::cout << prompt;
		std::cin >> value;

		if (!std::cin.fail() && value > 0)
		{
			return value;
		}

		std::cout << "Incorrect input. Enter a natural number.\n";
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}
}

int task_2_5()
{
	std::string line;

	std::cout << "Enter a line: ";
	std::getline(std::cin, line);

	std::cout << "Number of words: " << countWords(line) << "\n";

	const int n = readNatural("\nEnter n (number of symbols): ");
	std::string sequence;

	while (true)
	{
		std::cout << "Enter sequence s1...sn without spaces: ";
		std::cin >> sequence;

		if (static_cast<int>(sequence.size()) == n)
		{
			break;
		}

		std::cout << "Length mismatch. Expected " << n
			<< " symbols, got " << sequence.size() << ".\n";
	}

	const int abcCount = countOccurrences(sequence, "abc");
	const int abaCount = countOccurrences(sequence, "aba");

	std::cout << "Occurrences of abc: " << abcCount << "\n";
	std::cout << "Occurrences of aba: " << abaCount << "\n";

	return 0;
}
