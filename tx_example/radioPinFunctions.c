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

//#define USE_HARDWARE_SPI

//IO pin register and bit mapping
#define NRF24_DDR_MISO	DDRC
#define NRF24_PORT_MISO	PORTC
#define NRF24_PIN_MISO	PINC
#define NRF24_BIT_MISO	4
#define NRF24_DDR_MOSI	DDRC
#define NRF24_PORT_MOSI	PORTC
#define NRF24_BIT_MOSI	3
#define NRF24_DDR_SCK	  DDRC
#define NRF24_PORT_SCK	PORTC
#define NRF24_BIT_SCK	  2
#define NRF24_DDR_CS	  DDRC
#define NRF24_PORT_CS	  PORTC
#define NRF24_BIT_CS	  1
#define NRF24_DDR_CE	  DDRC
#define NRF24_PORT_CE	  PORTC
#define NRF24_BIT_CE	  0

//IRQ pin (currently not used)
#define NRF24_DDR_IRQ	  DDRB
#define NRF24_PORT_IRQ	PORTB
#define NRF24_BIT_IRQ	  2

//SPI register and bit remapping (some have SPCR, others SPCR0, etc.)
#define NRF24_SPCR	SPCR0
#define NRF24_SPIE  SPIE
#define NRF24_SPE   SPE
#define NRF24_DORD  DORD
#define NRF24_MSTR  MSTR
#define NRF24_CPOL  CPOL
#define NRF24_CPHA  CPHA
#define NRF24_SPR0  SPR0

#define NRF24_SPSR	SPSR0
#define NRF24_SPIF  SPIF
#define NRF24_SPI2X SPI2X

#define NRF24_SPDR	SPDR0


#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))

/* ------------------------------------------------------------------------- */
void nrf24_setupPins()
{
  set_bit(NRF24_DDR_CE,NRF24_BIT_CE); // CE output
  set_bit(NRF24_DDR_CS,NRF24_BIT_CS); // CSN output
  set_bit(NRF24_DDR_SCK,NRF24_BIT_SCK); // SCK output
  set_bit(NRF24_DDR_MOSI,NRF24_BIT_MOSI); // MOSI output
  clr_bit(NRF24_DDR_MISO,NRF24_BIT_MISO); // MISO input

#ifdef USE_HARDWARE_SPI
	NRF24_SPCR = (0<<NRF24_SPIE) | (1<<NRF24_SPE) | (0<<NRF24_DORD) | (1<<NRF24_MSTR) | (0<< NRF24_CPOL) | (0<<NRF24_CPHA) | (0<<NRF24_SPR0);
	NRF24_SPSR = (1<<NRF24_SPI2X);
#endif
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(NRF24_PORT_CE,NRF24_BIT_CE);
    }
    else
    {
        clr_bit(NRF24_PORT_CE,NRF24_BIT_CE);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(NRF24_PORT_CS,NRF24_BIT_CS);
    }
    else
    {
        clr_bit(NRF24_PORT_CS,NRF24_BIT_CS);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(NRF24_PORT_SCK,NRF24_BIT_SCK);
    }
    else
    {
        clr_bit(NRF24_PORT_SCK,NRF24_BIT_SCK);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state)
{
    if(state)
    {
        set_bit(NRF24_PORT_MOSI,NRF24_BIT_MOSI);
    }
    else
    {
        clr_bit(NRF24_PORT_MOSI,NRF24_BIT_MOSI);
    }
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead()
{
    return check_bit(NRF24_PIN_MISO,NRF24_BIT_MISO);
}
/* ------------------------------------------------------------------------- */

#ifndef USE_HARDWARE_SPI
extern uint8_t spi_transfer_SW(uint8_t tx);
#endif
uint8_t spi_transfer(uint8_t tx)
{
#ifdef USE_HARDWARE_SPI
  uint8_t rx = 0;

  NRF24_SPDR = tx;
  while ( !(NRF24_SPSR & (1<<NRF24_SPIF)) ) {} //wait until done
  rx = NRF24_SPDR;
  return rx;
#else
  return spi_transfer_SW(tx);
#endif
}
/* ------------------------------------------------------------------------- */
