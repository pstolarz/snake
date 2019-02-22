#include <stdio.h>
#include <SDL.h>

int main(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        //std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        printf("SDL_Init_Error: %s", SDL_GetError());
        return 1;
    }
    SDL_Quit();
    system("pause");
    return 0;
}