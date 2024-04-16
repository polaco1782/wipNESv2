#include "ppu.h"
#include <stdio.h>
#include <memory.h>
#include <SDL3/SDL.h>

PPU_t ppu;

void ppu_init(void) { memset(&ppu, 0, sizeof(PPU_t)); }

// void ppu_exec(void) {
//     ppu.cycle++;

//     // a complete PPU scanline is 341 cycles long
//     if (ppu.cycle >= 341) {
//         ppu.cycle = 0;
//         ppu.scanline++;

//         // update vblank register, generate NMI
//         if (ppu.scanline == 241) {
//             ppu.stats.r7 = 1;
//         }

//         // a complete PPU frame is 262 scanlines long
//         if (ppu.scanline >= 262) {
//             ppu.scanline = 0;
//             ppu.even_frame = !ppu.even_frame;
//             ppu.stats.r7 = 0;
//         }
//     }
// }

void ppu_exec(void)
{
    if(ppu.scanline < VISIBLE_SCANLINES){
        // render scanlines 0 - 239
        if(ppu.dots > 0 && ppu.dots <= VISIBLE_DOTS){
            int x = (int)ppu.dots - 1;
            uint8_t fine_x = ((uint16_t)ppu.x + x) % 8, palette_addr = 0, palette_addr_sp = 0, back_priority = 0;

            if(ppu.mask.r4){
                //palette_addr = render_background(ppu);
                if(fine_x == 7) {
                    if ((ppu.v & COARSE_X) == 31) {
                        ppu.v &= ~COARSE_X;
                        // switch horizontal nametable
                        ppu.v ^= 0x400;
                    }
                    else
                        ppu.v++;
                }
            }
            if(ppu.mask.r3 && ((ppu.mask.r5) || x >=8)){
                //palette_addr_sp = render_sprites(ppu, palette_addr, &back_priority);
            }
            if((!palette_addr && palette_addr_sp) || (palette_addr && palette_addr_sp && !back_priority))
                palette_addr = palette_addr_sp;

            //palette_addr = !palette_addr ? ppu.palette[0] : ppu.palette[palette_addr];
            //ppu.screen[ppu.scanline * VISIBLE_DOTS + ppu.dots - 1] = nes_palette[palette_addr];
        }
        if(ppu.dots == VISIBLE_DOTS + 1 && ppu.mask.r4){
            if((ppu.v & FINE_Y) != FINE_Y) {
                // increment coarse x
                ppu.v += 0x1000;
            }
            else{
                ppu.v &= ~FINE_Y;
                uint16_t coarse_y = (ppu.v & COARSE_Y) >> 5;
                if(coarse_y == 29){
                    coarse_y = 0;
                    // toggle bit 11 to switch vertical nametable
                    ppu.v ^= 0x800;
                }
                else if(coarse_y == 31){
                    // nametable not switched
                    coarse_y = 0;
                }
                else{
                    coarse_y++;
                }

                ppu.v = (ppu.v & ~COARSE_Y) | (coarse_y << 5);
            }
        }
        else if(ppu.dots == VISIBLE_DOTS + 2 && (ppu.mask.r3 && ppu.mask.r4)){
            ppu.v &= ~HORIZONTAL_BITS;
            ppu.v |= ppu.t & HORIZONTAL_BITS;
        }
        else if(ppu.dots == END_DOT){
            // memset(ppu.OAM_cache, 0, 8);
            // ppu.OAM_cache_len = 0;
            // uint8_t range = ppu.ctrl & LONG_SPRITE ? 16: 8;
            // for(size_t i = ppu.oam_address / 4; i < 64; i++){
            //     int diff = (int)ppu.scanline - ppu.OAM[i * 4];
            //     if(diff >= 0 && diff < range){
            //         ppu.OAM_cache[ppu.OAM_cache_len++] = i * 4;
            //         if(ppu.OAM_cache_len >= 8)
            //             break;
            //     }
            // }
        }
    }
    else if(ppu.scanline == VISIBLE_SCANLINES){
        // post render scanline 240/239
    }
    else if(ppu.scanline < NTSC_SCANLINES_PER_FRAME){
        // v blanking scanlines 241 - 261/311
        if(ppu.dots == 1 && ppu.scanline == VISIBLE_SCANLINES + 1){
            // set v-blank
            ppu.stats.r7 = 1;
            if(ppu.ctrl.r7){
                // generate NMI
                //interrupt(&ppu.emulator->cpu, NMI);
                printf("NMI\n");
            }
        }
    }
    else{
        // pre-render scanline 262/312
        if(ppu.dots == 1){
            // reset v-blank and sprite zero hit
            ppu.stats.r6 = 0;
            ppu.stats.r7 = 0;
        }
        else if(ppu.dots == VISIBLE_DOTS + 2 && (ppu.mask.r3 && ppu.mask.r4)){
            ppu.v &= ~HORIZONTAL_BITS;
            ppu.v |= ppu.t & HORIZONTAL_BITS;
        }
        else if(ppu.dots > 280 && ppu.dots <= 304 && (ppu.mask.r3 && ppu.mask.r4)){
            ppu.v &= ~VERTICAL_BITS;
            ppu.v |= ppu.t & VERTICAL_BITS;
        }
        else if(ppu.dots == END_DOT - 1 && ppu.frames & 1 && (ppu.mask.r3 && ppu.mask.r4)) {
            // skip one cycle on odd frames if rendering is enabled for NTSC
            ppu.dots++;
        }

        if(ppu.dots >= END_DOT) {
            // inform emulator to render contents of ppu on first dot
            ppu.render = 1;
            ppu.frames++;
        }
    }

    // increment dots and scanlines

    if(++ppu.dots >= DOTS_PER_SCANLINE) {
        if (ppu.scanline++ >= NTSC_SCANLINES_PER_FRAME)
            ppu.scanline = 0;
        ppu.dots = 0;
    }
}