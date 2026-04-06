#include <windows.h>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

enum Encoding
{
	ENC_ANSI,
	ENC_UTF8,
	ENC_UTF16_LE,
	ENC_UTF16_BE
};

struct LineInfo
{
	const wchar_t* data;
	int length;
};

bool ReadFileBytes(const char* path, std::vector<unsigned char>& bytes)
{
	FILE* file = nullptr;
	if (fopen_s(&file, path, "rb") != 0 || file == nullptr)
	{
		return false;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);
	if (size < 0)
	{
		fclose(file);
		return false;
	}

	bytes.resize(static_cast<size_t>(size));
	if (size > 0)
	{
		size_t count = fread(bytes.data(), 1, bytes.size(), file);
		if (count != bytes.size())
		{
			fclose(file);
			return false;
		}
	}

	fclose(file);
	return true;
}

bool WriteFileBytes(const char* path, const std::vector<unsigned char>& bytes)
{
	FILE* file = nullptr;
	if (fopen_s(&file, path, "wb") != 0 || file == nullptr)
	{
		return false;
	}

	if (!bytes.empty())
	{
		size_t count = fwrite(bytes.data(), 1, bytes.size(), file);
		if (count != bytes.size())
		{
			fclose(file);
			return false;
		}
	}

	fclose(file);
	return true;
}

bool IsUtf8(const std::vector<unsigned char>& bytes)
{
	size_t i = 0;
	while (i < bytes.size())
	{
		unsigned char c = bytes[i];
		if (c <= 0x7F)
		{
			++i;
			continue;
		}

		int tailCount = 0;
		if ((c & 0xE0) == 0xC0)
		{
			if (c < 0xC2)
			{
				return false;
			}
			tailCount = 1;
		}
		else if ((c & 0xF0) == 0xE0)
		{
			tailCount = 2;
		}
		else if ((c & 0xF8) == 0xF0)
		{
			if (c > 0xF4)
			{
				return false;
			}
			tailCount = 3;
		}
		else
		{
			return false;
		}

		if (i + static_cast<size_t>(tailCount) >= bytes.size())
		{
			return false;
		}

		for (int j = 1; j <= tailCount; ++j)
		{
			if ((bytes[i + j] & 0xC0) != 0x80)
			{
				return false;
			}
		}

		i += static_cast<size_t>(tailCount) + 1;
	}

	return true;
}

Encoding DetectEncoding(const std::vector<unsigned char>& bytes, int& bomSize)
{
	bomSize = 0;

	if (bytes.size() >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
	{
		bomSize = 3;
		return ENC_UTF8;
	}

	if (bytes.size() >= 2 && bytes[0] == 0xFF && bytes[1] == 0xFE)
	{
		bomSize = 2;
		return ENC_UTF16_LE;
	}

	if (bytes.size() >= 2 && bytes[0] == 0xFE && bytes[1] == 0xFF)
	{
		bomSize = 2;
		return ENC_UTF16_BE;
	}

	if (!bytes.empty() && IsUtf8(bytes))
	{
		return ENC_UTF8;
	}

	return ENC_ANSI;
}

bool DecodeToWide(const std::vector<unsigned char>& bytes, Encoding encoding, int bomSize, std::vector<wchar_t>& wide)
{
	wide.clear();
	const unsigned char* raw = bytes.empty() ? nullptr : bytes.data() + bomSize;
	int rawSize = static_cast<int>(bytes.size()) - bomSize;
	if (rawSize <= 0)
	{
		return true;
	}

	if (encoding == ENC_UTF16_LE || encoding == ENC_UTF16_BE)
	{
		if (rawSize % 2 != 0)
		{
			return false;
		}

		int count = rawSize / 2;
		wide.resize(count);
		for (int i = 0; i < count; ++i)
		{
			unsigned char b1 = raw[i * 2];
			unsigned char b2 = raw[i * 2 + 1];
			if (encoding == ENC_UTF16_LE)
			{
				wide[i] = static_cast<wchar_t>(b1 | (b2 << 8));
			}
			else
			{
				wide[i] = static_cast<wchar_t>((b1 << 8) | b2);
			}
		}
		return true;
	}

	UINT codePage = encoding == ENC_UTF8 ? CP_UTF8 : CP_ACP;
	DWORD flags = encoding == ENC_ANSI ? 0 : MB_ERR_INVALID_CHARS;
	int count = MultiByteToWideChar(codePage, flags, reinterpret_cast<const char*>(raw), rawSize, nullptr, 0);
	if (count <= 0)
	{
		return false;
	}

	wide.resize(count);
	return MultiByteToWideChar(codePage, flags, reinterpret_cast<const char*>(raw), rawSize, wide.data(), count) > 0;
}

int CompareLines(const LineInfo& a, const LineInfo& b)
{
	int minLen = a.length < b.length ? a.length : b.length;
	for (int i = 0; i < minLen; ++i)
	{
		if (a.data[i] < b.data[i])
		{
			return -1;
		}
		if (a.data[i] > b.data[i])
		{
			return 1;
		}
	}

	if (a.length < b.length)
	{
		return -1;
	}
	if (a.length > b.length)
	{
		return 1;
	}
	return 0;
}

void SplitLines(const std::vector<wchar_t>& wide, std::vector<LineInfo>& lines, wchar_t newline[2], int& newlineSize, bool& hasLastNewline)
{
	lines.clear();
	newline[0] = L'\n';
	newline[1] = 0;
	newlineSize = 1;
	hasLastNewline = false;

	int start = 0;
	for (int i = 0; i < static_cast<int>(wide.size()); ++i)
	{
		if (wide[i] == L'\r' || wide[i] == L'\n')
		{
			LineInfo line;
			line.data = wide.data() + start;
			line.length = i - start;
			lines.push_back(line);

			if (lines.size() == 1)
			{
				if (wide[i] == L'\r' && i + 1 < static_cast<int>(wide.size()) && wide[i + 1] == L'\n')
				{
					newline[0] = L'\r';
					newline[1] = L'\n';
					newlineSize = 2;
				}
				else
				{
					newline[0] = wide[i];
					newline[1] = 0;
					newlineSize = 1;
				}
			}

			if (wide[i] == L'\r' && i + 1 < static_cast<int>(wide.size()) && wide[i + 1] == L'\n')
			{
				++i;
			}

			start = i + 1;
			hasLastNewline = (start == static_cast<int>(wide.size()));
		}
	}

	if (start < static_cast<int>(wide.size()))
	{
		LineInfo line;
		line.data = wide.data() + start;
		line.length = static_cast<int>(wide.size()) - start;
		lines.push_back(line);
	}
}

void JoinLines(const std::vector<LineInfo>& lines, const wchar_t newline[2], int newlineSize, bool hasLastNewline, std::vector<wchar_t>& wide)
{
	wide.clear();
	for (size_t i = 0; i < lines.size(); ++i)
	{
		for (int j = 0; j < lines[i].length; ++j)
		{
			wide.push_back(lines[i].data[j]);
		}

		if (i + 1 < lines.size() || hasLastNewline)
		{
			for (int j = 0; j < newlineSize; ++j)
			{
				wide.push_back(newline[j]);
			}
		}
	}
}

bool EncodeFromWide(const std::vector<wchar_t>& wide, Encoding encoding, int bomSize, std::vector<unsigned char>& bytes)
{
	bytes.clear();

	if (encoding == ENC_UTF8 && bomSize == 3)
	{
		bytes.push_back(0xEF);
		bytes.push_back(0xBB);
		bytes.push_back(0xBF);
	}
	if (encoding == ENC_UTF16_LE && bomSize == 2)
	{
		bytes.push_back(0xFF);
		bytes.push_back(0xFE);
	}
	if (encoding == ENC_UTF16_BE && bomSize == 2)
	{
		bytes.push_back(0xFE);
		bytes.push_back(0xFF);
	}

	if (wide.empty())
	{
		return true;
	}

	if (encoding == ENC_UTF16_LE || encoding == ENC_UTF16_BE)
	{
		for (size_t i = 0; i < wide.size(); ++i)
		{
			unsigned char low = static_cast<unsigned char>(wide[i] & 0xFF);
			unsigned char high = static_cast<unsigned char>((wide[i] >> 8) & 0xFF);
			if (encoding == ENC_UTF16_LE)
			{
				bytes.push_back(low);
				bytes.push_back(high);
			}
			else
			{
				bytes.push_back(high);
				bytes.push_back(low);
			}
		}
		return true;
	}

	UINT codePage = encoding == ENC_UTF8 ? CP_UTF8 : CP_ACP;
	int count = WideCharToMultiByte(codePage, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
	if (count <= 0)
	{
		return false;
	}

	std::vector<unsigned char> raw(static_cast<size_t>(count));
	if (WideCharToMultiByte(codePage, 0, wide.data(), static_cast<int>(wide.size()), reinterpret_cast<char*>(raw.data()), count, nullptr, nullptr) <= 0)
	{
		return false;
	}

	bytes.insert(bytes.end(), raw.begin(), raw.end());
	return true;
}

const char* GetEncodingName(Encoding encoding, int bomSize)
{
	if (encoding == ENC_ANSI)
	{
		return "ANSI";
	}
	if (encoding == ENC_UTF8)
	{
		return bomSize == 3 ? "UTF-8 with BOM" : "UTF-8 without BOM";
	}
	if (encoding == ENC_UTF16_LE)
	{
		return "UTF-16 LE";
	}
	return "UTF-16 BE";
}

int main()
{
	char inputPath[MAX_PATH] = {};
	char outputPath[MAX_PATH] = {};

	std::cout << "Input file: ";
	std::cin.getline(inputPath, MAX_PATH);
	std::cout << "Output file: ";
	std::cin.getline(outputPath, MAX_PATH);

	std::vector<unsigned char> inputBytes;
	if (!ReadFileBytes(inputPath, inputBytes))
	{
		std::cout << "Cannot read file.\n";
		return 1;
	}

	int bomSize = 0;
	Encoding encoding = DetectEncoding(inputBytes, bomSize);
	std::cout << "Detected encoding: " << GetEncodingName(encoding, bomSize) << "\n";

	std::vector<wchar_t> wideText;
	if (!DecodeToWide(inputBytes, encoding, bomSize, wideText))
	{
		std::cout << "Cannot decode text.\n";
		return 1;
	}

	std::vector<LineInfo> lines;
	wchar_t newline[2];
	int newlineSize = 1;
	bool hasLastNewline = false;
	SplitLines(wideText, lines, newline, newlineSize, hasLastNewline);

	std::sort(lines.begin(), lines.end(), [](const LineInfo& a, const LineInfo& b)
	{
		return CompareLines(a, b) < 0;
	});

	std::vector<wchar_t> sortedWide;
	JoinLines(lines, newline, newlineSize, hasLastNewline, sortedWide);

	std::vector<unsigned char> outputBytes;
	if (!EncodeFromWide(sortedWide, encoding, bomSize, outputBytes))
	{
		std::cout << "Cannot encode result.\n";
		return 1;
	}

	if (!WriteFileBytes(outputPath, outputBytes))
	{
		std::cout << "Cannot write file.\n";
		return 1;
	}

	std::cout << "Done.\n";
	return 0;
}
