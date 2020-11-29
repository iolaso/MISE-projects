/*
 * Proyecto: MISE Micros 16bits 
 * File:   main.c
 * Grupo: G5    Autor: Iñigo Olaso y Xabi Olazar
 * Fecha: 2017-12-11
*/

#include "oscilador_v2.h"
#include "p24hj256gp610A.h"
#include "UART.h"
#include "LCD_Explorer16_Gx_vx.h"
#include "IO_Explorer16_G0_v0.h"
#include "xc.h"
#include "main.h"
#include "Tiempos.h"
#include "CAD.h"


unsigned char Ventana_Datos [8][19] __attribute__((space(dma)));
char Ventana_LCD [n_filas][n_columnas];
int leds_counter;


const char Texto_1[]={ "G5:T=--C Pot=---"
                       "Tiempo: --- --- "};

unsigned char respuesta[9];
unsigned int Flag_trama_ok;
unsigned int modulo_CAD;
unsigned int DMA_CAD;
unsigned int DMA_MODE_CAD;
unsigned int modulo_TIM;
unsigned int DMA_TIM;
unsigned int DMA_MODE_TIM;
unsigned int DMA_UART;
unsigned int DMA_MODE_UART;

int flag_osc_80;

char Texto_2[] = {"==== I.X.G5 ====""\x0d\x0a\x0"
                  "TCPU:000 000 000""\x0d\x0a\x0"
                  "---- PIC24H ----""\x0d\x0a\x0"
                  "Potenciomet ____""\x0d\x0a\x0"
                  "Temperatura ____""\x0d\x0a\x0"
                  "Joystick X _____""\x0d\x0a\x0"
                  "Joystick Y _____""\x0d\x0a\x0"
                  "cmd: ---------  ""\x1b[H"};

    
unsigned short tiempo = 0;
long unsigned int contador_tiempo;
long unsigned int a,b,c,d,e,f,g,h,i,j,k,l,calculo_tiempo,n_muestreos;
int count = 0;
int count2 = 0;
long int ADCValue,ADCValue2,angulo;
int flag_CAD1=1;

void Copia_FLASH_Ventana_Datos(char *texto) {

    int i,j,k;
    k=0;
    for (i=0; i<8; i++){
        
        for (j=0; j<19; j++){
            
            Ventana_Datos [i][j]= texto[k];
            k++;

        }
    }     
}


void leds(void){


static int flag_leds_shifter,flag_leds_toggle;

    if(!_RD6){
        LATA = 0;
        count=0;
        flag_leds_shifter=1;
        flag_leds_toggle=0;

    }
     if(!_RD7){
         LATA = 0;
         count2=0;
         flag_leds_toggle=1;
         flag_leds_shifter=0;

    }
     if(!_RB3){
         count2=0;
         flag_leds_toggle=0;
         flag_leds_shifter=0;         
         LATA = 0xFF;


    }
    if(leds_counter >= 250){
        
        if (flag_leds_toggle==1){
            
        if (count2==0){  LATA = 0b0000000010000001; count2 ++;}//LATA=1;//
        else if (0<count2<=4){LATA = (LATA&0b0000000011110000)>>1 | (LATA&0b0000000000001111) << 1; count2 ++;}
        if( count2 >= 5) { LATA=0x00FF; }
        if (count2 >=6)count2 = 0;
        }
        else if( flag_leds_shifter == 1){
        if (count==0){  _LATA0 = 1; count ++;}//LATA=1;//
        else if (0<count<=7){LATA <<=  1; count ++;}
        if(count >= 9) { LATA=1; count = 1;}
        //if (count < 7) count ++;  
        }
    leds_counter=0;
    }
        if(!_RD13){
         flag_leds_toggle=0;
         flag_leds_shifter=0; 
         LATA=0;
    }
     
}


void Copia_FLASH_Ventana_LCD(const char *texto){
    
    
    int i,j,k;
    k=0;
    for (i=0; i<2; i++){
        
        for (j=0; j<16; j++){
            
            Ventana_LCD [i][j]= texto[k];
            k++;
        }
    }

}


void OSC_TO_8 (void){
    
static char flags[2][4]={""};
long unsigned int Fosc,Fcy,BAUDRATE2,BAUD_RATEREG_2_BRGH1,BAUD_RATEREG_2_BRGH0;

    flag_osc_80=0;
    ///////////Timers////////////
    flags[0][0] = _Timer_mili_IE;//Timer 6
    flags[0][1] = _T8IE;//Timer 8
    flags[0][2] = _T3IE;//Timer 2 Y 3
    //flags[0][3] = _T1IE;//Timer 1

    _Timer_mili_IE=0;
    _T8IE=0;
    _T3IE=0;
    //_T1IE=0;

    /////////////UART///////////////
     flags[1][0] = _UARTEN_U2 ; // Habilita la UART_2
     flags[1][1] = _UTXEN_U2 ; //Habilita transmisión,
     flags[1][2] = IEC0bits.DMA0IE ;        // Habilita interrupciones DMA 0 
     flags[1][3] = DMA0CONbits.CHEN ;   // Enable canal 0 DMA 

     _UARTEN_U2 = 0; // Habilita la UART_2
     _UARTEN_U2 = 0;
     _UTXEN_U2 = 0;
     _U2RXIE = 0; 
     DMA0CONbits.CHEN = 0;
     IEC0bits.DMA0IE = 0;

    ///////////////CAMBIO DE OSC///////////////
    RCONbits.SWDTEN = 0;
    PLLFBD = 0x0030;         

    CLKDIVbits.PLLPOST = 0; // N2
    CLKDIVbits.PLLPRE = 0;  // N1
    CLKDIVbits.DOZE = 0x03;  // N1
    CLKDIVbits.PLLPOST=0x01;

    __builtin_write_OSCCONH(0x02);	// Initiate Clock Switch to Primary
    __builtin_write_OSCCONL(0x01);	// Start clock switching
      
    while (OSCCONbits.COSC != 0b010);// Wait fot Clock switch to occur///////////////////////////////////////////

    /////////////UART///////////////
     Fosc = 8000000 ;
     Fcy = Fosc/2;
     BAUDRATE2 = 9600; // baudios (bits/sg) de transmision
     BAUD_RATEREG_2_BRGH1 = ((Fcy/BAUDRATE2)/4)-1; // Para BRGH = 1
     BAUD_RATEREG_2_BRGH0 = ((Fcy/BAUDRATE2)/16)-1; // Para BRGH = 0
     U2BRG = BAUD_RATEREG_2_BRGH1;
     _UARTEN_U2 = 1; // Habilita la UART_2
     _U2RXIE = 1;
     Nop();
     Nop();
     Nop();
     Nop();
     Nop();

     _UARTEN_U2 = flags[1][0]; // Habilita la UART_2
     _UTXEN_U2 = flags[1][1]; //Habilita transmisión,
     DMA0CONbits.CHEN = flags[1][3];   // Enable canal 0 DMA 
     IEC0bits.DMA0IE = flags[1][2];        // Habilita interrupciones DMA 0 
     DMA0REQbits.FORCE = 1;

    ///////////Timers////////////

    //TIMER 2 3//

      PR2 = 40000;
      PR3 = 0;

     //TIMER 1//
     PR1 = 12000-1;

    //TIMER 8//
     PR8 = 4000-1;
    //TIMER 6//
     PR_Timer_mili = 4000-1;	


     _Timer_mili_IE = flags[0][0]; //Timer 6
     _T8IE = flags[0][1];//Timer 8
     _T3IE = flags[0][2];//Timer 2 Y 3
     //_T1IE = flags[0][3];//Timer 8

}

void OSC_TO_80 (void){
    
static char flags[2][4]={""};
long unsigned int Fosc,Fcy,BAUDRATE2,BAUD_RATEREG_2_BRGH1,BAUD_RATEREG_2_BRGH0;
    flag_osc_80=1;
    ///////////Timers////////////
    flags[0][0] = _Timer_mili_IE;//Timer 6
    flags[0][1] = _T8IE;//Timer 8
    flags[0][2] = _T3IE;//Timer 2 Y 3
    flags[0][3] = _T1IE;//Timer 2 Y 3

    _Timer_mili_IE=0;
    _T8IE=0;
    _T3IE=0;
    _T1IE=0;

    /////////////UART///////////////
     flags[1][0] = _UARTEN_U2 ; // Habilita la UART_2
     flags[1][1] = _UTXEN_U2 ; //Habilita transmisión,
     flags[1][2] = IEC0bits.DMA0IE ;        // Habilita interrupciones DMA 0 
     flags[1][3] = DMA0CONbits.CHEN ;   // Enable canal 0 DMA 

     _UARTEN_U2 = 0; // Habilita la UART_2
     _UARTEN_U2 = 0;
     _UTXEN_U2 = 0;
     _U2RXIE = 1;
     DMA0CONbits.CHEN = 0;
     IEC0bits.DMA0IE = 0;

    ///////////////CAMBIO DE OSC///////////////
       Inic_Oscilador ();
    ///////////////////////////////////////////

    /////////////UART///////////////
     Fosc = 80000000 ;
     Fcy = Fosc/2;
     BAUDRATE2 = 9600; // baudios (bits/sg) de transmision
     BAUD_RATEREG_2_BRGH1 = ((Fcy/BAUDRATE2)/4)-1; // Para BRGH = 1
     BAUD_RATEREG_2_BRGH0 = ((Fcy/BAUDRATE2)/16)-1; // Para BRGH = 0
     U2BRG = BAUD_RATEREG_2_BRGH1;
     _UARTEN_U2 = 1; // Habilita la UART_2
     _U2RXIE = 1;
     Nop();
     Nop();
     Nop();
     Nop();
     Nop();

     _UARTEN_U2 = flags[1][0]; // Habilita la UART_2
     _UTXEN_U2 = flags[1][1]; //Habilita transmisión,
     DMA0CONbits.CHEN = flags[1][3];   // Enable canal 0 DMA 
     IEC0bits.DMA0IE = flags[1][2];        // Habilita interrupciones DMA 0 
     DMA0REQbits.FORCE = 1;

    ///////////Timers////////////

    //TIMER 2 3//

     PR2 = 0x1a80;
     PR3 = 0x0006;

    //TIMER 1//
     PR1 = 120000-1;
    //TIMER 8//
     PR8 = 40000-1;
    //TIMER 6//
     PR_Timer_mili = 40000-1;	

     _Timer_mili_IE = flags[0][0]; //Timer 6
     _T8IE = flags[0][1];//Timer 8
     _T3IE = flags[0][2];//Timer 2 Y 3
     _T1IE = flags[0][3];//Timer 2 Y 3
}


void CPU_cronometro(void){
    
    while(!_T1IF){
        contador_tiempo++;
    }
    
    if(flag_osc_80==1)contador_tiempo = contador_tiempo*5*25;
    else if(flag_osc_80==0) contador_tiempo *= 5*250;        

    contador_tiempo = 3000000 - contador_tiempo;
    
    d = contador_tiempo/100000000;
    e = (contador_tiempo-(100000000*d))/10000000;//(contador_tiempo-(100*a))/10;
    f = (contador_tiempo-(100000000*d)-(10000000*e))/1000000;//contador_tiempo-(100*a)-(10*b);
 
    g = (contador_tiempo-(100000000*d)-(10000000*e)-f*1000000)/100000;//(contador_tiempo - a*100000)/1000;
    h = (contador_tiempo-(100000000*d)-(10000000*e)-f*1000000-g*100000)/10000;
    i = (contador_tiempo-(100000000*d)-(10000000*e)-f*1000000-g*100000-h*10000)/1000;

    j = (contador_tiempo-(100000000*d)-(10000000*e)-f*1000000-g*100000-h*10000-i*1000)/100;
    k = (contador_tiempo-(100000000*d)-(10000000*e)-f*1000000-g*100000-h*10000-i*1000-j*100)/10;//(contador_tiempo-(100*a))/10;
    l = (contador_tiempo-(100000000*d)-(10000000*e)-f*1000000-g*100000-h*10000-i*1000-j*100-k*10);//contador_tiempo-(100*a)-(10*b);    

    Ventana_Datos[1][5] = d + '0';
    Ventana_Datos[1][6] = e + '0';
    Ventana_Datos[1][7] = f + '0';
   
    Ventana_Datos[1][9] = g + '0';
    Ventana_Datos[1][10] = h + '0';
    Ventana_Datos[1][11] = i + '0';
    
    Ventana_Datos[1][13] = j + '0';
    Ventana_Datos[1][14] = k + '0';
    Ventana_Datos[1][15] = l + '0';
    calculo_tiempo=0;

    contador_tiempo = 0;

    _T1IF=0;  
}


int main(void) {
    
    long int ADCValue;                                      
    int Flag_CAD_DMA = 1;   
    int Flag_SCATTER=0;
    
    
    flag_osc_80=0;
    AD1PCFGL= 0xFFFF;   // Pone todas las patas analógicas de I/O digital
    AD1PCFGH= 0xFFFF;  
   Inic_Pulsadores();
   while(_RA7);
   if(_RD13){
   Inic_Oscilador ();
   flag_osc_80=1;

   } 
   
   else if(!_RD13){
       
       flag_osc_80=0;
     
       RCONbits.SWDTEN = 0;
       PLLFBD = 0x0030;         
       
       	CLKDIVbits.PLLPOST = 0; // N2
        CLKDIVbits.PLLPRE = 0;  // N1
        CLKDIVbits.DOZE = 0x03;  // N1
        CLKDIVbits.PLLPOST=0x01;

        __builtin_write_OSCCONH(0x02);	// Initiate Clock Switch to Primary       	
       __builtin_write_OSCCONL(0x01);	// Start clock switching
      
      while (OSCCONbits.COSC != 0b010);// Wait fot Clock switch to occur
   }
  
   //Inicialicaciones//
   Inic_Leds(); //inic LEDS OFF
   Copia_FLASH_Ventana_LCD(Texto_1);
   Copia_FLASH_Ventana_Datos(Texto_2);
   Inic_LCD ();
   Inic_Tiempo_Miliseg();//inic TIMER
   initRS232_2_DMA();//inic UART DMA
   inic_RS232_2(); //inic UART
   inic_OFF_ADC2();//inic ADC2 DMA OFF
   Inic_CAD_DMA();//inic ADC1 DMA OFF
   AD1CON1bits.SAMP	=	1; 
   Inic_Timer1();

while(1){

    /************CAD_DMA************/
    if(Flag_CAD_DMA == 1){
    
                                    if(Flag_SCATTER && flag_CAD1) ADCValue	=	(CAD_BuffA[8]+CAD_BuffA[9])/2;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    else if(Flag_SCATTER && !flag_CAD1) ADCValue	=	CAD2_BuffA[0];
                                    else if(!Flag_SCATTER && flag_CAD1) ADCValue	=	CAD_BuffA[0];
                                    else if(!Flag_SCATTER && !flag_CAD1) ADCValue	=	CAD2_BuffA[0];
                                    angulo= ((ADCValue*125)/1790.25);
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[4][13]= a+'0';
                                    Ventana_Datos[4][14]=b+'0';
                                    Ventana_Datos[4][15]=c+'0';
                                    
                                    
                                    if(Flag_SCATTER && flag_CAD1) ADCValue	=	(CAD_BuffA[10]+CAD_BuffA[11])/2;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    else if(Flag_SCATTER && !flag_CAD1) ADCValue	=	CAD2_BuffA[1];
                                    else if(!Flag_SCATTER && flag_CAD1) ADCValue	=	CAD_BuffA[1];
                                    else if(!Flag_SCATTER && !flag_CAD1) ADCValue	=	CAD2_BuffA[1];
                                    angulo= (ADCValue*180)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[3][13]= a+'0';
                                    Ventana_Datos[3][14]=b+'0';
                                    Ventana_Datos[3][15]=c+'0';
                                    
                                    if(Flag_SCATTER && flag_CAD1) ADCValue	=	(CAD_BuffA[2]+CAD_BuffA[3])/2;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    else if(Flag_SCATTER && !flag_CAD1) ADCValue	=	CAD2_BuffA[8];
                                    else if(!Flag_SCATTER && flag_CAD1) ADCValue	=	CAD_BuffA[2];
                                    else if(!Flag_SCATTER && !flag_CAD1) ADCValue	=	CAD2_BuffA[2];
                                    angulo= (ADCValue*45)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[5][13]= a+'0';
                                    Ventana_Datos[5][14]=b+'0';
                                    Ventana_Datos[5][15]=c+'0';
   								
                                    if(Flag_SCATTER && flag_CAD1) ADCValue	=	(CAD_BuffA[0]+CAD_BuffA[1])/2;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    else if(Flag_SCATTER && !flag_CAD1) ADCValue	=	CAD2_BuffA[9];
                                    else if(!Flag_SCATTER && flag_CAD1) ADCValue	=	CAD_BuffA[3];
                                    else if(!Flag_SCATTER && !flag_CAD1) ADCValue	=	CAD2_BuffA[3];                              
                                    angulo= (ADCValue*45)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[6][13]= a+'0';
                                    Ventana_Datos[6][14]=b+'0';
                                    Ventana_Datos[6][15]=c+'0';
                                                    

    }            
    /************UART_RX************/
    if(Flag_trama_ok == 1){
        
                Ventana_Datos[7][5] = respuesta[0];
                Ventana_Datos[7][6] = respuesta[1];
                Ventana_Datos[7][7] = respuesta[2];
                Ventana_Datos[7][8] = respuesta[3];
                Ventana_Datos[7][9] = respuesta[4];
                Ventana_Datos[7][10] = respuesta[5];
                Ventana_Datos[7][11] = respuesta[6];
                Ventana_Datos[7][12] = respuesta[7];
                Ventana_Datos[7][13] = respuesta[8];

        ///////////////////////////   CAD-X-Y-M:  ///////////////////////////
        if((respuesta[0]=='C') && (respuesta[1]=='A') && (respuesta[2]=='D') ){
       
            /////////////   X   ///////////////
            if (respuesta[4] == '1' || respuesta[4] == '2') {
            
                if (respuesta[4] == '1') flag_CAD1=1;
                else flag_CAD1=0;


                /////////////   Y   ///////////////
                if (respuesta[6] == '2' || respuesta[6] == '3'){       
            
            
                    AD2CON1bits.ADON = 0;
                    _ADON = 0;
                    _AD1IE	=	0; // Deshabilita int.   
                    IEC1bits.AD2IE = 0;
                    IEC1bits.DMA2IE = 0; 
                    DMA2CONbits.CHEN = 0;

                /////////////   M   ///////////////
                if (respuesta[8] == '1'){      DMA2CONbits.MODE = 0b11;          
                } // 11: Un bloque + PingPong  1
                else if (respuesta[8] == '2'){  DMA2CONbits.MODE = 0b10;   
                }// 10: Continuo + PingPong 
                else if (respuesta[8] == '3'){  DMA2CONbits.MODE = 0b01;   
                }// 01: Un bloque 
                else if (respuesta[8] == '4'){  DMA2CONbits.MODE = 0b00;  
                }// 00: Continuo                                                    

                /////////////   Y   /////////////// DMA
                if (respuesta[6] == '2'){
                    if (respuesta[4] == '1') _ADDMABM = 1;
                    if (respuesta[4] == '2') AD2CON1bits.ADDMABM = 1;
                    Flag_SCATTER=0;
                }///////////DMA SCATTER
                else if  (respuesta[6] == '3' ){
                    if (respuesta[4] == '1') _ADDMABM = 0;
                    if (respuesta[4] == '2') AD2CON1bits.ADDMABM = 0;
                    Flag_SCATTER=1;
                }

                if (respuesta[4] == '1'){
                    _ASAM = 1;
                    _CSCNA = 1;
                    _SMPI	=	4-1;
                    _CH0SA =	4; 
                     ///DMA cambios para ADC1/////
                    DMA2REQbits.IRQSEL = 0x0D;//13;// ADC1 //DMA_IRQ_ADC1
                    DMA2STA = __builtin_dmaoffset(CAD_BuffA);
                    DMA2PAD = (volatile unsigned int)&ADC1BUF0;
                    _ADON = 1; //Habilita convertidor
                }
                else if (respuesta[4] == '2'){

                    AD2CON1bits.ASAM = 1;
                    AD2CON2bits.CSCNA = 1;
                    AD2CON2bits.SMPI = 4-1;
                    AD2CHS0bits.CH0SA = 4;
                    ///DMA cambios para ADC2/////
                    DMA2REQbits.IRQSEL = 0x15; // 0x15 ADC2 //DMA_IRQ_ADC2
                    DMA2STA = __builtin_dmaoffset(CAD2_BuffA);
                    DMA2PAD = (volatile unsigned int)&ADC2BUF0;
                    AD2CON1bits.ADON = 1;  
                } 

                    DMA2CONbits.CHEN = 1;  
                    IEC1bits.DMA2IE = 1;
                    Flag_CAD_DMA=1;

                } ///DMA cambios para Interrupt
                else if (respuesta[6] == '1' ){ 

                    AD2CON1bits.ADON = 0;
                    _ADON = 0;
                    _AD1IE	=	0; // Deshabilita int.   
                    IEC1bits.AD2IE = 0;
                    IEC1bits.DMA2IE = 0;
                    DMA2CONbits.CHEN = 0; 

                    if (respuesta[4] == '1'){

                    _ADDMABM = 0;
                    _ASAM = 0;
                    _CSCNA = 0;
                    _SMPI	=	0;

                    _CH0SA =	4; 
                    //AD1CON1bits
                    _ADON = 1; //Habilita convertidor
                    _AD1IE = 1;
                    AD1CON1bits.SAMP = 1;

                    }

                    else if (respuesta[4] == '2'){

                    AD2CON1bits.ADDMABM = 0;
                    AD2CON1bits.ASAM = 0;
                    AD2CON2bits.CSCNA = 0;
                    AD2CON2bits.SMPI = 0;

                    IFS1bits.AD2IF = 0;
                    IEC1bits.AD2IE = 1;
                    AD2CON1bits.ADON = 1;
                    AD2CON1bits.SAMP = 1;

                    }

                    Flag_CAD_DMA=0;
                }
            }
        }
    
        ///////////////////////////   TIM-X----:  ///////////////////////////
        else if(respuesta[0]=='T' && respuesta[1]=='I' && respuesta[2]=='M' ){

            if (respuesta[4] == '1' ){Inic_Tiempo_Miliseg();  Ventana_LCD[0][0]='Z';
             }
            else if( respuesta[4] == '2' ) { Inic_Timer8(); Ventana_LCD[0][0]='Y';
             }
            else if( respuesta[4] == '3' ) { Inic_Timer_32(); Ventana_LCD[0][0]='X';
            }

        }

        ///////////////////////////   UAR---Y-M:  ///////////////////////////
        else if(respuesta[0]=='U' && respuesta[1]=='A' && respuesta[2]=='R' ){

            DMA_UART = respuesta[6]+'0';
            DMA_MODE_UART = respuesta[8]+'0';

            if ( respuesta[6] == '1' ){ _U2TXIE = 0; // Habilita int. de transmisión
                DMA0CONbits.CHEN = 0; // Enable canal 0 DMA  
                IEC0bits.DMA0IE = 0;
                IFS0bits.DMA0IF = 0;
                DMACS0 = 0;         // Borra flags de Colisiones 
                INTCON1bits.DMACERR = 0;    // Borraa Flag de interrupción de colisiones 

                    if (respuesta[8] == '1'){  DMA0CONbits.MODE = 0b11;          
                    } // 11: Un bloque + PingPong  1
                    else if (respuesta[8] == '2'){  DMA0CONbits.MODE = 0b10;   
                    }// 10: Continuo + PingPong 2
                    else if (respuesta[8] == '3'){  DMA0CONbits.MODE = 0b01;   
                    }// 01: Un bloque 3
                    else if (respuesta[8] == '4'){  DMA0CONbits.MODE = 0b00;  
                    }// 00: Continuo 4                                                    
                    DMA0CONbits.CHEN = 1; IEC0bits.DMA0IE = 1; DMA0REQbits.FORCE = 1;
                }

            else if( respuesta[6] == '2' ){ 
            /////////////////////////////// Forzar el cambio para los casos de un bloque y un bloque ping pong//////////////////////////////////  
            IEC0bits.DMA0IE = 0; DMA0CONbits.CHEN = 1; DMA0CONbits.MODE = 0b00; DMA0REQbits.FORCE = 1;   DMA0CONbits.CHEN = 0; // Enable canal 0 DMA    
            _U2TXIE = 1; // Habilita int. de transmisión
            }        

        } 
        else if((respuesta[0]=='O') && (respuesta[1]=='S') && (respuesta[2]=='C') ){

            if((respuesta[4]=='1')) OSC_TO_8();
            else if((respuesta[4]=='2')) OSC_TO_80();
        //Inic_Timer1 ();
        }
        Flag_trama_ok=0;
         _U2RXIE = 1;
    }
    
    /************LEDS************/
    leds();
    
    /************CPU_cronometro************/
    CPU_cronometro();
   }
       
   return 0;
}
