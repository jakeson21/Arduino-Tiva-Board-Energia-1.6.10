/*
 ************************************************************************
 *	pins_energia.h
 *
 *	Energia core files for LM4F
 *		Copyright (c) 2012 Robert Wessels. All right reserved.
 *
 *     Contribution: Rei VILO
 *
 ***********************************************************************
  Derived from:
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
 */

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define __LM4F120H5QR__
#define __TM4C123GH6PM__

#ifndef BV 
#define BV(x) (1 << (x)) 
#endif 
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"

//
// Pin names based on the silkscreen
//
static const uint8_t PA_1 = 0;
static const uint8_t PA_0 = 1;
static const uint8_t PA_7 = 2;
static const uint8_t PA_6 = 3;
static const uint8_t PE_4 = 4;
static const uint8_t PE_5 = 5;
static const uint8_t PB_4 = 6;
static const uint8_t PB_5 = 7;
static const uint8_t PD_1 = 8;
static const uint8_t PD_0 = 9;
static const uint8_t PF_3 = 10;
static const uint8_t PF_1 = 11;
static const uint8_t PF_0 = 12;
static const uint8_t PF_2 = 13;
static const uint8_t PB_3 = 14;
static const uint8_t PB_2 = 15;
static const uint8_t PE_3 = 16;
static const uint8_t PE_2 = 17;
static const uint8_t PE_1 = 18;
static const uint8_t PE_0 = 19;
static const uint8_t PD_3 = 20;
static const uint8_t PD_2 = 21;
static const uint8_t PC_7 = 22;
static const uint8_t PC_6 = 23;
static const uint8_t PC_5 = 24;
static const uint8_t PC_4 = 25;
static const uint8_t PA_2 = 26;
static const uint8_t PA_3 = 27;
static const uint8_t PA_4 = 28;
static const uint8_t PA_5 = 29;
static const uint8_t PB_0 = 30;
static const uint8_t PB_1 = 31;
static const uint8_t PD_6 = 32;
static const uint8_t PD_7 = 33;
static const uint8_t PF_4 = 34;
static const uint8_t PB_6 = 35;
static const uint8_t PB_7 = 36;

static const uint8_t A0 = 16; // PE_3
static const uint8_t A1 = 17; // PE_2
static const uint8_t A2 = 18; // PE_1
static const uint8_t A3 = 19; // PE_0
static const uint8_t A4 = 20; // PD_3
static const uint8_t A5 = 21; // PD_2
static const uint8_t A6 = 4; // PE_4
static const uint8_t A7 = 5; // PE_5
static const uint8_t A8 = 6; // PB_4
static const uint8_t A9 = 7; // PB_5
static const uint8_t A10 = 8; // PD_1
static const uint8_t A11 = 9; // PD_0

//static const uint8_t RED_LED = 30;
//static const uint8_t GREEN_LED = 39;
//static const uint8_t BLUE_LED = 40;

//static const uint8_t PUSH1 = 31;
//static const uint8_t PUSH2 = 17;
static const uint8_t TEMPSENSOR = 40;

#ifdef ARDUINO_MAIN
const uint32_t port_to_base[] = {
    NOT_A_PORT,
    (uint32_t) GPIO_PORTA_BASE,
    (uint32_t) GPIO_PORTB_BASE,
    (uint32_t) GPIO_PORTC_BASE,
    (uint32_t) GPIO_PORTD_BASE,
    (uint32_t) GPIO_PORTE_BASE,
    (uint32_t) GPIO_PORTF_BASE
};
const uint8_t digital_pin_to_timer[] = {
    NOT_ON_TIMER,   /*  0 - PA_1 */
    NOT_ON_TIMER,   /*  1 - PA_0 */
    NOT_ON_TIMER,   /*  2 - PA_7 */
    NOT_ON_TIMER,   /*  3 - PA_6 */
    NOT_ON_TIMER,   /*  4 - PE_4 */
    NOT_ON_TIMER,   /*  5 - PE_5 */
    T1A0,           /*  6 - PB_4 */
    T1B0,           /*  7 - PB_5 */
    WT2B,           /*  8 - PD_1 */
    WT2A,           /*  9 - PD_0 */
    T1B1,           /* 10 - PF_3 */
    T0B1,           /* 11 - PF_1 */
    T0A1,           /* 12 - PF_0 */
    T1A1,           /* 13 - PF_2 */
    T3B,            /* 14 - PB_3 */
    T3A,            /* 15 - PB_2 */
    NOT_ON_TIMER,   /* 16 - PE_3 */
    NOT_ON_TIMER,   /* 17 - PE_2 */
    NOT_ON_TIMER,   /* 18 - PE_1 */
    NOT_ON_TIMER,   /* 19 - PE_0 */
    WT3B,           /* 20 - PD_3 */
    WT3A,           /* 21 - PD_2 */
    WT1B,           /* 22 - PC_7 */
    WT1A,           /* 23 - PC_6 */
    WT0B,           /* 24 - PC_5 */
    WT0A,           /* 25 - PC_4 */
    NOT_ON_TIMER,   /* 26 - PA_2 */
    NOT_ON_TIMER,   /* 27 - PA_3 */
    NOT_ON_TIMER,   /* 28 - PA_4 */
    NOT_ON_TIMER,   /* 29 - PA_5 */
    T2A0,           /* 30 - PB_0 */
    T2B,            /* 31 - PB_1 */
    WT5A,           /* 32 - PD_6 */
    WT5B,           /* 33 - PD_7 */
    T2A1,           /* 34 - PF_4 */
    T0A0,           /* 35 - PB_6 */
    T0B0,           /* 36 - PB_7 */
    NOT_ON_TIMER,   /* 37 - NC   */
    NOT_ON_TIMER,   /* 38 - NC   */
    NOT_ON_TIMER,   /* 39 - NC   */
    NOT_ON_TIMER,   /* 40 - TEMP */
};
const uint8_t digital_pin_to_port[] = {
    PA,         /*  0 - PA_1 */
    PA,         /*  1 - PA_0 */
    PA,         /*  2 - PA_7 */
    PA,         /*  3 - PA_6 */
    PE,         /*  4 - PE_4 */
    PE,         /*  5 - PE_5 */
    PB,         /*  6 - PB_4 */
    PB,         /*  7 - PB_5 */
    PD,         /*  8 - PD_1 */
    PD,         /*  9 - PD_0 */
    PF,         /* 10 - PF_3 */
    PF,         /* 11 - PF_1 */
    PF,         /* 12 - PF_0 */
    PF,         /* 13 - PF_2 */
    PB,         /* 14 - PB_3 */
    PB,         /* 15 - PB_2 */
    PE,         /* 16 - PE_3 */
    PE,         /* 17 - PE_2 */
    PE,         /* 18 - PE_1 */
    PE,         /* 19 - PE_0 */
    PD,         /* 20 - PD_3 */
    PD,         /* 21 - PD_2 */
    PC,         /* 22 - PC_7 */
    PC,         /* 23 - PC_6 */
    PC,         /* 24 - PC_5 */
    PC,         /* 25 - PC_4 */
    PA,         /* 26 - PA_2 */
    PA,         /* 27 - PA_3 */
    PA,         /* 28 - PA_4 */
    PA,         /* 29 - PA_5 */
    PB,         /* 30 - PB_0 */
    PB,         /* 31 - PB_1 */
    PD,         /* 32 - PD_6 */
    PD,         /* 33 - PD_7 */
    PF,         /* 34 - PF_4 */
    PB,         /* 35 - PB_6 */
    PB,         /* 36 - PB_7 */
    NOT_A_PIN, /* NC */
    NOT_A_PIN, /* NC */
    NOT_A_PIN, /* NC */
    NOT_A_PIN /* TEMP_SENSE */
};
const uint8_t digital_pin_to_bit_mask[] = {
    BV(1),      /*  0 - PA_1 */
    BV(0),      /*  1 - PA_0 */
    BV(7),      /*  2 - PA_7 */
    BV(6),      /*  3 - PA_6 */
    BV(4),      /*  4 - PE_4 */
    BV(5),      /*  5 - PE_5 */
    BV(4),      /*  6 - PB_4 */
    BV(5),      /*  7 - PB_5 */
    BV(1),      /*  8 - PD_1 */
    BV(0),      /*  9 - PD_0 */
    BV(3),      /* 10 - PF_3 */
    BV(1),      /* 11 - PF_1 */
    BV(0),      /* 12 - PF_0 */
    BV(2),      /* 13 - PF_2 */
    BV(3),      /* 14 - PB_3 */
    BV(2),      /* 15 - PB_2 */
    BV(3),      /* 16 - PE_3 */
    BV(2),      /* 17 - PE_2 */
    BV(1),      /* 18 - PE_1 */
    BV(0),      /* 19 - PE_0 */
    BV(3),      /* 20 - PD_3 */
    BV(2),      /* 21 - PD_2 */
    BV(7),      /* 22 - PC_7 */
    BV(6),      /* 23 - PC_6 */
    BV(5),      /* 24 - PC_5 */
    BV(4),      /* 25 - PC_4 */
    BV(2),      /* 26 - PA_2 */
    BV(3),      /* 27 - PA_3 */
    BV(4),      /* 28 - PA_4 */
    BV(5),      /* 29 - PA_5 */
    BV(0),      /* 30 - PB_0 */
    BV(1),      /* 31 - PB_1 */
    BV(6),      /* 32 - PD_6 */
    BV(7),      /* 33 - PD_7 */
    BV(4),      /* 34 - PF_4 */
    BV(6),      /* 35 - PB_6 */
    BV(7),      /* 36 - PB_7 */
    NOT_A_PIN,  /* 37 - NC */
    NOT_A_PIN,  /* 38 - NC */
    NOT_A_PIN,  /* 39 - NC */
    NOT_A_PIN   /* 40 - TEMP_SENSE */
};

const uint32_t timer_to_offset[] = {
    TIMER0,
    TIMER0,
    TIMER0,
    TIMER0,
    TIMER1,
    TIMER1,
    TIMER1,
    TIMER1,
    TIMER2,
    TIMER2,
    TIMER2,
    TIMER3,
    TIMER3,
    WTIMER0,
    WTIMER0,
    WTIMER1,
    WTIMER1,
    WTIMER2,
    WTIMER2,
    WTIMER3,
    WTIMER3,
    WTIMER5,
    WTIMER5,
};

const uint8_t timer_to_ab[] = {
    TIMA,
    TIMA,
    TIMB,
    TIMB,
    TIMA,
    TIMA,
    TIMB,
    TIMB,
    TIMA,
    TIMA,
    TIMB,
    TIMA,
    TIMB,
    TIMA,
    TIMB,
    TIMA,
    TIMB,
    TIMA,
    TIMB,
    TIMA,
    TIMB,
    TIMA,
    TIMB,
};
const uint32_t timer_to_pin_config[] = {
    GPIO_PB6_T0CCP0,
    GPIO_PF0_T0CCP0,
    GPIO_PB7_T0CCP1,
    GPIO_PF1_T0CCP1,
    GPIO_PB4_T1CCP0,
    GPIO_PF2_T1CCP0,
    GPIO_PB5_T1CCP1,
    GPIO_PF3_T1CCP1,
    GPIO_PB0_T2CCP0,
    GPIO_PF4_T2CCP0,
    GPIO_PB1_T2CCP1,
    GPIO_PB2_T3CCP0,
    GPIO_PB3_T3CCP1,
    GPIO_PC4_WT0CCP0,
    GPIO_PC5_WT0CCP1,
    GPIO_PC6_WT1CCP0,
    GPIO_PC7_WT1CCP1,
    GPIO_PD0_WT2CCP0,
    GPIO_PD1_WT2CCP1,
    GPIO_PD2_WT3CCP0,
    GPIO_PD3_WT3CCP1,
    GPIO_PD6_WT5CCP0,
    GPIO_PD7_WT5CCP1,
};
const uint32_t digital_pin_to_analog_in[] = {
    NOT_ON_ADC,     /*  0 - PA_1 */
    NOT_ON_ADC,     /*  1 - PA_0 */
    NOT_ON_ADC,     /*  2 - PA_7 */
    NOT_ON_ADC,     /*  3 - PA_6 */
    ADC_CTL_CH6,    /*  4 - PE_4 */
    ADC_CTL_CH7,    /*  5 - PE_5 */
    ADC_CTL_CH8,    /*  6 - PB_4 */
    ADC_CTL_CH9,    /*  7 - PB_5 */
    ADC_CTL_CH10,   /*  8 - PD_1 */
    ADC_CTL_CH11,   /*  9 - PD_0 */
    NOT_ON_ADC,     /* 10 - PF_3 */
    NOT_ON_ADC,     /* 11 - PF_1 */
    NOT_ON_ADC,     /* 12 - PF_0 */
    NOT_ON_ADC,     /* 13 - PF_2 */
    NOT_ON_ADC,     /* 14 - PB_3 */
    NOT_ON_ADC,     /* 15 - PB_2 */
    ADC_CTL_CH0,    /* 16 - PE_3 */
    ADC_CTL_CH1,    /* 17 - PE_2 */
    ADC_CTL_CH2,    /* 18 - PE_1 */
    ADC_CTL_CH3,    /* 19 - PE_0 */
    ADC_CTL_CH4,    /* 20 - PD_3 */
    ADC_CTL_CH5,    /* 21 - PD_2 */
    NOT_ON_ADC,     /* 22 - PC_7 */
    NOT_ON_ADC,     /* 23 - PC_6 */
    NOT_ON_ADC,     /* 24 - PC_5 */
    NOT_ON_ADC,     /* 25 - PC_4 */
    NOT_ON_ADC,     /* 26 - PA_2 */
    NOT_ON_ADC,     /* 27 - PA_3 */
    NOT_ON_ADC,     /* 28 - PA_4 */
    NOT_ON_ADC,     /* 29 - PA_5 */
    NOT_ON_ADC,     /* 30 - PB_0 */
    NOT_ON_ADC,     /* 31 - PB_1 */
    NOT_ON_ADC,     /* 32 - PD_6 */
    NOT_ON_ADC,     /* 33 - PD_7 */
    NOT_ON_ADC,     /* 34 - PF_4 */
    NOT_ON_ADC,     /* 35 - PB_6 */
    NOT_ON_ADC,     /* 36 - PB_7 */
    NOT_ON_ADC,     /* 37 - NC   */
    NOT_ON_ADC,     /* 38 - NC   */
    NOT_ON_ADC,     /* 39 - NC   */
    ADC_CTL_TS      /* 40 - TemperatureSensor */
};
#endif
#endif 
