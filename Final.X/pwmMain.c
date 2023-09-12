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
#include <avr/interrupt.h>
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

// function Prototype 
float adc_conversion(unsigned char );

volatile float decimalDuty;
volatile int ocr1bValue = 0;
volatile int dutyCycle = 0;
volatile int top;

int main(int argc, char** argv) {
    
    
    ioinit(); // initialize LCD
    stderr = &lcd_str;
    
    //ADC Initialization
    DIDR0 = (1<<ADC1D) | (1 <<ADC0D); //disable digital I/p
    ADCSRA = ((1 << ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) ); //Enable ADC and 128 prescaler clock
    ADMUX |= (1 << REFS0); //refrence voltage
    
    //O/P compare Pin
    DDRB |= (1<<PB2);
    PORTB &= ~(1<<PB2);
    
    //Reset button
    DDRB &= ~(1<<PB0);
    PORTB |= (1<<PB0);
    
    
    DDRB &= ~(1<<PB3);
    PORTB |= (1<<PB3);
    
    // 50 % duty cycle button
    DDRB &= ~(1<<PB4);
    PORTB |= (1<<PB4);
    
    //100% duty cycle button
    DDRC &= ~(1<<PC2);
    PORTC |=(1<<PC2);
    
    //25% button duty cycle button
    DDRD &= ~(1<<PD3);
    PORTD |= (1<<PD3);
    
   
//    ICR1 = 9216; //TOP value 
//    OCR1B = 4608;
    
    ICR1 = 28800; //TOP value 
    OCR1B = 14400;
    
    // phase & frequency correct PWM
    TCCR1A |= (1<<COM1B1);
    TCCR1B |= (1<<WGM13)|(1<<CS12);
    
    //interrupt on rising edge of int1
    EICRA |= (1<<ISC11);
    EIMSK |= (1<<INT1);

    
    PCMSK0 |= (1<<PCINT4)|(1<<PCINT3); //enable pin 4 & 3 interrupt
    PCICR |=(1<<PCIE0); //enable interrupt on bank 0
    
    PCMSK1 |= (1<<PCINT10);
    PCICR |=(1<<PCIE1);//enable interrupt on bank 1
    
    float freqValue;
    while(1){
        top = (28800)/adc_conversion('x');
        freqValue = adc_conversion('x');
        if((PIND&(1<<PD3)) == 0){
            sei();
        }
        else if((PINB&(1<<PB0)) == 0){
            ocr1bValue = 0;
            dutyCycle = 0;
        }
        
        else if((PINB&(1<<PB3)) == 0){
            sei();
        }
        else if((PINB&(1<<PB4)) == 0){
            sei();
        }
        
        else if((PINC&(1<<PC2)) == 0){
            sei();
        }
        
        ICR1 = top;
        OCR1B = ocr1bValue;
        
        fprintf(stderr, " PWM Generator\x1b\xc0%.2f Hz    %d%%",freqValue, dutyCycle);
        _delay_ms(200);
        fprintf(stderr, "\x1b\x01"); //Clear LCD
    }
    return (EXIT_SUCCESS);
}

/**
 * @brief converts Analog voltages of the x  position to digital voltages
 * the values are then adjusted to a perecentage in order to get sent to the UART
 *
 * @param coordinate needed to change the ADMUX 
 * @return int newprecent, percentage of the voltage 
 */
float adc_conversion(unsigned char coordinate)   
{
    int coordinateValue = 0;
    int percent = 0;
    int newPercent = 0;
    float heartFreq =0;
    ADCSRA |= (1<<ADEN); //ADC Enable
    
    if(coordinate == 'x'){
        //ADC0 SELECTION
        ADMUX &= ~(1<<MUX0);
        ADMUX &= ~(1<<MUX1);
        ADMUX &= ~(1<<MUX2);
        ADMUX &= ~(1<<MUX3);
    }

    
    ADCSRA |= (1 << ADSC); // Start CONVERSION
    
    while (ADCSRA & (1 << ADSC)); // SPIN Wait for conversion to complete
    coordinateValue = ADC; // reading the digital value from the ADC register
    
    //converting x voltage precentage 
    if(coordinate == 'x'){
        
        if((coordinateValue>=514) && (coordinateValue<=1023)){
            coordinateValue-=514;
            percent = (float)coordinateValue/509 *100;   
            heartFreq = (((float)percent*0.67)/100)+1;
        }else if((coordinateValue>=0) && (coordinateValue<514)){
            coordinateValue-=514;
            percent = (float)coordinateValue/514 *100;
            newPercent = (((float)percent+100)/4);
        }
        
    }

    
    return heartFreq;
    
}

ISR(INT1_vect){
    decimalDuty = 0.25;
    dutyCycle = 25;
    ocr1bValue = decimalDuty* top;
  
}

ISR(PCINT0_vect){
    decimalDuty = 0.50;
    dutyCycle = 50;
    ocr1bValue = decimalDuty* top;
  
}

ISR(PCINT1_vect){
    decimalDuty = 1.0;
    dutyCycle = 100;
    ocr1bValue = decimalDuty* top;
  
}



