

#include "oscilador_v2.h"
#include "p24hj256gp610A.h"
#include "UART.h"
#include "main.h"
#include "tiempos.h"

// ================ Inicializa la UART2 Serial Port ======================
void inic_RS232_2( void)
{
    
long unsigned int Fosc,Fcy,BAUDRATE2,BAUD_RATEREG_2_BRGH1,BAUD_RATEREG_2_BRGH0;

if (flag_osc_80) Fosc = 80000000 ;//Frecuencia reloj CPU
else Fosc = 8000000 ;
Fcy = Fosc/2;
BAUDRATE2 = 9600; // baudios (bits/sg) de transmision
BAUD_RATEREG_2_BRGH1 = ((Fcy/BAUDRATE2)/4)-1; // Para BRGH = 1
BAUD_RATEREG_2_BRGH0 = ((Fcy/BAUDRATE2)/16)-1; // Para BRGH = 0

// No requerido, lo hace el hardware
// UART2_TX_TRIS = 0;
// UART2_RX_TRIS = 1;
// UART2_TX_LAT = 1;
// UART2_RX_LAT = 1;
 U2MODE = 0; // 8bits, sin paridad, 1 stop, Uart parada
// _UARTEN_U2 =; // Habilita UART
// _USIDL_U2 =; // 0-> continua en modo Idle
// _IREN_U2 =; // Habilita IrDA
// _RTSMD_U2 =; // Modo pata U2RTS
// _UEN_U2 =; // Habilita U2TX, U2RX, U2CTS y U2RTS
// _WAKE_U2 =; // Habilita Wake-up al detectar Start en Sleep
// _LPBACK_U2 =; // Habilita modo Loopback
// _ABAUD_U2 =; // Habilita Auto-Baud
// _RXINV_U2 =; // Invierte polaridad para recepción
 _BRGH_U2 = 1; // BRGH
// _PDSEL_U2 =; // Paridad: 00= 8 bits sin paridad
// _STSEL_U2 =; // Duración bit Stop
 U2BRG = BAUD_RATEREG_2_BRGH1;
// U2BRG = 8; // Aplicando la formula da 7 y no va bien
 U2STA = 0;
// _UTXISEL_U2 =; // Tipo Interrupción Transmisión
// _UTXINV_U2 =; // Invierte polaridad pata transmisión
// _UTXBRK_U2 =; // Bit Breal
// _UTXEN_U2 =; // Habilita Transmisión
// _UTXBF_U2 =; // Estado Buffer transmisión (solo lectura)
// _TRMT_U2 =; // Estado Reg.Desplazamiento (solo lectura)
// _URXISEL_U2 =; // Tipo interrupción en recepción
 // 0x= interrupción con cada dato que llega
 // 10= interrupción a 3/4 del buffer
 // 11= interrupción cuando buffer lleno
// _ADDEN_U2 =; // Detección bit de dirección en 9 bits
// _RIDLE_U2 =; // Estado de la recepción (solo lectura)
// _PERR_U2 =; // Error de paridad /solo lectura)
// _FERR_U2 =; // Error de trama (solo lectura)
// _OERR_U2 =; // Error ocerrun (borrable/ solo lectura)
// _URXDA_U2 =; // 0=buffer vacío, 1= al menos un dato
 _U2RXIF = 0; // Borra flag int. RX
 _U2TXIF = 0; // Borra flag int. TX
 _U2EIF = 0; // Boorra flag de Error UART
 _U2TXIE = 0; // Habilita int. de transmisión
 _U2RXIE = 1;
// _U2EIE = 1; // Habilita Int_ de Error UART
 _OERR_U2 = 0; // Según versión corrige bugg
 _UARTEN_U2 = 1; // Habilita la UART_2
 _UTXEN_U2 = 1; //Habilita transmisión,
 //Debe activarse después de habilitar UART
// RUTINA QUE ESPERA LA DURACION DE UN BIT ANTES DE EMPEZAR A TRANSMITIR
// - - -
 Nop();
 Nop();
 Nop();
 Nop();
 Nop();
 //100useg
} // Fin Inicialización UART 2 

// ================ Servicio INTERRUPCION TRANSMISION RS232_2 ==============
// Trasmite un dato, si hay, al final de transmisión del anterior
void _ISR_NO_PSV _U2TXInterrupt(void)
{

static int flag_fila=0;
static int posiciones=0;

        if(posiciones>=0 && posiciones<19){
            U2TXREG = Ventana_Datos [flag_fila][posiciones];
            posiciones++;
            if(posiciones>=19){
            flag_fila++; 
            posiciones=0;
            }
            if(flag_fila>=8){
                posiciones=0;
                flag_fila=0;
            }
            
        
        }
 _U2TXIF = 0;
   
} 

// ================ TRANSMISION RS232_2 ==================
// Envía un caracter por la UART2
void putRS232_2( unsigned char c)
{
 while ( _UTXBF_U2 == 1); // Espera mientras buffer Tx está lleno
 U2TXREG = c; // Carga dato a transmitir
} // FIN putRS232_2
// ================ RECEPCION RS232_2 ==================
// Espera a recibir un caracter por la UART2
char getRS232_2( void)
{
 while ( !_URXDA_U2); // Espera a que llegue un dato
 return U2RXREG; // recoge el dato
}// FIN getRS232_2 


void initRS232_2_DMA( void) 
{ 
// === Configuracion DMA asociado a UART 2 ==== 
// Requiere desactivar interrupciones UART 2 
    DMA0CON = 0; 
    // CHEN CANAL habilitado / deshabilitado 
    DMA0CONbits.SIZE = 1;   // 1=Byte / 0=Word 
    DMA0CONbits.DIR = 1; // 1= lee de DPSRAM y escribe en periférico 
                         // 0= lee de periférico y escribe en DPSRAM 
    // DMA0CONbits.HALF =?      // 1= interrupcion a  mitad / 0= al final 
    // DMA0CONbits.NULLW =?     // 1= Escribe nulos / 0= operacion normal 
    // DMA0CONbits.AMODE =?     // 10: Direc.Indirect,Perif 
                                // 01 Indir.Reg sin Post-incr 
                                // 00 Indir.Reg con Post-Incr 
    DMA0CONbits.MODE = 0;   // 11: Un bloque + PingPong 
                            // 10: Continuo + PingPong 
                            // 01: Un bloque 
                            // 00: Continuo 
//   DMA0REQ Selecciona nº IRQ Periferico 
    DMA0REQbits.FORCE = 0; // 1= manual / 0= automático 
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
    DMA0REQbits.IRQSEL = 0x1F; // UART2TX 
        // 0x21 SPI2 
        // 0x22 ECAN1-RX 
        // 0x37 ECAN2-RX 
        // 0x46 ECAN1-TX 
        // 0x47 ECAN2-TX 
   // DMA0STA = __builtin_dmaoffset(uart2TxBuffA);  //0; // Inicio dir. offset 
   // DMA0STB = __builtin_dmaoffset(uart2TxBuffB);  //0x0020;// Inicio dir. offset 
    DMA0STA = __builtin_dmaoffset(Ventana_Datos);    //0;    // Inicio dir. offset 
    DMA0STB = __builtin_dmaoffset(Ventana_Datos);    //0x0020; // Inicio dir. offset 
    DMA0PAD = (volatile unsigned int)&U2TXREG;      // Dirección del periférico 
    DMA0CNT = 152-1;     // Nº de trasnsferencias -1 
    DMACS0 = 0;         // Borra flags de Colisiones 
//    INTCON1bits.DMACERR = 0;    // Borraa Flag de interrupción de colisiones 
    IFS0bits.DMA0IF = 0; 
    IEC0bits.DMA0IE = 1;        // Habilita interrupciones DMA 0 
    DMA0CONbits.CHEN = 1;   // Enable canal 0 DMA 
}

void _ISR_NO_PSV _DMA0Interrupt(void) 
{ 
Nop(); 
Nop(); 
Nop(); 
IFS0bits.DMA0IF = 0; 
} 

void _ISR_NO_PSV _U2ErrInterrupt (void)
{

  
 _U2EIF = 0;
}

void _ISR_NO_PSV _U2RXInterrupt(void) 
{ 
        static int count=0;
    
    respuesta[count]=U2RXREG;
    count++;
    if (count>8){
        Flag_trama_ok=1;
        count=0;
         _U2RXIE = 0;

    }
    
    //FlagIntRxRs232=1;
    
    _U2RXIF = 0; 
}  
