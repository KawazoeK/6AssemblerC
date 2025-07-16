#pragma once
#include <stdio.h>
#include <stdlib.h> // atoi関数のために追加
#include <string.h> // strdup関数のために追加
#include <ctype.h>  // isdigit関数のために追加

typedef struct assembler {
    char *program;
    int binary[16];
    struct assembler *next;
} assembler;