/**********************************************************************
* © 2017 LG
*
 Proyecto: MISE Micros 16bits 2017
* Fichero: LCD_Explorer16_G1_v0.h

Grupo: G1    Autor: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-17
 *  v0
*
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Luis                       LCD 2x16 Explorer 16
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "Main_MISE_G13_v0.h"
#include "IO_Explorer16_G13_v0.h"
// Pines de control LCD
#define  RW  _LATD5	// LCD R/W signal
#define  RS  _LATB15	// LCD RS signal
#define  E   _LATD4	// LCD E signal
//#define  E   LATFbits.LATF1       // LCD E signal

#define  RW_TRIS    _TRISD5
#define  RS_TRIS    _TRISB15
#define  E_TRIS	    _TRISD4
//#define  E_TRIS   TRISFbits.TRISF1

// Pines de datos
#define  DATA      LATE	    // Port for LCD data
#define  DATAPORT  PORTE
#define  TRISDATA  TRISE    // I/O setup for data Port, solo 8 de los 16 bits

/*****	LCD COMMAND FUCNTION PROTOTYPES  *****/
#define cursor_right()  lcd_cmd( 0x14 )     // 40us
#define cursor_left()   lcd_cmd( 0x10 )     // 40us
#define display_shift() lcd_cmd( 0x1C )     // 40us
#define inicio_fila_1() lcd_cmd(0x80)      //40us, DDRAM 0x00, cursor inicio fila 1
#define inicio_fila_2() lcd_cmd(0xC0)      //40us, DDRAM 0x40, cursor inicio fila 2
#define home_clr()      lcd_cmd( 0x01 )     // 1,64ms
#define home_it()       lcd_cmd( 0x02 )     // 1,64ms

#define function_set()  lcd_cmd( 0x38)      // 40us
#define display_on()    lcd_cmd( 0x0C)      // 40us
#define entry_mode()    lcd_cmd( 0x06)      // 40us
#define Timer_LCD 9

#if (Timer_LCD == 1)
#define TMR_Timer_LCD  TMR1
#define PR_Timer_LCD   PR1
#define TCON_Timer_LCD T1CON

#define _Timer_LCD_Interrupt _T1Interrupt
#define _Timer_LCD_IF _T1IF
#define _Timer_LCD_IP _T1IP
#define _Timer_LCD_IE _T1IE

#elif(Timer_LCD==2)
#define TMR_Timer_LCD  TMR2
#define PR_Timer_LCD   PR2
#define TCON_Timer_LCD T2CON

#define _Timer_LCD_Interrupt _T2Interrupt
#define _Timer_LCD_IF _T2IF
#define _Timer_LCD_IP _T2IP
#define _Timer_LCD_IE _T2IE

#elif (Timer_LCD == 3)
#define TMR_Timer_LCD  TMR3
#define PR_Timer_LCD   PR3
#define TCON_Timer_LCD T3CON

#define _Timer_LCD_Interrupt _T3Interrupt
#define _Timer_LCD_IF _T3IF
#define _Timer_LCD_IP _T3IP
#define _Timer_LCD_IE _T3IE

#elif (Timer_LCD == 4)
#define TMR_Timer_LCD  TMR4
#define PR_Timer_LCD   PR4
#define TCON_Timer_LCD T4CON

#define _Timer_LCD_Interrupt _T4Interrupt
#define _Timer_LCD_IF _T4IF
#define _Timer_LCD_IP _T4IP
#define _Timer_LCD_IE _T4IE

#elif (Timer_LCD == 5)
#define TMR_Timer_LCD  TMR5
#define PR_Timer_LCD   PR5
#define TCON_Timer_LCD T5CON

#define _Timer_LCD_Interrupt _T5Interrupt
#define _Timer_LCD_IF _T5IF
#define _Timer_LCD_IP _T5IP
#define _Timer_LCD_IE _T5IE

#elif (Timer_LCD == 6)
#define TMR_Timer_LCD  TMR6
#define PR_Timer_LCD   PR6
#define TCON_Timer_LCD T6CON

#define _Timer_LCD_Interrupt _T6Interrupt
#define _Timer_LCD_IF _T6IF
#define _Timer_LCD_IP _T6IP
#define _Timer_LCD_IE _T6IE

#elif (Timer_LCD == 7)
#define TMR_Timer_LCD  TMR7
#define PR_Timer_LCD   PR7
#define TCON_Timer_LCD T7CON

#define _Timer_LCD_Interrupt _T7Interrupt
#define _Timer_LCD_IF _T7IF
#define _Timer_LCD_IP _T7IP
#define _Timer_LCD_IE _T7IE

#elif(Timer_LCD==8)

#define TMR_Timer_LCD  TMR8
#define PR_Timer_LCD   PR8
#define TCON_Timer_LCD T8CON

#define _Timer_LCD_Interrupt _T8Interrupt
#define _Timer_LCD_IF _T8IF
#define _Timer_LCD_IP _T8IP
#define _Timer_LCD_IE _T8IE

#elif (Timer_LCD == 9)

#define TMR_Timer_LCD  TMR9
#define PR_Timer_LCD   PR9
#define TCON_Timer_LCD T9CON

#define _Timer_LCD_Interrupt _T9Interrupt
#define _Timer_LCD_IF _T9IF
#define _Timer_LCD_IP _T9IP
#define _Timer_LCD_IE _T9IE
#endif
/******	LCD FUNCTION PROTOYPES ******/

void Inic_LCD( void );		        // initialize display		
void lcd_cmd( char cmd );	        // write command to lcd
void lcd_data( char data );		    // write data to lcd
void inic_timer_LCD (void);
void espera_LCD_ms( unsigned int n_miliseg );
void espera_LCD_us( unsigned int n_microseg );
void inic_Maquina_Estado (void);
//void Visualiza_LCD_Fila_1 (void);
//void Visualiza_LCD_Fila_2 (void);

//#define fila_1	0x02;  // 0x80
//#define fila_2	0xC0;

extern unsigned int flag147;
