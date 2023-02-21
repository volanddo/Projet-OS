#ifndef __TRAITANTS__
#define __TRAITANTS__

#include <inttypes.h>
#include "processus.h"
void traitant_IT_32();

void init_traitant_IT(uint32_t num_IT, void (*traitant)(void));

void masque_IRQ(uint32_t num_IRQ, int masque);

#endif