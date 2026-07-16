#include "CPU.cpp"
#include <SDL.h>
#include <stdint.h>
#include <map>

// references:
// https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidebasicsinit.html
// https://thenumb.at/cpp-course/sdl2/01/01.html
// https://wiki.libsdl.org/SDL2/SDL_EventType
// https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
// https://thenumb.at/cpp-course/sdl2/05/05.html
// https://wiki.libsdl.org/SDL2/SDL_UpdateTexture
// https://wiki.libsdl.org/SDL2/SDL_RenderCopy
// https://freepascal-meets-sdl.net/sdl-2-0-key-code-lookup-table/
// 
// 
// 
// 

// create a map for key inputs
std::map<SDL_Keycode, uint8_t> key_map ={
    {SDLK_1, 0x01}, // 1
    {SDLK_2, 0x02}, // 2
    {SDLK_3, 0x03}, // 3
    {SDLK_4, 0x0C}, // C
    {SDLK_q, 0x04}, // 4
    {SDLK_w, 0x05}, // 5
    {SDLK_e, 0x06}, // 6
    {SDLK_r, 0x0D}, // D
    {SDLK_a, 0x07}, // 7
    {SDLK_s, 0x08}, // 8
    {SDLK_d, 0x09}, // 9
    {SDLK_f, 0x0E}, // E
    {SDLK_z, 0x0A}, // A
    {SDLK_x, 0x00}, // 0
    {SDLK_c, 0x0B}, // B
    {SDLK_v, 0x0F} // F

};


// display the screen 
void display (SDL_Renderer *renderer, SDL_Texture *texture, CPU &chip8){
    // update the texture
    SDL_UpdateTexture(texture,NULL,chip8.get_Display(),
        64* sizeof(uint32_t));
    
    // clear the renderer
    SDL_RenderClear(renderer);

    // Copy the texture onto the renderer
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    
    // present final frame
    SDL_RenderPresent(renderer);
}

void play_sound(CPU &chip8, SDL_AudioDeviceID &audio){

    int16_t audio_buffer[8192];

    if(chip8.get_sound_timer() > 0){
        //play sound
        SDL_PauseAudioDevice(audio,0);
    }
    else{
        // stop playing sound
        SDL_PauseAudioDevice(audio,1);
    }
}

int main(int argc, char*argv[]){
    // define SDL video and audio
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)) <= -1){
        // display error message if initialisation fails
        std::cerr << "ERROR:\t failed to initialise SDL" << std::endl;
        // End application
        system("pause");
        SDL_Quit();
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
        SDL_DestroyWindow(window);
        SDL_Quit();
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
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }

    // get texture from renderer
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64,32);

    // check if texture has been created:
    if(!texture){
        // display error message
        std::cerr << "ERROR:\t failed to create texture" << std::endl;
        // End application
        system("pause");
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // audio spec configuration
    SDL_AudioSpec audio_spec;
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16;
    audio_spec.channels = 1;
    audio_spec.samples = 8192;
    audio_spec.callback = NULL;

    SDL_AudioDeviceID audio = SDL_OpenAudioDevice(NULL,0,&audio_spec,NULL,0);
    
    if (audio == 0){
        // display error message
        std::cerr << "ERROR:\t failed to create audio" << std::endl;
        // End application
        system("pause");
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_PauseAudioDevice(audio,0);
    // Cycles per frame:
    const int CYCLES_PER_FRAME = 500 / 60;
    // creates CPU class for emulator
    CPU chip8 = CPU();
    // check if the user has send the ROM as an arg
    if(argc >=2){
        // load the ROM
        if(!chip8.load_ROM(argv[1])){
            std::cerr << "ERROR:\t failed to load ROM" << std::endl;
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
    }
    else{
        // display error message
        std::cerr << "No ROM preloaded" << std::endl;
        // TEMP
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_CloseAudioDevice(audio);
        SDL_Quit();
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
           if(event.type == SDL_KEYDOWN){
                // find key
                if (auto key = key_map.find(event.key.keysym.sym); key != key_map.end()){
                    chip8.set_key_press(key->second);
                }
           }
           if(event.type == SDL_KEYUP){
                // find key
                if (auto key = key_map.find(event.key.keysym.sym); key != key_map.end()){
                    chip8.set_key_release(key->second);
                }
           }
        }
        // perform the FDE cycle
        for(int i = 0; i < CYCLES_PER_FRAME; i++){
            chip8.cycle();
        }

        // update timers
        chip8.tick_timers();

        // render display
        display(renderer, texture, chip8);

        // play sound
        play_sound(chip8,audio);
        // set the cycle delay to 17ms
        SDL_Delay(17); 
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audio);
    SDL_Quit();
    
    return 0;
}