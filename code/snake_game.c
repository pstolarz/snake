#include "SDL.h"
#include "stdio.h"

void log(const char * s) {
    printf("%s\r\n", s);
}

typedef enum
{
    E_EMPTY,
    E_HEAD,
    E_TAIL,
    E_BODY,
    E_BUG
} field_type_t;

typedef enum
{
    E_KEY_UP,
    E_KEY_DOWN,
    E_KEY_LEFT,
    E_KEY_RIGHT
} key_type_t;

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 10

typedef struct
{
    field_type_t data[FIELD_WIDTH * FIELD_HEIGHT];
} field_t;

field_t field;

field_type_t GetFieldType(int x, int y) {
    // toto: sanity check
    return field.data[x + y * (FIELD_HEIGHT - 1)];
}

void SetFieldType(int x, int y, field_type_t type) {
    // toto: sanity check
    field.data[x + y * (FIELD_HEIGHT - 1)] = type;
}

typedef struct
{
    SDL_Window *win;
    SDL_Renderer *renderer;

    int posX;
    int posY;
    int screen_width;
    int screen_height;
    
    int running;
} game_data_t;

static game_data_t g_data;

void Snake_Init(void)
{
    g_data.win = NULL;
    g_data.renderer = NULL;

    g_data.posX = 100;
    g_data.posY = 100;
    g_data.screen_width = 600;
    g_data.screen_height = 600;

    SDL_Init(SDL_INIT_VIDEO);

    g_data.win = SDL_CreateWindow("LolSnake", g_data.posX, g_data.posY, g_data.screen_width, g_data.screen_height, 0);

    g_data.renderer = SDL_CreateRenderer(g_data.win, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(g_data.renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_Rect frame;
    frame.x = 0;
    frame.y = 0;
    frame.w = 50;
    frame.h = 50;
    SDL_RenderDrawRect(g_data.renderer, &frame);

}

void gameplay_tick_100ms(key_type_t key_press)
{
}

int Snake_Run(void)
{
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time = 0;

    while (1) {
        SDL_Event e;
        key_type_t key_press = E_KEY_RIGHT;

        current_time = SDL_GetTicks();

        if (SDL_PollEvent(&e)) {
            switch (e.type)
            {
            case SDL_QUIT:
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    key_press = E_KEY_UP;
                    break;
                case SDLK_DOWN:
                    key_press = E_KEY_DOWN;
                    break;
                case SDLK_LEFT:
                    key_press = E_KEY_LEFT;
                    break;
                case SDLK_RIGHT:
                    key_press = E_KEY_RIGHT;
                    break;
                }
            }
        }

        //SDL_RenderClear(g_data.renderer);
        if (current_time - last_time > 100){ // 100 ms timestamp
            SDL_RenderPresent(g_data.renderer);
            last_time = current_time;
        }
    }
    
    SDL_DestroyRenderer(g_data.renderer);
    SDL_DestroyWindow(g_data.win);

    SDL_Quit();

    return 0;
}
