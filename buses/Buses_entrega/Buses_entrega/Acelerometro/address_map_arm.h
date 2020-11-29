#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_SPAN        0x00001FFF

/* Cyclone V FPGA devices */
#define LW_BRIDGE_BASE			0xFF200000

#define LEDR_BASE             0x00000000
#define HEX3_HEX0_BASE        0x00000020
#define HEX5_HEX4_BASE        0x00000030
#define SW_BASE               0x00000040
#define KEY_BASE              0x00000050
#define JP1_BASE              0x00000060
#define JP2_BASE              0x00000070
#define PS2_BASE              0x00000100
#define PS2_DUAL_BASE         0x00000108
#define JTAG_UART_BASE        0x00001000
#define JTAG_UART_2_BASE      0x00001008
#define IrDA_BASE             0x00001020
#define TIMER1_BASE           0x00002000
#define TIMER2_BASE           0x00002020
#define AV_CONFIG_BASE        0x00003000
#define PIXEL_BUF_CTRL_BASE   0x00003020
#define CHAR_BUF_CTRL_BASE    0x00003030
#define AUDIO_BASE            0x00003040
#define VIDEO_IN_BASE         0x00003060
#define ADC_BASE              0x00004000


#define LW_BRIDGE_SPAN			0x00005000


#define HPS_TIMER0           0xFFC08000
#define HPS_TIMER0_SPAN       0x00000014
#define HPS_TIMER1            0xFFC09000
#define HPS_TIMER1_SPAN       0x00000014
#define HPS_TIMER2            0xFFD00000
#define HPS_TIMER2_SPAN       0x00000014
#define HPS_TIMER3            0xFFD01000
#define HPS_TIMER3_SPAN       0x00000014

#define HPS_LEDSG             0xFF709000
#define HPS_LEDSG_SPAN        0x00000014



//I2C
#define HPS_I2C0   		    0xFFC04000

#define I2C0_CON			0x00000000
#define I2C0_TAR        	0x00000004
#define I2C0_DATA_CMD       0x00000010
#define I2C0_FS_SCL_HCNT    0x0000001C
#define I2C0_FS_SCL_LCNT    0x00000020
#define I2C0_CLR_INTR       0x00000040
#define I2C0_ENABLE        	0x0000006C
#define I2C0_TXFLR        	0x00000074
#define I2C0_RXFLR        	0x00000078
#define I2C0_ENABLE_STATUS  0x0000009C

#define HPS_I2C0_SPAN       0x000000A0


#define HPS_MUX       0xFFD08000

#define SYSMGR_GENERALIO7 	0x0000049C
#define SYSMGR_GENERALIO8 	0x000004A0
#define SYSMGR_I2C0USEFPGA  0x00000704

#define HPS_MUX_SPAN        0x00000708

