/**********************************************************************
* © 2017 LG
*
 Proyecto: MISE Micros 16bits 2017
* Fichero: IO_Explorer16_G1_v0.h

Grupo: G1    Autor: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-20
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

// Los leds están conectados a tierra ->
// -> Un '1' en la salida del PIC lo enciende
// -> Un '0' lo Apaga

//#include "p24hj256gp610A.h"
#include "Main_MISE_G13_v0.h"

//==============================================================================
#define LED_D3      _LATA0  // LATAbits.LATA0 Para leer el valor interno. Con PORT la tension de la pata (PARA SALIDA)
#define LED_D6      _LATA3
#define LED_D4      _LATA1
#define LED_D5      _LATA2
#define LED_D7      _LATA4
#define LED_D8      _LATA5
#define LED_D9      _LATA6
#define LED_D10     _LATA7

#define TRIS_LED_D3 _TRISA0 // TRISAbits.TRISA0 (otra manera de decirlo)
#define TRIS_LED_D6 _TRISA3
#define TRIS_LED_D4 _TRISA1
#define TRIS_LED_D5 _TRISA2
#define TRIS_LED_D7 _TRISA4
#define TRIS_LED_D8 _TRISA5
#define TRIS_LED_D9 _TRISA6
#define TRIS_LED_D10 _TRISA7

#define led_encendido   1
#define led_apagado     0


#define PULSADOR_S3         _RD6    // PORTDbits.PORTD6 (Pones esto o _RD6) 
#define PULSADOR_S6         _RD7   
#define PULSADOR_S4         _RD13    
#define PULSADOR_S5         _RA7   

#define TRIS_PULSADOR_S3    _TRISD6    //Para definir los modos I/O
#define TRIS_PULSADOR_S6    _TRISD7
#define TRIS_PULSADOR_S4    _TRISD13
#define TRIS_PULSADOR_S5    _TRISA7 

#define IE_PULSADOR_S3      _CN15IE //Puede definir una interrupcion
#define IE_PULSADOR_S6      _CN16IE
#define IE_PULSADOR_S4      _CN19IE

//==============================================================================
extern unsigned int Pulsado_S3;
extern unsigned int Pulsado_S6;
extern unsigned int Pulsado_S4;
extern unsigned int Pulsado_S5;
extern unsigned short fscroll;
extern unsigned short flagCAD;
//==============================================================================
void Inic_Leds (void);
void Inic_Pulsadores (void);
void _ISR _CNInterrupt (void);

