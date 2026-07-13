#include "CPU.cpp"
#include <SDL.h>

// references:
// https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidebasicsinit.html
// 
// 

int main(int argc, char*argv[]){
    // define SDL video and audio
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) <= -1){
        // display error message if initialisation fails
        std::cerr << "ERROR:\t failed to initialise SDL" << std::endl; 
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
    std::cout<< "Loading ROM:" << std::endl;
    while(running){
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