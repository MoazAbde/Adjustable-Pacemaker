/* 
 * File:   pwmMain.c
 * Author: Moaz
 *
 * Created on December 1, 2022, 5:29 PM
 */

#include <avr/io.h>
#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "hd44780.h"
#include "lcd.h"
/*
 * 
 */

// initialization of LCD 
static void
ioinit(void)
{
 
  lcd_init();
}

FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);  

int main(int argc, char** argv) {
    DDRC = 0b1111111;
    DDRB = 0b00000000;
    
    
    
    
//            fprintf(stderr, "  Press Button\x1b\xc0 0");
    
    ioinit(); // initialize LCD
    stderr = &lcd_str;
    
    while(1){
        
        PORTC = 0b0000100;
        
        if((PINB & (1 << 0)) == 1){
            fprintf(stderr, " 1");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 <<1)) == 1)
        {
            fprintf(stderr, "4");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 <<2)) == 1){
            fprintf(stderr, "7");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 << 3)) == 1){
            fprintf(stderr, "*");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }
        
        PORTC = 0b0000010;
        if((PINB & (1 << 0)) == 1){
            fprintf(stderr, "2");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 <<1)) == 1)
        {
            fprintf(stderr, "5");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 <<2)) == 1){
            fprintf(stderr, "8");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 << 3)) == 1){
            fprintf(stderr, "0");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }
        
        PORTC = 0b0000001;
        if((PINB & (1 << 0)) == 1){
            fprintf(stderr, "3");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 <<1)) == 1)
        {
            fprintf(stderr, "6");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 <<2)) == 1){
            fprintf(stderr, "9");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }else if((PINB & (1 << 3)) == 1){
            fprintf(stderr, "#");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }
        else{
            fprintf(stderr, "na");
            _delay_ms(100); 
            fprintf(stderr, "\x1b\x01");
        }
        

    }
    return (EXIT_SUCCESS);
}


