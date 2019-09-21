
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <snake.h>

#define LOG(a) printf(a)
#define ASSERT(a) assert(a)

void Snake_Init(struct snake_body_s* const snake, uint32_t maxXLinks, uint32_t maxYinks)
{
    ASSERT(snake);
    {
        snake->maxLinks = maxXLinks * maxYinks;
        snake->link = malloc(sizeof(struct snake_body_link_s) * snake->maxLinks);
    }
}

void Snake_Deinit(struct snake_body_s* const snake)
{
    ASSERT(snake);
    {
        free(snake->link);
    }
}

void Snake_SetStartLocation(struct snake_body_s* const snake, uint32_t headX, uint32_t headY, uint32_t tailX, uint32_t tailY)
{
    ASSERT(snake);
    {
        snake->link[0].x = tailX;
        snake->link[0].y = tailY;
        snake->head++;

        snake->link[1].x = headX;
        snake->link[1].y = headY;
        snake->head++;
    }
}

void Snake_Move(struct snake_body_s* const snake, uint32_t newX, uint32_t newY)
{
    ASSERT(snake);
    {
        // traverse from tail to head
        uint32_t prev = 0;
        uint32_t next = 1;

        while (next < snake->head) {
            snake->link[prev].x = snake->link[next].x;
            snake->link[prev].y = snake->link[next].y;
            prev++; next++;
        }

        snake->link[next - 1].x = newX;
        snake->link[next - 1].y = newY;
    }
}

void Snake_AddLinkAndMove(struct snake_body_s* const snake, uint32_t newX, uint32_t newY)
{
    ASSERT(snake);
    {    
        // todo: sanity for snake->head
        snake->link[snake->head].x = newX;
        snake->link[snake->head].y = newY;
        snake->head++;
    }
}

// !0 - collision occurred; 0 - collision not occurred
int Snake_CheckCollision(const struct snake_body_s* const snake, uint32_t newX, uint32_t newY)
{
    ASSERT(snake);
    {
        // traverse for x,y co-ord
        uint32_t i = 0;
        for (; i < snake->head; i++) {
            if (snake->link[i].x == newX && snake->link[i].y == newY) {
                return 1;
            }
        }
    }
    return 0;
}
