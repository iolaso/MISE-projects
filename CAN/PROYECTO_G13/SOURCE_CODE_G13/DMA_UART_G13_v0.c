/*
Proyecto: MOISE Micros 16bits 2017
Fichero: DMA_UART_G1_v0.c

Grupo: G1    Autores: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0
 *

v0: 
Descripcion:
 * Inicializacion RS232_2 con DMA
 * Rutinas de atencion a la interrupcion de la DMA
 */


#include "IO_Explorer16_G13_v0.h"
#include "Main_MISE_G13_v0.h"
#include "LCD_Explorer16_G13_v0.h"
#include "p24hj256gp610A.h"

//unsigned int uart2RxBuffA[32] __attribute__((space(dma))); 
//unsigned int uart2RxBuffB[32] __attribute__((space(dma))); 
unsigned char uart2TxBuffA[n_filas][n_columnas] __attribute__((space(dma)));    //Reservamos un espacio lo suficienteme grande para Ventana_DATOS 
//unsigned char uart2TxBuffB[2][16] __attribute__((space(dma))); 
//extern unsigned char Ventana_DATOS[n_filas][n_columnas] __attribute__((space(dma))); 

// =====================Inicializa la UART2 Serial Port========================= 
void initRS232_2_DMA( void) 
{ 
//=====================Configuracion DMA asociado a UART 2======================
// Requiere desactivar interrupciones UART 2 
    
    DMA5CON = 0; 
    // CHEN CANAL habilitado // deshabilitado por defecto al poner 0
    DMA5CONbits.SIZE = 1;   // 1=Byte / 0=Word 
    DMA5CONbits.DIR = 1; // 1= lee de DPSRAM y escribe en periférico 
                         // 0= lee de periférico y escribe en DPSRAM 
    DMA5CONbits.HALF =0;      // 1= interrupcion a  mitad / 0= al final 
    DMA5CONbits.NULLW =0;    // 1= Escribe nulos / 0= operacion normal 
    DMA5CONbits.AMODE =0;    // 10: Direc.Indirect,Perif 
                                // 01 Indir.Reg sin Post-incr 
                                // 00 Indir.Reg con Post-Incr 
    DMA5CONbits.MODE = 0;   // 11: Un bloque + PingPong                         //Transmitira continuamente la cantidad de datos que le digamos en DMA0CNT
                            // 10: Continuo + PingPong 
                            // 01: Un bloque 
                            // 00: Continuo 
//   DMA5REQ Selecciona nº IRQ Periferico 
    DMA5REQbits.FORCE = 0; // 1= manual / 0= automático 
    // IRQSEL Selecciona nº IRQ Periferico 
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
        // 0x0D ADC1 
        // 0x15 ADC2 
        // 0x1E UART2RX 
    DMA5REQbits.IRQSEL = 0x1F; // UART2TX 
        // 0x21 SPI2 
        // 0x22 ECAN1-RX 
        // 0x37 ECAN2-RX 
        // 0x46 ECAN1-TX 
        // 0x47 ECAN2-TX 
   // DMA5STA = __builtin_dmaoffset(uart2TxBuffA);  //0; // Inicio dir. offset 
   // DMA5STB = __builtin_dmaoffset(uart2TxBuffB);  //0x0020;// Inicio dir. offset 
    DMA5STA = __builtin_dmaoffset(Ventana_DATOS);    //0;    // Inicio dir. offset          //Apuntamos a Ventana_DATOS para poder trabajar en el espacio reservado antes
    //DMA5STB = __builtin_dmaoffset(Ventana_DATOS);    //0x0020; // Inicio dir. offset 
    DMA5PAD = (volatile unsigned int)&U2TXREG;      // Dirección del periférico 
    DMA5CNT = n_filas*n_columnas-1;     // Nº de trasnsferencias -1 
    DMACS0 = 0;         // Borra flags de Colisiones 
//    INTCON1bits.DMACERR = 0;    // Borraa Flag de interrupción de colisiones 
    IFS3bits.DMA5IF = 0; 
    IEC3bits.DMA5IE = 1;        // Habilita interrupciones DMA 0 
    DMA5CONbits.CHEN = 1;   // Enable canal 0 DMA 
}
// =======================Atención Interrupciones DMA=========================== 

//void __attribute__((__interrupt__)) _DMA0Interrupt(void) 
void _ISR _DMA5Interrupt(void) 
{ 
Nop(); 
Nop();                                                                          //La utilizamos pra asegurarnos de que está funcionando
Nop(); 
        IFS3bits.DMA5IF = 0; 
} 
//=========================== Example 22-13: DMA Controller Trap Handling
void __attribute__((__interrupt__)) _DMACError(void) 
{ 
static unsigned int ErrorLocation; 
// Peripheral Write Collision Error Location if(DMACS0 & 0x0100) 
{ 
ErrorLocation = DMA5STA; 
} 
// DMA RAM Write Collision Error Location if(DMACS0 & 0x0002) 
{ 
ErrorLocation = DMA1STA; 
} 
DMACS0 = 0; //Clear Write Collision Flag 
INTCON1bits.DMACERR = 0; //Clear Trap Flag 
}
