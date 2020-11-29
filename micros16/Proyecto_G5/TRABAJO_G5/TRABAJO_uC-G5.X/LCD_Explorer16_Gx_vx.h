/**********************************************************************
* © 2017 LG
*
 Proyecto: MISE Micros 16bits 2017
* Fichero: LCD_Explorer16_Gx_vx.h

Grupo: G0    Autor: x
	Fecha: 2017-X-15
 *  vx
*
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Luis                       LCD 2x16 Explorer 16
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/

// Pines de control LCD
#define  RW  _LATD5	// LCD R/W signal D5
#define  RS  _LATB15	// LCD RS signal
#define  E   _LATD4	// LCD E signal
//#define  E   LATFbits.LATF1       // LCD E signal

#define  RW_TRIS    _TRISD5
#define  RS_TRIS    _TRISB15
#define  E_TRIS	    _TRISD4
//#define  E_TRIS   TRISFbits.TRISF1

// Pines de datos
#define  DATA      LATE	    // Port for LCD data
#define  DATAPORT  PORTE
#define  TRISDATA  TRISE    // I/O setup for data Port, solo 8 de los 16 bits

/*****	LCD COMMAND FUCNTION PROTOTYPES  *****/
#define cursor_right()  lcd_cmd( 0x14 )     // 40us
#define cursor_left()   lcd_cmd( 0x10 )     // 40us
#define display_shift() lcd_cmd( 0x1C )     // 40us
#define home_clr()      lcd_cmd( 0x01 )     // 1,64ms
#define home_it()       lcd_cmd( 0x02 )     // 1,64ms
//#define line_1()        lcd_cmd( 0x80 )     // 40us, DDRAM 0x00
//#define line_2()        lcd_cmd( 0xC0 )     // 40us, DDRAM 0x40
#define inicio_fila_1()        lcd_cmd( 0x80 )     // 40us, DDRAM 0x00
#define inicio_fila_2()        lcd_cmd( 0xC0 )     // 40us, DDRAM 0x40

#define function_set()  lcd_cmd( 0x38)      // 40us
#define display_on()    lcd_cmd( 0x0C)      // 40us
#define entry_mode()    lcd_cmd( 0x06)      // 40us

/******	LCD FUNCTION PROTOYPES ******/

void Inic_LCD( void );		        // initialize display		
void lcd_cmd( char cmd );	        // write command to lcd
void lcd_data( char data );		    // write data to lcd
void inic_timer_LCD (void);
void espera_LCD_ms( unsigned int n_miliseg );
void espera_LCD_us( unsigned int n_microseg );


//void Visualiza_LCD_Fila_1 (void);
//void Visualiza_LCD_Fila_2 (void);

//#define fila_1	0x02;  // 0x80
//#define fila_2	0xC0;


