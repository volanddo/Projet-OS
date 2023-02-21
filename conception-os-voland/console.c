#include "console.h"
#include "cpu.h"
#include <string.h>

struct position{
    uint32_t lig;
    uint32_t col;
};
struct position pos_curseur = {1,0};

void get_pos(int *lig, int *col){
    *lig = pos_curseur.lig;
    *col = pos_curseur.col;
}
void set_pos(int lig, int col)
{
    pos_curseur.lig = lig;
    pos_curseur.col = col;
}

extern void console_putbytes(const char *s, int len){
    for(int i = 0; i < len; i++)
    {
        traite_car(s[i]);
    }
}


uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
    return (uint16_t *)(START_MEM + 2*(lig*N_ROW+col));
}


void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t color_text, uint8_t color_bg, uint8_t cl)
{
    *ptr_mem(lig, col) = c | (cl << 15) | (color_bg << 12) | (color_text << 8);
}

extern void efface_ecran(void)
{
    for(uint32_t i = 0; i < N_LINE; i++)
    {
        for(uint32_t j = 0; j < N_ROW; j++)
        {
            ecrit_car(i,j,' ', WHITE, BLACK, 0);
        } 
    }
}

void place_curseur(uint32_t lig, uint32_t col)
{
    uint16_t pos = col + lig*80;
    // Envoie de la parti basse
    outb(COMMANDE_BASSE, PORT_CURS_COM);
    outb( pos & 0xFF,  PORT_CURS_DON);

    // Envoie de la parti haute
    outb(COMMANDE_HAUTE, PORT_CURS_COM);
    outb((pos >> 8) & 0xFF, PORT_CURS_DON);

}

void check_and_mouve_pos_curseur()
{
    uint32_t mod_col = pos_curseur.col%N_ROW;
    uint32_t div_col = pos_curseur.col/N_ROW;
    pos_curseur.lig += div_col;
    pos_curseur.col = mod_col;
    if( pos_curseur.lig >= N_LINE){
        defilement();
    }else{
        place_curseur(pos_curseur.lig, pos_curseur.col);
    }
}

void backslash_b(){
    // Recule le curseur d’une colonne (si colonne != 0)
    if(pos_curseur.col > 0)
    {
        pos_curseur.col --;
        check_and_mouve_pos_curseur();
    }
    
}
void backslash_t_(){
    // Avance à la prochaine tabulation (colonnes 0, 8, 16, ..., 72, 79)
    pos_curseur.col = (pos_curseur.col/8)*8 + 8;
    check_and_mouve_pos_curseur();
}
void backslash_n(){
    // Déplace le curseur sur la ligne suivante, colonne 0
    pos_curseur.lig += 1;
    pos_curseur.col = 0;
    check_and_mouve_pos_curseur();
}
void backslash_f(){
    // Efface l’écran et place le curseur sur la colonne 0 de la ligne 0
    efface_ecran();
    pos_curseur.col = 0;
    pos_curseur.lig = 0;
    place_curseur(pos_curseur.lig, pos_curseur.col);
}
void backslash_r(){
    // Déplace le curseur sur la ligne actuelle, colonne 0
    pos_curseur.col = 0;
    place_curseur(pos_curseur.lig, pos_curseur.col);
}

void traite_car(char c)
{
    switch (c)
    {
    case 32 ... 126:
        ecrit_car(pos_curseur.lig, pos_curseur.col, c, WHITE, BLACK, 0);
        pos_curseur.col += 1;
        check_and_mouve_pos_curseur();
        break;
    case 8: 
        backslash_b();
        break;
    case 9: 
        backslash_t_();
        break;
    case 10: 
        backslash_n();
        break;
    case 12: 
        backslash_f();
        break;
    case 13:  
        backslash_r();
        break;
    default:
        break;
    }
}

void defilement(void)
{
    size_t n = 2*((N_LINE-1)*N_ROW);
    memmove(ptr_mem(1,0), ptr_mem(2,0), n);
    pos_curseur.lig = N_LINE-1;
    pos_curseur.col = 0;
    place_curseur(pos_curseur.lig, pos_curseur.col);
}