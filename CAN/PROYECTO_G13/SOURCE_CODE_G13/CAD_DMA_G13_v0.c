/*
Proyecto: MOISE Micros 16bits 2017
Fichero: CAD_DMA_G1_v0.c

Grupo: G1    Autores: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0
 *

v0: 
Descripcion:
 * Inicializacion CAD y DMA para el CAD
 * Rutina de atencion a la interrupcion de la DMA
 */

#include "p24HJ256GP610A.h"
#include "CAD_DMA_G13_v0.h"


int CAD_BuffA[TAMBUF] __attribute__((space(dma)));

//====================Inicializacion CAD y DMA para el CAD====================== 

void Inic_CAD_DMA(void) 
{ 
AD1CON1= 0;                                                                     // 0x00E0 
    //_ADON= 0;                                                                 // CAD OFF 
    //_ADSIDL = 0;                                                              //1=Se para en modo Idle / 0=Continua en modo Idle 

    //_AD12B = 0;                                                               //1=Modo operacion de 12 bits / 0=Modo operacion de 10 bits  
    //_FORM = 0;                                                                //Formato datos salida: 00=Integer (0000 00dd dddd dddd) 
                                                                                // 01=Signed integer (ssss sssd dddd dddd) 
_SSRC = 0b111;                                                                  //Termina muestreo y comienca conversion del CAD 
                                                                                    //111=Auto-coonvert            
                                                                                    //010=TMR3 ADC1 y TMR5 ADC2      
                                                                                    //001=INT0 / 000= hay que borrar SAMP 
    //_SIMSAM = 0;                                                              //1=Muestrea simultaneamente 
                                                                                //0=Muestrea canales individuales sequencialmente 
_ASAM = 1;                                                                      //1=Muestreo comienza seguido de la conversion 
                                                                                // 0=comienza cuando SAMP=1 
    //_SAMP    
    //_DONE 
AD1CON2= 0;                                                                     // 0x00E0 
    //_VCFG = 0;                                                                //Vref+- 000=ACdd y AVss / 001=Vref+ y AVss 
                                                                                // 010=AVdd y Vref- / 011=Vref+ y Vref- 
_CSCNA = 1;                                                                     //1=Escaneo canal cero / 0= no escaneo 
    //_CHPS = 0;                                                                //Canales utilizados:00=CH0 /01=CH0 y CH1 /1x= CH0,CH1,CH2 y CH3 
                                                                                //_BUFS   es solo de lectura: con BUFM=1-> 1=activa 2º mitad de buffer 
                                                                                // 0= activa 1ª mitad 
    //_SMPI = 0;                                                                //Incrementos dir DMA +1 o sin DMA genera una interrupción cada cada SMPI+1 
    //_BUFM = 0;                                                                //1= 2 medios Buffers de 8 / 0= 1 buffer de 16 
    //_ALTS = 0;                                                                //Modo de muestreo: 1= Alternado / 0= Siempre canal A 
AD1CON3 = 0; 
    //_ADRC = 0;                                                                //1= reloj RC / 0= Reloj CPU 
_SAMC = 31;                                                                     //Tiempo auto muestreo0 nºde Tad (de 0 a 31Tad) 
_ADCS = 3;    
                                                                                //Tad= >75ns = Tcy(ADCS+1) -> a 80MHz Tcy= 25ns 
                                                                                // Tad = (3+1)Tcy 
AD1CON4= 0; 
    //_DMABL = --;                                                              //Cada buffer contiene 8 palabras  
AD1CHS123= 0;                                                                   // reg. seleccion de entrada canal 1,2 y 3  
    //_CH123NA= 0;                                                              //Selección entrada negativa Canales 1, 2, y 3 pata Sample A  
                                                                                //0x=      CH1=CH2=CH3->      Vref-      
                                                                                // 10= CH1=AN6,CH2=AN7,CH3=AN8 
                                                                                // 11= CH1=AN9,CH2=AN10,CH3=AN11 
    //_CH123SA= 0;                                                                  //Selección entrada positiva Canales 1, 2, y 3 para Sample A 
                                                                                //1=      AN3-AN4-AN5      /      0=      AN0-AN1-AN2      
    //_CH123NB = 0; 
    //_CH123SB = 0; 
AD1CHS0= 0;                                                                     // Seleccion de entrada canal 0 
    //_CH0NA = 0;                                                               //Entrada negativa 1=  AN1 / 0 = Vref- 
    //_CH0SA = 5;                                                               //Entrada positiva = canal(CH0SA) 
    //_CH0SA = 4; 
    //_CH0SB = 3; 

    //_CH0NB = 0;  
AD1CSSH = 0x0000;                                                               //Selección entradas escaneo de la 16 a la 31 
AD1CSSL = 0x0330;                                                               //Selección entradas escaneo de 0 a 15. 
AD1PCFGH = 0xFFFF;                                                              //Configura entradas: 1= digital 0= Analog 
AD1PCFGL = 0xFFFF; 
    //_PCFG0=     
    // AN0 
_PCFG4 =  0;                                                                    // AN4 -> RB4 -> Sensor Temperatura 
_PCFG5 =  0;                                                                    // AN5 -> RB5 -> Potenciómetro 
_PCFG8 =  0;                                                                    // AN8 -> RB8 -> Joystick_X 
_PCFG9 =  0;                                                                    // AN9 -> RB9 -> Joystick_Y 
    //IFS0 
_AD1IF = 0;                                                                     //Flag a cero del A/D interrrupt. 
    //IEC0 
_AD1IE = 0;                                                                     // Habilita la interrupcion A/D. 
//----------------------------------------------------------------------------- 
DMA4CON = 0; 
                                                                                // CHEN CANAL habilitado / deshabilitado 
    DMA4CONbits.SIZE = 0;                                                       // 1=Byte / 0=Word 
    DMA4CONbits.DIR = 0;                                                        // 1=lee de RAM y escribe a periférico 
                                                                                // 0=lee de periférico y escribe en RAM 
    // HALF interrupcion a  mitad / al final 
    // NULLW Escribe nulos / operacion normal 
    // AMODE                                                                    //10: Direc.Indirect,Perif / 01 Indir.Reg sin Post-incr 
                                                                                // 00 Indir.Reg con Post-Incr 
 //   DMA4CONbits.MODE =                                                        // 11:One PP / 10:Continuo + PP 
                                                                                // 01: One / 00: Continuo 
//   DMA0REQ 
    DMA4REQbits.FORCE = 0;                                                      // manual / automático 
    // IRQSEL 
        // 0x00 INT0 
        // 0x01 IC1 
        // 0x02 OC1 
        // 0x05 IC2 
        // 0x06 OC2 
        // 0x07 TMR2 
        // 0x08 TMR3 
        // 0x0A SPI1 
        // 0x0B UART1RX 
        // 0x0C UART1TX 
    DMA4REQbits.IRQSEL = 0x0D;                                                  // ADC1 //DMA_IRQ_ADC1 
        // 0x15 ADC2 
        // 0x1E UART2RX 
//    DMA2REQbits.IRQSEL = 0x1F; // UART2TX 
        // 0x21 SPI2 
        // 0x22 ECAN1-RX 
        // 0x37 ECAN2-RX 
        // 0x46 ECAN1-TX 
        // 0x47 ECAN2-TX 
    DMA4STA = __builtin_dmaoffset(CAD_BuffA);                                   // Inicio dir. offset 
    //DMA2STB = __builtin_dmaoffset(CAD_BuffB);                                 // Inicio dir. offset 
    //DMA2STA = __builtin_dmaoffset(Ventana_LCD);                               // Inicio dir. offset 
    //DMA2STB = __builtin_dmaoffset(Ventana_LCD);                               // Inicio dir. offset 
    DMA4PAD = (volatile unsigned int)&ADC1BUF0; 
    AD1CON2bits.SMPI=3;                                                         //Numero de patas analogicas (0 a 31)
    AD1CON4bits.DMABL=0;                                                        //Nº de palabras del buffer por entrada analogica 
                                                                                //0->1...7->128
    DMA4CONbits.AMODE=0;                                                        //2 direc. Indirect, Perif
                                                                                //3 Reservado
                                                                                //1 Indir. Reg sin Post-incr
                                                                                //0 Indir. Reg con Post-incr (Para que no deje huecos en CAD_BuffA)
    DMA4CONbits.MODE=0;                                                         //3: Una+PP
                                                                                //2:Continuo+PP
                                                                                //1:Una
                                                                                //0:Continuo
    _ADDMABM = 1;                                                               //1=bufer DMA en orden de conversión -> AMODE=0
                                                                                //0=bufer DMA en modo Scatter/Gather ->AMODE=2
    DMA4CNT = 16-1;                                                             // Nº de trasnsferencias -1 
    
    DMACS0 = 0;                                                                 // Borra Colisiones 
    //IPC9bits.DMA4IP = --; 
    IFS2bits.DMA4IF =0; 
    IEC2bits.DMA4IE = 1;                                                        //Probar con 0
    DMA4CONbits.CHEN = 1;                                                       // Activa canal 2 DMA 
//AD1CON1bits 
_ADON = 1;   
//Habilita convertidor 
_SAMP= 1;    
//Lanza conversion 
} // Fin Inic_CAD DMA 

//===============Rutina de atencion a la interrupcion de la DMA=================

void __attribute__((__interrupt__)) _DMA4Interrupt(void) 
{ 
Nop(); 
Nop(); 
Nop(); 
IFS2bits.DMA4IF = 0; 
} 