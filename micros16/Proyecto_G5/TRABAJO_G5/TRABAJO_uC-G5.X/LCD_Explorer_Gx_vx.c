/**********************************************************************
* © 2013 LG
*
* Proyecto: MOISE Micros 16bits 2014
* Fichero: LCD_Explorer16_24H_v2.C

Grupo: G0    Autor: Luis
	Fecha: 2014-X-15
 *  v2
 *
* Dependencies:    none
* Processor:       PIC24H
* Compiler:        
*
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Luis   2014-X-15  LCD 2x16 Explorer 16
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES: Modificacioes a partir de lcd.c y 24H_LCD_Int.c
*
**********************************************************************/

#include "p24HJ256GP610A.h"
#include "LCD_Explorer16_Gx_vx.h"
//#include --

//#include "24H_LCD_Int.h"
//#include "delay_L.h"
//#include "Main_MISE_G0_V0.h"
#include "Tiempos.h"


/* Patas utilizadas por el LCD en la Explorer 16
   RS -> R--
   E  -> R--
   RW -> R--
   DATA -> RE-- - RE--   
*/

/*****	LCD  *****/
//========= Envio de COMANDO =========
// Es bloqueante, espera a que termine el tiempor de TMR4 lo reprograma y
// al salir recupera la configuracion anterior de TMR4 y la interrupcion
void lcd_cmd( char cmd )          // subrutina para comandos
{
    DATA &= 0xFF00;               // prepare RD0 - RD7
    DATA |= cmd;                  // command byte to lcd
    RW = 0;                       // ensure RW is 0
    RS = 0;
    E = 1;                        // toggle E line
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();

    E = 0;
    RW = 1;
}

//======= Envio de DATO ===============
void lcd_data( char data )        // subroutine for lcd data
{
    RW = 0;       				 // ensure RW is 0
    RS = 1;                       // assert register select to 1
    DATA &= 0xFF00;               // prepare RD0 - RD7
    DATA |= data;                 // data byte to lcd
    E = 1;
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop(); 
    E = 0;                       // toggle E signal
    RW = 1;
    //    RS = 0;                      // negate register select to 0
}    

// Inicialización de patas
void init_Pines_LCD(){
    
    LATE &= 0xFF00;
    RW = 0;	    // R/W state set low
    RS = 0;	    // RS state set low
    E = 0;	    // E state set low

    /* set data and control pins to outputs */
    TRISE &= 0xFF00;
    RW_TRIS = 0;    // RW pin set as output
    RS_TRIS = 0;    // RS pin set as output
    E_TRIS = 0;	    // E pin set as output

}
void espera_LCD_ms( unsigned int n_miliseg ){
    
    Inic_Tiempo_Miliseg ();
    while( milisegundos != n_miliseg);
    milisegundos=0;
     //_Timer_mili_IF = 1;
     _Timer_mili_IE = 0;

}

// Genera sa secuencia de comandos de inicialización del LCD display
void Inic_LCD( void )	    
{
    //Init=0;
    
    init_Pines_LCD();// Inicialización de patas
    
    espera_LCD_ms( 15 ); //espera 15ms a que alimentación del LCD se estabilice
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    function_set();     // Primer Comando
    espera_LCD_ms( 5 );	//Bucle de 5 ms bloqueante
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    function_set();     // Repite comando
    espera_LCD_ms( 5 );
    //espera_LCD_us( 100 );	//Bucle de 40us bloqueante
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    function_set();     // Repite comando
    espera_LCD_ms( 5 );
    //espera_LCD_us( 100 );	//Bucle de 15 ms bloqueante
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    display_on();       // Display on/off control, cursor blink off (0x0C)
    
    espera_LCD_ms( 5 );
    //espera_LCD_us( 100 );	//Bucle de 40us bloqueante
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    entry_mode();       // entry mode set (0x06)
    
    espera_LCD_ms( 5 );
    //espera_LCD_us( 100 );	//Bucle de 40us bloqueante
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
   // genera dos caracteres nuevos
 //  setLCDG(0);
 /*   lcd_cmd(0x40);
    lcd_cmd( 0 & 0x3F | 0x40 );
    lcd_data (0b00010);
    lcd_data ( 0b00010);
    lcd_data ( 0b00110);
    lcd_data ( 0b11111);
    lcd_data ( 0b00110);
    lcd_data ( 0b00010);
    lcd_data ( 0b00010);
    lcd_data ( 0);     // alignment

    lcd_data ( 0b00000);
    lcd_data ( 0b00100);
    lcd_data ( 0b01100);
    lcd_data ( 0b11100);
    lcd_data ( 0b00000);
    lcd_data ( 0b00000);
    lcd_data ( 0b00000);
    lcd_data ( 0);     // alignment
  */
Nop();
Nop();  
Nop();
Nop();  
Nop();
Nop();  
Nop();
Nop();  

     _Timer_mili_IE = 1;
     //Init=1;

} //FIN Init_LCD

//================================================
void inic_timer_LCD (void)
{
Nop();
Nop();  
}


// Eespera a que pasen n microsegundos
void espera_LCD_us( unsigned int n_microseg )
{
Nop();
Nop();
}

