#include "p24hj256gp610A.h"
#include "main.h"

void	Inic_CAD	(void)	 //		
{	
//CONFIGURAMOS	EL	ADC1	PARA	MUESTREAR	AN2,AN3,AN4,AN5,	por	canal	CH0	
AD1CON1=	0;	 	 	 //	0x00E0	
//_ADON=	0;		 	 	 //	CAD	OFF	
//_ADSIDL	=	0;	 	 	 //1=Se	para	en	modo	Idle	/	0=Continua	en	modo	Idle	
//_ADDMABM	=	0;	 	 //1=bufer	DMA	en	orden	de	conversión	
																 	 	 	 	 //	0=bufer	DMA	en	modo	Scatter/Gather	
//_AD12B	=	0;		 	 	 //1=Modo	operacion	de	12	bits	/	0=Modo	operacion	de	10	bits		
//_FORM	=	0;		 	 	 //Formato	datos	salida:	00=Integer	(0000	00dd	dddd	dddd)	
																																						 //	01=Signed	integer	(ssss	sssd	dddd	dddd)	
_SSRC	=	0b111;			 //Termina	muestreo	y	comienza	conversion	del	CAD	
	 	 	 	 	 	 //111=Auto?convert	/	110,	101,	100,	011	RESERVADOS	
																		 	 	 //	010=TMR3		ADC1	y	TMR5	ADC2	
																 	 	 	 //	001=INT0		
																	 	 	 	 //	000=	hay	que	borrar	SAMP	
//_SIMSAM	=	0;		//1=Muestrea	simultaneamente	
																 	 	 	 //	0=Muestrea	canales	individuales	secuencialmente	
//_ASAM	=	1;		 //1=Muestreo	comienza	seguido	de	la	conversion	
																 	 		 //	0=comienza	cuando	SAMP=1	
//_SAMP	 	
//_DONE	
AD1CON2=	0;	 //		
//_VCFG	=	0;		 //Vref+?	000=	AVdd	y	AVss		
																	 	 	 //	001=Vref+	y	AVss	
																	 	 	 //	010=AVdd	y	Vref?		
																	 	 	 //	011=Vref+	y	Vref?	
//_CSCNA	=	0;	 //1=Escaneo	canal	cero	/	0=	no	escaneo	
//_CHPS	=	0;		 	 //Canales	utilizados:00=CH0	/01=CH0	y	CH1	/1x=	CH0,CH1,CH2	y	CH3	
//_BUFS	 es	solo	de	lectura:	con	BUFM=1?>	1=activa	2º	mitad	de	buffer	
																	 	 	 //	0=	activa	1ª	mitad	
//_SMPI	=	0;		 //Incrementos	dir	DMA	+1	o	
																	 	 	 //	sin	DMA	genera	una	interrupción	cada	cada	SMPI+1	
																			 //_BUFM	=	0;		 //1=	2	medios	Buffers	de	8	/	0=	1	buffer	de	16	
//_ALTS	=	0;		 //Modo	de	muestreo:	1=	Alternado	/	0=	Siempre	canal	A	
//AD1CON3	
_ADRC	=	0;		 	 //1=	reloj	RC	/	0=	Reloj	CPU	
_SAMC	=	31;	 //Tiempo	auto	muestreo0	nºde	Tad	(de	0	a	31Tad)	
_ADCS	=	3;		 	 //Tad=	>75ns	=	Tcy(ADCS+1)	?>	a	80MHz	Tcy=	25ns		
AD1CON4=	0;	
//_DMABL	=	3;		 //Cada	buffer	contiene	8	palabras		
AD1CHS123=	0;		 //	reg.	seleccion	de	entrada	canal	1,2	y	3		
//_CH123NA=	0;	 	 //Selección	entrada	negativa	Canales	1,	2,	y	3	pata	Sample	A		
	 	 	 	 	 	 //0x=	CH1=CH2=CH3?>	Vref?	/	10=	CH1=AN6,CH2=AN7,CH3=AN8	
																 	 	 	 //	11=	CH1=AN9,CH2=AN10,CH3=AN11	
//_CH123SA=	0;		 	 //Selección	entrada	positiva	Canales	1,	2,	y	3	para	Sample	A	
	 	 	 	 	 	 //1=	AN3?AN4?AN5	 /	0=	AN0?AN1?AN2	
//_CH123NB	=	0;	
//_CH123SB	=	0;	
AD1CHS0=	0;	 //	registro	de	seleccion	de	entrada	canal	0	
//_CH0NA	=	0;		 //Entrada	negativa	1=		AN1	/	0	=	Vref?	
_CH0SA	=	4;	 	 //Entrada	positiva	=	canal(CH0SA)	
//_CH0SB	=	3;	
//_CH0NB	=	0;		
AD1CSSH	=	0x0000;	 //Selección	entradas	escaneo	de	la	16	a	la	31
AD1CSSL	=	0x0330;		 //Selección	entradas	escaneo	de	0	a	15.		
	 	 	 	 	 	 	 //	1	=selecciona	para	escaneo	/	no	escanea	
AD1PCFGH	=	0xFFFF;	 	 //Configura	entradas:	1=	digital	/	0=	Analog	
AD1PCFGL	=	0xFFFF;	
//_PCFG0=		 	 //	AN0	
_PCFG4 = 0; // AN4 -> RB4 -> Sensor Temperatura
_PCFG5 = 0; // AN5 -> RB5 -> Potenciómetro
_PCFG8 = 0; // AN8 -> RB8 -> Joystick_X
_PCFG9 = 0; // AN9 -> RB9 -> Joystick_Y
//IFS0	
_AD1IF	=	0;	 	 	//Flag	a	cero	del	A/D	interrrupt.	
//IEC0	
_AD1IE	=	1;	 	//	Habilita	la	interrupcion	A/D.		
//AD1CON1bits	
_ADON	=	1;		 	 //Comienzo	de	conversion	
//_SAMP=	1;	
}	//	Fin	Inic_CAD	

 void	_ISR	_ADC2Interrupt	(void)	 //	para	interrumpir	cada	conversión	
{	
    static unsigned int long secuenciaCAD =0;
    long    int a,b,c;


if	(secuenciaCAD	>3)	secuenciaCAD =	0;	
    
				switch	(secuenciaCAD)	
								{	
								case	0:	 	 //		
												
                                    ADCValue2	=	ADC2BUF0;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    angulo= ((ADCValue2*125)/1790.25);
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[4][13]= a+'0';
                                    Ventana_Datos[4][14]=b+'0';
                                    Ventana_Datos[4][15]=c+'0';
                                    
									AD2CHS0bits.CH0SA = 5;
                                    secuenciaCAD++;	
                                    break;
                                    
								case	1:	 	 //		
									
                                    
                                    ADCValue2	=	ADC2BUF0;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    angulo= (ADCValue2*180)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[3][13]= a+'0';
                                    Ventana_Datos[3][14]=b+'0';
                                    Ventana_Datos[3][15]=c+'0';
                                    
                                    AD2CHS0bits.CH0SA = 8;

									secuenciaCAD++;	
									break;
                                              
                               case	2:	 	 //		
												
                                    ADCValue2	=	ADC2BUF0;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    angulo= (ADCValue2*45)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[5][13]= a+'0';
                                    Ventana_Datos[5][14]=b+'0';
                                    Ventana_Datos[5][15]=c+'0';
                                    
                                    AD2CHS0bits.CH0SA = 9;

									secuenciaCAD++;	
                                    break;
                                    
								case	3:	 	 //		
									
                                    
                                    ADCValue2	=	ADC2BUF0;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    angulo= (ADCValue2*45)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[6][13]= a+'0';
                                    Ventana_Datos[6][14]=b+'0';
                                    Ventana_Datos[6][15]=c+'0';
                                    
                                    AD2CHS0bits.CH0SA = 4;
                                    
									secuenciaCAD++;	
									break;
                                    
                                default:
                                    
                                
                                    break;		
	 }	
                
 IFS1bits.AD2IF = 0;//_SAMP=	1;	 	 //Comienzo	de	conversion	
 AD2CON1bits.SAMP = 1;
}	//	FIN	ADC1Interrupt 
//	----------------------------------------------------------------------------------	
//	Bucle	por	encuesta	
	
//	----------------------------------------------------------------------------------	

void	_ISR	_ADC1Interrupt	(void)	 //	para	interrumpir	cada	conversión	
{	
    static unsigned int long secuenciaCAD =0;
    long    int a,b,c;


if	(secuenciaCAD	>3)	secuenciaCAD =	0;	
    
				switch	(secuenciaCAD)	
								{	
								case	0:	 	 //		
												
                                    ADCValue	=	ADC1BUF0;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    angulo= ((ADCValue*165)/542.5)-40;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[4][13]= a+'0';
                                    Ventana_Datos[4][14]=b+'0';
                                    Ventana_Datos[4][15]=c+'0';
                                    
									_CH0SA	=	5;	//	
	//								_SAMP=	1;	 	 	 	 //Comienzo	de	conversion	
									secuenciaCAD++;	
                                    break;
                                    
								case	1:	 	 //		
									
                                    
                                    ADCValue	=	ADC1BUF0;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    angulo= (ADCValue*180)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[3][13]= a+'0';
                                    Ventana_Datos[3][14]=b+'0';
                                    Ventana_Datos[3][15]=c+'0';
                                    
                                    _CH0SA	=	8;	//	
	//								_SAMP=	1;	 	 	 	 //Comienzo	de	conversion	
									secuenciaCAD++;	
									break;
                                              
                               case	2:	 	 //		
												
                                    ADCValue	=	ADC1BUF0;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    angulo= (ADCValue*180)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[5][13]= a+'0';
                                    Ventana_Datos[5][14]=b+'0';
                                    Ventana_Datos[5][15]=c+'0';
                                    
									_CH0SA	=	9;	//	
	//								_SAMP=	1;	 	 	 	 //Comienzo	de	conversion	
									secuenciaCAD++;	
                                    break;
                                    
								case	3:	 	 //		
									
                                    
                                    ADCValue	=	ADC1BUF0;		 	 	 //		Si	?	deja	el	valor	en	ADCValue	y	repetimos	(0-1023)
                                    angulo= (ADCValue*180)/1023;
                                    a=angulo/100;
                                    b=(angulo-(100*a))/10;
                                    c=angulo-(100*a)-(10*b);
                                    Ventana_Datos[6][13]= a+'0';
                                    Ventana_Datos[6][14]=b+'0';
                                    Ventana_Datos[6][15]=c+'0';
                                    
                                    _CH0SA	=	4;	//	
									secuenciaCAD++;	
									break;
                                    
                                default:
                                    
                                
                                    break;		
	 }	
                
_AD1IF	=	0;		 	 //Flag	a	cero	del	A/D	interrrupt.	
//_SAMP=	1;	 	 //Comienzo	de	conversion	
AD1CON1bits.SAMP	=	1;
}	//	FIN	ADC1Interrupt 
    unsigned int CAD_BuffA [64] __attribute__((space(dma)));
    //int CAD_BuffB [9] __attribute__((space(dma)));
    unsigned int CAD2_BuffA [64] __attribute__((space(dma)));

    
//    
void inic_OFF_ADC2(void){
AD2CON1 = 0;
AD2CON1bits.ADDMABM = 1;
AD2CON1bits.SSRC =0b111;
AD2CON1bits.ASAM = 1;
AD2CON2 = 0;
AD2CON2bits.CSCNA = 1;
AD2CON2bits.SMPI = 4-1;
AD2CON3 = 0;
AD2CON3bits.ADRC = 0;
AD2CON3bits.SAMC = 31;
AD2CON3bits.ADCS = 3;
AD2CON4= 0;
AD2CHS123 = 0;
AD2CHS0= 0;
AD2CSSL = 0x0330; //0x0330//Selección entradas escaneo de 0 a 15.
//AD2PCFGH = 0xFFFF; //Configura entradas: 1= digital 0= Analog
AD2PCFGL = 0xFCCF;
AD2CHS0bits.CH0SA = 4;
AD2PCFGLbits.PCFG4 = 0;
AD2PCFGLbits.PCFG5 = 0;
AD2PCFGLbits.PCFG8 = 0;
AD2PCFGLbits.PCFG9 = 0;
IFS1bits.AD2IF = 0;
IEC1bits.AD2IE = 0;

}
void Inic_CAD_DMA(void) //
{
//ORDER MODE
//**************************
//CONFIGURAMOS EL ADC1 PARA MUESTREAR AN4,AN5, por canal CH2
AD1CON1= 0; // 0x00E0
//_ADON= 0; // CAD OFF
//_ADSIDL = 0; //1=Se para en modo Idle / 0=Continua en modo Idle
_ADDMABM = 1; //1=bufer DMA en orden de conversión
 // 0=bufer DMA en modo Scatter/Gather
//_AD12B = 0; //1=Modo operacion de 12 bits / 0=Modo operacion de 10 bits
//_FORM = 0; //Formato datos salida: 00=Integer (0000 00dd dddd dddd)
 // 01=Signed integer (ssss sssd dddd dddd)
_SSRC = 0b111; //Termina muestreo y comienca conversion del CAD
 //111=Auto-coonvert
 // 010=TMR3 ADC1 y TMR5 ADC2
 // 001=INT0 / 000= hay que borrar SAMP
//_SIMSAM = 0; //1=Muestrea simultaneamente
 // 0=Muestrea canales individuales sequencialmente
_ASAM = 1; //1=Muestreo comienza seguido de la conversion
 // 0=comienza cuando SAMP=1
//_SAMP
//_DONE
AD1CON2= 0; // 0x00E0
//_VCFG = 0; //Vref+- 000=ACdd y AVss / 001=Vref+ y AVss
 // 010=AVdd y Vref- / 011=Vref+ y Vref-
_CSCNA = 1; //1=Escaneo canal cero / 0= no escaneo
//_CHPS = 0; //Canales utilizados:00=CH0 /01=CH0 y CH1 /1x= CH0,CH1,CH2 y CH3
//_BUFS es solo de lectura: con BUFM=1-> 1=activa 2º mitad de buffer
 // 0= activa 1ª mitad
_SMPI =4-1; //Incrementos dir DMA +1 o
//_SMPI = 0; //Incrementos dir DMA +1 o
 // sin DMA genera una interrupción cada cada SMPI+1
//_BUFM = 0; //1= 2 medios Buffers de 8 / 0= 1 buffer de 16
//_ALTS = 0; //Modo de muestreo: 1= Alternado / 0= Siempre canal A
AD1CON3 = 0;
 _ADRC = 0; //1= reloj RC / 0= Reloj CPU
 _SAMC = 31; //Tiempo auto muestreo0 nºde Tad (de 0 a 31Tad)
 _ADCS = 3; //Tad= >75ns = Tcy(ADCS+1) -> a 80MHz Tcy= 25ns
 // Tad = (3+1)Tcy
AD1CON4= 0;
//_DMABL = --; //Cada buffer contiene 8 palabras
AD1CHS123= 0; // reg. seleccion de entrada canal 1,2 y 3
//_CH123NA= 0; //Selección entrada negativa Canales 1, 2, y 3 pata Sample A
 //0x= CH1=CH2=CH3-> Vref-
 // 10= CH1=AN6,CH2=AN7,CH3=AN8
 // 11= CH1=AN9,CH2=AN10,CH3=AN11
//_CH123SA= 0; //Selección entrada positiva Canales 1, 2, y 3 para Sample A
 //1= AN3-AN4-AN5 / 0= AN0-AN1-AN2
//_CH123NB = 0;
//_CH123SB = 0;
AD1CHS0= 0; // Seleccion de entrada canal 0
//_CH0NA = 0; //Entrada negativa 1= AN1 / 0 = Vref-
//_CH0SA = 5; //Entrada positiva = canal(CH0SA)
//_CH0SA = 4;
//_CH0SB = 3; 
//_CH0NB = 0;
AD1CSSH = 0x0000; //Selección entradas escaneo de la 16 a la 31
AD1CSSL = 0x0330; //0x0330//Selección entradas escaneo de 0 a 15.
AD1PCFGH = 0xFFFF; //Configura entradas: 1= digital 0= Analog
AD1PCFGL = 0xFFFF;
//_PCFG0= // AN0
_CH0SA	=	4;	 	 //Entrada	positiva	=	canal(CH0SA)	

_PCFG4 = 0; // AN4 -> RB4 -> Sensor Temperatura
_PCFG5 = 0; // AN5 -> RB5 -> Potenciómetro
_PCFG8 = 0; // AN8 -> RB8 -> Joystick_X
_PCFG9 = 0; // AN9 -> RB9 -> Joystick_Y
//IFS0
_AD1IF = 0; //Flag a cero del A/D interrrupt.
//IEC0
_AD1IE = 0; // Habilita la interrupcion A/D.
//------------------------------------------------------------------

DMA2CON = 0;
 // CHEN CANAL habilitado / deshabilitado
 DMA2CONbits.SIZE = 0; //0// 1=Byte / 0=Word
 DMA2CONbits.DIR = 0; //0// 1=lee de RAM y escribe a periférico
 // 0=lee de periférico y escribe en RAM
 // HALF interrupcion a mitad / al final
 // NULLW Escribe nulos / operacion normal
 // AMODE //10: Direc.Indirect,Perif / 01 Indir.Reg sin Post-incr
 // 00 Indir.Reg con Post-Incr
 DMA2CONbits.AMODE = 0b10;
 DMA2CONbits.MODE = 0; // 11:One PP / 10:Continuo + PP
 // 01: One / 00: Continuo
// DMA0REQ
 DMA2REQbits.FORCE = 0; //0// manual / automático
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
 DMA2REQbits.IRQSEL = 0x0D;//13;// ADC1 //DMA_IRQ_ADC1
 // 0x15 ADC2
 // 0x1E UART2RX
// DMA2REQbits.IRQSEL = 0x1F; // UART2TX
 // 0x21 SPI2
 // 0x22 ECAN1-RX
 // 0x37 ECAN2-RX
 // 0x46 ECAN1-TX
 // 0x47 ECAN2-TX
 DMA2STA = __builtin_dmaoffset(CAD_BuffA); // Inicio dir. offset
 //DMA2STB = __builtin_dmaoffset(CAD_BuffB); // Inicio dir. offset
// DMA2STA = __builtin_dmaoffset(Ventana_LCD); // Inicio dir. offset
// DMA2STB = __builtin_dmaoffset(Ventana_LCD); // Inicio dir. offset
 DMA2PAD = (volatile unsigned int)&ADC1BUF0;
// DMA2CNT =8-1; // Nº de trasnsferencias -1 
DMACS0 = 0; // Borra Colisiones


DMA2CNT = 8-1;   // Nº de trasnsferencias -1 
DMA2CONbits.AMODE = 0b10; // 10: Direc.Indirect,Perif 
                                // 01 Indir.Reg sin Post-incr 
                                // 00 Indir.Reg con Post-Incr -->  modo normal
DMA2CONbits.MODE = 0;   // 11:One PP / 10:Continuo + PP 
                            // 01: One / 00: Continuo 
_ADDMABM = 1; //1=bufer DMA en orden de conversión 
                  // 0=bufer DMA en modo Scatter/Gather 
_SMPI = 4-1;  //nº patas a realizar conversion-1
_DMABL = 1;  //Cada buffer contiene 2^DMABL palabras  
DMACS0 = 0;      // Borra Colisiones 


 //IPC9bits.DMA2IP = --;
 IFS1bits.DMA2IF =0;
 IEC1bits.DMA2IE = 1;
 DMA2CONbits.CHEN = 1; // Activa canal 2 DMA
//AD1CON1bits
_ADON = 1; //Habilita convertidor
_SAMP= 1; //Lanza conversion

} // Fin Inic_CAD DMA
// -------------------------------------------------------------------
void __attribute__((__interrupt__)) _DMA2Interrupt(void)
{


if(flag_CAD1) _SAMP=	1;	 	 //Comienzo	de	conversion	
else  AD2CON1bits.SAMP = 1;

//_AD1IF	=	0;		 	 //Flag	a	cero	del	A/D	interrrupt.
IFS1bits.DMA2IF = 0;
}     