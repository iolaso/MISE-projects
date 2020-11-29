/**********************************************************************
* © 2017 LG
*
 Proyecto: MISE Micros 16bits 2017
* Fichero: UART_G1_v0.h

Grupo: G1    Autor: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0
*
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/


//====================================================================== 
// timing and baud rate calculations 
#define Fosc    80000000 //Frecuencia reloj CPU 
#define Fcy     Fosc/2 
#define BAUDRATE2 19200     // baudios (bits/sg) de transmision 
#define BAUD_RATEREG_2_BRGH1 ((Fcy/BAUDRATE2)/4)-1  // Para BRGH = 1 
#define BAUD_RATEREG_2_BRGH0 ((Fcy/BAUDRATE2)/16)-1  // Para BRGH = 0 
// Reg/bits UART2 serial port 
// U2MODE  no definido en P24FJ12GA010A.h 
#define _STSEL_U2   U2MODEbits.STSEL 
#define _PDSEL0_U2  U2MODEbits.PDSEL0 
#define _PDSEL1_U2  U2MODEbits.PDSEL1 
#define _BRGH_U2    U2MODEbits.BRGH 
#define _RXINV_U2   U2MODEbits.RXINV 
#define _ABAUD_U2   U2MODEbits.ABAUD 
#define _LPBACK_U2  U2MODEbits.LPBACK 
#define _WAKE_U2    U2MODEbits.WAKE 
#define _UEN0_U2    U2MODEbits.UEN0 
#define _UEN1_U2    U2MODEbits.UEN1 
#define _RTSMD_U2   U2MODEbits.RTSMD 
#define _IREN_U2    U2MODEbits.IREN 
#define _USIDL_U2   U2MODEbits.USIDL 
#define _UARTEN_U2  U2MODEbits.UARTEN 
#define _PDSEL_U2   U2MODEbits.PDSEL 
#define _UEN_U2  U2MODEbits.UEN 
// U2STA 
#define _URXDA_U2   U2STAbits.URXDA 
#define _OERR_U2    U2STAbits.OERR 
#define _FERR_U2    U2STAbits.FERR 
#define _PERR_U2    U2STAbits.PERR 
#define _RIDLE_U2   U2STAbits.RIDLE 
#define _ADDEN_U2   U2STAbits.ADDEN 
#define _URXISEL0_U2 U2STAbits.URXISEL0 
#define _URXISEL1_U2 U2STAbits.URXISEL1 
#define _TRMT_U2    U2STAbits.TRMT 
#define _UTXBF_U2   U2STAbits.UTXBF 
#define _UTXEN_U2   U2STAbits.UTXEN 
#define _UTXBRK_U2  U2STAbits.UTXBRK 
#define _UTXISEL0_U2 U2STAbits.UTXISEL0 
#define _UTXINV_U2  U2STAbits.UTXINV 
#define _UTXISEL1_U2 U2STAbits.UTXISEL1 
#define _URXISEL_U2  U2STAbits.URXISEL 
#define UART2_TX_TRIS   TRISFbits.TRISF5 
#define UART2_RX_TRIS   TRISFbits.TRISF4 
#define UART2_TX_LAT    LATFbits.LATF5 
#define UART2_RX_LAT    LATFbits.LATF4 
// I/O definitions  
#define    CR  0x0D 
#define    LF  0x0A 
#define BACKSPACE 0x8   // ASCII backspace character code             
// useful macros 
//#define clrscr    "\x1b[2J" 
//#define home      "\x1b[H" 
//#define pcr       "\r" 