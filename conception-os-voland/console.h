#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include <inttypes.h>
/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
#define N_LINE 25
#define N_ROW 80

#define START_MEM 0xB8000
#define PORT_CURS_COM 0x3D4
#define PORT_CURS_DON 0x3D5
#define COMMANDE_BASSE 0x0F
#define COMMANDE_HAUTE 0x0E

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define WHITE 15

struct position;

void get_pos(int *lig, int *col);

void set_pos(int lig, int col);

extern void console_putbytes(const char *s, int len);

extern uint16_t *ptr_mem(uint32_t lig, uint32_t col);

extern void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t color_text, uint8_t color_bg, uint8_t clignote);

extern void efface_ecran(void);

extern void place_curseur(uint32_t lig, uint32_t col);

extern void traite_car(char c);

extern void defilement(void);

#endif
