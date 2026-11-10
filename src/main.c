/**
 * @file main.c
 * @brief MSP430G2553 Low-Power ADC Sampling & Dynamic Timer_A PWM Testbed
 * @author Phan Minh Huan
 * @details Reads analog voltage via ADC10 channel A0, maps the result to a
 *          10% - 90% duty cycle, and generates PWM on Timer_A0 (P1.2 / TA0.1).
 *          Verified using a digital storage oscilloscope.
 */

#include <msp430.h>
#include <stdint.h>

// PWM Parameters for 1 kHz output at 1 MHz DCO clock
#define TIMER_PERIOD      1000     // 1 MHz / 1000 = 1 kHz frequency
#define DUTY_MIN_10       100      // 10% duty cycle (100 / 1000)
#define DUTY_MAX_90       900      // 90% duty cycle (900 / 1000)

// Function Prototypes
void initClock(void);
void initGPIO(void);
void initTimerA_PWM(void);
void initADC10(void);
uint16_t readADC10(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;      // Stop Watchdog Timer

    initClock();
    initGPIO();
    initTimerA_PWM();
    initADC10();

    __enable_interrupt();          // Enable global interrupts

    while (1) {
        uint16_t raw_adc = readADC10(); // 10-bit resolution: 0 to 1023

        // Map ADC value (0 - 1023) linearly to 10% - 90% duty cycle (100 - 900)
        // Duty = DUTY_MIN_10 + (raw_adc * (DUTY_MAX_90 - DUTY_MIN_10)) / 1023
        uint16_t duty = DUTY_MIN_10 + ((uint32_t)raw_adc * (DUTY_MAX_90 - DUTY_MIN_10)) / 1023;

        CCR1 = duty;               // Update PWM duty cycle compare register

        __delay_cycles(10000);     // Sampling delay ~10ms
    }
}

void initClock(void) {
    // Configure DCO to calibrated 1 MHz
    if (CALBC1_1MHZ == 0xFF) {
        while (1);                 // Trap CPU if calibration data erased
    }
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
}

void initGPIO(void) {
    // P1.2 as Timer_A0 Output (TA0.1) -> PWM pin
    P1DIR |= BIT2;
    P1SEL |= BIT2;
    P1SEL2 &= ~BIT2;

    // P1.0 as Status LED indicator
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
}

void initTimerA_PWM(void) {
    CCR0 = TIMER_PERIOD - 1;       // PWM Period
    CCTL1 = OUTMOD_7;              // Reset/Set output mode
    CCR1 = DUTY_MIN_10;            // Initial duty cycle: 10%
    TACTL = TASSEL_2 + MC_1;       // SMCLK (1MHz), Up-mode
}

void initADC10(void) {
    // ADC10 ON, Reference: Vcc/Vss, 64 sample-and-hold clocks
    ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON;
    // Channel A0 (P1.0 or dedicated A0 pin), Clock: ADC10OSC
    ADC10CTL1 = INCH_0 | ADC10SSEL_0;
    // Enable analog input buffer on A0
    ADC10AE0 |= BIT0;
}

uint16_t readADC10(void) {
    ADC10CTL0 |= ENC | ADC10SC;    // Start conversion
    while (ADC10CTL1 & BUSY);      // Polling: wait until conversion completes
    return ADC10MEM;               // Return 10-bit result
}
