
typedef enum
{
    E_KEY_ESC,
    E_KEY_ENTER,
    E_KEY_UP,
    E_KEY_DOWN,
    E_KEY_LEFT,
    E_KEY_RIGHT
} key_press_t;

void Render_Init(int width, int height);
void Render_Run(void);
key_press_t Render_GetLastKeyPressed(void);