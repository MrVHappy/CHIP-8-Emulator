#include <cstdint>   // fixed-width types: uint8_t, uint16_t, uint32_t
#include <cstring>   // memset, memcpy
#include <fstream>   // reading the ROM file
#include <array>     // std::array for fixed-size buffers (safer than raw C arrays)
#include <iostream>  // debugging/console output
#include <string>    // file paths
// references: https://github.com/eshyong/Chip-8-Emulator/blob/master/chip8.c
class CPU{
    private:
        // 4K RAM 
        uint8_t memory[4096];
        // the start adress for the Program counter
        const uint16_t START_ADRS = 0x200;
        
        // the 16 registers for the CHIP-8
        uint8_t V[16]; 

        // 16b register used to hold the memory addresses
        uint16_t index_reg;

        // points to the next instruction
        uint16_t Program_Counter = START_ADRS;

        // holds the return adresses when a function is called
        uint16_t stack[16];

        // points to the address of the stack
        uint8_t stack_ptr;

        // Timers: runs at 60Hz if non-zero
        // general purpose countdown a program can poll
        uint8_t delay_timer;
        // does the same as the delay timer but beebs at non zero
        uint8_t sound_timer;

        // used to display the screen at 64px:32px
        uint8_t display[64*32];

        // CHIP-8 Controls:
        uint8_t keypad[16];

        // determines the font size of the font set
        static constexpr int FONTSET_SIZE = 80;

        // font set for rendering
        const unsigned char fontset[FONTSET_SIZE] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
            0x20, 0x60, 0x20, 0x20, 0x70,		// 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
            0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
            0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
            0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
            0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
            0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
            0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
            0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
            0xF0, 0x80, 0xF0, 0x80, 0x80		// F
        };
    public:
        // constructor
        CPU(){
            // Temp
        }
        // load ROM to the emulator
        bool load_ROM(const std::string &path){
            // temp return
            return false;
        }
        // one FDE step
        void cycle(){
            // Temp
        }
        // decrement delay/sound at 60Hz
        void tick_timers(){
            // Temp
        }
        // read render
        const uint8_t* get_Display() const{
            return this->display;
        }

};