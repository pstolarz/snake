#include "SDL.h"
#include "stdio.h"

#include "stdlib.h"
#include "time.h"

void log(const char * s) {
    printf("%s\r\n", s);
}

typedef struct node_s node_t;

typedef struct node_s
{
    node_t * prev;
    node_t * next;
    int x;
    int y;
} node_t;

typedef struct snake_s
{
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

    tail->next = head; // [tail]-->[head]
    tail->x = tail_x;
    tail->y = tail_y;

    parent->head = head;
    parent->tail = tail;
}

void DeinitSnakeNode(node_t ** start) {
    // todo: dealloc all shizzles
}

void MoveSnake(snake_t * snake, int new_x, int new_y) {
    // traverse from tail to head
    node_t * curr = snake->tail;
    node_t * next = curr->next;

    while (1) {
        if (next) {
            curr->x = next->x;
            curr->y = next->y;
            curr = next; // increment
            next = next->next;
        }
        else { // empty 'next' means its head
            curr->x = new_x;
            curr->y = new_y;
            break;
        }
    }
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
int CollisionOccurred(snake_t * parent, int x, int y) {
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

#define MAX_BUGS 1

typedef struct {
    int count;
    struct
    {
        int x, y;
    } bug[MAX_BUGS];
} bugs_t;

bugs_t; void InitBugs(bugs_t * bugs) {
    bugs->count = 0;
    srand((unsigned)time(0));
}

void AddBug(bugs_t * bugs, snake_t * snake) {
    int upper = 9;
    int lower = 0;

    int x, y;
    while (1)
    {
        // generate random x,y
        x = (rand() % (upper - lower + 1)) + lower;
        y = (rand() % (upper - lower + 1)) + lower;

        // check collisions with snake
        if (CollisionOccurred(snake, x, y)) {
            continue;
        }
        else {
            break;
        }
    }
    // add new bug to bugs struct
    if (bugs->count < MAX_BUGS){
        bugs->bug[bugs->count].x = x;
        bugs->bug[bugs->count].y = y;
        printf("bug added: x=%d, y=%d\r\n", x, y);
        bugs->count++;
    }
}

int BugEaten(bugs_t * bugs, int x, int y) {
    int bug_found = 0;
    // search bugs
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
    snake_t         snake;
    bugs_t          bugs;

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

    InitSnakeNode(&g_data.snake, 2, 3, 1, 3);
    InitBugs(&g_data.bugs);
    AddBug(&g_data.bugs, &g_data.snake);
}

void gameplay_tick_100ms(key_type_t key_press, snake_t * snake, bugs_t * bugs) 
{
    static unsigned     game_speed = 5; // 1 second
    static unsigned     current_time = 0;
    static int          game_over = 0;
    static key_type_t   previous_key = E_KEY_RIGHT; // start direction

    if (!game_over) {
        if (current_time >= game_speed) {
            int new_x = snake->head->x;
            int new_y = snake->head->y;

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
                if (CollisionOccurred(snake, new_x, new_y)) {
                    game_over = 1;
                }
            }

            // change snake body
            if (!game_over) {
                if (BugEaten(bugs, new_x, new_y)) {
                    AddAndMoveHead(snake, new_x, new_y);
                    AddBug(bugs, snake);
                    
                    if (game_speed >= 2 && game_speed < 10) {
                        game_speed--;
                        printf("gamespeed=%d\r\n", game_speed);
                    }
                }
                else
                    MoveSnake(snake, new_x, new_y);
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
    //else log("gameover\r\n");
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

void game_render(SDL_Renderer * renderer, snake_t * snake, bugs_t * bugs) {
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
        node_t * i = snake->tail;
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        while (1)
        {
            if (i) {
                draw_rect(renderer, i->x, i->y);
            }
            else {
                break;
            }
            i = i->next;
        }
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
    
    SDL_DestroyRenderer(g_data.renderer);
    SDL_DestroyWindow(g_data.win);

    SDL_Quit();

    return 0;
}
