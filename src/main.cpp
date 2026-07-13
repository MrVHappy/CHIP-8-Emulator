#include "CPU.cpp"
#include <SDL.h>

// references:
// https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidebasicsinit.html
// https://thenumb.at/cpp-course/sdl2/01/01.html
// https://wiki.libsdl.org/SDL2/SDL_EventType
// https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
// https://thenumb.at/cpp-course/sdl2/05/05.html
// 
// 
// 
// 
// 
// 
// 

// display the screen 
void display (SDL_Renderer *renderer, SDL_Texture *texture, CPU &chip8){

}


int main(int argc, char*argv[]){
    // define SDL video and audio
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) <= -1){
        // display error message if initialisation fails
        std::cerr << "ERROR:\t failed to initialise SDL" << std::endl;
        // End application
        system("pause");
        return 1;
    }
    // create a window called "Chip 8 Emulator" @ 1280:720 displayed at the centre
    SDL_Window *window = SDL_CreateWindow("Chip 8 Emulator",SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,1280, 720, SDL_WINDOW_SHOWN);
    
    // check if the window has been created:
    if(!window){
        // display error mesage if window creation failed 
        std::cerr << "ERROR:\t window creation failed" << std::endl;
        // End application
        system("pause");
        return 1;
    }

    // get renderer from the window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // check if renderer has been created:
    if(renderer == NULL){
        // display error message
        std::cerr << "ERROR:\t failed to create renderer" << std::endl;
        // End application
        system("pause");
        return 1;
    }

    // get texture from renderer
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 64,32);

    // check if texture has been created:
    if(!texture){
        // display error message
        std::cerr << "ERROR:\t failed to create texture" << std::endl;
        // End application
        system("pause");
        return 1;
    }
    
    // Cycles per frame:
    const int CYCLES_PER_FRAME = 500 / 60;
    // creates CPU class for emulator
    CPU chip8 = CPU();
    // check if the user has send the ROM as an arg
    if(argc >=2){
        // load the ROM
        if(!chip8.load_ROM(argv[1])){
            std::cerr << "ERROR:\t failed to load ROM" << std::endl;
            return 1;
        }
    }
    else{
        // display error message
        std::cerr << "No ROM preloaded" << std::endl;
        // TEMP
        return 1;
    }
    bool running = true;
    // used to check if the window is open
    SDL_Event event;
    std::cout<< "Loading ROM:" << std::endl;
    while(running){
        while(SDL_PollEvent(&event)){
            // checks if the user has closed the window 
           if(event.type == SDL_QUIT){
                // stop the application
                running = false;
           }
            
        }
        // perform the FDE cycle
        for(int i = 0; i < CYCLES_PER_FRAME; i++){
            chip8.cycle();
        }

        // update timers
        chip8.tick_timers();

        // render display
    }

    
    return 0;
}