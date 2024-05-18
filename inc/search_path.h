#ifndef _SEARCH_PATH_H
#define _SEARCH_PATH_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

//计算二维数组行数
#define ROW_COUNT(x)  (sizeof(x) / sizeof(x[0]))

void walk(const char *path, const char **suffix, size_t num_suffix);

#endif