#include <SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//sdl needs the arguments for main to look like this for some reason
int main(int argc, char* args[]){
    //window to render to
    SDL_Window* window = nullptr; 
    
    //an image that we will see in the window
    SDL_Surface* screenSurface = nullptr; 

    //initialize cant call anything else unless if i initialize first. Here we will just be using the video subsystem, which is the only flag passed in
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        //SDL_GetError() always says what goes wrong in a function -> very useful
        std::cout<<"couldnt initialize. SDL error: "<<SDL_GetError();
    }
    else{
        window = SDL_CreateWindow("sdl-test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        screenSurface = SDL_GetWindowSurface(window);
        SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);
        
        //getting the window to stay up
        SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
        
        //do this to free up memory
        SDL_DestroyWindow( window );

        //Quit SDL subsystems
        SDL_Quit();

    return 0;
    }

    
}