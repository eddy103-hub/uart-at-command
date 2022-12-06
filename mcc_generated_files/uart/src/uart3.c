/**
 * UART3 Generated Driver API Header File
 * 
 * @file uart3.c
 * 
 * @ingroup uart3
 * 
 * @brief This is the generated driver implementation file for the UART3 driver using CCL
 *
 * @version UART3 Driver Version 3.0.1
*/

/*
© [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

/**
  Section: Included Files
*/
#include "../uart3.h"

/**
  Section: Macro Declarations
*/

#define UART3_TX_BUFFER_SIZE (8) //buffer size should be 2^n
#define UART3_TX_BUFFER_MASK (UART3_TX_BUFFER_SIZE - 1) 

#define UART3_RX_BUFFER_SIZE (8) //buffer size should be 2^n
#define UART3_RX_BUFFER_MASK (UART3_RX_BUFFER_SIZE - 1)

/**
  Section: Driver Interface
 */

const uart_drv_interface_t UART3 = {
    .Initialize = &UART3_Initialize,
    .Deinitialize = &UART3_Deinitialize,
    .Read = &UART3_Read,
    .Write = &UART3_Write,
    .IsRxReady = &UART3_IsRxReady,
    .IsTxReady = &UART3_IsTxReady,
    .IsTxDone = &UART3_IsTxDone,
    .TransmitEnable = &UART3_TransmitEnable,
    .TransmitDisable = &UART3_TransmitDisable,
    .AutoBaudSet = NULL,
    .AutoBaudQuery = NULL,
    .BRGCountSet = NULL,
    .BRGCountGet = NULL,
    .BaudRateSet = NULL,
    .BaudRateGet = NULL,
    .AutoBaudEventEnableGet = NULL,
    .ErrorGet = &UART3_ErrorGet,
    .TxCompleteCallbackRegister = &UART3_TxCompleteCallbackRegister,
    .RxCompleteCallbackRegister = &UART3_RxCompleteCallbackRegister,
    .TxCollisionCallbackRegister = NULL,
    .FramingErrorCallbackRegister = &UART3_FramingErrorCallbackRegister,
    .OverrunErrorCallbackRegister = &UART3_OverrunErrorCallbackRegister,
    .ParityErrorCallbackRegister = &UART3_ParityErrorCallbackRegister,
    .EventCallbackRegister = NULL,
};

/**
  Section: UART3 variables
*/
static volatile uint8_t uart3TxHead = 0;
static volatile uint8_t uart3TxTail = 0;
static volatile uint8_t uart3TxBuffer[UART3_TX_BUFFER_SIZE];
volatile uint8_t uart3TxBufferRemaining;

static volatile uint8_t uart3RxHead = 0;
static volatile uint8_t uart3RxTail = 0;
static volatile uint8_t uart3RxBuffer[UART3_RX_BUFFER_SIZE];
static volatile uart3_status_t uart3RxStatusBuffer[UART3_RX_BUFFER_SIZE];
volatile uint8_t uart3RxCount;

volatile uart3_status_t uart3RxLastError;

/**
  Section: UART3 APIs
*/

void (*UART3_FramingErrorHandler)(void);
void (*UART3_OverrunErrorHandler)(void);
void (*UART3_ParityErrorHandler)(void);
void (*UART3_TxInterruptHandler)(void);
void (*UART3_RxInterruptHandler)(void);

static void UART3_DefaultFramingErrorCallback(void);
static void UART3_DefaultOverrunErrorCallback(void);
static void UART3_DefaultParityErrorCallback(void);
void UART3_TransmitISR (void);
void UART3_ReceiveISR(void);

/**
  Section: UART3  APIs
*/

void UART3_Initialize(void)
{
    PIE9bits.U3RXIE = 0;   
    UART3_RxCompleteCallbackRegister(UART3_ReceiveISR);  
    PIE9bits.U3TXIE = 0; 
    UART3_TxCompleteCallbackRegister(UART3_TransmitISR);

    // Set the UART3 module to the options selected in the user interface.

    //RXB disabled; 
    U3RXB = 0x0; 
    //TXB disabled; 
    U3TXB = 0x0; 
    //P1L 0x0; 
    U3P1L = 0x0; 
    //P2L 0x0; 
    U3P2L = 0x0; 
    //P3L 0x0; 
    U3P3L = 0x0; 
    //MODE Asynchronous 8-bit mode; RXEN enabled; TXEN enabled; ABDEN disabled; BRGS high speed; 
    U3CON0 = 0xB0; 
    //SENDB disabled; BRKOVR disabled; RXBIMD Set RXBKIF on rising RX input; WUE disabled; ON enabled; 
    U3CON1 = 0x80; 
    //FLO off; TXPOL not inverted; STP Transmit 1Stop bit, receiver verifies first Stop bit; RXPOL not inverted; RUNOVF RX input shifter stops all activity; 
    U3CON2 = 0x0; 
    //BRGL 68; 
    U3BRGL = 0x44; 
    //BRGH 0; 
    U3BRGH = 0x0; 
    //STPMD in middle of first Stop bit; TXWRE No error; 
    U3FIFO = 0x0; 
    //ABDIE disabled; ABDIF Auto-baud not enabled or not complete; WUIF WUE not enabled by software; 
    U3UIR = 0x0; 
    //TXCIF 0x0; RXFOIF not overflowed; RXBKIF No Break detected; CERIF No Checksum error; ABDOVF Not overflowed; 
    U3ERRIR = 0x0; 
    //TXCIE disabled; RXFOIE disabled; RXBKIE disabled; FERIE disabled; CERIE disabled; ABDOVE disabled; PERIE disabled; TXMTIE disabled; 
    U3ERRIE = 0x0; 

    UART3_FramingErrorCallbackRegister(UART3_DefaultFramingErrorCallback);
    UART3_OverrunErrorCallbackRegister(UART3_DefaultOverrunErrorCallback);
    UART3_ParityErrorCallbackRegister(UART3_DefaultParityErrorCallback);

    uart3RxLastError.status = 0;  
    uart3TxHead = 0;
    uart3TxTail = 0;
    uart3TxBufferRemaining = sizeof(uart3TxBuffer);
    uart3RxHead = 0;
    uart3RxTail = 0;
    uart3RxCount = 0;
    PIE9bits.U3RXIE = 1;
}

void UART3_Deinitialize(void)
{
    PIE9bits.U3RXIE = 0;   
    PIE9bits.U3TXIE = 0;
    U3RXB = 0x00;
    U3TXB = 0x00;
    U3P1L = 0x00;
    U3P2L = 0x00;
    U3P3L = 0x00;
    U3CON0 = 0x00;
    U3CON1 = 0x00;
    U3CON2 = 0x00;
    U3BRGL = 0x00;
    U3BRGH = 0x00;
    U3FIFO = 0x00;
    U3UIR = 0x00;
    U3ERRIR = 0x00;
    U3ERRIE = 0x00;
}

inline void UART3_Enable(void)
{
    U3CON1bits.ON = 1; 
}

inline void UART3_Disable(void)
{
    U3CON1bits.ON = 0; 
}

inline void UART3_TransmitEnable(void)
{
    U3CON0bits.TXEN = 1;
}

inline void UART3_TransmitDisable(void)
{
    U3CON0bits.TXEN = 0;
}

inline void UART3_ReceiveEnable(void)
{
    U3CON0bits.RXEN = 1;
}

inline void UART3_ReceiveDisable(void)
{
    U3CON0bits.RXEN = 0;
}

inline void UART3_SendBreakControlEnable(void)
{
    U3CON1bits.SENDB = 1;
}

inline void UART3_SendBreakControlDisable(void)
{
    U3CON1bits.SENDB = 0;
}

inline void UART3_TransmitInterruptEnable(void)
{
    PIE9bits.U3TXIE = 1;
}

inline void UART3_TransmitInterruptDisable(void)
{ 
    PIE9bits.U3TXIE = 0;
}

inline void UART3_ReceiveInterruptEnable(void)
{
    PIE9bits.U3RXIE = 1;
}
inline void UART3_ReceiveInterruptDisable(void)
{
    PIE9bits.U3RXIE = 0;
}

bool UART3_IsRxReady(void)
{
    return (uart3RxCount ? true : false);
}

bool UART3_IsTxReady(void)
{
    return (uart3TxBufferRemaining ? true : false);
}

bool UART3_IsTxDone(void)
{
    return U3ERRIRbits.TXMTIF;
}

size_t UART3_ErrorGet(void)
{
    uart3RxLastError.status = uart3RxStatusBuffer[(uart3RxTail + 1) & UART3_RX_BUFFER_MASK].status;

    return uart3RxLastError.status;
}

uint8_t UART3_Read(void)
{
    uint8_t readValue  = 0;
    uint8_t tempRxTail;
    
    tempRxTail = (uart3RxTail + 1) & UART3_RX_BUFFER_MASK; // Buffer size of RX should be in the 2^n  
    uart3RxTail = tempRxTail;  
    readValue = uart3RxBuffer[uart3RxTail];
    PIE9bits.U3RXIE = 0; 
    if(uart3RxCount != 0)
    {
        uart3RxCount--;
    }
    PIE9bits.U3RXIE = 1;
    return readValue;
}


void UART3_ReceiveISR(void)
{
    uint8_t regValue;
	uint8_t tempRxHead;
    // use this default receive interrupt handler code
    uart3RxStatusBuffer[uart3RxHead].status = 0;

    if(U3ERRIRbits.FERIF)
    {
        uart3RxStatusBuffer[uart3RxHead].ferr = 1;
        if(NULL != UART3_FramingErrorHandler)
        {
            UART3_FramingErrorHandler();
        } 
    }
    if(U3ERRIRbits.RXFOIF)
    {
        uart3RxStatusBuffer[uart3RxHead].oerr = 1;
        if(NULL != UART3_OverrunErrorHandler)
        {
            UART3_OverrunErrorHandler();
        }   
    }   
 
    regValue = U3RXB;
    
    tempRxHead = (uart3RxHead + 1) & UART3_RX_BUFFER_MASK;
    if (tempRxHead == uart3RxTail) 
    {
		// ERROR! Receive buffer overflow 
	} 
    else
    {
		uart3RxHead = tempRxHead;
		uart3RxBuffer[tempRxHead] = regValue;
		uart3RxCount++;
	}   
}

void UART3_Write(uint8_t txData)
{
    uint8_t tempTxHead;
    
    if(0 == PIE9bits.U3TXIE)
    {
        U3TXB = txData;
    }
    else if(uart3TxBufferRemaining) // check if at least one byte place is available in TX buffer
    {
       tempTxHead = (uart3TxHead + 1) & UART3_TX_BUFFER_MASK;
       
       uart3TxBuffer[tempTxHead] = txData;
       uart3TxHead = tempTxHead;
       PIE9bits.U3TXIE = 0; //Critical value decrement
       uart3TxBufferRemaining--; // one less byte remaining in TX buffer
    }
    else
    {
        //overflow condition; uart3TxBufferRemaining is 0 means TX buffer is full
    }
    PIE9bits.U3TXIE = 1;
}


void UART3_TransmitISR(void)
{
    uint8_t tempTxTail;
    // use this default transmit interrupt handler code
    if(sizeof(uart3TxBuffer) > uart3TxBufferRemaining) // check if all data is transmitted
    {
       tempTxTail = (uart3TxTail + 1) & UART3_TX_BUFFER_MASK;
       
       uart3TxTail = tempTxTail;
       U3TXB = uart3TxBuffer[tempTxTail];
       uart3TxBufferRemaining++; // one byte sent, so 1 more byte place is available in TX buffer
    }
    else
    {
        PIE9bits.U3TXIE = 0;
    }
}





static void UART3_DefaultFramingErrorCallback(void)
{
    
}

static void UART3_DefaultOverrunErrorCallback(void)
{
    
}

static void UART3_DefaultParityErrorCallback(void)
{
    
}

void UART3_FramingErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        UART3_FramingErrorHandler = callbackHandler;
    }
}

void UART3_OverrunErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        UART3_OverrunErrorHandler = callbackHandler;
    }    
}

void UART3_ParityErrorCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
        UART3_ParityErrorHandler = callbackHandler;
    } 
}
void UART3_RxCompleteCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
       UART3_RxInterruptHandler = callbackHandler; 
    }   
}

void UART3_TxCompleteCallbackRegister(void (* callbackHandler)(void))
{
    if(NULL != callbackHandler)
    {
       UART3_TxInterruptHandler = callbackHandler;
    }   
}

