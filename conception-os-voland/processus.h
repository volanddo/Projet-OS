#ifndef __PROCESSUS__
#define __PROCESSUS__

#include "inttypes.h"

#define T_PROC_TABLE 2
#define T_MAX_STR 100
#define T_STACK 512
#define T_REG 5

typedef enum {ELU, ACTIVABLE, ENDORMI, MORT} STATE_T;

typedef struct _pidll
{
    uint32_t pid;
    struct _pidll* next;
} pidll;

typedef struct  Processus_t_{
    pidll *pid;
    char name[T_MAX_STR];
    STATE_T state;
    int reg[T_REG];
    int *stack;
    uint32_t wake_up_at;
    struct Processus_t_ *next;
} Processus_t;

typedef struct pid_{
    int id;
    int next_avaliable;
} pid_t;

// Process function
uint32_t cree_processus(void (*fct)(void), char *name);

uint32_t mon_pid(void);

char *mon_nom(void);

void dors(uint32_t nbr_secs);

uint32_t nbr_secondes();

void idle(void);
void proc1(void);
void proc2(void);
void proc3(void);

//Ordonanceur
void ordonnance(void);

//ctx_w
void ctx_sw(int *reg1, int *reg2);

#endif