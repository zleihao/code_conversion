#include "search_path.h"

#define FILE_PATH_MAX 2048

int is_directory(const char *path) 
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        perror("stat");
        return -1; // 错误，无法获取路径信息
    }

    return S_ISDIR(path_stat.st_mode);
}

int has_any_suffix(const char *filename, const char **suffixes, size_t num_suffixes) {
    size_t filename_len = strlen(filename);

    for (size_t i = 0; i < num_suffixes; ++i) {
        size_t suffix_len = strlen(suffixes[i]);
        if (filename_len >= suffix_len) {
            if (strcmp(filename + filename_len - suffix_len, suffixes[i]) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

// 函数来替换字符串中的反斜杠为正斜杠
char *replace_backslashes_with_slashes(const char *input) {
    size_t input_len = strlen(input);

    // 为新字符串分配内存
    char* result = (char*)malloc(input_len + 1); // +1 for the null terminator
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // 构建新字符串
    size_t j = 0;
    for (size_t i = 0; i < input_len; i++) {
        if (input[i] == '\\') {
            if (i + 1 < input_len && input[i + 1] == '\\') {
                i++;
            }
            result[j++] = '/';
        } else {
            result[j++] = input[i];
        }
    }

    // 终止新字符串
    result[j] = '\0';

    return result;
}

#ifdef UNIX
void walk(const char *path, const char **suffix, size_t num_suffix)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(1);
    }

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL)
    {
        char *name = ent->d_name;
        // 忽略以点号开头的文件夹
        if (name[0] == '.')
            continue;

        char file_name[FILE_PATH_MAX];
        snprintf(file_name, FILE_PATH_MAX, "%s/%s", path, name);
        if (ent->d_type == DT_REG) {
            if (has_any_suffix(name, suffix, num_suffix)) {
                /* 将文件路径入队 */
                queue_node_t *node = queue_node_create(file_name);
                enqueue(q_root, node);
            }
        }

        if (ent->d_type == DT_DIR) {
            char new_path[FILE_PATH_MAX];
            snprintf(new_path, FILE_PATH_MAX, "%s/%s", path, name);
            walk(new_path, suffix, num_suffix);
        }
    }

    closedir(dir);
}
#endif

#ifdef WINDOWS
#include <windows.h>

void walk(const char *path, const char **suffixes, size_t num_suffixes)
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    char dirPath[FILE_PATH_MAX];
    snprintf(dirPath, FILE_PATH_MAX, "%s\\*", path);

    hFind = FindFirstFile(dirPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%ld)\n", GetLastError());
        return;
    }

    do {
        const char *name = findFileData.cFileName;

        // 忽略以点号开头的文件夹
        if (name[0] == '.') {
            continue;
        }

        char file_name[FILE_PATH_MAX];
        snprintf(file_name, FILE_PATH_MAX, "%s\\%s", path, name);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            walk(file_name, suffixes, num_suffixes);
        } else {
            if (has_any_suffix(name, suffixes, num_suffixes)) {
                /* 将文件路径入队 */
                queue_node_t *node = queue_node_create(replace_backslashes_with_slashes(file_name));
                enqueue(q_root, node);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

#endif