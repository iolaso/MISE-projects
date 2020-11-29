/*
Proyecto:  
Fichero: IO_Explorer16_G0_v0.c

Grupo: 0    Autor: Luis
v1.0	Fecha: 2017-XI-12

 *
 * Entradas:
 * Pulsadores S3= D6, 
 *
 * Salidas:
 * LEDS D3= A0, D6=A3, D9= A6
 */

#include "p24hj256gp610A.h"
//#include "Main_MISE_G0_v0.h"
#include "IO_Explorer16_G0_v0.h"
#include "Tiempos.h"


unsigned int Pulsado_S3, Pulsado_S4, Pulsado_S5, Pulsado_S6;
char ledActual = 3;
char derecha = 0;
char funcionando = 1;

/* LEDS Explorer16
 Led3= RA0, Led4= RA1, ... , Led10= RA7
 */

// ============================================
// Inicializa LEDS placa Explorer16: RA0, ..., RA7

void Inic_Leds(void) // Inicializa las patas de los leds de salida y el valor inicial
{
    // -> faltan algunos

    TRIS_LED_D3 = 0; // _TRISA0= 0;    // Led_3 -> A0,  '0' -> Salida (Output) // '1' -> Entrada (Input) 
    TRIS_LED_D4 = 0; // _TRISA0= 0;    // Led_3 -> A0,
    TRIS_LED_D5 = 0; // _TRISA0= 0;    // Led_3 -> A0,
    TRIS_LED_D6 = 0; // _TRISA3= 0;	  // Led_6 -> A3
    TRIS_LED_D7 = 0; // _TRISA0= 0;    // Led_3 -> A0,
    TRIS_LED_D8 = 0; // _TRISA0= 0;    // Led_3 -> A0,
    TRIS_LED_D9 = 0; // _TRISA0= 0;    // Led_3 -> A0,
    TRIS_LED_D10 = 0;
    // Los leds están conectados a tierra ->
    // -> Un '1' en la salida del PIC lo enciende
    // -> Un '0' lo Apaga
    _LATA0 = 0; // Valor de inicio LED_3 -> Apagado
    _LATA1 = 0;
    _LATA2 = 0;
    _LATA3 = 0; // Valor de inicio LED_6
    _LATA4 = 0;
    _LATA5 = 0;
    _LATA6 = 0;
    _LATA7 = 0;

} // Fin Inic_Leds


void Inic_Pulsadores(void) {
    //  TRISDbits.TRISD6 = 1;	//1= IN (entrada) 0= Out (salida)
    TRIS_PULSADOR_S3 = 1; //1= IN (entrada) 0= Out (salida)
    TRIS_PULSADOR_S4 = 1;
    TRIS_PULSADOR_S5 = 1;
    TRIS_PULSADOR_S6 = 1;
    _TRISB3=1;

    Pulsado_S3 = 0; // Estado despulsado
    Pulsado_S4 = 0; // Estado despulsado
    Pulsado_S6 = 0; // Estado despulsado
    Pulsado_S5 = 0;
    IE_PULSADOR_S3 = 0; //Habilita interrupción de S3 -> RD6
    IE_PULSADOR_S4 = 0; //Habilita interrupción de S3 -> RD6
    IE_PULSADOR_S6 = 0; //Habilita interrupción de S3 -> RD6
    IE_PULSADOR_S5 = 0;
    
   // _CNIP = 2; // Determina Prioridad, 1-> minima, 2 ..., 6, 7 -> maxima
    _CNIF = 0; // Borra Flag de todas la CN
    _CNIE = 0; // Habilita interrupcion general de CN
    
    Nop();
    Nop();

    Nop();
    Nop();

} //FIN Inic_Pulsadores_Interrupcion

// Rutina atencion interrupciones pulsadores

void _ISR_NO_PSV _CNInterrupt(void) {
    Nop();
    Nop();

    Nop();
    Nop();
    if (PULSADOR_S3 == 0) {
        Pulsado_S3 = 1;
    }

    if (PULSADOR_S3 == 1) {
        Pulsado_S3 = 0;
    }
    if (PULSADOR_S4 == 0) {
        Pulsado_S4 = 1;
    }

    if (PULSADOR_S4 == 1) {
        Pulsado_S4 = 0;
    }
    if (PULSADOR_S6 == 0) {
        Pulsado_S6 = 1;
    }

    if (PULSADOR_S6 == 1) {
        Pulsado_S6 = 0;
    }
    _CNIF = 0; //Borra flag de interrupción general
} // Fin 
