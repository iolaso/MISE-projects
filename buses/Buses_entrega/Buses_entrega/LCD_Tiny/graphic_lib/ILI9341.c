/*
 * ILI9341.c
 *
 *  Created on: 2014-2-13
 *      Author: Administrator
 */
 // Nuevos includes (G.A.)
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include "terasic_includes.h"
#include "ILI9341.h"
#include "address_map_arm.h"

// #include "system.h"
// #include "altera_avalon_pio_regs.h"
//#include "type.h"



/*
The chip-select CSX (active
low) is used to enable or disable ILI9341 chip.
The RESX (active low) is an external reset signal.
WRX is the parallel data write strobe, RDX is the parallel data read strobe and D[17:0] is parallel data bus.
ILI9341  latches  the  input  data  at  the  rising  edge  of  WRX  signal.
The  D/CX  is  the  signal  of  data/command selection.
When  D/CX=��1��,  D  [17:0]  bits  are  display  RAM  data  or  command��s  parameters.
When  D/CX=��0��,  D [17:0] bits are commands.
*/




// Defines de las direcciones base f�sicas de los dos dispositivos que controlan el LCD
// LCD_RESET_N y LT24_CONTROLLER_0
// Ojo tendreis que modificar los valores de las BaseAddress para que correspondan 
// con las direcciones virtuales

#define LCD_RESET       ptr_LCD_RESET
#define LT24_CONTROLLER ptr_LT24_CONTROLLER

//#define LCD_RESET       ((int32_t *) 0xFF204220)
//#define LT24_CONTROLLER ((int32_t *) 0xFF204230)

// Defines para utilizar las funciones IORW IORD desde Linux
// Ojo habr� que modificar los valores de BaseAddress para que correspondan con las direcciones virtuales

#define IOWR(BaseAddress, RegId, Value) (*(BaseAddress + RegId) = Value )
#define IORD(BaseAddress, RegId)        ( *(BaseAddress + RegId) )

#define  LCD_WR_REG(value)  IOWR((int32_t *)LT24_CONTROLLER,0x00,value)
#define  LCD_WR_DATA(value)  IOWR((int32_t *)LT24_CONTROLLER,0x01,value)

#define IOWR_ALTERA_AVALON_PIO_DATA(base, data)   IOWR(base, 0, data)

#define  Set_LCD_RST  IOWR_ALTERA_AVALON_PIO_DATA(LCD_RESET,0x01)
#define  Clr_LCD_RST  IOWR_ALTERA_AVALON_PIO_DATA(LCD_RESET,0x00)

//void LCD_Init();
//void LCD_Clear(alt_u16 Color);
void LCD_Pattern_Horizon(void);
void LCD_Pattern_Vertical(void);


 //void LCD_DrawPoint(alt_u16 x,alt_u16 y,alt_u16 color );
#define RED 0xf800
#define GREEN 0x07e0
#define BLUE 0x001f
#define BLACK  0x0000
#define WHITE 0xffff
/*
void LCD_WR_REG(int value){
	
	IOWR((int32_t *)LT24_CONTROLLER, 0x00, value);
}	

void LCD_WR_DATA(int value){
	
	IOWR((int32_t *)LCD_RESET, 0x01, value);
}	
*/
void Delay_Ms(alt_u16 count_ms)
{
    while(count_ms--)
    {
        usleep(1000);
    }
}



void LCD_SetCursor(alt_u16 Xpos, alt_u16 Ypos)
{
	 LCD_WR_REG(0x002A);	//Comando Column Address Set (Columna X)
		 LCD_WR_DATA(Xpos>>8);	// Se env�a  1 byte (Parte Alta del valor de la columna)
		 LCD_WR_DATA(Xpos&0XFF); // Se env�a 1 byte (Parte Baja del valor de la columna)
	 LCD_WR_REG(0x002B);	//Comando Page Address Set (Fila Y)
		 LCD_WR_DATA(Ypos>>8);	// Idem con la fila
		 LCD_WR_DATA(Ypos&0XFF);// Idem con la fila
	 LCD_WR_REG(0x002C); // Comando Memory Write
}

// Rellena toda la pantalla con el color indicado
void LCD_Clear(alt_u16 Color)
{
        alt_u32 index=0;
        LCD_SetCursor(0x00,0x0000); // Se posiciona en el primer pixel de 
        LCD_WR_REG(0x002C); // Comando Memory Write
        for(index=0;index<76800;index++) // Dibuja el color en toda la pantalla
        {
        	LCD_WR_DATA(Color); // Datos de la memoria (Color del pixel)
        }
}

void LCD_DrawPoint(alt_u16 x,alt_u16 y,alt_u16 color )
{
        LCD_SetCursor(x,y); // Posici�n X,Y
        LCD_WR_REG(0x002C);	// Comando Memory Write
        LCD_WR_DATA(color);	// Datos de la memoria (Color del pixel)
}

void LCD_Init()
{
	
		

	alt_u16 data1,data2;
	alt_u16 data3,data4;
	Set_LCD_RST;
	Delay_Ms(1);
	Clr_LCD_RST;
	Delay_Ms(10);       // Delay 10ms // This delay time is necessary
	Set_LCD_RST;
	Delay_Ms(120);       // Delay 120 ms
//	Clr_LCD_CS;



	LCD_WR_REG(0x0011); 	// Exit Sleep= Sleep Out
	LCD_WR_REG(0x00CF);		// Power Control
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x0081);
		LCD_WR_DATA(0X00c0);

	LCD_WR_REG(0x00ED);		//  Power on sequence control
		LCD_WR_DATA(0x0064);
		LCD_WR_DATA(0x0003);
		LCD_WR_DATA(0X0012);
		LCD_WR_DATA(0X0081);

	LCD_WR_REG(0x00E8);		// Driver timing control A
		LCD_WR_DATA(0x0085);
		LCD_WR_DATA(0x0001);
		LCD_WR_DATA(0x00798);

	LCD_WR_REG(0x00CB);		// Power control A
		LCD_WR_DATA(0x0039);
		LCD_WR_DATA(0x002C);
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x0034);
		LCD_WR_DATA(0x0002);

	LCD_WR_REG(0x00F7);		// Pump ratio control
		LCD_WR_DATA(0x0020);

	LCD_WR_REG(0x00EA);		// Driver timing control B (
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x00B1);		// Frame Rate Control (In Normal Mode/Full Colors)
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x001b);

	LCD_WR_REG(0x00B6);		// 	Display Function Control
		LCD_WR_DATA(0x000A);
		LCD_WR_DATA(0x00A2);

	LCD_WR_REG(0x00C0);    //Power control
		LCD_WR_DATA(0x0005);   //VRH[5:0]

	LCD_WR_REG(0x00C1);    //Power control
		LCD_WR_DATA(0x0011);   //SAP[2:0];BT[3:0]

	LCD_WR_REG(0x00C5);    //VCM control
		LCD_WR_DATA(0x0045);       //3F
		LCD_WR_DATA(0x0045);       //3C

	 LCD_WR_REG(0x00C7);    //VCM control2
		 LCD_WR_DATA(0X00a2);

	LCD_WR_REG(0x0036);    // Memory Access Control
		LCD_WR_DATA(0x0008);//48

	LCD_WR_REG(0x00F2);    // 3Gamma Function Disable
		LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x0026);    //Gamma curve selected
		LCD_WR_DATA(0x0001);

	LCD_WR_REG(0x00E0);    //Set Gamma
		LCD_WR_DATA(0x000F);
		LCD_WR_DATA(0x0026);
		LCD_WR_DATA(0x0024);
		LCD_WR_DATA(0x000b);
		LCD_WR_DATA(0x000E);
		LCD_WR_DATA(0x0008);
		LCD_WR_DATA(0x004b);
		LCD_WR_DATA(0X00a8);
		LCD_WR_DATA(0x003b);
		LCD_WR_DATA(0x000a);
		LCD_WR_DATA(0x0014);
		LCD_WR_DATA(0x0006);
		LCD_WR_DATA(0x0010);
		LCD_WR_DATA(0x0009);
		LCD_WR_DATA(0x0000);

	LCD_WR_REG(0X00E1);    //Set Gamma
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x001c);
		LCD_WR_DATA(0x0020);
		LCD_WR_DATA(0x0004);
		LCD_WR_DATA(0x0010);
		LCD_WR_DATA(0x0008);
		LCD_WR_DATA(0x0034);
		LCD_WR_DATA(0x0047);
		LCD_WR_DATA(0x0044);
		LCD_WR_DATA(0x0005);
		LCD_WR_DATA(0x000b);
		LCD_WR_DATA(0x0009);
		LCD_WR_DATA(0x002f);
		LCD_WR_DATA(0x0036);
		LCD_WR_DATA(0x000f);

	LCD_WR_REG(0x002A);		//
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x00ef);

	 LCD_WR_REG(0x002B);		//
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x0000);
		LCD_WR_DATA(0x0001);
		LCD_WR_DATA(0x003f);

	LCD_WR_REG(0x003A);		//
		LCD_WR_DATA(0x0055);

	LCD_WR_REG(0x00f6);		//
		LCD_WR_DATA(0x0001);
		LCD_WR_DATA(0x0030);
		LCD_WR_DATA(0x0000);

	LCD_WR_REG(0x0029); //display on

	LCD_WR_REG(0x002c);    // 0x2C

}


void LCD_Pattern_Horizon(void)
{
        alt_u32 index=0;
        alt_u16  data=0;
        LCD_SetCursor(0x00,0x0000);
        data=0x0000;
        for(index=0;index<240*80;index++)
        {
        	LCD_WR_DATA(data);
        }
        LCD_SetCursor(0x00,80);
        data=RED;
		for(index=0;index<240*80;index++)
		{
			LCD_WR_DATA(data);
		}
	    LCD_SetCursor(0x00,160);
		data=GREEN;
		for(index=0;index<240*80;index++)
		{
			LCD_WR_DATA(data);
		}
		LCD_SetCursor(0x00,240);
		data=BLUE;
		for(index=0;index<240*80;index++)
		{
			LCD_WR_DATA(data);
		}
}

void LCD_Pattern_Vertical(void)
{
        int i,j,m;
        alt_u16  data=0;
        alt_u16  color[4]={WHITE,BLUE,GREEN,RED};
        LCD_SetCursor(0x00,0x0000);
        for(m=0;m<320;m++)
        {
			for(i=0;i<4;i++)
			{
				data=color[i];
				for(j=0;j<60;j++)
				{
					LCD_WR_DATA(data);
				}
			}
        }
}



