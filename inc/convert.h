#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <stdio.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int gbk_to_utf8(char *inbuf, size_t *inlen, char *outbuf, size_t *outlen);
int utf8_to_gbk(char *inbuf, size_t *inlen, char *outbuf, size_t *outlen);

#endif