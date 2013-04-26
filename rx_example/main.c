/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
*/

#include <avr/io.h>
#include <stdint.h>
#include "../nrf24.h"

/* ------------------------------------------------------------------------- */
/* Software UART routine. */
/* 9600 bps. Transmit only */
/* Transmit pin is: B2 */
/* ------------------------------------------------------------------------- */
/* Hardware specific section ... */
/* ------------------------------------------------------------------------- */
#include <util/delay.h>
#define uart_pin_output()    DDRB |= (1<<2)
#define uart_set_pin()        PORTB |= (1<<2)
#define uart_clr_pin()        PORTB &= ~(1<<2)
#define uart_bit_dly()        _delay_us(100)
/* ------------------------------------------------------------------------- */
/* Printing functions */
/* ------------------------------------------------------------------------- */
#include "./util/xprintf.h"
/* ------------------------------------------------------------------------- */
void uart_init()
{
    uart_pin_output();
}
/* ------------------------------------------------------------------------- */
void uart_put_char(uint8_t tx)
{
    uint8_t i;

    /* Start condition */
    uart_clr_pin();
    uart_bit_dly();

    for(i=0;i<8;i++)
    {
        if(tx & (1<<i))
        {
            uart_set_pin();
        }
        else
        {
            uart_clr_pin();
        }

        uart_bit_dly();
    }

    /* Stop condition */
    uart_set_pin();
    uart_bit_dly();
}
/* ------------------------------------------------------------------------- */
uint8_t temp;
uint8_t q = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
/* ------------------------------------------------------------------------- */
int main()
{
    /* init the software uart */
    uart_init();

    /* init the xprintf library */
    xdev_out(uart_put_char);

    /* simple greeting message */
    xprintf("\r\n> RX device ready\r\n");

    /* init hardware pins */
    nrf24_init();
    
    /* Channel #2 , payload length: 4 */
    nrf24_config(2,4);
 
    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);

    while(1)
    {    
        if(nrf24_dataReady())
        {
            nrf24_getData(data_array);        
            xprintf("> ");
            xprintf("%2X ",data_array[0]);
            xprintf("%2X ",data_array[1]);
            xprintf("%2X ",data_array[2]);
            xprintf("%2X\r\n",data_array[3]);
        }
    }
}
/* ------------------------------------------------------------------------- */