#include "SDL.h"

#include <debug.h>

#include <snake.h>
#include <bugs.h>

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

struct game_data_s
{
    struct snake_body_s    snake;
    struct bugs_s          bugs;

    int running;
};

typedef struct
{
    SDL_Window *    win;
    SDL_Renderer *  renderer;

    int posX;
    int posY;
    int screen_width;
    int screen_height;
} render_data_t;

static struct game_data_s   g_game_data;
static render_data_t g_render_data;

void GenerateNewBug(struct game_data_s * const game_data)
{
    int upper = 9;
    int lower = 0;

    int x, y;
    while (1)
    {
        // generate random x,y
        x = (rand() % (upper - lower + 1)) + lower;
        y = (rand() % (upper - lower + 1)) + lower;

        // check collisions with snake
        if (Snake_CheckCollision(&game_data->snake, x, y)) {
            continue;
        }
        else {
            break;
        }
    }

    Bugs_Add(&game_data->bugs, x, y);
}

void Game_Init(void)
{
    srand((unsigned)time(0));

    // init renderer
    g_render_data.win = NULL;
    g_render_data.renderer = NULL;

    g_render_data.posX = 100;
    g_render_data.posY = 100;
    g_render_data.screen_width = LOGICAL_FIELD_WIDTH * PIXEL_FIELD_WIDTH;
    g_render_data.screen_height = LOGICAL_FIELD_HEIGHT * PIXEL_FIELD_WIDTH;

    SDL_Init(SDL_INIT_VIDEO);

    g_render_data.win = SDL_CreateWindow("LolSnake", g_render_data.posX, g_render_data.posY, g_render_data.screen_width, g_render_data.screen_height, 0);
    g_render_data.renderer = SDL_CreateRenderer(g_render_data.win, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(g_render_data.renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

    // init game logic
    Snake_Init(&g_game_data.snake, LOGICAL_FIELD_WIDTH, LOGICAL_FIELD_HEIGHT);
    Snake_SetStartLocation(&g_game_data.snake, 2, 3, 1, 3);

    Bugs_Init(&g_game_data.bugs);
    GenerateNewBug(&g_game_data);
}

void gameplay_tick_100ms(struct game_data_s * const game_data, key_type_t key_press)
{
    static unsigned     game_speed = 5; // 1 second
    static unsigned     current_time = 0;
    static int          game_over = 0;
    static key_type_t   previous_key = E_KEY_RIGHT; // start direction

    if (!game_over) {
        if (current_time >= game_speed) {
            int new_x = game_data->snake.pLinks[game_data->snake.head - 1].x;
            int new_y = game_data->snake.pLinks[game_data->snake.head - 1].y;

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

            // check logical boundaries of new x, y
            if (!IsPointValid(new_x, new_y)) {
                game_over = 1;
            }

            // check collisions with snake body
            if (!game_over) {
                if (Snake_CheckCollision(&game_data->snake, new_x, new_y)) {
                    game_over = 1;
                }
            }

            // change snake body
            if (!game_over) {
                if (Bugs_Remove(&game_data->bugs, new_x, new_y)) {
                    Snake_AddLinkAndMove(&game_data->snake, new_x, new_y);
                    GenerateNewBug(&g_game_data);
                    GenerateNewBug(&g_game_data);
                    
                    if (game_speed >= 1 && game_speed < 10) {
                        game_speed--;
                        Log("gamespeed=%d\r\n", game_speed);
                    }
                }
                else
                    Snake_Move(&game_data->snake, new_x, new_y);
            }
            else {
                const char * keys[4] = { "E_KEY_UP", "E_KEY_DOWN", "E_KEY_LEFT", "E_KEY_RIGH" };
                Log("gameover at x=%d, y=%d, gamespeed=%d lastkey=%s, previous_key=%s\r\n",
                    new_x, new_y, game_speed, keys[key_press], keys[previous_key]);
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

void game_render(SDL_Renderer * renderer, struct game_data_s * const game_data) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // render bugs
    {
        int i = 0;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        for (; i < game_data->bugs.count; i++) {
            draw_rect(renderer, game_data->bugs.bug[i].x, game_data->bugs.bug[i].y);
        }
    }

    // render snake
    {
        uint32_t i = 0;
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        for (; i < game_data->snake.head; i++) {
            draw_rect(renderer, game_data->snake.pLinks[i].x, game_data->snake.pLinks[i].y);
        }
    }

    SDL_RenderPresent(renderer);
}

int Game_Run(void)
{
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time = 0;

    Log("run");
    g_game_data.running = 1;

    while (g_game_data.running) {
        SDL_Event e;
        static key_type_t key_press = E_KEY_RIGHT;

        current_time = SDL_GetTicks();

        if (SDL_PollEvent(&e)) {
            switch (e.type)
            {
            case SDL_QUIT:
                g_game_data.running = 0;
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

        if (current_time - last_time >= 100){ // 100 ms timestamp
            gameplay_tick_100ms(&g_game_data, key_press);
            game_render(g_render_data.renderer, &g_game_data);
            last_time = current_time;
        }
    }
    
    Snake_Deinit(&g_game_data.snake);

    SDL_DestroyRenderer(g_render_data.renderer);
    SDL_DestroyWindow(g_render_data.win);

    SDL_Quit();

    return 0;
}
