#include <string.h>
#include "coding.h"

static bool is_valid_gbk(const unsigned char *bytes, size_t length)
{
    size_t i = 0;
    while (i < length) {
        if (bytes[i] <= 0x7F) {
            // 单字节（ASCII字符）
            i += 1;
        } else if (bytes[i] >= 0x81 && bytes[i] <= 0xFE) {
            // 双字节字符
            if (i + 1 >= length) {
                return false;
            }
            unsigned char next_byte = bytes[i + 1];
            if ((next_byte >= 0x40 && next_byte <= 0x7E) || (next_byte >= 0x80 && next_byte <= 0xFE)) {
                i += 2;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

bool is_file_gbk(const char *filename)
{
    char line[2048];

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return false;
    }

    while (fgets(line, 2048, file) != NULL) {
        if (!is_valid_gbk((unsigned char*)line, strlen(line))) {
            return false;
        }
    }

    fclose(file);

    return true;
}