/**********************************************************************
* © 2017 LG
*
Proyecto: MISE Micros 16bits 2017
 * Fichero: Main_MISE_G0_v0.h
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
#include "p24HJ256GP610A.h"


#define _ISR_NO_PSV __attribute__((interrupt, no_auto_psv))
#define n_filas 8
#define n_columnas 19
#define INTENSIDAD 50


//======================Definicion variables externas===========================

extern unsigned char Ventana_DATOS[n_filas][n_columnas] __attribute__((space(dma))); 
extern const char Texto_1[];//={"-- PIC24H -- Gx Tiempos:--/--/--"}; //Caracteres fila1 y aracteres fila 2
extern int  selector_de_leds, carga_led1, carga_led2, carga_led3, carga_led4, carga_led5, flag_timeout,flag_timeout_enable,flag_5seg;
//==========Porototipos de las funciones definidas en el main===================

void Copia_FLASH_RAM (unsigned char *texto, unsigned short longitud);
void Maquina_Estados (void);
void bits_to_ASCII (short fila,int ADCV);
void int_to_ASCII (short fila,int ADCV);
void Actualizar_Ventana_Continuo(void);
void Actualizar_Ventana_SG(void);
void Uso_CPU(long ciclos);