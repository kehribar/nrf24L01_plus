# nrf24L01+ Radio Library

This library is probably not the most efficient way of using the nrf24L01+ modules but probably the most portable and the basic one.

This library enables the auto acknowledgement and auto retransmission features of the nrf24L01+ in static length payload mode. 

These two features are basically the most important features of the nrf24L01+ modules. With this library, after you make a tranmission attempt, you'll know for sure wheter the slave device got your message properly or not with minimal MCU involvement. Also, nrf24L01+ modules automatically handle the retranmission of the same message if it they lost in transmission, up to o limited trials with adjustable delays in between attempts.

## Usage

### File strutcture

The library basically consists of only three files: `nrf24.c `, `nrf24.h` and `nRF24L01.h` . Hardware spesific functions are defined at the `radioPinFunctions.c` file. This file can be different at each device / platform.

### Configuration

Configuration of the module is simple. Max retransmission count is set to 15 and delays between each retranmission is set to 1000 us. Also the CRC is enabled with 1byte length. Data rate is set to 1Mbps.

Via the config function, you can chose the channel and the payload length. Max payload length is 32. After the config function the module automatically goes to RX mode. Payload length and the RF channel values have to be consistent among the devices.

	/* initializes hardware pins */
	nrf24_init();
	
	/* RF channel: #2 , payload length: 4 */
	nrf24_config(2,4)

### Addressing

Address length is 5 bytes. You can configure the transmit and receive addresses as follows:
	
	uint8_t rx_mac[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
	uint8_t tx_mac[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
		
	/* Set the module's own address */
	nrf24_rx_address(rx_mac);
	
	/* Set the transmit address */
	nrf24_tx_address(tx_mac);

### Transmit

Let's say the payload length is set to 4 and you have the following payload.
	
	uint8_t data_array[4];
	
	data_array[0] = 0x00;
	data_array[1] = 0xAA;
	data_array[2] = 0x55;
	data_array[3] = 0xFF;
	
The basic transmit function is the `nrf24_send()` function. The module automatically switches to the TX mode and power up if required.

	/* Automatically goes to TX mode */
	nrf24_send(data_array);		
	
After the `nrf24_send()`, you must wait for transmission to end. MCU can sleep or do another tasks during that period.
	
	/* Wait for transmission to end */
	while(nrf24_isSending());
	
After the transmission end, optionally you can make analysis on the last transmission attempt.

	uint8_t temp;
	
	/* Make analysis on last tranmission attempt */
	temp = nrf24_lastMessageStatus();

	if(temp == NRF24_TRANSMISSON_OK)
	{					
		xprintf("Tranmission went OK\r\n");
	}
	else if(temp == NRF24_MESSAGE_LOST)
	{					
		xprintf("Message is lost ...\r\n");	
	}
      	
    /* Retranmission count indicates the tranmission quality */
    temp = nrf24_retransmissionCount();
    xprintf("Retranmission count: %d\r\n",temp);
    
After the tranmission finishes, nrf24L01+ module stays in Standby mode. You can manually go back to RX mode:

	/* Go back to RX mode ... */
    nrf24_powerUpRx();
 
Or you can power down the module to lower the current consumption.

    /* Power down the module */
    nrf24_powerDown();	
    
### Receive

This library doesn't use the IRQ pin of the nrf24L01+ , therefore you need to poll the `nrf24_dataReady()` function periodically. Otherwise you might miss some packets. 

Also, you need to be in RX mode in order to be able to receive messages.

`nrf24_dataReady()` function returns non-zero only if a valid payload is awaiting in the RX fifo. `nrf24_getData(uint8_t* buf)` function copies the received message into the given buffer. 

	uint8_t data_array[4];

	if(nrf24_dataReady())
	{
		nrf24_getData(data_array);		
	}

## Porting the library

Hardware spesific definitions of the library are in the `radioPinFunctions.c` file which is located at under example folders. This library uses software SPI module and requires only basic digital input / output functions.

## References

This project is proudly and heavily based on the following libraries:

- https://github.com/aaronds/arduino-nrf24l01
- http://www.tinkerer.eu/AVRLib/nRF24L01
