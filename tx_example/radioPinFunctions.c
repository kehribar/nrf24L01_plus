/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform spesific functions in this file ...
* -----------------------------------------------------------------------------
*/

#include <avr/io.h>

// Modify these variables to customize the ports/pins the RF module will use
#define RF_DDR  DDRC
#define RF_PORT PORTC
#define RF_PIN  PINC

#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))

/* ------------------------------------------------------------------------- */

void nrf24_setupPins()
{
    set_bit(RF_DDR,0); // CE output
    set_bit(RF_DDR,1); // CSN output
    set_bit(RF_DDR,2); // SCK output
    set_bit(RF_DDR,3); // MOSI output
    clr_bit(RF_DDR,4); // MISO input
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(RF_PORT,0);
    }
    else
    {
        clr_bit(RF_PORT,0);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(RF_PORT,1);
    }
    else
    {
        clr_bit(RF_PORT,1);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(RF_PORT,2);
    }
    else
    {
        clr_bit(RF_PORT,2);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(RF_PORT,3);
    }
    else
    {
        clr_bit(RF_PORT,3);
    }
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead()
{
    return check_bit(RF_PIN,4);
}
/* ------------------------------------------------------------------------- */
