
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <snake.h>

#define LOG(a) printf(a)
#define ASSERT(a) assert(a)

void Snake_Init(struct snake_body_s* const snake, const uint32_t maxXLinks, const uint32_t maxYlinks)
{
    ASSERT(snake);
    {
        snake->maxLinksCount = maxXLinks * maxYlinks;
        snake->pLinks = malloc(sizeof(struct snake_body_link_s) * snake->maxLinksCount);
    }
}

void Snake_Deinit(struct snake_body_s* const snake)
{
    ASSERT(snake);
    {
        free(snake->pLinks);
    }
}

void Snake_SetStartLocation(struct snake_body_s* const snake, const uint32_t headX, const uint32_t headY, const uint32_t tailX, const uint32_t tailY)
{
    ASSERT(snake);
    {
        snake->pLinks[0].x = tailX;
        snake->pLinks[0].y = tailY;
        snake->head++;

        snake->pLinks[1].x = headX;
        snake->pLinks[1].y = headY;
        snake->head++;
    }
}

void Snake_Move(struct snake_body_s* const snake, const uint32_t newX, const uint32_t newY)
{
    ASSERT(snake);
    {
        // traverse from tail to head
        uint32_t prev = 0;
        uint32_t next = 1;

        while (next < snake->head) {
            snake->pLinks[prev].x = snake->pLinks[next].x;
            snake->pLinks[prev].y = snake->pLinks[next].y;
            prev++; next++;
        }

        snake->pLinks[next - 1].x = newX;
        snake->pLinks[next - 1].y = newY;
    }
}

void Snake_AddLinkAndMove(struct snake_body_s* const snake, const uint32_t newX, const uint32_t newY)
{
    ASSERT(snake);
    {    
        // todo: sanity for snake->head
        snake->pLinks[snake->head].x = newX;
        snake->pLinks[snake->head].y = newY;
        snake->head++;
    }
}

// !0 - collision occurred; 0 - collision not occurred
int Snake_CheckCollision(const struct snake_body_s* const snake, const uint32_t newX, const uint32_t newY)
{
    ASSERT(snake);
    {
        // traverse for x,y co-ord
        uint32_t i = 0;
        for (; i < snake->head; i++) {
            if (snake->pLinks[i].x == newX && snake->pLinks[i].y == newY) {
                return 1;
            }
        }
    }
    return 0;
}
