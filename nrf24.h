/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* This library is based on this library: 
*   https://github.com/aaronds/arduino-nrf24l01
* Which is based on this library: 
*   http://www.tinkerer.eu/AVRLib/nRF24L01
* -----------------------------------------------------------------------------
*/
#ifndef NRF24
#define NRF24

#include "nRF24L01.h"
#include <stdint.h>

/* -------------------------------------------------------------------------- */
/* PLATFORM SPESIFIC DEFINITIONS BEGIN */
/* -------------------------------------------------------------------------- */
#include <util/delay.h>
#include <avr/io.h>
#define nrf24_ce_output() DDRC |= (1<<0)
#define nrf24_ce_high() PORTC |= (1<<0)
#define nrf24_ce_low() PORTC &= ~(1<<0)
/* -------------------------------------------------------------------------- */
#define nrf24_csn_output() DDRC |= (1<<1)
#define nrf24_csn_high() PORTC |= (1<<1)
#define nrf24_csn_low() PORTC &= ~(1<<1)
/* -------------------------------------------------------------------------- */
#define nrf24_sck_output() DDRC |= (1<<2)
#define nrf24_sck_high() PORTC |= (1<<2)
#define nrf24_sck_low() PORTC &= ~(1<<2)
/* -------------------------------------------------------------------------- */
#define nrf24_mosi_output() DDRC |= (1<<3)
#define nrf24_mosi_high() PORTC |= (1<<3)
#define nrf24_mosi_low() PORTC &= ~(1<<3)
/* -------------------------------------------------------------------------- */
#define nrf24_miso_input() DDRC &= ~(1<<4)
#define nrf24_miso_read() (PINC & (1<<4))
/* -------------------------------------------------------------------------- */
#define nrf24_delay_microseconds(x) _delay_us(x)
/* -------------------------------------------------------------------------- */
/* PLATFORM SPESIFIC DEFINITIONS END */
/* -------------------------------------------------------------------------- */

#define nrf24_ADDR_LEN 5
#define nrf24_CONFIG ((1<<EN_CRC)|(0<<CRCO))

#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST   1

/* adjustment functions */
void    nrf24_init();
void    nrf24_rx_address(uint8_t* adr);
void    nrf24_tx_address(uint8_t* adr);
void    nrf24_config(uint8_t channel, uint8_t pay_length);

/* state check functions */
uint8_t nrf24_dataReady();
uint8_t nrf24_isSending();
uint8_t nrf24_getStatus();
uint8_t nrf24_rxFifoEmpty();

/* core TX / RX functions */
void    nrf24_send(uint8_t* value);
void    nrf24_getData(uint8_t* data);

/* use in dynamic length mode */
uint8_t nrf24_payloadLength();

/* post transmission analysis */
uint8_t nrf24_lastMessageStatus();
uint8_t nrf24_retransmissionCount();

/* power management */
void    nrf24_powerUpRx();
void    nrf24_powerUpTx();
void    nrf24_powerDown();

/* low level interface ... */
uint8_t spi_transfer(uint8_t tx);
void    nrf24_transmitSync(uint8_t* dataout,uint8_t len);
void    nrf24_transferSync(uint8_t* dataout,uint8_t* datain,uint8_t len);
void    nrf24_configRegister(uint8_t reg, uint8_t value);
void    nrf24_readRegister(uint8_t reg, uint8_t* value, uint8_t len);
void    nrf24_writeRegister(uint8_t reg, uint8_t* value, uint8_t len);

#endif
