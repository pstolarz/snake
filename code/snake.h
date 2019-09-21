#pragma once

#include <stdint.h>

struct snake_body_link_s
{
    uint32_t x, y;
};
struct snake_body_s
{
    uint32_t head;
    uint32_t maxLinks;
    struct snake_body_link_s * link;
};

void Snake_Init(struct snake_body_s* const snake, uint32_t maxXLinks, uint32_t maxYinks);
void Snake_Deinit(struct snake_body_s* const snake);

void Snake_SetStartLocation(struct snake_body_s* const snake, uint32_t headX, uint32_t headY, uint32_t tailX, uint32_t tailY);

void Snake_Move(struct snake_body_s* const snake, uint32_t newX, uint32_t newY);

// create new node and change head co-ord
void Snake_AddLinkAndMove(struct snake_body_s* const snake, uint32_t newX, uint32_t newY);

// !0 - collision occurred; 0 - collision not occurred
int Snake_CheckCollision(const struct snake_body_s* const snake, uint32_t newX, uint32_t newY);
