#include <cstdint>   // fixed-width types: uint8_t, uint16_t, uint32_t
#include <cstring>   // memset, memcpy
#include <fstream>   // reading the ROM file
#include <array>     // std::array for fixed-size buffers (safer than raw C arrays)
#include <iostream>  // debugging/console output
#include <string>    // file paths
#include <bitset>
#include <climits>
#include <fstream>
#include <utility>
#include <vector>

// references: 
// https://github.com/eshyong/Chip-8-Emulator/blob/master/chip8.c
// https://en.cppreference.com/cpp/string/byte/memset
// https://en.cppreference.com/cpp/string/byte/memcpy
// https://stackoverflow.com/questions/1163624/memcpy-with-startindex
// https://austinmorlan.com/posts/chip8_emulator/#loading-a-rom
// https://cplusplus.com/reference/istream/istream/seekg/
// https://www.geeksforgeeks.org/cpp/function-pointer-to-member-function-in-cpp/

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
            // zeroing out memory, V, stack, display and keypad
            std::memset(this->memory, 0, sizeof(this->memory));
            std::memset(this->V,0,sizeof(this->V));
            std::memset(this->stack,0,sizeof(this->stack));
            std::memset(this->display,0,sizeof(this->display));
            std::memset(this->keypad,0,sizeof(this->keypad));

            // reset index_reg, stack_ptr, delay_timer and sound_timer to 0
            this->index_reg = 0;
            this->stack_ptr = 0;
            this->delay_timer = 0;
            this->sound_timer = 0;

            // copy font set to memory starting at postion 0x50
            memcpy(&this->memory[0x50], this->fontset, sizeof(this->fontset));
        }
        // load ROM to the emulator
        bool load_ROM(const char* path){
            // read the file and format it to binary at the end position
            std::ifstream file(path,std::ios::binary | std::ios::ate);
            // check if the file exsists
            if(file.is_open()){
                // checks to see if the file can be stored in RAM or not
                std::streamsize file_size = file.tellg();
                if(file_size > (4096 - START_ADRS)){
                    // return false if the file size is too big
                    return false;
                }
                // create a buffer vector of length of the ROM
                std::vector<char> buffer(file_size);
                file.seekg(0,std::ios::beg);
                // read data as a block
                file.read(buffer.data(),file_size);

                // checks to see if the file isn't corrupted
                if (file.fail()){
                    return false;
                }

                // load ROM to memory
                for (long i = 0; i <file_size; ++i){
                    memory[START_ADRS + i] = buffer[i];
                }

                return true;
            }
            else{
                // if it doesn't exist then return false
                return false;
            }
            
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

        // instruction set functions

        void clear(){

        }

        void exit_sub(){

        }

        void jump(){

        }

        void call_sub(){

        }

        void not_equal_NN(){

        }

        void equal_NN(){

        }

        void VX_not_equal_VY(){

        }

        void assign_VX(){

        }

        void increment_VX(){

        }

        void VX_copy_VY(){

        }

        void VX_OR_VY(){

        }

        void VX_AND_VY(){

        }
        
        void VX_XOR_VY(){

        }

        void VX_VY_carry(){

        }

        void VX_VY_borrow(){

        }

        void OLD_LEAST_SIGb(){

        }

        void VX_VY_0_on_borrow(){

        }

        void OLD_MOST_SIGb(){

        }

        void VX_EQUAL_VY(){

        }

        void set_i(){

        }

        void jump_0_NNN(){

        }

        void sprite(){

        }

        void key_not_pressed(){

        }

        void key_pressed(){

        }

        void VX_delay(){

        }

        void wait_for_key(){

        }

        void delay_VX(){

        }

        void buzzer_VX(){

        }

        void i_ADD_VX(){

        }

        void i_HEX_VX(){

        }

        void decode(){

        }

        void save_VX(){

        }

        void load_VX(){

        }
};