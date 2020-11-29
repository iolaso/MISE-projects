/*
Proyecto: MOISE Micros 16bits 2017
Fichero: UART_G1_v0.c

Grupo: G1    Autores: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0
 *

v0: 
Descripcion:
 * inicializacion RS232_2 sin DMA
 * Rutinas de atencion a la interrupcion
 * Rutinas de atencion por encuesta
 * ENTRADAS:
 * * Datos del terminal en modo recepción
 * * Ventana_DATOS 
 * 
 * SALIDAS
 *  Ventana_DATOS 
 */

#include "IO_Explorer16_G13_v0.h"
#include "Main_MISE_G13_v0.h"
#include "LCD_Explorer16_G13_v0.h"
#include "UART_G13_v0.h"
int i;                                                                          //Para la gestion de Ventana_DATOS en Transmision por interrupción

// ================ Inicializa la UART2 Serial Port ====================== 
void inic_RS232_2( void)                                                        //Inicializamos todos los registros a cero y configuramos justo los bits que necesitemos
{ 
// No requerido, lo hace el hardware 
//  UART2_TX_TRIS = 0; 
//  UART2_RX_TRIS = 1; 
//  UART2_TX_LAT  = 1; 
//  UART2_RX_LAT  = 1; 
    U2MODE = 0;                                                                 // 8bits, sin paridad, 1 stop, Uart parada 
//  _UARTEN_U2 =; // Habilita UART 
//  _USIDL_U2  =; // 0-> continua en modo Idle 
//  _IREN_U2  =; // Habilita IrDA 
//  _RTSMD_U2  =; // Modo pata U2RTS 
//  _UEN_U2   =; // Habilita U2TX, U2RX, U2CTS y U2RTS 
//  _WAKE_U2  =; // Habilita Wake-up al detectar Start en Sleep 
//  _LPBACK_U2 =; // Habilita modo Loopback 
//  _ABAUD_U2  =; // Habilita Auto-Baud 
//  _RXINV_U2  =; // Invierte polaridad para recepciÃ³n 
    _BRGH_U2  = 0;                                                              // BRGH=0. Si con BRGH=0 no funcionase bien, podriamos utilizar BRGH=1
//  _PDSEL_U2  =; // Paridad: 00= 8 bits sin paridad 
//  _STSEL_U2  =; // DuraciÃ³n bit Stop 
    U2BRG = BAUD_RATEREG_2_BRGH0;                                               //Calculamos U2BRG arreglo a que hemos elegido BRGH=0 
//  U2BRG = 8; // Aplicando la formula da 7 y no va bien 
    U2STA = 0; 
//  _UTXISEL_U2 =;   // Tipo Interrupcion Transmision 
//  _UTXINV_U2 =;    // Invierte polaridad pata transmision 
//  _UTXBRK_U2 =;    // Bit Breal 
//  _UTXEN_U2 =;     // Habilita Transmision 
//  _UTXBF_U2 =;     // Estado Buffer transmision (solo lectura) 
//  _TRMT_U2 =;      // Estado Reg.Desplazamiento (solo lectura) 
//  _URXISEL_U2 =;   // Tipo interrupcipn en recepcipn 
                     // 0x= interrupcipn con cada dato que llega 
                     // 10= interrupcipn a 3/4 del buffer 
                     // 11= interrupcipn cuando buffer lleno 
//  _ADDEN_U2 =;     // Deteccipn bit de direccipn en 9 bits 
//  _RIDLE_U2 =;     // Estado de la recepcipn (solo lectura) 
//  _PERR_U2 =;      // Error de paridad /solo lectura) 
//  _FERR_U2 =;      // Error de trama (solo lectura) 
//  _OERR_U2 =;      // Error ocerrun (borrable/ solo lectura) 
//  _URXDA_U2 =;     // 0=buffer vacio, 1= al menos un dato 
    _U2RXIF = 0;     // Borra flag int. RX 
    _U2TXIF = 0;     // Borra flag int. TX 
    _U2EIF = 0;      // Boorra flag de Error UART 
    _U2TXIE = 0;     // Habilita int. de transmisiÃ³n 
    _U2RXIE = 1;     //Habilita int. de recepcion
//  _U2EIE = 1;      // Habilita Int_ de Error UART 
    _OERR_U2=0;      // Segun version corrige bug 
    _UARTEN_U2 = 1;  // Habilita la UART_2 
    _UTXEN_U2 = 1;   //Habilita transmision,
                     //Debe activarse despues de habilitar UART 
    
// RUTINA QUE ESPERA LA DURACION DE UN BIT ANTES DE EMPEZAR A TRANSMITIR 
U2TXREG = 0;                                                                    //Enviamos un dato vacio (nada) para que esté listo para enviar (activa flag interupcion)
 
}   // Fin Inicializacion UART 2 

// ================ Servicio INTERRUPCION TRANSMISION RS232_2 ==================

void _ISR_NO_PSV _U2TXInterrupt(void)                                           // Trasmite un dato, si hay, al final de transmisión del anterior 
{ 
    U2TXREG=Ventana_DATOS[0][i];                                                //Ventana_DATOS es un apuntador, por lo que no hace falta ir por lineas, cuando rebasa el numero de columnas, pasa a la siguiente
    i++;
    if(i>n_filas*n_columnas) i=0;
    _U2TXIF = 0; 
}   // FIN _U2RXInterruptt 
void _ISR_NO_PSV _U2ErrInterrupt (void)                                         //Interrupcion que se activa si hay algun error
{ 
    Nop();    
    Nop();
   
    _U2EIF = 0; 
} 
void _ISR_NO_PSV _U2RXInterrupt(void)                                           //Recepcion de datos mediate interupcion. Se activa en cuanto recibe un dato
{   
    
    Ventana_DATOS[0][15]=U2RXREG;
    _U2RXIF = 0; 
}   // FIN _U2RXInterruptt 

// ======================TRANSMISION RS232_2 por encuenta======================= 
// Envía un caracter por la UART2  

void putRS232_2( unsigned char c) 
{ 
    while ( _UTXBF_U2 == 1);                                                    // Espera mientras buffer Tx está lleno 
    U2TXREG = c;                                                                // Carga dato a transmitir 
} // FIN putRS232_2 

// =======================RECEPCION RS232_2 por encuesta========================
// Espera a recibir un caracter por la UART2 

char getRS232_2( void) 
{ 
    while ( !_URXDA_U2);                                                        // Espera a que llegue un dato 
    return U2RXREG;                                                             // Recoge el dato (hace return))
}// FIN getRS232_2 