/*
Proyecto: MISE Micros 16bits 
Fichero: LCD_Explorer_G1_v0.c

Grupo: G1    Autores: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0

Descripción:
 * Envio de COMANDO
 * Envio de DATO
 * Genera la secuencia de comandos de inicialización del LCD display
 * Inicialización del timer para actualizar el LCD
 * Espera a que pasen n milisegundos
 * Espera a que pasen n microsegundos=
 * Inicialización pines LCD
 * Funcion que configura el timer para la maquina de estados
 * Interrupcion del Timer_LCD
 */

#include "IO_Explorer16_G13_v0.h"
#include "p24HJ256GP610A.h"
#include "LCD_Explorer16_G13_v0.h"
#include "Main_MISE_G13_v0.h"
#include "UART_G13_v0.h"

unsigned char Ventana_DATOS [n_filas][n_columnas]; //Array RAM
unsigned int flag147;

/* Patas utilizadas por el LCD en la Explorer 16
   RS -> RB15
   E  -> RD4
   RW -> RD5
   DATA -> RE0 - RE7   
*/

/*****	LCD  *****/
//===========================Envio de COMANDO===================================

void lcd_cmd( char cmd )                                                        //Subrutina para comandos
{                                                       
    //TRISE &= 0xFF00;                                                          //RE7-RE0 salida (//RE7-RE0 salida (lo inicializamos con los pines))
    RW = 0;                                                                     //Selecciona escritura
    RS = 0;                                                                     //Seleccionamos registro de comando
    DATA &= 0xFF00;                                                             //Prepara bits de dato RD0 - RD7
    DATA |= cmd;                                                                //Copia comando en los 8 bits de menos peso
    // Entre el cambio de RS y/o RW y E=! hay que esperar 60ns
    E = 1;                                                                      //Inicio de operacion E=1 (enable=1). Ha de permanecer un tiempo minimo
    Nop();                                                                      //Tiempo de espera, en teoria con un Nop() es suficiente (PONEMOS VARIOS PARA ASEGURAR TIEMPO SUFICIENTE PARA ACTUALIZAR LCD)
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    E = 0;                                                                      //Fin de operación (enable=0)
    RW = 1;                                                                     //Desactivamos escritura por seguridad
}

//=============================Envio de DATO====================================
void lcd_data( char data )                                                      //Subrutina para el envio de datos al LCD
{
   //TRISE &= 0xFF00;                                                           //RE7-RE0 salida (lo inicializamos con los pines)
    RW = 0;                                                                     //RW=0 para escritura
    RS = 1;                                                                     //Seleccionamos registro de datos
    DATA &= 0xFF00;                                                             //Prepara bits de dato RD0 - RD7
    DATA |= data;                                                               //Copia dato en los 8 bits de menos peso
    E = 1;                                                                      //Inicio de operacion E=1 (enable=1). Ha de permanecer un tiempo minimo
    Nop();                                                                      //Tiempo de espera, en teoria con un Nop() es suficiente (PONEMOS VARIOS PARA ASEGURAR TIEMPO SUFICIENTE PARA ACTUALIZAR LCD)
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    E = 0;                                                                      //Fin de operación (enable=0)
    RW = 1;                                                                     //Desactivamos escritura por seguridad
}

//======Genera la secuencia de comandos de inicialización del LCD display=======

void Inic_LCD( void )	    
{ 
    inic_timer_LCD();
    inic_pines_LCD();
    espera_LCD_ms(15);
    Nop();
    Nop();
    function_set();
    espera_LCD_ms(5);
    Nop();
    Nop();
    function_set();
    espera_LCD_us(100);
    Nop();
    Nop();
    function_set();
    espera_LCD_us(100);
    display_on();
    espera_LCD_us(100);
    entry_mode();
    espera_LCD_us(100);
} //FIN Init_LCD

//==========Inicialización del timer para actualizar el LCD=====================

void inic_timer_LCD (void)
{
    TMR_Timer_LCD = 0;                                                          // Reinicia el contador del timer
    if(Fosc==80000000) PR_Timer_LCD = (40000-1);                                // Define periodo (supongamos que está a 80MHz->40Mhz->cada 25ns), para contar 1ms=40000*25ns
    else PR_Timer_LCD = (4000-1);                                               // Define periodo (supongamos que está a 80MHz->40Mhz->cada 25ns), para contar 1ms=4000*250ns 
	TCON_Timer_LCD = 0x8000;                                                    // Habilitado, prescaler 1:1, CK interno
//  TCON_Timer_LCD = 0b1000000000000000;                                        // Equivalente a la linea anterior
	
// Inicializa interrupción Timer 8
//  _Timer_LCD_IP = 4;                                                          // Prioridad por defecto (1 minima - 6 máxima)
    _Timer_LCD_IF = 0;                                                          // Borra flag de interrupción
    _Timer_LCD_IE = 0;                                                          // Para trabajar por encuesta
}

//==================Espera a que pasen n milisegundos===========================

void espera_LCD_ms( unsigned int n_miliseg )
{
    int contador=0;
    TMR_Timer_LCD = 0;                                                          // Reiniciamos contador del timer
    if(Fosc==80000000) PR_Timer_LCD = (40000-1);                                // Configuramos para contar un ms (40000*25ns=1ms)
    else PR_Timer_LCD = (4000-1);                                               // Configuramos para contar un ms (4000*250ns=1ms)
    while(contador<=n_miliseg){                                                 // Hasta que pase la cantidad de milisegundos pasado a la funcion...
        while(_Timer_LCD_IF==0);                                                // Espera por bloqueado hasta que el timer termine
        contador++;
        _Timer_LCD_IF=0;
    }
    TMR_Timer_LCD = 0;
}

//==================Espera a que pasen n microsegundos==========================

void espera_LCD_us( unsigned int n_microseg )
{
    int contador=0;
    TMR_Timer_LCD = 0;                                                          // Reiniciamos contador del timer
    if(Fosc==80000000) PR_Timer_LCD = (40-1);                                   // Configuramos para contar un us (40*25ns=1us)
    else PR_Timer_LCD = (4-1);                                                  // Configuramos para contar un us (4*250ns=1us)
    while(contador<=n_microseg){                                                // Hasta que pase la cantidad de microsegundos pasado a la funcion...
        while(_Timer_LCD_IF==0);                                                // Espera por bloqueado hasta que el timer termine
        contador++;  
        _Timer_LCD_IF=0;
    }
    TMR_Timer_LCD = 0;                                                          // Reiniciamos contador del timer
}

//========================Inicialización pines LCD==============================

void inic_pines_LCD(void){
    // Pines de control LCD
    _TRISD5=0;                                                                  //Pin R/W como salida
    _TRISB15=0;                                                                 //Pin RS como salida                
    _TRISD4=0;                                                                  //Pin E como salida                 
    RW=0;                                                                       //Ponemos modo escritura por defecto
    RS=0;                                                                       //Ponemos registro comando por defecto
    E=0;                                                                        //Operacion en disable por defecto          
    DATA=0;                                                                     //Inicializar datos a 0
    TRISE &= 0xFF00;                                                            //Pines que van a la LCD en modo output
}

//===========Funcion que configura el timer para la maquina de estados==========

void inic_Maquina_Estado (void){
    TMR_Timer_LCD = 0;                                                          // Reinicia el contador del timer
    if(Fosc==80000000) PR_Timer_LCD = (58800-1);                                // Configuramos para contar un 1,47ms (58800*25ns=1,47ms)
    else PR_Timer_LCD = (5880-1);                                               // Configuramos para contar un 1,47ms (5880*250ns=1,47ms)
	TCON_Timer_LCD = 0x8000;                                                    // Habilitado, prescaler 1:1, CK interno
//  TCON_Timer_LCD = 0b1000000000000000;                                        // Equivalente a la linea anterior
	
// Inicializa interrupción Timer  
//  _Timer_LCD_IP = 4;                                                          // Prioridad por defecto (1 minima - 6 máxima)
    _Timer_LCD_IF = 0;                                                          // Borra flag de interrupción
    _Timer_LCD_IE = 1;                                                          // Para trabajar por interrupcion
}

//=====================Interrupcion del Timer_LCD===============================

void _ISR_NO_PSV _Timer_LCD_Interrupt( void)
{
   flag147=1;                                                                   //Flag que comprobaremos en el bucle principal  
   _Timer_LCD_IF=0;                                                             //Ponemos el flag de la interrupcion a cero
   TMR_Timer_LCD=0;                                                             //Reiniciamos el contador del timer
} // Fin Interrupcion
