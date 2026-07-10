#include "CPU.cpp"

int main(int argc, char*argv[]){
    // creates CPU class for emulator
    CPU chip8 = CPU();
    // check if the user has send the ROM as an arg
    if(argc >=2){
        // load the ROM
        if(!chip8.load_ROM(argv[1])){
            std::cerr << "ERROR:\t failed to load ROM" << std::endl;
            
        }
    }
    else{
        // display error message
        std::cerr << "No ROM preloaded" << std::endl;
    }
    
    return 0;
}