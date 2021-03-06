/**
  * @file    hal_uart.c
  * @author  Jorge Guzman (jorge.gzm@gmail.com); Rafael lopes (faellf@hotmail.com); 
  * @date    Mar 11, 2014
  * @version 0.1.0.0 (beta)
  * @brief   Codigo do Driver UART para o microcontrolador PIC18f4550.
  * @details
  * @section LICENSE
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation; either version 2 of
  * the License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * General Public License for more details at
  * http://www.gnu.org/copyleft/gpl.html
*/

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "hal_uart.h"
#include "device/hal_device.h"
#include "interrupt/hal_interrupts.h"

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

//==============================================================================
// PRIVATE VARIABLES			
//==============================================================================


//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

/**@brief TODO documentar*/
void (*uart0_rx_callback)(uint8_t);

/**@brief TODO documentar*/
void (*uart0_tx_callback)(void);

//==============================================================================
// SOURCE CODE
//==============================================================================

PUBLIC void uart_setup_interrupt(uint8_t module, uint8_t rx, uint8_t tx)
{
    switch(module)
    {
        case _UART0:

            PIE1bits.RCIE = rx;
            PIE1bits.TXIE = tx;

//            interrupt_setup(_UART_RX, rx);
//            interrupt_setup(_UART_TX, tx);

            break;
    }
}

PUBLIC void uart_priority_interrupt(uint8_t module, uint8_t priority_rx, uint8_t priority_tx)
{
    switch(module)
    {
    case _UART0:

        IPR1bits.RCIP = priority_rx; //modulo receptor
        IPR1bits.TXIP = priority_tx; //modulo transmissao

//        interrupt_set_priority(_UART_RX, priority_rx);
//        interrupt_set_priority(_UART_TX, priority_tx);

        break;
    }
}

PUBLIC void uart_set_baudRate(uint8_t module, uint8_t value)
{
    switch(module)
    {
        case _UART0:
            if(value == BAUD_4800)
            {
                //aki igual 9600
                SPBRGH = 0x09;
                SPBRG = 0xC3;
            }
            else if(value == BAUD_9600)
            {
                //valor baseado com um clock de 48MHz
                SPBRGH = 0x04;
                SPBRG = 0xE1;
            }
            else if(value == BAUD_19200)
            {
                //valor baseado com um clock de 48MHz
                SPBRGH = 0x02;
                SPBRG = 0x70;
            }
            else if(value == BAUD_115200)
            {
                //valor baseado com um clock de 48MHz
                SPBRGH = 0x00;
                SPBRG = 0x67;
            }
        break;
    }
}

PUBLIC void uart_attach(uint8_t rx, uint8_t tx)
{  
    //Configura pino do botao como entrada
    pinMode(rx, INPUT);
    pinMode(tx, INPUT);
}

PUBLIC void uart_set_conf(uint8_t module, uint8_t sync, uint8_t high_com, uint8_t rx9bits, uint8_t tx9bits,  uint8_t bd16bits)
{
    switch(module)
    {
        case _UART0:

            TXSTAbits.SYNC = sync;
            TXSTAbits.BRGH = high_com;
            RCSTAbits.RX9 = rx9bits;
            TXSTAbits.TX9 = tx9bits;
            BAUDCONbits.BRG16 = bd16bits;
        break;
    }
}

PUBLIC void uart_set_enable(uint8_t module, uint8_t enable_uart, uint8_t enable_rx, uint8_t enable_tx)
{
    switch(module)
    {
        case _UART0:
            
            RCSTAbits.SPEN = enable_uart;
            RCSTAbits.CREN = enable_rx;
            TXSTAbits.TXEN = enable_tx;
        break;
    }
}

PUBLIC void uart_putc(uint8_t UI8_caracter)
{
    TXREG = UI8_caracter;
    while(!TXSTAbits.TRMT);

    if(RCSTAbits.FERR)
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
}

inline uint8_t uart_get_data(void)
{
    return(RCREG);
}

PUBLIC void uart_rx_set_callback(uint8_t module, void (*func)(uint8_t))
{
    switch(module)
    {
        case _UART0:
            uart0_rx_callback = func;
        break;
    }
}

PUBLIC void uart_tx_set_callback(uint8_t module, void (*func)(void))
{
    switch(module)
    {
        case _UART0:
            uart0_tx_callback = func;
        break;
    }
}

inline void uart_rx_isr(void)
{
    uint8_t data;
    
    if(PIR1bits.RCIF)
    {
        data = uart_get_data();
        uart0_rx_callback(data);
        
        //Limpa flag da interrupcao
        PIR1bits.RCIF = 0;
    }
}

inline void uart_tx_isr(void)
{
    if(PIR1bits.TXIF)
    {
        uart0_tx_callback();
        
        //Limpa flag da interrupcao
        PIR1bits.TXIF = 0;
    }
}


