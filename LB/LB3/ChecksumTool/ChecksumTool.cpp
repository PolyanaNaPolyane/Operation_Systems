#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <vector>

static uint32_t ComputeChecksum(const uint8_t* data, size_t size)
{
    uint32_t sum = 0;
    for (size_t i = 0; i < size; ++i)
        sum += data[i];
    return sum;
}

static int AppendChecksum(const char* path)
{
    FILE* f = nullptr;
    if (fopen_s(&f, path, "rb") != 0 || !f)
    {
        printf("[ERROR] Cannot open '%s' for reading.\n", path);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    rewind(f);

    if (fileSize <= 0)
    {
        fclose(f);
        printf("[ERROR] File is empty or ftell failed.\n");
        return 1;
    }

    std::vector<uint8_t> buf(static_cast<size_t>(fileSize));
    if (fread(buf.data(), 1, buf.size(), f) != buf.size())
    {
        fclose(f);
        printf("[ERROR] Read error.\n");
        return 1;
    }
    fclose(f);

    uint32_t cs = ComputeChecksum(buf.data(), buf.size());
    printf("File size         : %ld bytes\n", fileSize);
    printf("Computed checksum : 0x%08X (%u)\n", cs, cs);

    if (fopen_s(&f, path, "ab") != 0 || !f)
    {
        printf("[ERROR] Cannot open '%s' for appending.\n", path);
        return 1;
    }
    if (fwrite(&cs, sizeof(cs), 1, f) != 1)
    {
        fclose(f);
        printf("[ERROR] Write error.\n");
        return 1;
    }
    fclose(f);

    printf("Checksum (4 bytes) appended to '%s'.\n", path);
    return 0;
}

static int VerifyChecksum(const char* path)
{
    FILE* f = nullptr;
    if (fopen_s(&f, path, "rb") != 0 || !f)
    {
        printf("[ERROR] Cannot open '%s'.\n", path);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    rewind(f);

    if (fileSize < static_cast<long>(sizeof(uint32_t)))
    {
        fclose(f);
        printf("[ERROR] File too small to contain a checksum.\n");
        return 1;
    }

    std::vector<uint8_t> buf(static_cast<size_t>(fileSize));
    if (fread(buf.data(), 1, buf.size(), f) != buf.size())
    {
        fclose(f);
        printf("[ERROR] Read error.\n");
        return 1;
    }
    fclose(f);

    size_t dataSize = buf.size() - sizeof(uint32_t);
    uint32_t stored   = 0;
    memcpy(&stored, buf.data() + dataSize, sizeof(uint32_t));
    uint32_t computed = ComputeChecksum(buf.data(), dataSize);

    printf("File size         : %ld bytes\n", fileSize);
    printf("Data size (no cs) : %zu bytes\n", dataSize);
    printf("Stored   checksum : 0x%08X (%u)\n", stored,   stored);
    printf("Computed checksum : 0x%08X (%u)\n", computed, computed);

    if (stored == computed)
    {
        printf("Result: CHECKSUM OK\n");
        return 0;
    }
    else
    {
        printf("Result: CHECKSUM MISMATCH!\n");
        return 1;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    const char* path   = argv[1];
    bool        verify = (argc >= 3 && strcmp(argv[2], "--verify") == 0);

    return verify ? VerifyChecksum(path) : AppendChecksum(path);
}
