/*
Proyecto: MISE Micros 16bits 
Fichero: Tiempos.c

Grupo: G5    Autor: Iñigo Olaso y Xabi Olazar
v0.0	Fecha: 2017-X-15

Descripción:
 * Programa un Timer para que actualice una variable y/o 
 * genere una interrupción cada cierto tiempo
 * Hay que darle la frecuencia de reloj y el Nº de timer
 *
 * Entrada: Frecuencia, Timer
 * Salida: int microsegundos

 */

#include "p24hj256gp610A.h"
#include "Tiempos.h"
#include "LCD_Explorer16_Gx_vx.h"
#include "main.h"

    unsigned int milisegundos = 0;


//====================================================================================================


//void Inic_Tiempo_Miliseg (mili)
void Inic_Tiempo_Miliseg ()
{
	TMR_Timer_mili = 0;		// Borra valor del timer
    if(flag_osc_80) PR_Timer_mili = 40000-1;	// Define periodo
    else PR_Timer_mili = 4000-1;	// Define periodo
	TCON_Timer_mili = 0x8000;	// Habilitado, prescaler 1:1, CK interno
//  TCON_Timer_mili = 0b1000000000000000;	// Equivalente a la linea anterior
	
// Inicializa interrupción Timer 2
//  _Timer_mili_IP = 4;  // Prioridad por defecto (1 minima - 6 máxima)
    _T3IF = 0; // Borra flag de interrupción
    _T3IE = 0; // Habilita Interrupción del Timer 2
    _T8IF = 0; // Borra flag de interrupción
    _T8IE = 0; // Habilita Interrupción del Timer 2
    
    _Timer_mili_IF = 0;  // Borra flag de interrupción
    _Timer_mili_IE = 1;  // Habilita Interrupción del Timer 2
}

void Inic_Timer1 ()
{
    TMR1 = 0;
    
    if(flag_osc_80)PR1 = 120000-1;
    else  PR1 = 12000-1;
    
    T1CON = 0x8000;

    _T1IF = 0; // Borra flag de interrupción
    _T1IE = 0; // Habilita Interrupción del Timer 2
    

}

void Inic_Timer8 ()
{
    TMR8 = 0;
    
    if(flag_osc_80)PR8 = 40000-1;
    else  PR8 = 4000-1;
    
    T8CON = 0x8000;
    
    _T3IF = 0; // Borra flag de interrupción
    _T3IE = 0; // Habilita Interrupción del Timer 2
    _Timer_mili_IF = 0;  // Borra flag de interrupción
    _Timer_mili_IE = 0;  // Habilita Interrupción del Timer 2
    
    _T8IF = 0; // Borra flag de interrupción
    _T8IE = 1; // Habilita Interrupción del Timer 2
    

}

void Inic_Timer_32() {
    TMR2 = 0;
    TMR3 = 0;
    
    if(!flag_osc_80){
    PR2 = 40000;
    PR3 = 0;
    }
    else if(flag_osc_80){
    PR2 = 0x1a80;
    PR3 = 0x0006;
    }
    T2CON = 0x8008;
    
    _T8IF = 0; // Borra flag de interrupción
    _T8IE = 0; // Habilita Interrupción del Timer 2
    _Timer_mili_IF = 0;  // Borra flag de interrupción
    _Timer_mili_IE = 0;  // Habilita Interrupción del Timer 2
    
    _T3IF = 0; // Borra flag de interrupción
    _T3IE = 1; // Habilita Interrupción del Timer 2
    


}


void _ISR_NO_PSV _T8Interrupt( void){

    static int flag_fila=0;
    static int posiciones=0;
    
    
        milisegundos++ ;    // se incrementa cada milisegundo
        leds_counter++;
        
    _Timer_mili_IF = 0;	// Borra flag de interrpción
    
    if( milisegundos >= 50)//Init=1 &&
    {


        if(flag_fila==0 && posiciones==0){
            inicio_fila_1();
            posiciones++;
            flag_fila++;
        }
        else if(flag_fila==1 && posiciones !=0){
            
            switch (posiciones){
                        
                    case 6:                      
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[4][14];
                        break;
                   
                    case 7:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[4][15];
                        break;
                        
                    case 14:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][13];
                        break;
                        
                    case 15:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][14];
                        break;
                        
                    case 16:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][15];
                        break;                        
                
                    default:
                                break;
                        
            }
            lcd_data(Ventana_LCD[0][posiciones-1]);
            posiciones++;
            
                if(posiciones==17){
                posiciones=0;
                flag_fila++;
            }
        }
        else if(flag_fila==2 && posiciones==0){
            inicio_fila_2();
            posiciones++;
            flag_fila++;

        }
        else if(flag_fila==3 && posiciones !=0){
            
            switch (posiciones){
                        
                    case 9:                      
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][5];
                        break;
                   
                    case 10:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][6];
                        break;
                        
                    case 11:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][7];
                        break;
                        
                    
                        
                    case 13:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][9];
                        break;
                        
                    case 14:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][10];
                        break;
                        
                    case 15:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][11];
                        break;
                        default:
                                break;
                        
            }
            lcd_data(Ventana_LCD[1][posiciones-1]);
            posiciones++;

            if(posiciones==17){
                posiciones=0;
                flag_fila=0;
            }
        }
 
    milisegundos=0;

    }
}

//========================================================

void _ISR_NO_PSV _T3Interrupt( void){

    static int flag_fila=0;
    static int posiciones=0;
    
    
        milisegundos++ ;    // se incrementa cada milisegundo
        leds_counter++;
        
    _Timer_mili_IF = 0;	// Borra flag de interrpción
    
    if( milisegundos >= 50)//Init=1 &&
    {


        if(flag_fila==0 && posiciones==0){
            inicio_fila_1();
            posiciones++;
            flag_fila++;
        }
        else if(flag_fila==1 && posiciones !=0){
            
            switch (posiciones){
                        
                    case 6:                      
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[4][14];
                        break;
                   
                    case 7:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[4][15];
                        break;
                        
                    case 14:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][13];
                        break;
                        
                    case 15:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][14];
                        break;
                        
                    case 16:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][15];
                        break;                        
                
                    default:
                                break;
                        
            }
            lcd_data(Ventana_LCD[0][posiciones-1]);
            posiciones++;
            
                if(posiciones==17){
                posiciones=0;
                flag_fila++;
            }
        }
        else if(flag_fila==2 && posiciones==0){
            inicio_fila_2();
            posiciones++;
            flag_fila++;

        }
        else if(flag_fila==3 && posiciones !=0){
            
            switch (posiciones){
                        
                    case 9:                      
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][5];
                        break;
                   
                    case 10:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][6];
                        break;
                        
                    case 11:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][7];
                        break;
                        
                    
                        
                    case 13:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][9];
                        break;
                        
                    case 14:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][10];
                        break;
                        
                    case 15:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][11];
                        break;
                        default:
                                break;
                        
            }
            lcd_data(Ventana_LCD[1][posiciones-1]);
            posiciones++;

            if(posiciones==17){
                posiciones=0;
                flag_fila=0;
            }
        }
 
    milisegundos=0;

    }
    
}
//========================================================
void _ISR_NO_PSV _Timer_mili_Interrupt( void)
{
    static int flag_fila=0;
    static int posiciones=0;
    
    
        milisegundos++ ;    // se incrementa cada milisegundo
        leds_counter++;
        
    _Timer_mili_IF = 0;	// Borra flag de interrpción
    
    if( milisegundos >= 50)//Init=1 &&
    {


        if(flag_fila==0 && posiciones==0){
            inicio_fila_1();
            posiciones++;
            flag_fila++;
        }
        else if(flag_fila==1 && posiciones !=0){
            
            switch (posiciones){
                        
                    case 6:                      
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[4][14];
                        break;
                   
                    case 7:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[4][15];
                        break;
                        
                    case 14:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][13];
                        break;
                        
                    case 15:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][14];
                        break;
                        
                    case 16:
                        Ventana_LCD[0][posiciones-1]=Ventana_Datos[3][15];
                        break;                        
                
                    default:
                                break;
                        
            }
            lcd_data(Ventana_LCD[0][posiciones-1]);
            posiciones++;
            
                if(posiciones==17){
                posiciones=0;
                flag_fila++;
            }
        }
        else if(flag_fila==2 && posiciones==0){
            inicio_fila_2();
            posiciones++;
            flag_fila++;

        }
        else if(flag_fila==3 && posiciones !=0){
            
            switch (posiciones){
                        
                    case 9:                      
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][5];
                        break;
                   
                    case 10:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][6];
                        break;
                        
                    case 11:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][7];
                        break;
                        
                    
                        
                    case 13:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][9];
                        break;
                        
                    case 14:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][10];
                        break;
                        
                    case 15:
                        Ventana_LCD[1][posiciones-1]=Ventana_Datos[1][11];
                        break;
                        default:
                                break;
                        
            }
            lcd_data(Ventana_LCD[1][posiciones-1]);
            posiciones++;

            if(posiciones==17){
                posiciones=0;
                flag_fila=0;
            }
        }
 
    milisegundos=0;

    }
    
} // Fin T1Interrupt


                                    