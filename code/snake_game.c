#include "SDL.h"

typedef enum
{
    E_MENU,
    E_RUNNING,
    E_PAUSED
} game_state_t;

typedef enum
{
    E_START,
//    E_OPTIONS,
    E_EXIT
} menu_pos_t;


typedef struct
{
    game_state_t state;
    menu_pos_t   menu_position;

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
    frame.x = 50;
    frame.y = 50;
    frame.w = 500;
    frame.h = 500;
    SDL_RenderDrawRect(g_data.renderer, &frame);

}


int Snake_Run(void)
{
    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        //SDL_RenderClear(g_data.renderer);
        SDL_RenderPresent(g_data.renderer);
    }
    
    SDL_DestroyRenderer(g_data.renderer);
    SDL_DestroyWindow(g_data.win);

    SDL_Quit();

    return 0;
}
