/**********************************************************************
* © 2017 LG
*
 Proyecto: MISE Micros 16bits 2017
* Fichero: Tiempos_G1_v0.h

Grupo: G1    Autor: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-20
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

#include "Main_MISE_G13_v0.h"
#include "IO_Explorer16_G13_v0.h"
#define Timer_mili 8 // Define el nº Timer a usar
#define Timer_RA7 6
#define Timer_B_32 2
#define Timer_C_32 3
#define TCPU 10                                                                 //ms que usaremos para el calculo de CPU
extern short flagW;

#if (Timer_mili == 1)
#define TMR_Timer_mili  TMR1
#define PR_Timer_mili   PR1
#define TCON_Timer_mili T1CON

#define _Timer_mili_Interrupt _T1Interrupt
#define _Timer_mili_IF _T1IF
#define _Timer_mili_IP _T1IP
#define _Timer_mili_IE _T1IE

#elif(Timer_mili==2)
#define TMR_Timer_mili  TMR2
#define PR_Timer_mili   PR2
#define TCON_Timer_mili T2CON

#define _Timer_mili_Interrupt _T2Interrupt
#define _Timer_mili_IF _T2IF
#define _Timer_mili_IP _T2IP
#define _Timer_mili_IE _T2IE

#elif (Timer_mili == 3)
#define TMR_Timer_mili  TMR3
#define PR_Timer_mili   PR3
#define TCON_Timer_mili T3CON

#define _Timer_mili_Interrupt _T3Interrupt
#define _Timer_mili_IF _T3IF
#define _Timer_mili_IP _T3IP
#define _Timer_mili_IE _T3IE

#elif (Timer_mili == 4)
#define TMR_Timer_mili  TMR4
#define PR_Timer_mili   PR4
#define TCON_Timer_mili T4CON

#define _Timer_mili_Interrupt _T4Interrupt
#define _Timer_mili_IF _T4IF
#define _Timer_mili_IP _T4IP
#define _Timer_mili_IE _T4IE

#elif (Timer_mili == 5)
#define TMR_Timer_mili  TMR5
#define PR_Timer_mili   PR5
#define TCON_Timer_mili T5CON

#define _Timer_mili_Interrupt _T5Interrupt
#define _Timer_mili_IF _T5IF
#define _Timer_mili_IP _T5IP
#define _Timer_mili_IE _T5IE

#elif (Timer_mili == 6)
#define TMR_Timer_mili  TMR6
#define PR_Timer_mili   PR6
#define TCON_Timer_mili T6CON

#define _Timer_mili_Interrupt _T6Interrupt
#define _Timer_mili_IF _T6IF
#define _Timer_mili_IP _T6IP
#define _Timer_mili_IE _T6IE

#elif (Timer_mili == 7)
#define TMR_Timer_mili  TMR7
#define PR_Timer_mili   PR7
#define TCON_Timer_mili T7CON

#define _Timer_mili_Interrupt _T7Interrupt
#define _Timer_mili_IF _T7IF
#define _Timer_mili_IP _T7IP
#define _Timer_mili_IE _T7IE

#elif(Timer_mili==8)

#define TMR_Timer_mili  TMR8
#define PR_Timer_mili   PR8
#define TCON_Timer_mili T8CON

#define _Timer_mili_Interrupt _T8Interrupt
#define _Timer_mili_IF _T8IF
#define _Timer_mili_IP _T8IP
#define _Timer_mili_IE _T8IE

#elif (Timer_mili == 9)

#define TMR_Timer_mili  TMR9
#define PR_Timer_mili   PR9
#define TCON_Timer_mili T9CON

#define _Timer_mili_Interrupt _T9Interrupt
#define _Timer_mili_IF _T9IF
#define _Timer_mili_IP _T9IP
#define _Timer_mili_IE _T9IE

#elif(Timer_mili==0||Timer_mili>9)
#error ("Timer_mili NO DEFINIDO")
#endif

#if (Timer_RA7 == 1)
#define TMR_Timer_RA7  TMR1
#define PR_Timer_RA7   PR1
#define TCON_Timer_RA7 T1CON

#define _Timer_RA7_Interrupt _T1Interrupt
#define _Timer_RA7_IF _T1IF
#define _Timer_RA7_IP _T1IP
#define _Timer_RA7_IE _T1IE

#elif(Timer_RA7==2)
#define TMR_Timer_RA7  TMR2
#define PR_Timer_RA7   PR2
#define TCON_Timer_RA7 T2CON

#define _Timer_RA7_Interrupt _T2Interrupt
#define _Timer_RA7_IF _T2IF
#define _Timer_RA7_IP _T2IP
#define _Timer_RA7_IE _T2IE

#elif (Timer_RA7 == 3)
#define TMR_Timer_RA7  TMR3
#define PR_Timer_RA7   PR3
#define TCON_Timer_RA7 T3CON

#define _Timer_RA7_Interrupt _T3Interrupt
#define _Timer_RA7_IF _T3IF
#define _Timer_RA7_IP _T3IP
#define _Timer_RA7_IE _T3IE

#elif (Timer_RA7 == 4)
#define TMR_Timer_RA7  TMR4
#define PR_Timer_RA7   PR4
#define TCON_Timer_RA7 T4CON

#define _Timer_RA7_Interrupt _T4Interrupt
#define _Timer_RA7_IF _T4IF
#define _Timer_RA7_IP _T4IP
#define _Timer_RA7_IE _T4IE

#elif (Timer_RA7 == 5)
#define TMR_Timer_RA7  TMR5
#define PR_Timer_RA7   PR5
#define TCON_Timer_RA7 T5CON

#define _Timer_RA7_Interrupt _T5Interrupt
#define _Timer_RA7_IF _T5IF
#define _Timer_RA7_IP _T5IP
#define _Timer_RA7_IE _T5IE

#elif (Timer_RA7 == 6)
#define TMR_Timer_RA7  TMR6
#define PR_Timer_RA7   PR6
#define TCON_Timer_RA7 T6CON

#define _Timer_RA7_Interrupt _T6Interrupt
#define _Timer_RA7_IF _T6IF
#define _Timer_RA7_IP _T6IP
#define _Timer_RA7_IE _T6IE

#elif (Timer_RA7 == 7)
#define TMR_Timer_RA7  TMR7
#define PR_Timer_RA7   PR7
#define TCON_Timer_RA7 T7CON

#define _Timer_RA7_Interrupt _T7Interrupt
#define _Timer_RA7_IF _T7IF
#define _Timer_RA7_IP _T7IP
#define _Timer_RA7_IE _T7IE

#elif(Timer_RA7==8)

#define TMR_Timer_RA7  TMR8
#define PR_Timer_RA7   PR8
#define TCON_Timer_RA7 T8CON

#define _Timer_RA7_Interrupt _T8Interrupt
#define _Timer_RA7_IF _T8IF
#define _Timer_RA7_IP _T8IP
#define _Timer_RA7_IE _T8IE

#elif (Timer_RA7 == 9)

#define TMR_Timer_RA7  TMR9
#define PR_Timer_RA7   PR9
#define TCON_Timer_RA7 T9CON

#define _Timer_RA7_Interrupt _T9Interrupt
#define _Timer_RA7_IF _T9IF
#define _Timer_RA7_IP _T9IP
#define _Timer_RA7_IE _T9IE

#elif(Timer_RA7==0||Timer_RA7>9)
#error ("Timer_RA7 NO DEFINIDO")
#endif


#if(Timer_B_32==2)
#define TMR_Timer_B_32  TMR2
#define PR_Timer_B_32   PR2
#define TCON_Timer_B_32 T2CON

#define _Timer_B_32_Interrupt _T2Interrupt
#define _Timer_B_32_IF _T2IF
#define _Timer_B_32_IP _T2IP
#define _Timer_B_32_IE _T2IE


#elif (Timer_B_32 == 4)
#define TMR_Timer_B_32  TMR4
#define PR_Timer_B_32   PR4
#define TCON_Timer_B_32 T4CON

#define _Timer_B_32_Interrupt _T4Interrupt
#define _Timer_B_32_IF _T4IF
#define _Timer_B_32_IP _T4IP
#define _Timer_B_32_IE _T4IE



#elif (Timer_B_32 == 6)
#define TMR_Timer_B_32  TMR6
#define PR_Timer_B_32   PR6
#define TCON_Timer_B_32 T6CON

#define _Timer_B_32_Interrupt _T6Interrupt
#define _Timer_B_32_IF _T6IF
#define _Timer_B_32_IP _T6IP
#define _Timer_B_32_IE _T6IE



#elif(Timer_B_32==8)

#define TMR_Timer_B_32  TMR8
#define PR_Timer_B_32   PR8
#define TCON_Timer_B_32 T8CON

#define _Timer_B_32_Interrupt _T8Interrupt
#define _Timer_B_32_IF _T8IF
#define _Timer_B_32_IP _T8IP
#define _Timer_B_32_IE _T8IE



#elif(Timer_B_32==1||Timer_B_32==3||Timer_B_32==5||Timer_B_32==7||Timer_B_32>=9)
#error ("Timer_B_32 NO DEFINIDO")
#endif

#if(Timer_C_32==3)
#define TMR_Timer_C_32  TMR3
#define PR_Timer_C_32   PR3
#define TCON_Timer_C_32 T3CON

#define _Timer_C_32_Interrupt _T3Interrupt
#define _Timer_C_32_IF _T3IF
#define _Timer_C_32_IP _T3IP
#define _Timer_C_32_IE _T3IE

#elif (Timer_C_32 == 5)
#define TMR_Timer_C_32  TMR5
#define PR_Timer_C_32   PR5
#define TCON_Timer_C_32 T5CON

#define _Timer_C_32_Interrupt _T5Interrupt
#define _Timer_C_32_IF _T5IF
#define _Timer_C_32_IP _T5IP
#define _Timer_C_32_IE _T5IE



#elif (Timer_C_32 == 7)
#define TMR_Timer_C_32  TMR7
#define PR_Timer_C_32   PR7
#define TCON_Timer_C_32 T7CON

#define _Timer_C_32_Interrupt _T7Interrupt
#define _Timer_C_32_IF _T7IF
#define _Timer_C_32_IP _T7IP
#define _Timer_C_32_IE _T7IE



#elif(Timer_C_32==9)

#define TMR_Timer_C_32  TMR9
#define PR_Timer_C_32   PR9
#define TCON_Timer_C_32 T9CON

#define _Timer_C_32_Interrupt _T9Interrupt
#define _Timer_C_32_IF _T9IF
#define _Timer_C_32_IP _T9IP
#define _Timer_C_32_IE _T9IE

#elif(Timer_C_32==1||Timer_C_32==2||Timer_C_32==4||Timer_C_32==6||Timer_C_32==8||Timer_C_32>9)
#error ("Timer_C_32 NO DEFINIDO")
#endif
//===========================================
extern unsigned int milisegRA7, dosmilisegundos, cincomilisegundos, diezmilisegundos;
extern short flagRA7;
