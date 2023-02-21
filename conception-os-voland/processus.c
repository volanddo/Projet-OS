#include "processus.h"
#include "clock_print.h"
#include "cpu.h"
#include <tinyalloc.h>
#include <string.h>
#include <stdio.h>
#include <tinyalloc.h>

/* Stack property*/
/* TODO
 * Gérer un pointeur sur le processus élu (celui en cours d’exécution) pour accéder à sa structure interne si besoin est.
 * Fonction d’ordonnancement : Cette fonction est appelée à chaque fois qu’un processus a épuisé le temps d’exécution qui lui était alloué 0, 1, 2, 3, 0, 1, 2, 3
 * context switch : une fois processus choisi par odonanceur -> provoque l’arrêt de l’exécution du processus actif et démarrer le processus élu
 *
 * Le changement de contexte d’exécution
 *
 * 1. sauvegarder les valeurs des registres importants du processeur dans tab
 * 2. restaurer le contexte du processus élu (pointeur pile : %esp = pile d’exécution du processus élu)
 * 2.2 SI PREMIERE EXEC : stockant en sommet de pile l’adresse de la fonction principale du processus et la stocker dans %esp
 *
 */
/*
 * Définition variable global processus
 */


// LinkedList 1 : process list Activable
Processus_t *process_table_head = NULL;
Processus_t *process_table_tail = NULL;

// LinkedList 2 : process list sleep
Processus_t *process_sleep_head = NULL;
Processus_t *process_sleep_tail = NULL;

// LinkedList 3 : process list death
Processus_t *process_death_head = NULL;
Processus_t *process_death_tail = NULL;

Processus_t *processus_ELU = NULL;

// LinkedList 3 : PID avaliable
pidll *pidAvaliable = NULL;
uint32_t lastPid_need = 0;
/*
 * Create a list of Avaliable PID dynamicly
*/
void createNewPid(){
    uint32_t start = lastPid_need;
    if (lastPid_need == 0){
        lastPid_need = 32;
    }else{
        lastPid_need *= 2;
    }
    pidll sent; sent.next = pidAvaliable;
    pidll *prev = &sent;
    for (int i = start; i< lastPid_need; i++){
        prev->next = malloc(sizeof(pidll));
        prev->next->next = NULL;
        prev->next->pid = i;
        prev = prev->next;
    }
    pidAvaliable = sent.next;
}


/*
 * llist processus insert and pop
 */
void insert_tail(Processus_t **head, Processus_t **tail, Processus_t *p){
    if (*tail == NULL){
        *head = p;
        *tail = p;
        return;
    }
    (*tail)->next = p;
    *tail = (*tail)->next;
}
Processus_t * pop_head(Processus_t **head, Processus_t **tail){
    Processus_t * p = *head;
    if (*head != NULL){
        *head = (*head)->next;
        if(*head == NULL){
            *tail = *head;
        }
    }
    if (p != NULL){
        p->next = NULL;
    }
    return p;
}

/*Creer un processus a partir d'une fonction*/
uint32_t cree_processus(void (*fct)(void), char *name)
{
    Processus_t *p = malloc(sizeof(Processus_t));
    if (pidAvaliable == NULL){
        createNewPid();
    }
    p->pid = pidAvaliable;
    pidAvaliable = pidAvaliable->next;

    strcpy(p->name, name);

    // define process as activable
    p->state = ACTIVABLE;
    if(p->pid->pid == 0){
        p->state = ELU;   
    }

    // add function of process in process stack
    p->stack = malloc(sizeof(int)*T_STACK);
    p->stack[T_STACK - 1] = (int) fct;

    p->reg[1] = (int) (p->stack + T_STACK - 1);

    p->wake_up_at = 0;
    
    // add process to table
    insert_tail(&process_table_head, &process_table_tail, p);
    // On est dans le start
    if (processus_ELU == NULL){
        processus_ELU = pop_head(&process_table_head, &process_table_tail);
    }

    return p->pid->pid;
}

uint32_t mon_pid(void){
    return processus_ELU->pid->pid;
}
char *mon_nom(void){
    return processus_ELU->name;
}

void dors(uint32_t nbr_secs){
    processus_ELU->state = ENDORMI;
    processus_ELU->wake_up_at = nbr_secs + get_time();
    if (process_sleep_tail == NULL || process_sleep_head == NULL){
        process_sleep_head = processus_ELU;
        process_sleep_tail = processus_ELU;
        ordonnance();
        return;
    }

    Processus_t sent;
    sent.next = process_sleep_head;
    Processus_t * previous = &sent;
    Processus_t * current = previous->next;
    while(previous != NULL){
        if(current == NULL || current->wake_up_at > processus_ELU->wake_up_at){
            previous->next = processus_ELU;
            processus_ELU->next = current;
            break;
        }
        previous = previous->next;
        current = current->next;
    }
    process_sleep_head = sent.next;
    if (process_sleep_head->next == NULL){
        process_sleep_tail = process_sleep_head;
    }
    ordonnance();
}
void reveille_processus(){
    Processus_t sent;
    sent.next = process_sleep_head;
    Processus_t *previous = &sent;
    Processus_t *current = process_sleep_head;
    while( current != NULL && get_time() >= current->wake_up_at){
        current->state = ACTIVABLE;
        insert_tail(&process_table_head, &process_table_tail, current);
        previous->next = current->next;
        current = current->next;
    }
    process_sleep_head = sent.next;
    if (process_sleep_head != NULL && process_sleep_head->next == NULL){
        process_sleep_tail = process_sleep_head;
    }
}
void libere_processur_mort(){
    Processus_t *current = process_death_head;
    //supprimer si processus_ELU != current
    
}
void fin_processus(void){
    //détruire le processus actif
    //Passer main prochain processus
    processus_ELU->state = NULL;
    insert_tail(&process_death_head, &process_death_tail, processus_ELU);
    ordonnance();
}

/*
 * Ordonanceur
 */
void ordonnance(void) {
    //verification processus endormi
    reveille_processus();
    libere_processur_mort();
    // si au moins un processus en attente
    if (process_table_tail != NULL) {
        if (processus_ELU->state == ELU){
            // processus ELU ne dors pas
            insert_tail(&process_table_head, &process_table_tail, processus_ELU);
            processus_ELU = pop_head(&process_table_head, &process_table_tail);
            process_table_tail->state = ACTIVABLE;
            processus_ELU->state = ELU;
            ctx_sw(process_table_tail->reg, processus_ELU->reg);
        }
        else{
            // processus ELU dors
            Processus_t *sleepProcess = processus_ELU;
            processus_ELU = pop_head(&process_table_head, &process_table_tail);
            processus_ELU->state = ELU;
            ctx_sw(sleepProcess->reg, processus_ELU->reg);
        }        
    }

}

uint32_t nbr_secondes(){
    return get_time();
}

/*
 * Processus test definition
*/
void idle()
{
    for (;;) {
        sti();
        hlt();
        cli();
    }
}

void proc1(void)
{
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        dors(2);
    }
    }

void proc2(void)
{
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        dors(3);
    }
}
void proc3(void)
{
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        dors(5);
    }
}