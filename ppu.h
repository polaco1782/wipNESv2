#include <stdbool.h>
#include <stdint.h>

#ifndef __PPU_H__
#define __PPU_H__

#define VISIBLE_SCANLINES 240
#define VISIBLE_DOTS 256
#define NTSC_SCANLINES_PER_FRAME 261
#define PAL_SCANLINES_PER_FRAME 311
#define DOTS_PER_SCANLINE 341
#define END_DOT 340

enum{
    BG_TABLE        = 1 << 4,
    SPRITE_TABLE    = 1 << 3,
    SHOW_BG_8       = 1 << 1,
    SHOW_SPRITE_8   = 1 << 2,
    SHOW_BG         = 1 << 3,
    SHOW_SPRITE     = 1 << 4,
    LONG_SPRITE     = 1 << 5,
    SPRITE_0_HIT    = 1 << 6,
    FLIP_HORIZONTAL = 1 << 6,
    FLIP_VERTICAL   = 1 << 7,
    V_BLANK         = 1 << 7,
    GENERATE_NMI    = 1 << 7,
    RENDER_ENABLED  = 0x18,
    BASE_NAMETABLE  = 0x3,
    FINE_Y          = 0x7000,
    COARSE_Y        = 0x3E0,
    COARSE_X        = 0x1F,
    VERTICAL_BITS   = 0x7BE0,
    HORIZONTAL_BITS = 0x41F,
    X_SCROLL_BITS   = 0x1f,
    Y_SCROLL_BITS   = 0x73E0
};

typedef struct {
    uint8_t oamdata[265];
    uint8_t palette[32];
    uint8_t nametabledata[2048];

    uint32_t screen[VISIBLE_DOTS * VISIBLE_SCANLINES];

    uint8_t nametablebyte;
    uint8_t attrbyte;
    uint8_t tile_lo;
    uint8_t tile_hi;
    uint64_t tiledata;

    bool write;
    bool odd_frame;

    struct {
        union {
            struct {
                uint8_t r0 : 2; // nametable address;
                uint8_t r2 : 1; // vram address increment
                uint8_t r3 : 1; // sprite table address
                uint8_t r4 : 1; // background table address
                uint8_t r5 : 1; // sprite size
                uint8_t r6 : 1; // ppu master slave select
                uint8_t r7 : 1; // nmi on blanking interval
            };
            uint8_t R;
        };

    } ctrl; // 0x2000

    struct {
        union {
            struct {
                uint8_t r0 : 1; // Emphasize blue
                uint8_t r1 : 1; // Emphasize green
                uint8_t r2 : 1; // Emphasize red
                uint8_t r3 : 1; // Show sprites
                uint8_t r4 : 1; // Show background
                uint8_t r5 : 1; // Show sprites in leftmost 8 pixels
                uint8_t r6 : 1; // Show background in leftmost 8 pixels
                uint8_t r7 : 1; // grayscale
            };
            uint8_t R;
        };

    } mask; // 0x2001

    struct {
        union {
            struct {
                uint8_t r0 : 5; // bits previously written
                uint8_t r5 : 1; // sprite overflow
                uint8_t r6 : 1; // sprite 0 hit
                uint8_t r7 : 1; // vblank started
            };
            uint8_t R;
        };

    } stats; // 0x2002

    uint8_t OAM_addr; // 0x2003
    uint8_t OAM_data; // 0x2004
    uint8_t scroll;   // 0x2005
    uint8_t address;  // 0x2006
    uint8_t data;     // 0x2007

    uint32_t cycle;    // current PPU cycle
    uint32_t scanline; // current PPU scanline
    uint64_t frames;   // current PPU frame

    uint32_t dots;

    uint16_t v;
    uint16_t t;
    uint8_t x;
    uint8_t w;
    uint8_t oam_address;
    uint8_t buffer;

    bool render;
    bool even_frame;

} PPU_t;

extern PPU_t ppu;

void ppu_init(void);
void ppu_exec(void);

#endif