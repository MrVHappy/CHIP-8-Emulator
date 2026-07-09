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
#include <cstdlib>

// references: 
// https://github.com/eshyong/Chip-8-Emulator/blob/master/chip8.c
// https://en.cppreference.com/cpp/string/byte/memset
// https://en.cppreference.com/cpp/string/byte/memcpy
// https://stackoverflow.com/questions/1163624/memcpy-with-startindex
// https://austinmorlan.com/posts/chip8_emulator/#loading-a-rom
// https://cplusplus.com/reference/istream/istream/seekg/
// https://www.geeksforgeeks.org/cpp/function-pointer-to-member-function-in-cpp/
// https://www.geeksforgeeks.org/cpp/cpp-bitwise-operators/
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#8xy4
// https://www.geeksforgeeks.org/dsa/print-kth-least-significant-bit-number/



class CPU{
    private:
        // 4K RAM 
        uint8_t memory[4096];

        // flag
        bool flag = false;

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
        uint32_t display[64*32];

        // CHIP-8 Controls:
        uint8_t keypad[16];

        // opcode parameters:
        uint8_t opcode_x, opcode_y, opcode_nn, opcode_n;
        uint16_t opcode_nnn;

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
            // Fetch instruction
            uint8_t high_byte = this->memory[this->Program_Counter];
            uint8_t low_byte = this->memory[this->Program_Counter + 1];

            // combine the bytes together
            uint16_t opcode = (high_byte << 8) | low_byte;

            // update the PC
            this->Program_Counter += 2;

            // decoding the opcode
            this->opcode_x = (opcode & 0x0F00) >> 8;
            this->opcode_y = (opcode & 0x00F0) >> 4;
            this->opcode_nnn = (opcode & 0x0FFF);
            this->opcode_nn = (opcode & 0x00FF);
            this->opcode_n = opcode & 0x000F;

        }
        // decrement delay/sound at 60Hz
        void tick_timers(){
            // Temp
        }
        // read render
        const uint32_t* get_Display() const{
            return this->display;
        }

        // instruction set functions
        // 00E0
        void clear(){
            // clear display
            for(int x = 0; x < 2048; x++){
                display[x] = 0;
            }
        }
        // 00EE
        void exit_sub(){
            // decrement the stack pointer
            this->stack_ptr--;
            // get the assign the PC to the new stack address
            this->Program_Counter = this->stack[this->stack_ptr];
        }
        // 1NNN
        void jump(){
            // move PC to input address (opcode_nnn)
            this->Program_Counter = this->opcode_nnn;
        }
        // 2NNN
        void call_sub(){
            // record the PC position to the stack
            this->stack[this->stack_ptr] = this->Program_Counter;
            // increment the stack pointer
            this->stack_ptr++;
            // update the PC at line stored in opcode_nnn
            this->Program_Counter = this->opcode_nnn; 
        }
        // 3XNN
        void not_equal_NN(){
            // if the register X != to num then go to next instruction
            if (this->V[this->opcode_x] != opcode_nn)
                this->Program_Counter +=2;
        }
        // 4XNN
        void equal_NN(){
            // if the register X == to opcode_nn then go to next instruction
            if (this->V[this->opcode_x] == opcode_nn)  
                this->Program_Counter +=2;
        }
        // 5XY0
        void VX_not_equal_VY(){
            // if the register X != to register Y then go to next instruction
            if (this->V[this->opcode_x] != this->V[this->opcode_y])
                this->Program_Counter +=2;
        }
        // 6XNN
        void assign_VX(){
            this->V[this->opcode_x] = this->opcode_nn;
        }
        // 7XNN
        void increment_VX(){
            this->V[this->opcode_x] += this->opcode_nn;
        }
        // 8XY0
        void VX_copy_VY(){
            this->V[this->opcode_x] = this->V[this->opcode_y];
        }
        // 8XY1
        void VX_OR_VY(){
            this->V[this->opcode_x] =  this->V[this->opcode_x] | this->V[this->opcode_y];
        }
        // 8XY2
        void VX_AND_VY(){
            this->V[this->opcode_x] =  this->V[this->opcode_x] & this->V[this->opcode_y];
        }
         // 8XY3       
        void VX_XOR_VY(){
            this->V[this->opcode_x] =  this->V[this->opcode_x] ^ this->V[this->opcode_y];
        }
        // 8XY4
        void VX_VY_carry(){
            // add register X and Y
            uint16_t carry = this->V[this->opcode_x] + this->V[this->opcode_y];
            // check if the sum is > 255
            if (carry > 255){
                // add carry
                this->V[0xF] = 1;
            }
            else{
                // don't add carry
                this->V[0xF] = 0;
            }
            // assign register X to sum
            this->V[this->opcode_x] = carry;
        }
        // 8XY5
        void VX_VY_borrow(){
            // check if register X is >= to register Y
            if (this->V[this->opcode_x] >= this->V[this->opcode_y]){
                // add carry
                V[0xF] = 1;
            }
            else{
                // don't add carry
                V[0xF] = 0;
            }
            // subtract register X by register Y
            this->V[this->opcode_x] -= this->V[this->opcode_y];
        }
        // 8XY6
        void right_shift(){
            // checks the least significant bit
            if(this->V[this->opcode_x] & (1 << 0)){
                // set V[0xF] to 1
                this->V[0xF] = 1;
            }
            else{
                // set V[0xF] to 0
                this->V[0xF] = 0;
            }
            // then perform a right shift
            this->V[this->opcode_x] = this->V[this->opcode_x] >> 1;
        }
        // 8XY7
        void VX_VY_0_on_borrow(){
           if(this->V[this->opcode_x] <= this->V[this->opcode_y]){
                this->V[0xF] = 1;
           }
           else{
                this->V[0xF] = 0;
           }

           this->V[this->opcode_x] = this->V[this->opcode_y] - this->V[this->opcode_x];
        }
        // 8XYE
        void left_shift(){
            // checks the most significant bit
            if(this->V[this->opcode_x] & (1 << 7)){
                // set V[0xF] to 1
                this->V[0xF] = 1;
            }
            else{
                // set V[0xF] to 0
                this->V[0xF] = 0;
            }
            // then perform a left shift
            this->V[this->opcode_x] = this->V[this->opcode_x] << 1;
        }
        // 9XY0
        void VX_EQUAL_VY(){
            if (this->V[this->opcode_x] == this-> V[this->opcode_y])
                this->Program_Counter +=2;
        }
        // ANNN
        void set_index(){
            this->index_reg = this->opcode_nnn;
        }
        // BNNN
        void jump_0_NNN(){
            this->Program_Counter = this->opcode_nnn + this->V[0];
        }
        // CXNN
        void rand_VX_VY(){
            this->V[this->opcode_x] = (rand() % 256) & this->opcode_nn;
        }
        // DXYN
        void draw(){
            // get the x and y positions
            uint8_t x_pos = this->V[this->opcode_x] % 64;
            uint8_t y_pos = this->V[this->opcode_y] % 32;

            // set register 0xF to 0
            this->V[0xF] = 0;

            for (unsigned int row = 0; row < this->opcode_n; ++row){
                // assign a byte of memory based on the sum of the index and row
                uint8_t sprite_byte = memory[index_reg + row];
                
                for(unsigned int col = 0; col < 8; ++col){
                    
                    // the size of the pixel and make sure to warp to the start
                    uint8_t sprite_pixel = sprite_byte & (0x80u >> col);
                    // the size of the screen
                    uint32_t* screen_pixel = &display[(y_pos + row) * 64 + (x_pos + col)];

                    // if sprite pixel is on
                    if(sprite_pixel){
                        // check for collison with screen pixel
                        if(*screen_pixel == 0xFFFFFFFF){
                            V[0xF] = 1;
                        }

                        // XOR pixel
                        *screen_pixel ^= 0xFFFFFFFF;
                    }
                }
            }
        }
        // EX9E
        void key_pressed(){
            if(this->keypad[this->V[this->opcode_x]] >= 1){
                this->Program_Counter += 2;
            }
        }
        // EXA1
        void key_not_pressed(){
            if(this->keypad[this->V[this->opcode_x]] == 0){
                this->Program_Counter += 2;
            }
        }
        // FX07
        void VX_delay(){
            this->V[this->opcode_x] = this->delay_timer;
        }
        // FX0A
        void wait_for_key(){
            for (int i = 0; i < 16; i++){
                if (this->keypad[i] != 0){
                    this->V[this->opcode_x] = i;
                    return;
                }
            }
            this->Program_Counter-= 2;
        }
        // FX15
        void delay_VX(){
            this->delay_timer = this->V[this->opcode_x];
        }
        // FX18
        void buzzer_VX(){
            this->sound_timer = this->V[this->opcode_x];
        }
        // FX1E
        void i_ADD_VX(){
            this->index_reg += this->V[this->opcode_x];
        }
        // FX29
        void i_HEX_VX(){
            this->index_reg = 0x50 + (this->V[this->opcode_x] * 5);
        }
        // FX33
        void decode(){
            this->memory[this->index_reg] = this->V[this->opcode_x] / 100;
            this->memory[this->index_reg + 1] = (this->V[this->opcode_x] % 100 / 10);
            this->memory[this->index_reg + 2] = this->V[this->opcode_x] % 10;
        }
        // FX55
        void save_VX(){
            for (int i = 0; i <= this->opcode_x; i++){
                this->memory[index_reg + i] = this->V[i];
            }
        }
        // FX65
        void load_VX(){
            for (int i = 0; i <= this->opcode_x; i++){
                this->V[i] = this->memory[index_reg + i];
            }
        }
};