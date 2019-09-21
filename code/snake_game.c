#include "SDL.h"
#include "stdio.h"

#include "stdlib.h"
#include "time.h"

#include <snake.h>

void printLog(const char * s) {
    printf("%s\r\n", s);
}

#define MAX_BUGS 2

typedef struct {
    int count;
    struct
    {
        int valid;
        int x, y;
    } bug[MAX_BUGS];
} bugs_t;

bugs_t; void InitBugs(bugs_t * const bugs) {
    bugs->count = 0;
    srand((unsigned)time(0));
}

void AddBug(bugs_t * const bugs, const struct snake_body_s* const snake) {
    int upper = 9;
    int lower = 0;

    int x, y;
    while (1)
    {
        // generate random x,y
        x = (rand() % (upper - lower + 1)) + lower;
        y = (rand() % (upper - lower + 1)) + lower;

        // check collisions with snake
        if (Snake_CheckCollision(snake, x, y)) {
            continue;
        }
        else {
            break;
        }
    }
    // add new bug to bugs struct
    if (bugs->count < MAX_BUGS){
        int i = 0;
        for (; i < bugs->count; i++) {
            if (bugs->bug[i].valid == 0) // search for first non-valid bug
                break;
        }

        bugs->bug[i].x = x;
        bugs->bug[i].y = y;
        bugs->bug[i].valid = 1;
        printf("bug added: x=%d, y=%d\r\n", x, y);
        bugs->count++;
    }
}

int BugEaten(bugs_t * bugs, int x, int y) {
    int bug_found = 0;
    // search eaten bug
    int i = 0;
    for (; i < bugs->count; i++) {
        if (bugs->bug[i].x == x && bugs->bug[i].y == y) {
            bug_found = 1;
            break;
        }
    }
    //if found, remove it
    if (bug_found) {
        printf("bug found: x=%d, y=%d\r\n", x, y);
        bugs->bug[i].valid = 0;
        bugs->count--;
    }
    
    return bug_found;
}

typedef enum
{
    E_KEY_UP = 0,
    E_KEY_DOWN,
    E_KEY_LEFT,
    E_KEY_RIGHT
} key_type_t;

#define LOGICAL_FIELD_WIDTH 10
#define LOGICAL_FIELD_HEIGHT 10

#define PIXEL_FIELD_WIDTH 50
#define PIXEL_FIELD_HEIGHT 50

int IsPointValid(int x, int y) {
    if (x >= 0 && x < LOGICAL_FIELD_WIDTH)
        if (y >= 0 && y < LOGICAL_FIELD_HEIGHT)
            return 1;
    return 0;
}

typedef struct
{
    SDL_Window *    win;
    SDL_Renderer *  renderer;
    struct snake_body_s    snake;
    bugs_t          bugs;

    int posX;
    int posY;
    int screen_width;
    int screen_height;
    
    int running;
} game_data_t;

static game_data_t g_data;

void Game_Init(void)
{
    g_data.win = NULL;
    g_data.renderer = NULL;

    g_data.posX = 100;
    g_data.posY = 100;
    g_data.screen_width = LOGICAL_FIELD_WIDTH * PIXEL_FIELD_WIDTH;
    g_data.screen_height = LOGICAL_FIELD_WIDTH * PIXEL_FIELD_WIDTH;

    SDL_Init(SDL_INIT_VIDEO);

    g_data.win = SDL_CreateWindow("LolSnake", g_data.posX, g_data.posY, g_data.screen_width, g_data.screen_height, 0);
    g_data.renderer = SDL_CreateRenderer(g_data.win, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(g_data.renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

    Snake_Init(&g_data.snake, 50, 50);
    Snake_SetStartLocation(&g_data.snake, 2, 3, 1, 3);

    InitBugs(&g_data.bugs);
    AddBug(&g_data.bugs, &g_data.snake);
}

void gameplay_tick_100ms(key_type_t key_press, struct snake_body_s* snake, bugs_t * bugs)
{
    static unsigned     game_speed = 5; // 1 second
    static unsigned     current_time = 0;
    static int          game_over = 0;
    static key_type_t   previous_key = E_KEY_RIGHT; // start direction

    if (!game_over) {
        if (current_time >= game_speed) {
            int new_x = snake->link[snake->head - 1].x;
            int new_y = snake->link[snake->head - 1].y;

            switch (key_press) // check direction
            {
            case E_KEY_UP: // dont update when direction is not changed
                if (E_KEY_DOWN == previous_key){  new_y++;} else {new_y--; previous_key = key_press;}
                break;
            case E_KEY_DOWN:
                if (E_KEY_UP == previous_key){    new_y--;} else {new_y++; previous_key = key_press;}
                break;
            case E_KEY_LEFT:
                if (E_KEY_RIGHT == previous_key){ new_x++;} else {new_x--; previous_key = key_press;}
                break;
            case E_KEY_RIGHT:
                if (E_KEY_LEFT == previous_key){  new_x--;} else {new_x++; previous_key = key_press;}
                break;
            }

            // check boundaries
            if (!IsPointValid(new_x, new_y)) {
                game_over = 1;
            }

            // check collisions with snake body
            if (!game_over) {
                if (Snake_CheckCollision(snake, new_x, new_y)) {
                    game_over = 1;
                }
            }

            // change snake body
            if (!game_over) {
                if (BugEaten(bugs, new_x, new_y)) {
                    Snake_AddLinkAndMove(snake, new_x, new_y);
                    AddBug(bugs, snake);
                    AddBug(bugs, snake);
                    
                    if (game_speed >= 1 && game_speed < 10) {
                        game_speed--;
                        printf("gamespeed=%d\r\n", game_speed);
                    }
                }
                else
                    Snake_Move(snake, new_x, new_y);
            }
            else {
                const char * keys[4] = { "E_KEY_UP", "E_KEY_DOWN", "E_KEY_LEFT", "E_KEY_RIGH" };
                printf("gameover at x=%d, y=%d, gamespeed=%d lastkey=%s, previous_key=%s\r\n", new_x, new_y, game_speed, keys[key_press], keys[previous_key]);
            }

            current_time = 0;
        }
        else {
            current_time++;
        }
    }
    //else printLog("gameover\r\n");
}

void draw_rect(SDL_Renderer * renderer, int x, int y) {
    const int border_size = 1;
    
    SDL_Rect frame; // 50x50 pixel
    frame.x = PIXEL_FIELD_WIDTH * x + border_size;
    frame.y = PIXEL_FIELD_HEIGHT * y + border_size;
    frame.w = PIXEL_FIELD_WIDTH - 2 * border_size;
    frame.h = PIXEL_FIELD_HEIGHT - 2 * border_size;
    SDL_RenderFillRect(renderer, &frame);
}

void game_render(SDL_Renderer * renderer, struct snake_body_s* snake, bugs_t * bugs) {
    // render bugs
    {
        int i = 0;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        for (; i < bugs->count; i++) {
            draw_rect(renderer, bugs->bug[i].x, bugs->bug[i].y);
        }
    }

    // render snake
    {
        uint32_t i = 0;
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        for (; i < snake->head; i++) {
            draw_rect(renderer, snake->link[i].x, snake->link[i].y);
        }
    }
}

int Game_Run(void)
{
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time = 0;

    printLog("run");
    g_data.running = 1;

    while (g_data.running) {
        SDL_Event e;
        static key_type_t key_press = E_KEY_RIGHT;

        current_time = SDL_GetTicks();

        if (SDL_PollEvent(&e)) {
            switch (e.type)
            {
            case SDL_QUIT:
                g_data.running = 0;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:   key_press = E_KEY_UP;   break;
                case SDLK_DOWN: key_press = E_KEY_DOWN; break;
                case SDLK_LEFT: key_press = E_KEY_LEFT; break;
                case SDLK_RIGHT:key_press = E_KEY_RIGHT;break;
                }
            }
        }

        SDL_SetRenderDrawColor(g_data.renderer, 0x00, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(g_data.renderer);

        if (current_time - last_time >= 100){ // 100 ms timestamp
            gameplay_tick_100ms(key_press, &g_data.snake, &g_data.bugs);
            game_render(g_data.renderer, &g_data.snake, &g_data.bugs);
            SDL_RenderPresent(g_data.renderer);
            last_time = current_time;
        }
    }
    
    Snake_Deinit(&g_data.snake);

    SDL_DestroyRenderer(g_data.renderer);
    SDL_DestroyWindow(g_data.win);

    SDL_Quit();

    return 0;
}
