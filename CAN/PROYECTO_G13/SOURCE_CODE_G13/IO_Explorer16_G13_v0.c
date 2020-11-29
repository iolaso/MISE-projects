/*
Proyecto:  
Fichero: IO_Explorer16_G1_v0.c

Grupo: G1    Autor: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0

 Descripcion:
 * Inicializacion LEDS placa Explorer16: RA0, ..., RA7
 * Inicializacion pulsadores
 * Rutina de atencion al pulsador gestiona el cambio de modo de la DMA (CAD)
 * Atencion al estado de los pulsadores
 * 
 * Entradas:
 * Pulsadores S3=D6, S6=D7, S5=A7, S4=D13 
 *
 * Salidas:
 * LEDS D3=A0, D6=A3, D4=A1, D5=A2, D7=A4, D8=A5, D9=A6, D10=A7
 *  
*/

#include "DMA_UART_G13_v0.h"
#include "IO_Explorer16_G13_v0.h"
#include "Main_MISE_G13_v0.h"
#include "LCD_Explorer16_G13_v0.h"
#include "UART_G13_v0.h"
#include "CAD_NO_DMA_G13_v0.h"
#include "CAD_DMA_G13_v0.h"
#include "oscilador_v2.h"
#include "Tiempos_G13_v0.h"

unsigned int Pulsado_S3;
unsigned int Pulsado_S6;
unsigned int Pulsado_S4;
unsigned int Pulsado_S5;
unsigned short fscroll,flagCAD;

/* LEDS Explorer16
 Led3= RA0, Led4= RA1, ... , Led10= RA7
*/

//=========Inicializacion LEDS placa Explorer16: RA0, ..., RA7==================

void Inic_Leds (void)                                                           // Inicializa las patas de los leds de salida y el valor inicial
{

   TRIS_LED_D3 = 0; // _TRISA0= 0;    // Led_3 -> A0,  '0' -> Salida (Output) //TRIS es uno de los registros asociados a las pata I/O
                                                    // '1' -> Entrada (Input) 
   TRIS_LED_D6 = 0; // _TRISA3= 0;	  // Led_6 -> A3
   TRIS_LED_D4 = 0; // _TRISA1= 0;	  // Led_4 -> A1
   TRIS_LED_D5 = 0; // _TRISA2= 0;	  // Led_5 -> A2
   TRIS_LED_D7 = 0; // _TRISA4= 0;	  // Led_7 -> A4
   TRIS_LED_D8 = 0; // _TRISA5= 0;	  // Led_8 -> A5
   TRIS_LED_D9 = 0; // _TRISA6= 0;	  // Led_9 -> A6
   TRIS_LED_D10 = 0; // _TRISA7= 0;	  // Led_10 -> A7
 
// Los leds están conectados a tierra ->
// -> Un '1' en la salida del PIC lo enciende
// -> Un '0' lo Apaga
    _LATA0= 0;  // Valor de inicio LED_3 -> Apagado
    _LATA3= 0;  // Valor de inicio LED_6
    _LATA1= 0;  // Valor de inicio LED_4
    _LATA2= 0;  // Valor de inicio LED_5
    _LATA4= 0;  // Valor de inicio LED_7
    _LATA5= 0;  // Valor de inicio LED_8
    _LATA6= 0;  // Valor de inicio LED_9
    _LATA7= 0;  // Valor de inicio LED_10

} // Fin Inic_Leds

//=======================Inicializacion pulsadores==============================

void Inic_Pulsadores (void)
{
//  TRISDbits.TRISD6 = 1;                                                       //1= IN (entrada) 0= Out (salida)
    TRIS_PULSADOR_S3 = 1;                                                       //1= IN (entrada) 0= Out (salida)
    TRIS_PULSADOR_S6 = 1;
    TRIS_PULSADOR_S4 = 1;
    TRIS_PULSADOR_S5 = 0;
    
    Pulsado_S3 = 0;                                                             // Estado despulsado
    Pulsado_S6 = 0;
    Pulsado_S4 = 0;
    Pulsado_S5 = 0;
    
    IE_PULSADOR_S3 = 1;                                                         //Habilita interrupción de S3 -> RD6
    IE_PULSADOR_S6 = 1;                                                         //Habilita interrupción de S6 -> RD7
    IE_PULSADOR_S4 = 1;                                                         //Habilita interrupción de S4 -> RD13
    
    _CNIP = 2;                                                                  // Determina Prioridad, 1-> minima, 2 ..., 6, 7 -> maxima
    _CNIF = 0;                                                                  // Borra Flag de todas la CN (porque no se desactivan solos)
    _CNIE = 1;                                                                  // Habilita interrupcion general de CN

}   //FIN Inic_Pulsadores_Interrupcion

void _ISR_NO_PSV _CNInterrupt (void)
{
Nop();
Nop();

//==Rutina de atencion al pulsador gestiona el cambio de modo de la DMA (CAD)===
/*
    if (PULSADOR_S3 == 0)
    {
        Pulsado_S3 = 1;
        flagCAD=!flagCAD;                                                       //Invertimos el flag para alternar el modo de la DMA (CAD)
        if(flagCAD==0){                                                         //Cambiamos de modo el DMA del CAD para funcionar por continuo
            DMA2CONbits.CHEN = 0;                                               //Desactivamos el canal de la DMA                                          
            AD1CON4bits.DMABL=0;                                                //Nº de palabras del buffer por entrada analogica                                     
            DMA2CONbits.AMODE=0;                                                //0 Indir. Reg con Post-incr. Para que no deje huecos en CAD_BuffA (continuo)                                              
            _ADDMABM = 1;                                                       //1=bufer DMA en orden de conversión -> AMODE=0
            DMA2CNT = 16-1;                                                     //16 lecturas (4 por pata)
            DMA2CONbits.CHEN = 1;                                               //Reactivamos el canal de la DMA
            
            if(Fosc==80000000){ Nop();Nop();Nop();Nop();Nop();Nop();Nop();      //Mediante prueba y error hemos visto que sin los tiempos de espera no funcionaba
                                Nop();Nop();Nop();Nop();Nop();Nop();Nop();
                                Nop();Nop();Nop();Nop();Nop();Nop();}
            else{Nop();Nop();Nop();}
        }
        else{                                                                   //Cambiamos de modo el DMA del CAD para funcionar por continuo
            DMA2CONbits.CHEN = 0;                                               //Desactivamos el canal de la DMA                                                  
            AD1CON4bits.DMABL=2;                                                //Cuantas lecturas por pata (2^2=4)
            DMA2CONbits.AMODE=2;                                                //2 direc. Indirect, Perif 
            _ADDMABM = 0;                                                       //1=bufer DMA en orden de conversión -> AMODE=0
            DMA2CNT = 4-1;                                                      //En S/G, hacemos DMA2CNT*DMABL
            DMA2CONbits.CHEN = 1;                                               //Reactivamos el canal de la DMA
            if(Fosc==80000000){ Nop();Nop();Nop();Nop();Nop();Nop();Nop();      //Mediante prueba y error hemos visto que sin los tiempos de espera no funcionaba
                                Nop();Nop();Nop();Nop();Nop();Nop();Nop();
                                Nop();Nop();Nop();Nop();Nop();Nop();}
            else{Nop();Nop();Nop();}
        }   
    }
*/
//===================Atencion al estado de los pulsadores=======================

    if (PULSADOR_S3 == 0)
    {
        Pulsado_S3 = 1;
    }
    if (PULSADOR_S3 == 1)
    {
        Pulsado_S3 = 0;
    }
    if (PULSADOR_S6 == 0)                                                       //Pulsador que activa el flag que consultamos en el main para la gestion de scroll
    {
        Pulsado_S6 = 1;
        fscroll=1;    
    }
    if (PULSADOR_S6 == 1)
    {
        Pulsado_S6 = 0;
    }

    if (PULSADOR_S4 == 0)
    {
        Pulsado_S4 = 1;
    }

    if (PULSADOR_S4 == 1)
    {
        Pulsado_S4 = 0;
    }

_CNIF = 0;                                                                      //Borra flag de interrupción general
} // Fin _CNInterrupt


