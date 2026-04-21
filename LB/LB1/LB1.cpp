#include <windows.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace std;

int compareWStr(const void* a, const void* b)
{
    return wcscmp(*(const wchar_t**)a, *(const wchar_t**)b);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    printf("=== Task 2: TCHAR type check ===\n");
    printf("sizeof(TCHAR) = %d byte(s)\n", (int)sizeof(TCHAR));
    if (sizeof(TCHAR) == 1)
        printf("TCHAR = char (1 byte) -> ASCII/MBCS mode\n");
    else
        printf("TCHAR = wchar_t (2 bytes) -> UNICODE mode\n");

    printf("\n=== Task 3: Encoding type detection by macros ===\n");
#ifdef UNICODE
    printf("Macro UNICODE defined (/DUNICODE) -> UNICODE encoding\n");
#else
    printf("Macro UNICODE not defined -> ASCII/MBCS encoding\n");
#endif

#ifdef _UNICODE
    printf("Macro _UNICODE defined (/D_UNICODE)\n");
#else
    printf("Macro _UNICODE not defined\n");
#endif

#ifdef _MBCS
    printf("Macro _MBCS defined (/D_MBCS) -> multibyte encoding\n");
#else
    printf("Macro _MBCS not defined\n");
#endif

    printf("\n=== Tasks 4-5: Current mode (after switching and recompiling) ===\n");
    printf("sizeof(TCHAR) = %d -> ", (int)sizeof(TCHAR));
#ifdef UNICODE
    printf("UNICODE mode active\n");
#else
    printf("ASCII/MBCS mode active\n");
#endif

    printf("\n=== Task 6: Family members' full names in ASCII ===\n");
    TCHAR* locResult = _tsetlocale(LC_ALL, _T("Ukrainian"));
    if (locResult == NULL) {
        locResult = _tsetlocale(LC_ALL, _T("Russian"));
        if (locResult == NULL) {
            _tsetlocale(LC_ALL, _T(".1251"));
            printf("Warning: fallback locale .1251 set\n");
        } else {
            printf("Locale set: Russian\n");
        }
    } else {
        printf("Locale set: Ukrainian\n");
    }

    const int FAMILY_COUNT = 5;
    const char* familyASCII[FAMILY_COUNT] = {
        "Борисенко Анжела Олександрівна",
        "Борисенко Андрій Валерійович",
        "Борисенко Поліна Андріївна",
        "Борисенко Леся Андріївна",
        "Борисенко Буся Андріївна"
    };

    printf("Family members' full names (ASCII, cp1251):\n");
    for (int i = 0; i < FAMILY_COUNT; i++)
        printf("%d. %s\n", i + 1, familyASCII[i]);


    printf("\n=== Task 7: Conversion ASCII -> UNICODE (MultiByteToWideChar) ===\n");
    wchar_t* familyUnicode[FAMILY_COUNT] = {};
    for (int i = 0; i < FAMILY_COUNT; i++) {
        int size = MultiByteToWideChar(CP_ACP, 0, familyASCII[i], -1, NULL, 0);
        if (size == 0) {
            printf("MultiByteToWideChar error for string %d (code: %lu)\n",
                   i + 1, GetLastError());
            continue;
        }
        familyUnicode[i] = new wchar_t[size];
        MultiByteToWideChar(CP_ACP, 0, familyASCII[i], -1, familyUnicode[i], size);
        printf("String %d converted to UNICODE (%d wchar_t characters)\n", i + 1, size - 1);
    }

    printf("\n=== Task 8: Output UNICODE array in 3 ways ===\n");
    printf("\n--- Method 1: _tprintf (with locale set) ---\n");
    for (int i = 0; i < FAMILY_COUNT; i++)
        if (familyUnicode[i])
            _tprintf(_T("%d. %ls\n"), i + 1, familyUnicode[i]);

    printf("\n--- Method 2: wcout (with locale set) ---\n");
    wcout.imbue(locale(""));
    for (int i = 0; i < FAMILY_COUNT; i++)
        if (familyUnicode[i])
            wcout << i + 1 << L". " << familyUnicode[i] << L"\n";
    wcout.flush();

    printf("\n--- Method 3: MessageBox (independent of mode) ---\n");
    printf("Opening MessageBox windows...\n");
    for (int i = 0; i < FAMILY_COUNT; i++) {
        if (familyUnicode[i]) {
            wchar_t title[64];
            swprintf(title, 64, L"Family member #%d", i + 1);
            MessageBoxW(0, familyUnicode[i], title, MB_OK);
        }
    }

    printf("\n=== Task 9: Sorting UNICODE string array ===\n");
    printf("\n--- qsort ---\n");
    wchar_t* sortedQsort[FAMILY_COUNT];
    for (int i = 0; i < FAMILY_COUNT; i++)
        sortedQsort[i] = familyUnicode[i];
    qsort(sortedQsort, FAMILY_COUNT, sizeof(wchar_t*), compareWStr);

    wprintf(L"After sorting (qsort):\n");
    for (int i = 0; i < FAMILY_COUNT; i++)
        if (sortedQsort[i])
            wprintf(L"%d. %ls\n", i + 1, sortedQsort[i]);

    printf("\n--- std::sort (template function) ---\n");
    vector<wchar_t*> sortedStd(familyUnicode, familyUnicode + FAMILY_COUNT);

    sort(sortedStd.begin(), sortedStd.end(),
        [](const wchar_t* a, const wchar_t* b) -> bool {
            return wcscmp(a, b) < 0;
        });

    wprintf(L"After sorting (std::sort):\n");
    for (int i = 0; i < (int)sortedStd.size(); i++)
        if (sortedStd[i])
            wprintf(L"%d. %ls\n", i + 1, sortedStd[i]);

    printf("\n=== Task 10: Conversion UNICODE -> ASCII (WideCharToMultiByte) ===\n");
    char* familyBackASCII[FAMILY_COUNT] = {};
    for (int i = 0; i < FAMILY_COUNT; i++) {
        if (!sortedStd[i]) continue;
        int size = WideCharToMultiByte(CP_ACP, 0, sortedStd[i], -1,
                                       NULL, 0, NULL, NULL);
        if (size == 0) {
            printf("WideCharToMultiByte error for string %d\n", i + 1);
            continue;
        }
        familyBackASCII[i] = new char[size];
        WideCharToMultiByte(CP_ACP, 0, sortedStd[i], -1,
                            familyBackASCII[i], size, NULL, NULL);
        printf("String %d converted back to ASCII (%d characters)\n", i + 1, size - 1);
    }

    printf("\n=== Task 11: Sorted array (ASCII after conversion from UNICODE) ===\n");
    for (int i = 0; i < FAMILY_COUNT; i++)
        if (familyBackASCII[i])
            printf("%d. %s\n", i + 1, familyBackASCII[i]);

    printf("\n=== Task 12: Reverse characters in text files ===\n");
    auto processFile = [](const char* inputFileName, const char* outputFileName) {
        printf("\n--- File: %s ---\n", inputFileName);
        ifstream fin(inputFileName, ios::binary | ios::ate);
        if (!fin.is_open()) {
            printf("Could not open file '%s'\n", inputFileName);
            return;
        }

        streamsize fileSize = fin.tellg();
        fin.seekg(0, ios::beg);

        vector<unsigned char> fileData((size_t)fileSize);
        fin.read(reinterpret_cast<char*>(fileData.data()), fileSize);
        fin.close();

        printf("File size: %d bytes\n", (int)fileSize);

        enum Encoding { ENC_ANSI, ENC_UTF8_BOM, ENC_UTF16_LE, ENC_UTF16_BE } enc = ENC_ANSI;
        size_t dataOffset = 0;

        if (fileData.size() >= 2) {
            if      (fileData[0] == 0xFF && fileData[1] == 0xFE)
                { enc = ENC_UTF16_LE; dataOffset = 2; printf("BOM: UTF-16 LE\n"); }
            else if (fileData[0] == 0xFE && fileData[1] == 0xFF)
                { enc = ENC_UTF16_BE; dataOffset = 2; printf("BOM: UTF-16 BE\n"); }
            else if (fileData.size() >= 3 &&
                     fileData[0] == 0xEF && fileData[1] == 0xBB && fileData[2] == 0xBF)
                { enc = ENC_UTF8_BOM; dataOffset = 3; printf("BOM: UTF-8\n"); }
            else
                printf("No BOM detected: ANSI\n");
        }

        vector<unsigned char> bom(fileData.begin(), fileData.begin() + dataOffset);
        vector<unsigned char> content(fileData.begin() + dataOffset, fileData.end());

        if (enc == ENC_UTF16_LE || enc == ENC_UTF16_BE) {
            size_t charCount = content.size() / 2;
            for (size_t i = 0; i < charCount / 2; i++) {
                swap(content[2 * i],     content[2 * (charCount - 1 - i)]);
                swap(content[2 * i + 1], content[2 * (charCount - 1 - i) + 1]);
            }
            printf("Reversed by 2-byte character units (UTF-16)\n");
        } else {
            reverse(content.begin(), content.end());
            printf("Reversed byte-by-byte (ANSI)\n");
        }

        ofstream fout(outputFileName, ios::binary);
        if (fout.is_open()) {
            if (!bom.empty())
                fout.write(reinterpret_cast<const char*>(bom.data()), (streamsize)bom.size());
            fout.write(reinterpret_cast<const char*>(content.data()), (streamsize)content.size());
            fout.close();
            printf("Saved to '%s'\n", outputFileName);
        } else {
            printf("Error writing file '%s'\n", outputFileName);
        }
    };

    processFile("input_ascii.txt", "output_ascii_reversed.txt");

    processFile("input_unicode.txt", "output_unicode_reversed.txt");

    for (int i = 0; i < FAMILY_COUNT; i++) {
        delete[] familyUnicode[i];
        delete[] familyBackASCII[i];
    }

    printf("\n=== Program finished ===\n");
    system("pause");
    return 0;
}

