#include "clock_print.h"
#include "console.h"
#include "cpu.h"
#include "processus.h"
#include <stdio.h>
#include <string.h>

#define QUARTZ      0x1234DD
#define CLOCKFREQ   20


uint32_t total_call = 0;
uint32_t time_in_seconde = 0;

uint32_t get_time(){
    return time_in_seconde;
}

void clock_print(const char * s, int len){
    // get current cursor potion
    int old_lig;
    int old_col;
    get_pos(&old_lig, &old_col);

    //compute place of the new cursor
    int lig = 0;
    int col = N_ROW - len;
    //place and write 
    set_pos(lig, col);
    place_curseur(lig, col);
    console_putbytes(s, len);
    //Replace cursor position
    set_pos(old_lig, old_col);
    place_curseur(old_lig, old_col);
}

void convert_sec(int ttTime, int *h, int *m, int *s){
    *m = ttTime/60;
    *h = *m/60;
    *s = ttTime%60;
    *m = *m%60;
}

void tic_PIT(void){
    
    // signaler au contrôleur d’interruptions le traitement de cette dernieres
    outb(0x20,0x20);
    //check and update timer
    total_call += 1;
    if (total_call%CLOCKFREQ == 0){
        time_in_seconde = total_call/CLOCKFREQ;
        char format[N_ROW+1];
        int h,m,s;

        convert_sec(time_in_seconde, &h,&m,&s);
        
        sprintf(format, "|MonOS|---------|----------|----------|----------|----------|----------|%02d:%02d:%02d",h , m, s);
        clock_print(format, strlen(format));
    }
    ordonnance();
}

void setClockFreq(void)
{
    outb(0x34, 0x43);
    outb((QUARTZ / CLOCKFREQ) & 0xFF, 0x40);
    outb((QUARTZ / CLOCKFREQ) >> 8, 0x40);
}