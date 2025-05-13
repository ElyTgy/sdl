#include <SDL.h>
#include <iostream>
#include "2_render_image.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 426;

//window to render to
SDL_Window* window = nullptr; 

//an image that we will see in the window
SDL_Surface* screenSurface = nullptr; 

//The image we will load and show on the screen
SDL_Surface* image = nullptr;


bool init(){
    bool success = true;

    //initialize cant call anything else unless if i initialize first. Here we will just be using the video subsystem, which is the only flag passed in
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        //SDL_GetError() always says what goes wrong in a function => very useful
        std::cout<<"couldnt initialize. SDL error: "<<SDL_GetError();
        success = false;
    }
    else{
        window = SDL_CreateWindow("sdl-test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == nullptr){
            std::cout<<"couldnt create window. SDL error: "<<SDL_GetError();
        }
        else{
            screenSurface = SDL_GetWindowSurface(window);
        }
    }   
    return success;
}

bool myLoadMedia(){
    image = SDL_LoadBMP("2_image.bmp");
    if(image == nullptr){
        std::cout<<"couldnt create window. SDL error: "<<SDL_GetError();
        return false;
    }
    return true;
}

void close(){
    SDL_FreeSurface(image);
    image = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}

//sdl needs the arguments for main to look like this for some reason
int main(int argc, char* args[]){
    if(init()){
        if(myLoadMedia()){
            //a copy of src (image) is taken and stamped onto the destitionation surface
            SDL_BlitSurface(image, nullptr, screenSurface, nullptr);
            //we have drawn things to the screen but they wont show up until you decide to update the window
            //we draw to the back buffer and only update to front buffer when we are ready and everything is drawn
            //=> need to call update window after each blit, once all the blits are done
            SDL_UpdateWindowSurface(window);
            SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
            
            //Free resources and close SDL
             close();
        }
    }
    return 0;
}