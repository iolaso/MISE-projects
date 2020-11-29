/*
Proyecto: MOISE Micros 16bits 2017
Fichero: Main_MISE_G1_v0.c

Grupo: G1    Autores: Iñigo Olaso y Edurne Sagasta
	Fecha: 2017-11-27
 *  v0
 *

v0: 
Descripcion:
 * Usa EXPLORER16 con PIC24HJ256GP610A
 * Inicializaciones
 * Atencion a los flags festionados en las interrupciones
 * Atencion al flag que identifica el modo del DMA(CAD)
 * Atencion al flag del pulsador especial(S5)
 * Llamada a maquina de estado
 * Calculo de consumo de CPU
 * Cuerpo de las funciones creadas
 * * Copia_FLASH_RAM
 * * Maquina_Estados 
 * * bits_to_ASCII 
 * * int_to_ASCII
 * * Actualizar_Ventana_Continuo
 * * Actualizar_Ventana_SG
 * * Uso_CPU
 * 
 * ENTRADAS:
 *  Pulsadores S3,S4,S5,S6
 *  Potenciometro (DMA CAD)
 *  Sensor de temperatura (DMA CAD)
 *  Joystick (DMA CAD)
 * 
 * SALIDAS
 *  Leds D3, D4, D5, D6, D7, D8, D9 ,D10
 *  UART (DMA, RS232)
 *  PANTALLA LCD
 */
/****************CAN*********************/
#if defined(__dsPIC33F__)
#include "p33fxxxx.h"
#elif defined(__PIC24H__)
#include "p24hxxxx.h"
#endif

#include "ECAN1Config.h"
#include "ECAN2Config.h"
#include "common.h"
/**************CAN****************/
#include "DMA_UART_G13_v0.h"
#include "IO_Explorer16_G13_v0.h"
#include "Main_MISE_G13_v0.h"
#include "LCD_Explorer16_G13_v0.h"
#include "UART_G13_v0.h"
#include "CAD_NO_DMA_G13_v0.h"
#include "CAD_DMA_G13_v0.h"
#include "oscilador_v2.h"
#include "Tiempos_G13_v0.h"

/*******************************************CAN********************************************/
// Define ECAN Message Buffers
ECAN1MSGBUF ecan1msgBuf __attribute__((space(dma),aligned(ECAN1_MSG_BUF_LENGTH*16)));
ECAN2MSGBUF ecan2msgBuf __attribute__((space(dma),aligned(ECAN2_MSG_BUF_LENGTH*16)));

// CAN Messages in RAM
mID mensajeLuis,rx_ecan1message;//
mID rx_ecan2message;

// Prototype Declaration
//void oscConfig(void);
void clearIntrflags(void);
void ecan1WriteMessage(void);
void ecan2WriteMessage(void);

/*******************************************CAN********************************************/
int temp=0, id_PS;//id:peticion de servicio
int flag_RX=0,no_hay_id;
int modo=0;//0;1=Peticion de serv.;2=Dar servicio;
unsigned long id_emisor;
unsigned char Tipo,u;
int  selector_de_leds, carga_led1, carga_led2, carga_led3, carga_led4, carga_led5;
int flag_timeout,flag_timeout_enable,flag_5seg;
//#pragma config FWDTEN = OFF                                                   //Watchdog Timer: Disabled (desactivado en .....)
const char Texto_1[]={"Valor leido:    ""\x0d\x0a\x0"
                      "- PIC24H -- G13 ""\x0d\x0a\x0"
                      "3- REAL ICE ----""\x0d\x0a\x0"
                      "Consumo CPU%:   ""\x0d\x0a\x0"
                      "Joystick X: 0x--""\x0d\x0a\x0"
                      "Joystick Y: 0x--""\x0d\x0a\x0"
                      "Lectura Pot:0x--""\x0d\x0a\x0"
                      "Lectura Tem:0x--""\x1b[H"}; 

//==================Definicion de las variables globales========================
unsigned short scroll_V,f_correcta,f_deseada,columna, contador;
long usocpu;

//==============================================================================
int main (void)
{
Nop(); //Puntos de Ruptura
Nop();

/***********************************CAN***************************************/
/* Clear Interrupt Flags 				*/
	clearIntrflags();
/***********************************CAN***************************************/
    



    AD1PCFGL= 0xFFFF;                                                           // Pone todas las patas analogicas de I/O digital ya que por defecto el convertidor esta desactivado
    AD1PCFGH= 0xFFFF;                                                           // Por defecto las patas analogicas estan habilitadas
    
//=========================Inicializaciones=====================================

Nop(); 
Nop(); 

    if(Fosc==80000000)Inic_Oscilador();                                         //Si se selecciona en UART_G1_v0.h 8Mhz, no se inicializa
    Inic_Leds ();   
    Inic_Pulsadores ();
    //Inic_Tiempo_Miliseg ();
    //Inic_Tiempo_2Miliseg ();
    //Inic_Tiempo_5Miliseg ();
    Inic_Timer_32();
    Inic_Tiempo_RA7 ();
    Inic_LCD ();
    Copia_FLASH_RAM (Texto_1,n_filas*n_columnas);
    inic_Maquina_Estado ();
    initRS232_2_DMA();
    inic_RS232_2();
    Inic_CAD_DMA();
     //Inic_CAD();
    
/***********************************CAN***************************************/

    /* ECAN1 Initialisation 		
   Configure DMA Channel 0 for ECAN1 Transmit
   Configure DMA Channel 2 for ECAN1 Receive */
	ecan1Init();
	dma0init();	
	dma2init();

/* Enable ECAN1 Interrupt */ 				
    	
	IEC2bits.C1IE = 1;
	C1INTEbits.TBIE = 1;	
	C1INTEbits.RBIE = 1;

/* ECAN2 Initialisation 		
   Configure DMA Channel 1 for ECAN2 Transmit
   Configure DMA Channel 3 for ECAN2 Receive */
	ecan2Init();
	dma1init();	
	dma3init();

/* Enable ECAN2 Interrupt */ 
	
	IEC3bits.C2IE = 1;
	C2INTEbits.TBIE = 1;	
	C2INTEbits.RBIE = 1;

 
/* Write a Message in ECAN1 Transmit Buffer	
   Request Message Transmission			*/
//	ecan1WriteMessage();
//	C1TR01CONbits.TXREQ0=1;	
	
long IDENTIFICADOR;


/* Write a Message in ECAN2 Transmit Buffer
   Request Message Transmission			*/
//	ecan2WriteMessage();
//	C2TR01CONbits.TXREQ0=1;
/***********************************CAN***************************************/

    
Nop();
Nop();   
Nop();
Nop();
//==============================Bucle Principal=================================

    while(1)
    {
Nop();
Nop();  
//==============Inicializacion del timer para medir el consumo de CPU===========
         
     /*    selector_de_leds = 0x01F;//F;
        carga_led1=10;
        carga_led2=20;
        carga_led3=40;
        carga_led4=30;
        carga_led5=50;*/
                        
        Inic_Tiempo_Miliseg();                                                  //Timer que se usa para medir el consumo de CPU

//============Atencion a los flags gestionados en las interrupciones============
        
        if (fscroll==1){                                                        //Para que en la LCD se puedan visualizar mas de 2 lineas haciendo scroll (vertical) con un pulsador
            scroll_V++;
            if(scroll_V>n_filas-2) scroll_V=0;
            fscroll=0;
        }
        if (Pulsado_S3 == 1)
        {
            //Funcion de peticion a G0
            if (modo == 0){
            modo=1;
            //Mandas la peticion
            switch(Ventana_DATOS[0][15]){
                
                    case '0':
                        id_PS = 0x00;
                        no_hay_id = 0;                    
                        break;
                    case '1':
                        id_PS = 0x01;
                        no_hay_id = 0;
                        break;
                    case '2':
                        id_PS = 0x02;
                        no_hay_id = 0;
                        break;
                    case '3':
                        id_PS = 0x03;
                        no_hay_id = 0;
                        break;
                    case '4':
                        id_PS = 0x04;
                        no_hay_id = 0;
                        break;
                    case '5':
                        id_PS = 0x05;
                        no_hay_id = 0;
                        break;
                    case '6':
                        id_PS = 0x06;
                        no_hay_id = 0;
                        break;
                    case '7':
                        id_PS = 0x07;
                        no_hay_id = 0;
                        break;
                    case '8':
                        id_PS = 0x08;
                        no_hay_id = 0;
                        break;
                    case '9':
                        id_PS = 0x09;
                        no_hay_id = 0;
                        break;
                    case 'a':
                        id_PS = 0x0a;
                        no_hay_id = 0;
                        break;
                    case 'b':
                        id_PS = 0x0b;
                        no_hay_id = 0;
                        break;
                    case 'c':
                        id_PS = 0x0b;
                        no_hay_id = 0;
                        break;
                    case 'd':
                        id_PS = 0x0d;
                        no_hay_id = 0;
                        break;     
                        
                    default:
                        
                        no_hay_id = 1;
                        break;
            }
            if (no_hay_id == 0){
                IDENTIFICADOR =  0x1FFFFFF0 | id_PS;
                IDENTIFICADOR = IDENTIFICADOR <<18;
                ecan1WriteTxMsgBufId(0,IDENTIFICADOR,0,0);//0 sid                               s5|s4|s3|s2|s1|
                ecan1WriteTxMsgBufData(0,8,0x181D,0x2222,0x3333,0x4444);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                C1TR01CONbits.TXREQ0=1;	
               }
            else if  (no_hay_id == 1){
                
                modo = 0;
                no_hay_id = 0;
                }
            }

        }
Nop();
Nop();
        if (Pulsado_S3 == 0)
        {
            /*LED_D7 = led_apagado;   
            LED_D8 = led_encendido;*/
        }

        if (Pulsado_S6 == 1)
        {
            /*//LED_D5 = led_encendido;
            LED_D6 = led_apagado;*/
            //
        }
Nop();
Nop();
        if (Pulsado_S6 == 0)
        {
            /*//LED_D5 = led_apagado;
            LED_D6 = led_encendido;*/
        }
        if (Pulsado_S4 == 1)
        {

           /* LED_D4 = led_apagado;*/
        }

        if (flag_timeout == 1){
            ecan1WriteTxMsgBufId(0,IDENTIFICADOR,0,0);//0 sid   
            ecan1WriteTxMsgBufData(0,8,0x180D,0x2222,0x3333,0x0044);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
            C1TR01CONbits.TXREQ0=1;
            flag_timeout = 0;
            modo=0;         
        }
Nop();
Nop();
        if (Pulsado_S4 == 0)
        {
            /*LED_D4 = led_encendido;  */ 
        }
        ///Flag de Interrupcion en la rececpcion CAN1////////////
        if(flag_RX == 1){

        flag_RX = 0;
            if(modo == 1){
                
                    Tipo = rx_ecan1message.data[0] & 0x0030;
                    // Tipo Aceptar peticion mandado por Luis 10 de aceptacion y 0000 Luis(ID)
                    if (Tipo == 0x20){
                    IDENTIFICADOR =  0x1FFFFFF0 | id_PS;
                    IDENTIFICADOR = IDENTIFICADOR <<18;
                    //ecan1WriteTxMsgBufId(0,0x3F0,0,0);//0 sid                               s5|s4|s3|s2|s1|
                    ecan1WriteTxMsgBufId(0,IDENTIFICADOR,0,0);//0 sid   
                    ecan1WriteTxMsgBufData(0,8,0x1F3D,0x2222,0x3333,0x4444);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                    C1TR01CONbits.TXREQ0=1;	
                    flag_5seg = 1;
                    while (flag_5seg == 1);
                    ecan1WriteTxMsgBufId(0,IDENTIFICADOR,0,0);//0 sid   
                    ecan1WriteTxMsgBufData(0,8,0x1F3D,0x2222,0x3333,0x0044);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                    C1TR01CONbits.TXREQ0=1;	                    
                    while (flag_5seg == 1);
                    ecan1WriteTxMsgBufId(0,IDENTIFICADOR,0,0);//0 sid   
                    ecan1WriteTxMsgBufData(0,8,0x1F3D,0x4444,0x3333,0x0022);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                    C1TR01CONbits.TXREQ0=1;	  
                    while (flag_5seg == 1);
                    ecan1WriteTxMsgBufId(0,IDENTIFICADOR,0,0);//0 sid   
                    ecan1WriteTxMsgBufData(0,8,0x1F3D,0x3333,0x2222,0x0044);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                    C1TR01CONbits.TXREQ0=1;	  
                    
                    modo=0;
                    }else{
                        ecan1WriteTxMsgBufId(0,IDENTIFICADOR,0,0);//0 sid   
                        ecan1WriteTxMsgBufData(0,8,0x180D,0x2222,0x3333,0x0044);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                        C1TR01CONbits.TXREQ0=1;
                        modo=0;
                    } modo=0; break;
                    
            }else if (modo == 0){
                
                
                //rxECAN1(mensajeLuis);
            
                Tipo = rx_ecan1message.data[0] & 0x0030;
                // Tipo Aceptar peticion mandado por Luis 10 de aceptacion y 0000 Luis(ID)
                if (Tipo == 0x10){ 
                
                    id_emisor = rx_ecan1message.data[0] & 0x000F;
                    
                    IDENTIFICADOR =  0x1FFFFFF0 | id_emisor;
                    IDENTIFICADOR = IDENTIFICADOR <<18;
                    ecan1WriteTxMsgBufId(0,IDENTIFICADOR ,0,0);//0 sid                               s5|s4|s3|s2|s1|
                   //ACEPTACION 0x0020
                    ecan1WriteTxMsgBufData(0,8,0x182D,0x2222,0x3333,0x0044);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                    C1TR01CONbits.TXREQ0=1;	
                    
                    modo=2;
                    flag_timeout_enable = 1;
                
                
                }else{
                ecan1WriteTxMsgBufId(0,IDENTIFICADOR,0,0);//0 sid   
                ecan1WriteTxMsgBufData(0,8,0x180D,0x2222,0x3333,0x0044);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                C1TR01CONbits.TXREQ0=1;
                modo=0;
                }
            }
        else if (modo == 2){

                //rxECAN1(mensajeLuis);
                IDENTIFICADOR =  0x1FFFFFF0 | id_emisor;
                IDENTIFICADOR = IDENTIFICADOR <<18;
                
                
                int id_emisor1 = rx_ecan1message.data[0] & 0x000F;
                if (id_emisor1 == id_emisor){
                    
                    
                    
                    Tipo = rx_ecan1message.data[0] & 0x0030;
                    // Tipo Aceptar peticion mandado por Luis 10 de aceptacion y 0000 Luis(ID)
                    if (Tipo == (0x30)){ 
                        
                                                
                        selector_de_leds=rx_ecan1message.data[1] & 0x001F;
                        carga_led1=rx_ecan1message.data[2]*50/255;//0 85 170 255
                        carga_led2=rx_ecan1message.data[3]*50/255;
                        carga_led3=rx_ecan1message.data[4]*50/255;
                        carga_led4=rx_ecan1message.data[5]*50/255;
                        carga_led5=rx_ecan1message.data[6]*50/255;
                        

                    }
                     
                }else{
                ecan1WriteTxMsgBufId(0,(0x1FFFFFF0 | id_emisor1) << 18,0,0);//0 sid   
                ecan1WriteTxMsgBufData(0,8,0x180D,0x2222,0x3333,0x4444);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
                C1TR01CONbits.TXREQ0=1;
                
                }
            } 
                      
                //} 
            //flag_RX = 0;
            }
        
//============Atencion al flag que identifica el modo del DMA(CAD)==============

Nop();
Nop();

        if(flagCAD==0)
            Actualizar_Ventana_Continuo();
        else 
            Actualizar_Ventana_SG();

Nop();
Nop();

//=================Atencion al flag del pulsador especial(S5)===================

        if (Pulsado_S5 == 0)
        {
            /*LED_D3 = led_encendido;   //*/
        }
        if (Pulsado_S5 == 1)
        {
            /*LED_D3 = led_apagado;*/
                                    //
        }

//==================Llamada a maquina de estado cada 1.47 ms====================

        if(flag147==1){
            flag147=0;
            Maquina_Estados();
        }
//========================Calculo de consumo de CPU=============================
        while(flagW==0){                                                        //Mientras este flag, que se activa cuando el timer que se ha inicializado
            usocpu++;                                                           //al principio, sea 0, contaremos cuantas instrucciones podemos hacer (usocpu++)
        }                                                                       //para contabilizar el tiempo libre que le queda al bucle principal
        Nop();
        usocpu=usocpu/1000;                                                     //Como la magnitud de las instrucciones es muy alta, la dividimos entre mil        
        Uso_CPU(usocpu);                                                        //Llamamos a la funcions Uso_CPU())
        flagW=0;
        usocpu=0;
        
	} // Fin while(1)
} //Fin main

void send_can_message(void){

    //ecan1WriteMessage();
    ecan1WriteTxMsgBufId(0,0x1FFFFFF0,0,0);//0 sid                               s5|s4|s3|s2|s1|
	ecan1WriteTxMsgBufData(0,8,0x181D,0x2222,0x3333,0x4444);//data[0]=0x1D;0x000|1 |1 |0 | 0|0 |==>0x18
    C1TR01CONbits.TXREQ0=1;	

}


//======================Cuerpo de las funciones creadas=========================



void Copia_FLASH_RAM (unsigned char *texto, unsigned short longitud){           //Función para copiar la variable Texto_1 (flash) a Ventana_DATO (RAM), para no trabajar con la flash-
    int i;                                                                      //A la funcion se le pasa la varibale texto y la cantidad de datos que queremos copiar
    if(longitud<n_filas*n_columnas+1){                                          //Por si acaso al final hay un NULL (puede no hacer falta)
        for(i=0;i<longitud;i++){                                                
            Ventana_DATOS[i/n_columnas][i%n_columnas]=texto[i];
        }
    }
}

void Maquina_Estados (){                                                        //Funcion para actualizar el LCD, haciendo una sola instruccion cada llamada
    if(f_correcta==0){                                                          //Si estamos en una fila incorrecta (ya hemos llenado una fila)...
        f_correcta=1;                                                           //Aseguramos que la fila sera la correcta
        if(f_deseada==0){                                                       //Movemos el cursos dependiendo de la fila deseada
            inicio_fila_1(); 
        }
        else{
            inicio_fila_2();
        }
    }            
    else{                                                                       //Si estamos en una fila correcta
        if(columna<16){                                                         //Mientras queden espacios libres
          lcd_data(Ventana_DATOS[f_deseada+scroll_V][columna]);                 //Escribimos en el LCD el dato sacado de Ventana_DATO (teniendo cuenta el scroll)
          columna++;
        }
        else{                                                                   //Si ya no quedan espacios libres		
            columna=0;                                                          //Reiniciamos contador columna
            f_deseada=!f_deseada;                                               //Cambiamos la fila deseada
            f_correcta=0;                                                       //Informamos de que la fila es incorrecta
        }
    }  
}

void bits_to_ASCII (short fila,int ADCV) {                                      //Funcion que formatea lo que hay en ADCV para conseguir su equivalente ASCII en formato hexadecimal
    short maspeso,menospeso;
    ADCV=ADCV >> 2;                                                             //Desplazamos y nos quedamos con los 8 bits de mas peso
    maspeso=ADCV&0x00F0;                                                        //Guardamos en "maspeso" los 4 bits mas significativos
    maspeso=maspeso >> 4;                                                       //Los desplazamos para que estan en una posicion mas comoda
    if(maspeso<0x000A){                                                         //Si esta entre 0 y 9 hexa, le sumamos 0x30 para llevarlo al rango 0 a 9 ASCII
        maspeso+=0x30;
    }
    else{
        maspeso+=0x37;                                                          //Si no esta entre 0 y 9 hexa, le sumamos 0x37 para llevarlo al rango A a F ASCII
    }
    menospeso=ADCV&0x000F;                                                      //Guardamos en "menospeso" los 4 bits menos significativos
    if(menospeso<0x000A){                                                       //Seguimos la misma logica
        menospeso+=0x30;
    }
   else{
        menospeso+=0x37;
    }
    Ventana_DATOS[n_filas-1-fila][14]=maspeso;                                  //Guardamos el valor en Ventana_DATOS, teniendo en cuenta en que fila lo queremos guardar
    Ventana_DATOS[n_filas-1-fila][15]=menospeso;
}

void int_to_ASCII (short fila,int ADCV){                                        //Funcion que formatea lo que hay en ADCV para conseguir su equivalente ASCII en formato decimal
    short centenas,decenas,unidades;
    ADCV=ADCV >> 2;                                                             //Desplazamos y nos quedamos con los 8 bits de mas peso
    centenas=ADCV/100;                                                          //Dividimos entre 100 para conseguir las centenas
    decenas=(ADCV-centenas*100)/10;                                             //Le restamos las centenas y dividimos entre 10 para conseguir las decenas
    unidades=ADCV-centenas*100-decenas*10;                                      //Le restamos las centenas y las decenas para conseguir las unidades
    Ventana_DATOS[n_filas-1-fila][13]=centenas+0x30;                            //Guardamos el valor en Ventana_DATOS, teniendo en cuenta en que fila lo queremos guardar
    Ventana_DATOS[n_filas-1-fila][14]=decenas+0x30;                             //y le ponemos +0x30 para conseguir su equivalente en ASCII
    Ventana_DATOS[n_filas-1-fila][15]=unidades+0x30;
}

void Actualizar_Ventana_Continuo(void){                                         //Actualiza Ventana_DATOS con las medias de las medidas del CAD teniendo en cuenta que trabaja en modo continuo
    int j,temp=0,pot=0,x=0,y=0;
    for(j=0;j<16;j+=4){                                                         //Ya que leemos 16 valores (4 por entrada)
        temp+=CAD_BuffA[j];                                                     //El pimer valor (j) sera el del sensor de temperatura (0,4,8,12)
        pot+=CAD_BuffA[j+1];                                                    //El segundo valor (j+1) sera el del potenciometro (1,5,9,13)
        x+=CAD_BuffA[j+2];                                                      //El tercer valor (j+2) serael del Joystick X (2,6,10,14)
        y+=CAD_BuffA[j+3];                                                      //El cuarto valor (j+3) sera del Joystick Y (3,7,11,15)
    }
    //int_to_ASCII(0,temp>>2));                                                 //Elegimos que funcion de las descritas arriba queremos usar para formatear y actualizar Ventana_DATOS
    bits_to_ASCII(0,temp>>2);
    //int_to_ASCII(1,pot>>2);
    bits_to_ASCII(1,pot>>2);
    //int_to_ASCII(2,x>>2);
    bits_to_ASCII(2,x>>2);
    //int_to_ASCII(3,y>>2));
    bits_to_ASCII(3,y>>2);
}

void Actualizar_Ventana_SG(void){                                               //Actualiza Ventana_DATOS con las medias de las medidas del CAD teniendo en cuenta que trabaja en Scatter/Gather
    int j,temp=0,pot=0,x=0,y=0;
    for(j=0;j<4;j++){                                                           //En este caso, tendremos que tner en cuenta que Scatter/Gather esparce las mediciones pero las agrupa por pata
        temp+=CAD_BuffA[j+16];                                                  //Hay que tener en cuenta que el sensor de temepratura está en AN4, por lo que sus datos empezaran en la posición 16 (16,17,18,19)
        pot+=CAD_BuffA[j+20];                                                   //Hay que tener en cuenta que el potenciometro está en AN5, por lo que sus datos empezaran en la posición 20 (20,21,22,23)
        x+=CAD_BuffA[j+32];                                                     //Hay que tener en cuenta que el Joystick X está en AN8, por lo que sus datos empezaran en la posición 32 (32,33,34,35)
        y+=CAD_BuffA[j+36];                                                     //Hay que tener en cuenta que el Joystick Y está en AN9, por lo que sus datos empezaran en la posición 36 (36,37,38,39)
    }
    //int_to_ASCII(0,temp>>2);                                                  //Elegimos que funcion de las descritas arriba queremos usar para formatear y actualizar Ventana_DATOS
    bits_to_ASCII(0,temp>>2);
    //int_to_ASCII(1,pot>>2);
    bits_to_ASCII(1,pot>>2);
    //int_to_ASCII(2,x>>2);
    bits_to_ASCII(2,x>>2);
    //int_to_ASCII(3,y>>2);
    bits_to_ASCII(3,y>>2);
}

void Uso_CPU(long ciclos){                                                      //Calculamos el porcentaje de tiempo ocupado basandonos en el tiempo libre
    int CPU;
    short centenas,decenas,unidades;
    if(Fosc==80000000) CPU=100*(1-(float)ciclos/(40*TCPU));                     //Dividimos las instrucciones hechas en el tiempo libre entre las instrucciones que da tiempo de hacer en lo que tarda el timer en activar el flag   (100ms)    
    else CPU=100*(1-(float)ciclos/(4*TCPU));                                    //Teniendo en cuenta que la velocidad del oscilador puede ser 8Mhz u 80Mhz 
    
    centenas=CPU/100;                                                           //Convertimos el valor obtenido a un formato que se pueda enviar por la UART y al LCD
    decenas=(CPU-centenas*100)/10;                                              //Es decir, lo convertimos de un int a su equivalente ASCII
    unidades=CPU-centenas*100-decenas*10;                                   
    Ventana_DATOS[n_filas-1-4][13]=centenas+0x30;                               //Guardamos el valor en Ventana_DATOS, teniendo en cuenta en que fila lo queremos guardar                               
    Ventana_DATOS[n_filas-1-4][14]=decenas+0x30;                                //y le ponemos +0x30 para conseguir su equivalente en ASCII
    Ventana_DATOS[n_filas-1-4][15]=unidades+0x30;
}

/****************************************************CAN****************************************************/
/* ECAN1 buffer loaded with Identifiers and Data */
void ecan1WriteMessage(void){

/* Writing the message for Transmission
ecan1WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
ecan1WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

buf -> Transmit Buffer number

txIdentifier -> SID<10:0> : EID<17:0>

ide = 0 -> Message will transmit standard identifier
ide = 1 -> Message will transmit extended identifier

remoteTransmit = 0 -> Normal message
remoteTransmit = 1 -> Message will request remote transmission

dataLength -> Data length can be from 0 to 8 bytes

data1, data2, data3, data4 -> Data words (2 bytes) each

*/

    /*****************PRUEBA BOTON********************/
        int j,pot=0,x=0,y=0;
    for(j=0;j<16;j+=4){                                                         //Ya que leemos 16 valores (4 por entrada)
        temp+=CAD_BuffA[j];                                                     //El pimer valor (j) sera el del sensor de temperatura (0,4,8,12)
 }
        temp=temp/4;
    /*****************************************************/    
	ecan1WriteTxMsgBufId(0,0x1FFEFFFF,0,0);//0 sid
	ecan1WriteTxMsgBufData(0,8,temp,0x2222,0x3333,0x4444);

}

/* ECAN2 buffer loaded with Identifiers and Data */

void ecan2WriteMessage(void){

/* Writing the message for Transmission

ecan2WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
ecan2WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

buf -> Transmit Buffer Number

txIdentifier -> SID<10:0> : EID<17:0>

ide = 0 -> Message will transmit standard identifier
ide = 1 -> Message will transmit extended identifier

remoteTransmit = 0 -> Normal message
remoteTransmit = 1 -> Message will request remote transmission


dataLength -> Data length can be from 0 to 8 bytes

data1, data2, data3, data4 -> Data words (2 bytes) each


*/

ecan2WriteTxMsgBufId(0,0x1FFEFFFF,0,0);
ecan2WriteTxMsgBufData(0,8,0xaaaa,0xbbbb,0xcccc,0xdddd);

}



/******************************************************************************
*                                                                             
*    Function:			rxECAN1
*    Description:       moves the message from the DMA memory to RAM
*                                                                             
*    Arguments:			*message: a pointer to the message structure in RAM 
*						that will store the message. 
*	 Author:            Jatinder Gharoo                                                      
*	                                                                 
*                                                                              
******************************************************************************/
void rxECAN1(mID *message)
{
	unsigned int ide=0;
	unsigned int srr=0;
	unsigned long id=0,d;
			
	/*
	Standard Message Format: 
	Word0 : 0bUUUx xxxx xxxx xxxx
			     |____________|||
 					SID10:0   SRR IDE(bit 0)     
	Word1 : 0bUUUU xxxx xxxx xxxx
			   	   |____________|
						EID17:6
	Word2 : 0bxxxx xxx0 UUU0 xxxx
			  |_____||	     |__|
			  EID5:0 RTR   	  DLC
	word3-word6: data bytes
	word7: filter hit code bits
	
	Substitute Remote Request Bit
	SRR->	"0"	 Normal Message 
			"1"  Message will request remote transmission
	
	Extended  Identifier Bit			
	IDE-> 	"0"  Message will transmit standard identifier
	   		"1"  Message will transmit extended identifier
	
	Remote Transmission Request Bit
	RTR-> 	"0"  Message transmitted is a normal message
			"1"  Message transmitted is a remote message
	*/
	/* read word 0 to see the message type */
	ide=ecan1msgBuf[message->buffer][0] & 0x0001;	
	srr=ecan1msgBuf[message->buffer][0] & 0x0002;	
	
	/* check to see what type of message it is */
	/* message is standard identifier */
	if(ide==0)
	{
		message->id=(ecan1msgBuf[message->buffer][0] & 0x1FFC) >> 2;		
		message->frame_type=CAN_FRAME_STD;
	}
	/* mesage is extended identifier */
	else
	{
		id=ecan1msgBuf[message->buffer][0] & 0x1FFC;		
		message->id=id << 16;
		id=ecan1msgBuf[message->buffer][1] & 0x0FFF;
		message->id=message->id+(id << 6);
		id=(ecan1msgBuf[message->buffer][2] & 0xFC00) >> 10;
		message->id=message->id+id;		
		message->frame_type=CAN_FRAME_EXT;
	}
	/* check to see what type of message it is */
	/* RTR message */
	if(srr==1)
	{
		message->message_type=CAN_MSG_RTR;	
	}
	/* normal message */
	else
	{
		message->message_type=CAN_MSG_DATA;
		message->data[0]=(unsigned char)ecan1msgBuf[message->buffer][3];
		message->data[1]=(unsigned char)((ecan1msgBuf[message->buffer][3] & 0xFF00) >> 8);
		message->data[2]=(unsigned char)ecan1msgBuf[message->buffer][4];
		message->data[3]=(unsigned char)((ecan1msgBuf[message->buffer][4] & 0xFF00) >> 8);
		message->data[4]=(unsigned char)ecan1msgBuf[message->buffer][5];
		message->data[5]=(unsigned char)((ecan1msgBuf[message->buffer][5] & 0xFF00) >> 8);
		message->data[6]=(unsigned char)ecan1msgBuf[message->buffer][6];
		message->data[7]=(unsigned char)((ecan1msgBuf[message->buffer][6] & 0xFF00) >> 8);
		message->data_length=(unsigned char)(ecan1msgBuf[message->buffer][2] & 0x000F);
	}	
}


/******************************************************************************
*                                                                             
*    Function:			rxECAN2
*    Description:       moves the message from the DMA memory to RAM
*                                                                             
*    Arguments:			*message: a pointer to the message structure in RAM 
*						that will store the message. 
*	 Author:            Jatinder Gharoo                                                      
*	                                                                 
*                                                                              
******************************************************************************/
void rxECAN2(mID *message)
{
	unsigned int ide=0;
	unsigned int srr=0;
	unsigned long id=0,a1;
			
	/*
	Standard Message Format: 
	Word0 : 0bUUUx xxxx xxxx xxxx
			     |____________|||
 					SID10:0   SRR IDE(bit 0)     
	Word1 : 0bUUUU xxxx xxxx xxxx
			   	   |____________|
						EID17:6
	Word2 : 0bxxxx xxx0 UUU0 xxxx
			  |_____||	     |__|
			  EID5:0 RTR   	  DLC
	word3-word6: data bytes
	word7: filter hit code bits
	
	Substitute Remote Request Bit
	SRR->	"0"	 Normal Message 
			"1"  Message will request remote transmission
	
	Extended  Identifier Bit			
	IDE-> 	"0"  Message will transmit standard identifier
	   		"1"  Message will transmit extended identifier
	
	Remote Transmission Request Bit
	RTR-> 	"0"  Message transmitted is a normal message
			"1"  Message transmitted is a remote message
	*/
	/* read word 0 to see the message type */
	ide=ecan2msgBuf[message->buffer][0] & 0x0001;	
	srr=ecan2msgBuf[message->buffer][0] & 0x0002;	
	
	/* check to see what type of message it is */
	/* message is standard identifier */
	if(ide==0)
	{
		message->id=(ecan2msgBuf[message->buffer][0] & 0x1FFC) >> 2;		
		message->frame_type=CAN_FRAME_STD;
	}
	/* mesage is extended identifier */
	else
	{
		id=ecan2msgBuf[message->buffer][0] & 0x1FFC;		
		message->id=id << 16;
		id=ecan2msgBuf[message->buffer][1] & 0x0FFF;
		message->id=message->id+(id << 6);
		id=(ecan2msgBuf[message->buffer][2] & 0xFC00) >> 10;
		message->id=message->id+id;		
		message->frame_type=CAN_FRAME_EXT;
	}
	/* check to see what type of message it is */
	/* RTR message */
	if(srr==1)
	{
		message->message_type=CAN_MSG_RTR;	
	}
	/* normal message */
	else
	{
		message->message_type=CAN_MSG_DATA;
		message->data[0]=(unsigned char)ecan2msgBuf[message->buffer][3];
		message->data[1]=(unsigned char)((ecan2msgBuf[message->buffer][3] & 0xFF00) >> 8);
		message->data[2]=(unsigned char)ecan2msgBuf[message->buffer][4];
		message->data[3]=(unsigned char)((ecan2msgBuf[message->buffer][4] & 0xFF00) >> 8);
		message->data[4]=(unsigned char)ecan2msgBuf[message->buffer][5];
		message->data[5]=(unsigned char)((ecan2msgBuf[message->buffer][5] & 0xFF00) >> 8);
		message->data[6]=(unsigned char)ecan2msgBuf[message->buffer][6];
		message->data[7]=(unsigned char)((ecan2msgBuf[message->buffer][6] & 0xFF00) >> 8);
		message->data_length=(unsigned char)(ecan2msgBuf[message->buffer][2] & 0x000F);
	}	
}





void clearIntrflags(void){
/* Clear Interrupt Flags */

	IFS0=0;
	IFS1=0;
	IFS2=0;
	IFS3=0;
	IFS4=0;
}


//void oscConfig(void){
//
///*  Configure Oscillator to operate the device at 40Mhz
// 	Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
// 	Fosc= 8M*40/(2*2)=80Mhz for 8M input clock */
//
//	PLLFBD=38;					/* M=40 */
//	CLKDIVbits.PLLPOST=0;		/* N1=2 */
//	CLKDIVbits.PLLPRE=0;		/* N2=2 */
//	OSCTUN=0;					/* Tune FRC oscillator, if FRC is used */
//
///* Disable Watch Dog Timer */
//
//	RCONbits.SWDTEN=0;
//
///* Clock switch to incorporate PLL*/
//	__builtin_write_OSCCONH(0x03);		// Initiate Clock Switch to Primary
//													// Oscillator with PLL (NOSC=0b011)
//	__builtin_write_OSCCONL(0x01);		// Start clock switching
//	while (OSCCONbits.COSC != 0b011);	// Wait for Clock switch to occur	
//
//
///* Wait for PLL to lock */
//
//	while(OSCCONbits.LOCK!=1) {};
//}


void __attribute__((interrupt, no_auto_psv))_C1Interrupt(void)  
{    
	IFS2bits.C1IF = 0;        // clear interrupt flag
	if(C1INTFbits.TBIF)
    { 
    	C1INTFbits.TBIF = 0;
    } 
 
    if(C1INTFbits.RBIF)
    {      
		// read the message 
	    if(C1RXFUL1bits.RXFUL1==1)
	    {
	    	rx_ecan1message.buffer=1;
	    	C1RXFUL1bits.RXFUL1=0;
	    }	    
	    rxECAN1(&rx_ecan1message); 	    	    
		C1INTFbits.RBIF = 0;
        /*******FLAG RX********/
        flag_RX=1;
	}
}


void __attribute__((interrupt, no_auto_psv))_C2Interrupt(void)  
{
	IFS3bits.C2IF = 0;        // clear interrupt flag
	if(C2INTFbits.TBIF)
    { 
		C2INTFbits.TBIF = 0;
    } 
    
    if(C2INTFbits.RBIF)
     {      
		// read the message 
	    if(C2RXFUL1bits.RXFUL1==1)
	    {
	    	rx_ecan2message.buffer=1;
	    	C2RXFUL1bits.RXFUL1=0;
	    }	    
	    rxECAN2(&rx_ecan2message); 	    	    
		C2INTFbits.RBIF = 0;
     }
}
 

//------------------------------------------------------------------------------
//    DMA interrupt handlers
//------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
   IFS0bits.DMA0IF = 0;          // Clear the DMA0 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void)
{
   IFS0bits.DMA1IF = 0;          // Clear the DMA1 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{
   IFS1bits.DMA2IF = 0;          // Clear the DMA2 Interrupt Flag;
}

void __attribute__((interrupt, no_auto_psv)) _DMA3Interrupt(void)
{
   IFS2bits.DMA3IF = 0;          // Clear the DMA3 Interrupt Flag;
}
/****************************************************CAN****************************************************/
