#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <stdio.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

int convert(const char *path);

#endif