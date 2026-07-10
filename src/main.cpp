#include "CPU.cpp"

int main(int argc, char*argv[]){
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