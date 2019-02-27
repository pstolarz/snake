#include "SDL.h"
#include "stdio.h"

#include "stdlib.h"

void log(const char * s) {
    printf("%s\r\n", s);
}

//typedef enum
//{
//    E_UP,
//    E_DOWN,
//    E_LEFT,
//    E_RIGHT
//} direction_t;

typedef struct node_s node_t;

typedef struct node_s
{
    node_t * prev;
    node_t * next;
    int x;
    int y;
    //direction_t direction;
} node_t;

typedef struct snake_s
{
    //node_t * data;
    node_t * head;
    node_t * tail;
} snake_t;

void InitSnakeNode(snake_t * parent, int head_x, int head_y, int tail_x, int tail_y) {
    // todo: sanity check
    node_t * head = (node_t*)calloc(1,sizeof(node_t));
    node_t * tail = (node_t*)calloc(1,sizeof(node_t));

    head->prev = tail; // [tail]<--[head]
    head->x = head_x;
    head->y = head_y;
    //head->direction = E_RIGHT;

    tail->next = head; // [tail]-->[head]
    tail->x = tail_x;
    tail->y = tail_y;
    //head->direction = E_LEFT;

    parent->head = head;
    parent->tail = tail;
}

void DeinitSnakeNode(node_t ** start) {
    // todo: dealloc all shizzles
}

void MoveSnake(snake_t * parent, int new_x, int new_y) {
    // move head an
}

// create new node and change head co-ord
void AddAndMoveHead(snake_t * parent, int new_x, int new_y) {
    node_t * new_head = (node_t*)calloc(1, sizeof(node_t));
    node_t * old_head = parent->head;

    // [tail]<--[new_body]--[head]
    SDL_memcpy(new_head, old_head, sizeof(node_t));

    // [tail]--[old_head]--[new_head]-->0
    new_head->next = 0;
    // [tail]--[old_head]<--[new_head]
    new_head->prev = old_head;
    new_head->x = new_x;
    new_head->y = new_y;
    // [tail]--[old_head]-->[new_head]
    old_head->next = new_head;

    parent->head = new_head;
}

// traverse for x,y co-ord
int CheckCollision(snake_t * parent, int x, int y) {
    node_t * i = parent->tail;

    while (1)
    {
        if (i) {
            if (i->x == x && i->y == y)
                return 1;
        }
        else {
            break;
        }
        i = i->next;
    }
    return 0;
}

//
void MoveTail(snake_t * parent) {

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

#define LOGICAL_FIELD_WIDTH 10
#define LOGICAL_FIELD_HEIGHT 10

#define PIXEL_FIELD_WIDTH 50
#define PIXEL_FIELD_HEIGHT 50

typedef struct
{
    // todo: change to linked list to track the tail?
    field_type_t data[LOGICAL_FIELD_WIDTH * LOGICAL_FIELD_HEIGHT];
    int head_x;
    int head_y;
    int tail_x;
    int tail_y;
} field_t;

field_t field;

field_type_t GetFieldType(int x, int y) {
    // toto: sanity check
    int pos = x + y * (LOGICAL_FIELD_HEIGHT);
    return field.data[pos];
}

void SetFieldType(int x, int y, field_type_t type) {
    // toto: sanity check
    int pos = x + y * (LOGICAL_FIELD_HEIGHT);
    field.data[pos] = type;
}

int IsPointValid(int x, int y) {
    if (x >= 0 && x < LOGICAL_FIELD_WIDTH)
        if (y >= 0 && y < LOGICAL_FIELD_HEIGHT)
            return 1;
    return 0;
}

typedef struct
{
    SDL_Window * win;
    SDL_Renderer * renderer;
    snake_t snake;

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
    g_data.screen_width = LOGICAL_FIELD_WIDTH * PIXEL_FIELD_WIDTH;
    g_data.screen_height = LOGICAL_FIELD_WIDTH * PIXEL_FIELD_WIDTH;

    SDL_Init(SDL_INIT_VIDEO);

    g_data.win = SDL_CreateWindow("LolSnake", g_data.posX, g_data.posY, g_data.screen_width, g_data.screen_height, 0);

    g_data.renderer = SDL_CreateRenderer(g_data.win, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(g_data.renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

    InitSnakeNode(&g_data.snake, 3, 3, 2, 3);

    // field.head_x = 3;
    // field.head_y = 3;
    // field.tail_x = 2;
    // field.tail_y = 3;
    // 
    // // todo: fix field type? remove this for renderer
    // SetFieldType(field.head_x, field.head_y, E_BUG);
    // SetFieldType(field.tail_x, field.tail_y, E_BUG);
}

void gameplay_tick_100ms(key_type_t key_press, field_t * const field)
{
    const int game_speed = 10; // 1 second
 
    static int current_time = 0;

    static int game_over = 0;
    static key_type_t previous_key = E_KEY_RIGHT;

    if (!game_over) {
        if (current_time == game_speed) {
            printf("gamespeed=%d\r\n", current_time);
            
            // moving
            {
                // check direction
                // int new_x = field->head_x;
                // int new_y = field->head_y;
                int new_x = g_data.snake.head->x;
                int new_y = g_data.snake.head->y;

                switch (key_press)
                {
                case E_KEY_UP:
                    if (E_KEY_DOWN == previous_key) new_y++;
                    else { // dont update when direction is not changed
                        new_y--;
                        previous_key = key_press;
                    }
                    log("E_KEY_UP");
                    break;
                case E_KEY_DOWN:
                    if (E_KEY_UP == previous_key)
                        new_y--;
                    else
                    {
                        new_y++;
                        previous_key = key_press;
                    }
                    log("E_KEY_DOWN");
                    break;
                case E_KEY_LEFT:
                    if (E_KEY_RIGHT == previous_key)
                        new_x++;
                    else
                    {
                        new_x--;
                        previous_key = key_press;
                    }
                    log("E_KEY_LEFT");
                    break;
                case E_KEY_RIGHT:
                    if (E_KEY_LEFT == previous_key)
                        new_x--;
                    else
                    {
                        new_x++;
                        previous_key = key_press;
                    }
                    log("E_KEY_RIGHT");
                    break;
                }

                // check boundaries
                if (!IsPointValid(new_x, new_y)) {
                    game_over = 1;
                }

                // check field type
#if 1
                if (!game_over) {
                    if (CheckCollision(&g_data.snake, new_x, new_y)) {
                        game_over = 1;
                    }
                }
#else
                if (!game_over) {
                    if (E_EMPTY != GetFieldType(new_x, new_y)){
                        game_over = 1;
                    }
                }
#endif

                // change accordingly
                if (!game_over) {
                    AddAndMoveHead(&g_data.snake, new_x, new_y);
                    //SetFieldType(field->head_x, field->head_y, E_BUG); // body
                    //SetFieldType(new_x, new_y, E_BUG); // head
                    field->head_x = new_x;
                    field->head_y = new_y;
                }
            }

            current_time = 0;
        }
        else {
            current_time++;
        }
    }
    else
        log("gameover");
}

void draw_field(SDL_Renderer * renderer, int i) {
    // 1 field is 50 h and 50 w
    // x + y * (LOGICAL_FIELD_HEIGHT - 1)

    // todo: sanity check?
    const int border_size = 1;

    SDL_Rect frame;
    frame.x = PIXEL_FIELD_WIDTH * (int)(i % LOGICAL_FIELD_WIDTH) + border_size;
    frame.y = PIXEL_FIELD_HEIGHT * (int)(i / LOGICAL_FIELD_HEIGHT) + border_size;
    frame.w = PIXEL_FIELD_WIDTH - 2 * border_size;
    frame.h = PIXEL_FIELD_HEIGHT - 2 * border_size;
    SDL_RenderFillRect(renderer, &frame);
}

void draw_field2(SDL_Renderer * renderer, int x, int y) {
    // 1 field is 50 h and 50 w
    // x + y * (LOGICAL_FIELD_HEIGHT - 1)

    // todo: sanity check?
    const int border_size = 1;

    SDL_Rect frame;
    frame.x = PIXEL_FIELD_WIDTH * x + border_size;
    frame.y = PIXEL_FIELD_HEIGHT * y + border_size;
    frame.w = PIXEL_FIELD_WIDTH - 2 * border_size;
    frame.h = PIXEL_FIELD_HEIGHT - 2 * border_size;
    SDL_RenderFillRect(renderer, &frame);
}

void game_render(SDL_Renderer * renderer , const field_t * const field) {
    // int i;
    // 
    // for (i = 0; i < LOGICAL_FIELD_WIDTH * LOGICAL_FIELD_HEIGHT; i++) {
    //     if (E_EMPTY != field->data[i])
    //         draw_field(renderer, i);
    // }
    node_t * i = g_data.snake.tail;
    while (1)
    {
        if (i) {
            draw_field2(renderer, i->x, i->y);
        }
        else {
            break;
        }
        i = i->next;
    }
}

int Snake_Run(void)
{
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time = 0;

    log("run");
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
        if (current_time - last_time >= 100){ // 100 ms timestamp
            gameplay_tick_100ms(key_press, &field);
            game_render(g_data.renderer, &field);
            SDL_RenderPresent(g_data.renderer);
            last_time = current_time;
        }
    }
    
    SDL_DestroyRenderer(g_data.renderer);
    SDL_DestroyWindow(g_data.win);

    SDL_Quit();

    return 0;
}
