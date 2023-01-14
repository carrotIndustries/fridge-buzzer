#include <msp430.h>
#include <stdint.h>

static volatile uint8_t beep_on = 0;

static void set_beep_freq(uint16_t freq)
{
    beep_on = freq!=0;
    if(freq) {
        P1SEL |= BIT1;
        TACCTL0 = OUTMOD_4;
        //TACCR0 = 150;
        //TACCR0 = 600;
        TACCR0 = freq;
        TACCR1 = 0;
        TACTL = TASSEL_2 | MC_1;
    }
    else {
        P1SEL &= ~BIT1;
        TACCTL0 = OUTMOD_5;
        TACTL = TASSEL_2 | MC_0;
    }
}

static uint8_t beep_on1;
static uint8_t beep_period1;
static uint8_t beep_on2;
static uint8_t beep_period2;
static uint16_t beep_freq;

static uint8_t beep_counter2;
static uint8_t beep_counter1;
static uint8_t beep_once = 0;

static void set_beep(uint8_t on1, uint8_t period1, uint8_t on2, uint8_t period2, uint16_t freq)
{
    beep_on1 = on1;
    beep_period1 = period1;
    beep_on2 = on2;
    beep_period2 = period2;
    beep_freq = freq;
    beep_counter1 = 0;
    beep_counter2 = 0;
    beep_once = 0;
}

static void set_beep_once(uint8_t on1, uint8_t period1, uint8_t on2, uint8_t period2, uint16_t freq)
{
    beep_on1 = on1;
    beep_period1 = period1;
    beep_on2 = on2;
    beep_period2 = period2;
    beep_freq = freq;
    beep_counter1 = 0;
    beep_counter2 = 0;
    beep_once = 1;
}

static void do_beep(void)
{

    //set_beep_freq(beep_freq);
    if(beep_counter1 < beep_on1 && (beep_counter2 < beep_on2)) {
        set_beep_freq(beep_freq);
    }
    else {
        set_beep_freq(0);
    }
    
    if(beep_counter1 < beep_period1) {
        beep_counter1++;
    }
    else {
        beep_counter1 = 0;
        if(beep_counter2 < beep_period2) {
            beep_counter2++;
        }
        else {
            beep_counter2 = 0;
            if(beep_once) {
                beep_on2 = 0;
            }
        }
    }
}

static uint16_t setup_counter = 25*20; //approx 25sec
static uint16_t door_open_counter = 0;
static uint8_t muted = 0;

void __attribute__((interrupt((WDT_VECTOR)))) WDT_ISR(void)
{
    static uint8_t div = 0;
    static uint8_t pin_last;
    
    if(setup_counter) {
        if((P1IN & BIT3) && !(pin_last & BIT3)) {
            set_beep(1, 2, 11, 10, 800); 
        }
        else if(!(P1IN & BIT3) && (pin_last & BIT3))
        {
            set_beep(0,0,0,0,0);
        }
        
        
        if(setup_counter == 1) {
            set_beep_once(5, 4, 3, 10, 150);
        }
        setup_counter--;
    }
    else {

        
        if(!(P1IN & BIT3)) {
            if(door_open_counter != 0xffff)
                door_open_counter++;
            if(!(P2IN & BIT6)) {
                muted = 1;
                set_beep_once(5, 4, 2, 10, 700);
            }
        }
        else {
            if(door_open_counter > 90*20 || muted) 
                set_beep_once(1, 2, 2, 10, 150);
            door_open_counter = 0;
            muted = 0;
            //set_beep(0,0,0,0,0);
        }
        
        if(door_open_counter == 90*20 && !muted) {
            set_beep(1, 4, 3, 15, 600);
        }
        if(door_open_counter == 120*20 && !muted) {
            set_beep(2, 4, 10, 20, 150);
        }
        if(door_open_counter == 5*60*20) {
            set_beep(0,0,0,0,0);
        }
        
        
    }
    
    pin_last = P1IN;
    do_beep();
    div = (div+1)%4;
    if(beep_on)
        LPM3_EXIT;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL3 = LFXT1S_2;
    
    P1DIR |= BIT1 | BIT0 | BIT2 | BIT4 | BIT5 | BIT6 | BIT7;
    P2DIR = ~BIT6;
    P2REN |= BIT6;
    P1OUT = 0;
    P2OUT = BIT6;
    
    P2SEL &= ~BIT7;
    
    WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL |  WDTIS1;
    IE1 |= WDTIE;
    __eint();
    set_beep_once(1, 4, 1, 10, 150);
    while(1) {
        if(!beep_on)
            LPM3;
    }

    return 0;
}
