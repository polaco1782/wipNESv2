#include "mem.h"
#include "cart.h"
#include "cpu.h"
#include "ppu.h"
#include <SDL3/SDL.h>

int main()
{
    // init the library, here we make a window so we only need the Video
    // capabilities.
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
        exit(1);
    }

    cart_load();
    mem_init();
    cpu_init();
    ppu_init();

    // PRG ROM dump
    // for(int i=0; i<cart.prgbanks*16384; i++)
    // {
    //     if(i==0)
    //         printf("0x%04X: ", ROM_ADDR + i);

    //     printf("%02x ", mem_read8(ROM_ADDR + i));
    //     if((i+1)%32==0 && i>0)
    //         printf("\n0x%04X: ", ROM_ADDR + i);
    // }

    while(1)
    {
        // each cpu cycle is 3 ppu ticks
        ppu_exec();
        ppu_exec();
        ppu_exec();

        cpu_execop();
    }

    return 0;
}
