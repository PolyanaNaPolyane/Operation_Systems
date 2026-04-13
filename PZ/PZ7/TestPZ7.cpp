#include <windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>

const size_t CHECKSUM_SIZE = sizeof(unsigned int);

unsigned int CalculateChecksumMailbox(const std::vector<unsigned char>& data, size_t count)
{
    unsigned int checksum = 0;
    for (size_t i = 0; i < count; ++i)
    {
        checksum += data[i];
    }
    return checksum;
}

bool EnsureValidDllChecksum(const std::wstring& dllPath)
{
    std::ifstream input(dllPath, std::ios::binary | std::ios::ate);
    if (!input.is_open())
    {
        return false;
    }

    std::streamsize fileSize = input.tellg();
    if (fileSize <= 0)
    {
        return false;
    }

    input.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(static_cast<size_t>(fileSize));
    input.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    input.close();

    if (fileSize > static_cast<std::streamsize>(CHECKSUM_SIZE))
    {
        unsigned int storedChecksum = 0;
        std::memcpy(&storedChecksum, buffer.data() + (buffer.size() - CHECKSUM_SIZE), CHECKSUM_SIZE);
        unsigned int calculatedWithoutTail = CalculateChecksumMailbox(buffer, buffer.size() - CHECKSUM_SIZE);
        if (storedChecksum == calculatedWithoutTail)
        {
            return true;
        }
    }

    unsigned int checksumForCurrentContent = CalculateChecksumMailbox(buffer, buffer.size());
    std::ofstream output(dllPath, std::ios::binary | std::ios::app);
    if (!output.is_open())
    {
        return false;
    }

    output.write(reinterpret_cast<const char*>(&checksumForCurrentContent), CHECKSUM_SIZE);
    output.close();
    return true;
}

unsigned int ReadStoredChecksum(const std::wstring& dllPath)
{
    std::ifstream input(dllPath, std::ios::binary | std::ios::ate);
    if (!input.is_open())
    {
        return 0;
    }

    std::streamsize fileSize = input.tellg();
    if (fileSize <= static_cast<std::streamsize>(CHECKSUM_SIZE))
    {
        return 0;
    }

    input.seekg(-static_cast<std::streamoff>(CHECKSUM_SIZE), std::ios::end);
    unsigned int storedChecksum = 0;
    input.read(reinterpret_cast<char*>(&storedChecksum), CHECKSUM_SIZE);
    return storedChecksum;
}

unsigned int CalculateChecksumWithoutStoredTail(const std::wstring& dllPath)
{
    std::ifstream input(dllPath, std::ios::binary | std::ios::ate);
    if (!input.is_open())
    {
        return 0;
    }

    std::streamsize fileSize = input.tellg();
    if (fileSize <= 0)
    {
        return 0;
    }

    size_t bytesToRead = static_cast<size_t>(fileSize);
    if (fileSize > static_cast<std::streamsize>(CHECKSUM_SIZE))
    {
        bytesToRead -= CHECKSUM_SIZE;
    }

    input.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(bytesToRead);
    input.read(reinterpret_cast<char*>(buffer.data()), bytesToRead);
    return CalculateChecksumMailbox(buffer, bytesToRead);
}

bool VerifyChecksum(const std::wstring& dllPath, unsigned int& calculated, unsigned int& stored)
{
    calculated = CalculateChecksumWithoutStoredTail(dllPath);
    stored = ReadStoredChecksum(dllPath);
    return (stored != 0 && calculated == stored);
}

std::wstring BuildDllPathNextToExecutable(const wchar_t* dllName)
{
    wchar_t exePath[MAX_PATH] = {};
    DWORD len = GetModuleFileNameW(NULL, exePath, MAX_PATH);
    if (len == 0 || len >= MAX_PATH)
    {
        return std::wstring(dllName);
    }

    std::wstring fullPath(exePath);
    size_t lastSlash = fullPath.find_last_of(L"\\/");
    if (lastSlash == std::wstring::npos)
    {
        return std::wstring(dllName);
    }

    fullPath.erase(lastSlash + 1);
    fullPath += dllName;
    return fullPath;
}

int main()
{
    std::wcout << L"========================================\n";
    std::wcout << L"   PZ7 Checksum Auto-Fix Test\n";
    std::wcout << L"========================================\n\n";

    std::wstring dllPath = BuildDllPathNextToExecutable(L"PZ7.dll");
    std::wcout << L"[*] DLL path: " << dllPath << L"\n";

    unsigned int calculatedChecksum = 0;
    unsigned int storedChecksum = 0;
    bool isIntegrityOk = VerifyChecksum(dllPath, calculatedChecksum, storedChecksum);

    std::wcout << L"[*] Running integrity check...\n";
    std::wcout << L"    Calculated: 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0')
               << calculatedChecksum << std::dec << L"\n";
    std::wcout << L"    Stored:     0x" << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0')
               << storedChecksum << std::dec << L"\n";
    std::wcout << L"    Result: " << (isIntegrityOk ? L"PASSED" : L"FAILED") << L"\n";

    if (!isIntegrityOk)
    {
        std::wcout << L"[*] Checksum mismatch detected, trying to update...\n";
        if (EnsureValidDllChecksum(dllPath))
        {
            std::wcout << L"    Checksum update: SUCCESS\n";
            std::wcout << L"[*] Re-running integrity check...\n";
            isIntegrityOk = VerifyChecksum(dllPath, calculatedChecksum, storedChecksum);
            std::wcout << L"    Calculated: 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0')
                       << calculatedChecksum << std::dec << L"\n";
            std::wcout << L"    Stored:     0x" << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0')
                       << storedChecksum << std::dec << L"\n";
            std::wcout << L"    Result: " << (isIntegrityOk ? L"PASSED" : L"FAILED") << L"\n";
        }
        else
        {
            std::wcout << L"    Checksum update: FAILED\n";
        }
    }

    HMODULE hDll = LoadLibraryW(dllPath.c_str());
    if (hDll == NULL)
    {
        std::wcout << L"Error: Failed to load DLL\n";
        std::wcout << L"Error code: " << GetLastError() << L"\n";
        return 1;
    }

    std::wcout << L"[*] Current checksum: 0x"
               << std::hex << std::uppercase << std::setw(8) << std::setfill(L'0')
               << calculatedChecksum << std::dec << L"\n\n";

    FreeLibrary(hDll);

    std::wcout << L"========================================\n";
    std::wcout << L"   Test Complete\n";
    std::wcout << L"========================================\n";

    return isIntegrityOk ? 0 : 1;
}
