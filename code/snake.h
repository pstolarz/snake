#pragma once

#include <stdint.h>

// Snake body link defined by logical x,y coordinates.
struct snake_body_link_s
{
    uint32_t x, y;
};

// Snake body keep information of all snake body links and snake head position.
struct snake_body_s
{
    uint32_t head;
    uint32_t maxLinksCount;
    struct snake_body_link_s * pLinks;
};

// Initialize snake_body_s instance based on input paramters.
void Snake_Init(struct snake_body_s* const snake, const uint32_t xCount, const uint32_t yCount);
void Snake_Deinit(struct snake_body_s* const snake);

void Snake_SetStartLocation(struct snake_body_s* const snake, const uint32_t headX, const uint32_t headY, const uint32_t tailX, const uint32_t tailY);

// Move snake without adding new link at the end
void Snake_Move(struct snake_body_s* const snake, const uint32_t newX, const uint32_t newY);

// Move snake and add link at the end
void Snake_AddLinkAndMove(struct snake_body_s* const snake, const uint32_t newX, const uint32_t newY);

// !0 - collision occurred; 0 - collision not occurred
int Snake_CheckCollision(const struct snake_body_s* const snake, const uint32_t newX, const uint32_t newY);
