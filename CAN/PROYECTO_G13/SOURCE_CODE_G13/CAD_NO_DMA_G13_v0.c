/*
Proyecto: MOISE Micros 16bits 2017
Fichero: CAD_NO_DMA_G1_v0.c

Grupo: G1    Autores: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0
 *

v0: 
Descripcion:
 * Configuramos el ADC1 para muestrear AN8,AN9,AN4,AN5, por canal CH0
 * Rutina de atencion a la interrupcion del ADC
 */
 


#include "p24hj256gp610A.h"
#include "CAD_NO_DMA_G13_v0.h"
#include "Main_MISE_G13_v0.h"

int ADCValue;
short secuenciaCAD;

//======CONFIGURAMOS EL ADC1 PARA MUESTREAR AN8,AN9,AN4,AN5, por canal CH0====== 

void Inic_CAD (void)    
{ 
  AD1CON1= 0;                                                                   // 0x00E0 
    //_ADON= 0;                                                                 // CAD OFF 
    //_ADSIDL = 0;                                                              //1=Se para en modo Idle / 0=Continua en modo Idle 
    //_ADDMABM = 0;                                                             //1=bufer DMA en orden de conversion 
                                                                                // 0=bufer DMAen modo Scatter/Gather 
    //_AD12B = 0;                                                               //1=Modo operacion de 12 bits / 0=Modo operacion de 10 bits  
    //_FORM = 0;                                                                //Formato datos salida: 00=Integer (0000 00dd dddd dddd) 
                                                                                // 01=Signed integer (ssss sssd dddd dddd) 
 _SSRC = 0b111;                                                                 //Termina muestreo y comienza conversion del CAD 
                                                                                    //111=Auto-convert / 110, 101, 100, 011 RESERVADOS 
                                                                                    //010=TMR3  ADC1 y TMR5 ADC2 
                                                                                    //001=INT0  
                                                                                    //000= hay que borrar SAMP 
    //_SIMSAM = 0;                                                              //1=Muestrea simultaneamente 
                                                                                // 0=Muestrea canales individuales secuencialmente 
    //_ASAM = 1;                                                                //1=Muestreo comienza seguido de la conversion 
                                                                                // 0=comienza cuando SAMP=1 
    //_SAMP  
    //_DONE 
AD1CON2= 0; 
    //_VCFG = 0;                                                                //Vref+? 000= AVdd y AVss  
                                                                                // 001=Vref+ y AVss 
                                                                                // 010=AVdd y Vref?  
                                                                                // 011=Vref+ y Vref? 
    //_CSCNA = 0;                                                               //1=Escaneo canal cero / 0= no escaneo 
    //_CHPS = 0;                                                                //Canales utilizados:00=CH0 /01=CH0 y CH1 /1x= CH0,CH1,CH2 y CH3 
                                                                                //_BUFS  es solo de lectura: con BUFM=1?> 1=activa 2� mitad de buffer 
                                                                                // 0= activa 1� mitad 
    //_SMPI = 0;                                                                //Incrementos dir DMA +1 o 
                                                                                // sin DMA genera una interrupci�n cada cada SMPI+1 
    //_BUFM = 0;                                                                //1= 2 medios Buffers de 8 / 0= 1 buffer de 16 
    //_ALTS = 0;                                                                //Modo de muestreo: 1= Alternado / 0= Siempre canal A 
    //AD1CON3
_ADRC = 0;                                                                      //1= reloj RC / 0= Reloj CPU 
_SAMC = 31;                                                                     //Tiempo auto muestreo0 n�de Tad (de 0 a 31Tad) 
_ADCS = 3;                                                                      //Tad= >75ns = Tcy(ADCS+1) ?> a 80MHz Tcy= 25ns
                             
AD1CON4= 0; 
    //_DMABL = 3;                                                               //Cada buffer contiene 8 palabras  
AD1CHS123= 0;                                                                   //reg. seleccion de entrada canal 1,2 y 3  
    //_CH123NA= 0;                                                              //Seleccion entrada negativa Canales 1, 2, y 3 pata Sample A  
                                                                                    //0x= CH1=CH2=CH3?> Vref? / 10= CH1=AN6,CH2=AN7,CH3=AN8 
                                                                                    // 11= CH1=AN9,CH2=AN10,CH3=AN11 
    //_CH123SA= 0;                                                              //Seleccion entrada positiva Canales 1, 2, y 3 para Sample A 
                                                                                    //1= AN3,AN4,AN5 / 0= AN0,AN1,AN2 
    //_CH123NB = 0; 
    //_CH123SB = 0; 
AD1CHS0= 0;                                                                     //Registro de seleccion de entrada canal 0 
    //_CH0NA = 0;                                                               //Entrada negativa 1=  AN1 / 0 = Vref? 
_CH0SA = 5;                                                                     //Entrada positiva = canal(CH0SA) Potenciometro -> A5
    //_CH0SB = 3; 
    //_CH0NB = 0;  
AD1CSSH = 0x0000;                                                               //Seleccion entradas escaneo de la 16 a la 31
AD1CSSL = 0x0000;                                                               //Seleccion entradas escaneo de 0 a 15.  
                                                                                    // 1 =selecciona para escaneo / no escanea 
AD1PCFGH = 0xFFFF;                                                              //Configura entradas: 1= digital / 0= Analog 
AD1PCFGL = 0xFFFF; 
    //_PCFG0=          
_PCFG4= 0;                                                                      // AN4 -> RB4 -> Sensor Temperatura 
_PCFG5= 0;                                                                      // AN5 -> RB5 -> Potenciometro 
_PCFG8=0;                                                                       // AN8 -> RB8 -> Joystick Y 
_PCFG9=0;                                                                       // AN9 -> RB9 -> Joystick X
    //IFS0 
_AD1IF = 0;                                                                     //Flag a cero del A/D interrrupt. 
    //IEC0 
_AD1IE = 0;                                                                     // Habilita la interrupcion A/D.  
    //AD1CON1bits 
_ADON = 1;                                                                      //Comienzo de conversion 
_SAMP= 1; 
} // Fin Inic_CAD

//================Rutina de atencion a la interrupcion del ADC==================

void _ISR _ADC1Interrupt(void)                                                  //Atencion a la interrupcion de fin de conversion	
{	
    if (secuenciaCAD>3) secuenciaCAD=0;	
        switch (secuenciaCAD)	
		{	
			case 0:                                                             //Primer caso de la secuencia	
				ADCValue=	ADC1BUF0;                                           //Lee	el	valor	de	la	conversion	
				_CH0SA	=	4;                                                  //Seleccionamos la entrada del sensor de temperatura (para el siguiente ciclo)	
                //bits_to_ASCII (secuenciaCAD, ADCValue);                       //Convierte el valor en algo enviable 
                int_to_ASCII (secuenciaCAD, ADCValue);
                //_SAMP= 1;                                                     //Comienzo	de	conversion	
				secuenciaCAD++;	
			break;
            case 1:                                                             //Segundo caso de la secuencia
				ADCValue=	ADC1BUF0;                                           //Lee	el	valor	de	la	conversion	
				_CH0SA	=	9;                                                  //Seleccionamos la entrada del Joystick X
                bits_to_ASCII (secuenciaCAD, ADCValue);                         //Convierte el valor en algo enviable
                //int_to_ASCII (secuenciaCAD, ADCValue);
                //_SAMP= 1;                                                     //Comienzo	de	conversion	
				secuenciaCAD++;	
			break;
            case 2:                                                             //Tercer caso de la secuencia
				ADCValue=	ADC1BUF0;                                           //Lee el valor de la conversion	
				_CH0SA	=	8;                                                  //Seleccionamos la entrada del Joystick Y
                //bits_to_ASCII (secuenciaCAD, ADCValue);                       //Convierte el valor en algo enviable
                int_to_ASCII (secuenciaCAD, ADCValue);
                //_SAMP= 1;                                                     //Comienzo	de	conversion	
				secuenciaCAD++;	
			break;
            case 3:                                                             //Cuarto caso de la secuencia
				ADCValue=	ADC1BUF0;                                           //	Lee el valor de la conversion	
				_CH0SA	=	5;                                                  //Seleccionamos la entrada potenciometro
                //bits_to_ASCII (secuenciaCAD, ADCValue);                       //Convierte el valor en algo enviable
                int_to_ASCII (secuenciaCAD, ADCValue);
                //_SAMP= 1;                                                     //Comienzo	de	conversion	
				secuenciaCAD++;	
			break;
            default:
            break;
	 }
	_SAMP=	1;                                                                  //Comienzo	de	conversion	
    _AD1IF	=	0;                                                              //Flag a cero del A/D interrrupt.	
}	//	FIN	ADC1Interrupt