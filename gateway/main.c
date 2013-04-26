/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* This example is work in progress ... 
* -----------------------------------------------------------------------------
*/
#include <avr/io.h>
#include <stdint.h>
#include "../nrf24.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "./util/xprintf.h"

/* ------------------------------------------------------------------------- */
volatile uint8_t uart_data;
volatile uint8_t uartBusy = 0;
volatile uint8_t message_length;
volatile uint8_t uartBuffer[48];
volatile uint8_t radioBuffer[32];
volatile uint8_t new_message = 0;
volatile uint8_t buffer_index = 0;
/* ------------------------------------------------------------------------- */
ISR(USART_RX_vect)
{
    /* Read the byte from the hardware in any case ... */
    uart_data = UDR0;

    /* The first arrived byte indicates the length of the incoming packet */
    if(!uartBusy)
    {
        /* Reset the state variables */
        /* TODO: Use one byte and utilize each bit as a state indicator */
        uartBusy = 1;
        new_message = 0;
        buffer_index = 0;
        message_length = uart_data;

        /* Be rational ... */
        if(message_length > sizeof(uartBuffer))
        {
            message_length = sizeof(uartBuffer);
        }

    }
    else
    {
        /* Fill the uart buffer until the end of the packet */
        if(!new_message)
        {
            uartBuffer[buffer_index++] = uart_data;

            /* If true, we reached the end of the packet */
            if(buffer_index >= message_length)
            {
                new_message = 1;
            }
        }            
    }
}
/* ------------------------------------------------------------------------- */
void send_char(char c)
{
    /* Wait for empty transmit buffer ... */
    while(!(UCSR0A & (1<<UDRE0)));

    /* Start sending the data! */
    UDR0 = c;

    /* Wait until the transmission is over ... */
    while(!(UCSR0A & (1<<TXC0)));
}
/* ------------------------------------------------------------------------- */
void handle_task()
{
    switch(uartBuffer[0])
    {
        // ------------------------------------------------------------
        // auto response
        // ------------------------------------------------------------
        case 0:            
        {
            send_char(0x01);
            send_char(0x00);
            
            break;
        }
        // ------------------------------------------------------------
        // change settings
        // ------------------------------------------------------------
        // 1: channel
        // 2: payload length
        // 3-8: gateway address
        case 1:
        {
            // first power down the module
            nrf24_powerDown();

            // wait a little?
            _delay_ms(1);
            
            // set the parameters
            nrf24_config(uartBuffer[1],uartBuffer[2]);

            // set the address
            nrf24_rx_address((uint8_t*)(uartBuffer+3));

            break;
        }
        // ------------------------------------------------------------
        // send message
        // ------------------------------------------------------------
        // 1-6: node address
        // 7: payload length 
        // 8-rest: data
        case 2:
        {
            // set node address
            nrf24_tx_address((uint8_t*)(uartBuffer+1));
            
            // send the data
            nrf24_send((uint8_t*)(uartBuffer+8));
            
            // return to receive mode
            nrf24_powerUpRx();

            break;
        }
    }
}
/* ------------------------------------------------------------------------- */
void init_serial()
{
    /* 115200 baud rate with 16 MHz Xtal ... */
    const uint8_t ubrr = 8;

    DDRD &= ~(1<<0);
    DDRD |=  (1<<1);

    /* Set baud rate */ 
    UBRR0H = (unsigned char)(ubrr>>8); 
    UBRR0L = (unsigned char)ubrr; 

    /* Enable receiver and transmitter and Receive interupt */ 
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); 

    /* Set frame format: 8data, 1stop bit no parity */ 
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00); 

    /* Enable interrupts ... */
    sei();
}
/* ------------------------------------------------------------------------- */
uint8_t temp;
uint8_t i = 0;
uint8_t data_array[4];
uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
/* ------------------------------------------------------------------------- */
int main()
{
    /* init the uart */
    init_serial();

    /* init the xprintf library */
    xdev_out(send_char);

    /* simple greeting message */
    // xprintf("\r\n> RX device ready\r\n");

    /* init hardware pins */
    nrf24_init();
    
    /* Channel #2 , payload length: 4 */
    nrf24_config(2,4);
 
    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);

    while(1)
    {           
        if(new_message)
        {
            handle_task();            
            new_message = 0;
            uartBusy = 0;
        }        
        if(nrf24_dataReady())
        {
            // get the data from the nrf24 module
            nrf24_getData(radioBuffer);            
            
            send_char(nrf24_payload_length()+1);

            send_char(0x01);

            for(i=0;i<nrf24_payload_length();i++)
            {
                send_char(radioBuffer[i]);
            }
        }
    }
}
/* ------------------------------------------------------------------------- */
