/*
Proyecto: MISE Micros 16bits 
Fichero: Tiempos_G1_v0.c

Grupo: G1    Autores: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0

Descripción:
 * INICIALIZACIONES:
 * * Timer_mili que cuenta milisegundos. Usado para el calculo de CPU
 * * Timer_RA7 que cuenta milisegundos. Usado para la gestion de la pata RA7
 * * Timer_32 que cuenta 100 milisegundos. Usado para  alternar el valor de D9
 * 
 * RUTINAS DE ATENCION A INTERRUPCION
 * * _Timer_mili_interrupt. Rutina de atencion a la interrupcion de Timer_mili
 * * _Timer_RA7_interrupt. Rutina de atencion a la interrupcion de Timer_RA7
 * * _Timer_C_32_interrupt. Rutina de atencion a la interrupcion del timer tipo C del timer de 32 bits
 * 
 * Entrada: Frecuencia, Timer
 * Salida: short flagW, Led D9, pulsado_S5

 */

#include "p24hj256gp610A.h"
#include "Tiempos_G13_v0.h"
#include "Main_MISE_G13_v0.h"
#include "IO_Explorer16_G13_v0.h"
#include "UART_G13_v0.h"

short flagW,estado_led10;
unsigned long milisegundos;
unsigned int milisegRA7, cienmilis;
//============================Inicializaciones==================================

void Inic_Tiempo_Miliseg ()
{
	//Inicializamos Timer que cuenta milisegundos
    TMR_Timer_mili = 0;                                                         // Borra valor del timer
    if(Fosc==80000000) PR_Timer_mili = 200-1;//40000-1;//                                // Define periodo (supongamos que está a 80MHz->40Mhz->cada 25ns), para contar 1ms=40000*25ns 
    else PR_Timer_mili = 4000-1;                                                // Define periodo (supongamos que está a 80MHz->40Mhz->cada 25ns), para contar 1ms=4000*250ns 
	TCON_Timer_mili = 0x8000;                                                   // Habilitado, prescaler 1:1, CK interno
//  TCON_Timer_mili = 0b1000000000000000;                                       // Equivalente a la linea anterior
	
// Inicializa interrupción Timer 
//  _Timer_mili_IP = 4;                                                         // Prioridad por defecto (1 minima - 6 máxima)
    _Timer_mili_IF = 0;                                                         // Borra flag de interrupción
    _Timer_mili_IE = 1;                                                         // Habilita Interrupción del Timer 2
}

void Inic_Timer_32 ()
{
	//Inicializamos Timer de 32 bits
    TMR_Timer_B_32 = 0;                                                         // Borra valor del timer
    TMR_Timer_C_32 = 0;
    if(Fosc==80000000) {
      PR_Timer_B_32 = 2304-1;                                                   // Define periodo (supongamos que está a 80MHz->40Mhz->cada 25ns), para contar 100ms=4000000*25ns
      PR_Timer_C_32 = 61-1;                                                     // Define periodo (supongamos que está a 8MHz->4Mhz->cada 250ns), para contar 100ms=400000*250ns
    }
	else{
      PR_Timer_B_32 = 6784-1;
      PR_Timer_C_32 = 6-1;                                                      // Cuidado, controlamos los 16bits superiores
    }                                                                           
	TCON_Timer_B_32 = 0x8008;                                                   // Habilitado, prescaler 1:1, CK interno, T32 habilitado
    //TCON_Timer_B_32 = 0b1000000000001000;                                     // Equivalente a la linea anterior
	
// Inicializa interrupción Timer 
 // _Timer_C_32_IP = 4;                                                         // Prioridad por defecto (1 minima - 6 máxima)
    _Timer_C_32_IF = 0;                                                         // Borra flag de interrupción
    _Timer_C_32_IE = 1;                                                         // Habilita Interrupción del Timer 3
 
}

void Inic_Tiempo_RA7()
{
	//Inicializamos Timer para pata RA7 (S5,D10)
    TMR_Timer_RA7 = 0;                                                          // Borra valor del timer
    if(Fosc==80000000)PR_Timer_RA7 = (40000-1);                                  // Define periodo (supongamos que está a 80MHz->40Mhz->cada 25ns), para contar 1ms=40000*25ns
    else PR_Timer_RA7 = (4000-1);                                                // Define periodo (supongamos que está a 8MHz->4Mhz->cada 250ns), para contar 1ms=4000*250ns
	TCON_Timer_RA7 = 0x8000;	// Habilitado, prescaler 1:1, CK interno
//  TCON_Timer_RA7 = 0b1000000000000000;	// Equivalente a la linea anterior
	
// Inicializa interrupción Timer 
//  _Timer_RA7_IP = 4;  // Prioridad por defecto (1 minima - 6 máxima)
    _Timer_RA7_IF = 0;  // Borra flag de interrupción
    _Timer_RA7_IE = 1;  // Habilita Interrupción del Timer 2
}
//================Rutinas de atencion a la interrupcion=========================

void _ISR_NO_PSV _Timer_mili_Interrupt( void)
{
        /*************PWM************/
    static unsigned int contador_total,contador_HIGH;
    
    if(contador_total<100){
        contador_total++;
        if(contador_HIGH<INTENSIDAD){//selector_de_leds, carga_led1, carga_led2, carga_led3, carga_led4, carga_led5;
          
            if (contador_HIGH<=carga_led1 && ((selector_de_leds & 0x0010) == 0x0010) )LED_D3 = led_encendido;
            else LED_D3 = led_apagado;
            if (contador_HIGH<=carga_led2 && ((selector_de_leds & 0x0008) == 0x0008)) LED_D4 = led_encendido;
            else LED_D4 = led_apagado;
            if (contador_HIGH<=carga_led3 && ((selector_de_leds & 0x0004) == 0x0004)) LED_D5 = led_encendido;
            else LED_D5 = led_apagado;  
            if (contador_HIGH<=carga_led4 && ((selector_de_leds & 0x0002) == 0x0002)) LED_D6 = led_encendido;
            else LED_D6 = led_apagado; 
            if (contador_HIGH<=carga_led5 && ((selector_de_leds & 0x0001) == 0x0001)) LED_D7 = led_encendido;
            else LED_D7 = led_apagado; 

            
            contador_HIGH++;
           
        }
        //else //LED_D5 = led_apagado;
    }
    else {
        contador_total=0;
        contador_HIGH=0;
    }
    
     milisegundos++ ;    
     // se incrementa cada milisegundo
     if(milisegundos/340>=TCPU){                                                    //Cuando se hayan contado 10 ms activaremos el flagW que usamos para el calculo de CPU
         flagW=1;
         milisegundos=0;
     }
    _Timer_mili_IF = 0;                                                         // Borra flag de interrpción
} // Fin interrupcion

void _ISR_NO_PSV _Timer_C_32_Interrupt( void)
{   
    
    if (flag_timeout_enable == 1){
        cienmilis++ ; 
	 
        if(cienmilis>=300){                                                         //Cuando se haya contado 1 segundo invertiremos el valor del LED D9
             
            flag_timeout =1;
            flag_timeout_enable = 0;
             cienmilis=0;
         }
    }

    if (flag_5seg == 1){
        
        cienmilis++ ; 
	 
        if(cienmilis >= 50){                                                         //Cuando se haya contado 1 segundo invertiremos el valor del LED D9
             
            flag_5seg = 0;
            cienmilis=0;
            
         }
        
    }
     TMR_Timer_B_32 = 0;
     TMR_Timer_C_32 = 0;
    _Timer_C_32_IF = 0;                                                         // Borra flag de interrupción
} // Fin interrupcion

void _ISR_NO_PSV _Timer_RA7_Interrupt( void)
{

    
     milisegRA7++ ;                                                             // se incrementa cada milisegundo
     if(milisegRA7>=50){                                                        //Cuando se hayan contado 50 milisegundo invertiremos el valor del LED D3
        estado_led10=LED_D10;                                                   //Guardamos el estado anterior del LED_D10
        TRIS_PULSADOR_S5= 1;                                                    //Convertimos RA7 en entrada
        if(Fosc==80000000){ Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();    //Espera necesaria para que el cambio de modo se haga efectivo
                            Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
                            Nop();Nop();Nop();Nop();}
        else{Nop();Nop();Nop();}
        if (PULSADOR_S5 == 1)                                                   //Evaluamos el pulsador S5
        { 
            Pulsado_S5=0;                                                       //Y guardamos su estado en un flag que consultamos en el bucle principal
        }
        if (PULSADOR_S5 == 0)
        {
            Pulsado_S5=1;
        }
        TRIS_PULSADOR_S5= 0;                                                    //Volvemos a convertir a RA7 en salida
        milisegRA7=0;
        LED_D10=estado_led10;                                                   //Devolvemos el estado de D10
     }
     TMR_Timer_RA7 = 0;
     _Timer_RA7_IF = 0;                                                         // Borra flag de interrpción
} // Fin interrupcion


