#include "cpu.h"
#include <inttypes.h>
#include <stdio.h>
#include "console.h"
#include "traitants_.h"
#include "processus.h"
#include <stdio.h>
#include "clock_print.h"
// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void)
{
    //initialisation Traitant
    masque_IRQ(0, 0);
    init_traitant_IT(32, traitant_IT_32);
    setClockFreq();
    // démasquage des interruptions externes
    //sti();

    // initialisation des structures de processus
    cree_processus(idle, "idle");
    cree_processus(proc1, "proc1");
    cree_processus(proc2, "proc2");
    cree_processus(proc3, "proc3");
    
    efface_ecran();
    //demarrage du processus par defaut
    idle();

}

