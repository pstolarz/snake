#pragma once

#include <stdint.h>

#include "time.h"
#include "stdlib.h" // srand


#define MAX_BUGS 2

struct bugs_s {
    int count;
    struct
    {
        int valid;
        int x, y;
    } bug[MAX_BUGS];
};

void Bugs_Init(struct bugs_s * const bugs);

void Bugs_Add(struct bugs_s * const bugs, const uint32_t x, const uint32_t y);

int Bugs_Remove(struct bugs_s * const bugs, const uint32_t x, const uint32_t y);

