#include "search_path.h"

#define FILE_PATH_MAX 2048

void walk(char *path)
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
        if (name[0] == '.' || strcmp(name, "build") == 0)
            continue;

        char file_name[FILE_PATH_MAX];
        snprintf(file_name, FILE_PATH_MAX, "%s/%s", path, name);
        if (ent->d_type == DT_REG) {
            // TODO: add to queue
            printf("%s\n", file_name);
        }

        if (ent->d_type == DT_DIR) {
            char new_path[FILE_PATH_MAX];
            snprintf(new_path, FILE_PATH_MAX, "%s/%s", path, name);
            walk(new_path);
        }
    }

    closedir(dir);
}
