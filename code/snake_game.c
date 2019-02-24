#include "snake_render.h"

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

    int running;
} game_data_t;

static game_data_t g_game_data;

void Snake_Init(void)
{
    Render_Init(800, 600);


}

void Snake_Update(key_press_t current_key_pressed)
{


}

int Snake_Run(void)
{
    while (g_game_data.running)
    {
        key_press_t key_pressed = Render_GetLastKeyPressed();
        Snake_Update(key_pressed);
        Render_Run();

        // sleep 100 ms
    }
}
