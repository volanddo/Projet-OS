
#include "traitants_.h"
#include "segment.h"
#include "cpu.h"
#define ADD_TAB_VECT 0x1000
#define CONSTANT_TRAITANT 0x8E00


void init_traitant_IT(uint32_t num_IT, void (*traitant)(void)){
    uint32_t first = KERNEL_CS << 16;

    first = first | ((uint32_t)traitant & 0xFFFF);

    uint32_t last = (uint32_t)traitant & 0xFFFF0000;
    last = last | (CONSTANT_TRAITANT & 0xFFFF);

    uint32_t *addr = ((uint32_t*)ADD_TAB_VECT) + 2*num_IT;
    *addr = first;
    *(addr +1) = last;
}

void masque_IRQ(uint32_t num_IRQ, int masque){
    uint32_t val = inb(0x21); 
    val = val&(~( ~masque << num_IRQ));
    outb(val, 0x21);
}