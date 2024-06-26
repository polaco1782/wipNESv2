/*
 * File:   cart.h
 * Author: cassiano
 *
 * Created on January 23, 2016, 10:04 PM
 */

#include <stdint.h>
#include <stdio.h>

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK(__Declaration__)                                                  \
    __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

PACK(typedef struct {
    uint32_t nes;
    uint8_t prg_banks;
    uint8_t chr_banks;
    uint8_t control1;
    uint8_t control2;
    uint8_t prg_ram;

    uint8_t pad[7];
} ines_t;)

typedef struct {
    uint8_t *prg;
    uint8_t *chr;
    uint8_t *sram;
    uint8_t mapper;
    uint8_t mirror;
    uint8_t battery;

    uint8_t prgbanks; // total PRG banks
} Cart;

extern ines_t header;
extern Cart cart;

int cart_load();
