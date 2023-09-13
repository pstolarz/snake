#include "bugs.h"

#include "debug.h"

void Bugs_Init(struct bugs_s * const bugs)
{
    bugs->count = 0;
    for (int i = 0; i < MAX_BUGS; i++)
    {
        bugs->bug[i].x = 0;
        bugs->bug[i].y = 0;
        bugs->bug[i].valid = 0;
    }
}

void Bugs_Add(struct bugs_s * const bugs, const uint32_t x, const uint32_t y)
{
    // add new bug to bugs struct
    for (int i = 0; i < MAX_BUGS; i++)
    {
        if (bugs->bug[i].valid == 0) // search for first non-valid bug
        {

            bugs->bug[i].x = x;
            bugs->bug[i].y = y;
            bugs->bug[i].valid = 1;
            Log("bug added: x=%d, y=%d\r\n", x, y);
            bugs->count++;
            break;
        }
    }
}

int Bugs_Remove(struct bugs_s * const bugs, const uint32_t x, const uint32_t y)
{
    int bug_found = 0;
    // search eaten bug
    int i = 0;

    for (; i < bugs->count; i++)
    {
        if (bugs->bug[i].x == x && bugs->bug[i].y == y) {
            bug_found = 1;
            break;
        }
    }
    //if found, remove it
    if (bug_found) {
        Log("bug found: x=%d, y=%d\r\n", x, y);
        bugs->bug[i].valid = 0;
        bugs->count--;
    }

    return bug_found;
}
