// --------------------------------------------------------------------
// Copyright (c) 2010 by Terasic Technologies Inc. 
// --------------------------------------------------------------------
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     Terasic Technologies Inc
//                     356 Fu-Shin E. Rd Sec. 1. JhuBei City,
//                     HsinChu County, Taiwan
//                     302
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// --------------------------------------------------------------------
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//GUREAK
//#include <linux/module.h>    // included for all kernel modules
//#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/interrupt.h>
#include <linux/timer.h>

#include "inc/altera_avalon_spi_regs.h"



//#include <fcntl.h>
//#include <sys/mman.h>

#include "inc/alt_types.h"  // alt_u32
#include "address_map_arm.h" //IOWR_ALTERA_AVALON_PIO_DATA


#define SUCCESS 0
#define DEVICE_NAME "SPI_POT"
static dev_t dev_no = 0;
static struct cdev *accel_cdev = NULL;
static struct class *accel_class = NULL;
#define MAX_SIZE 256 // assume that no message longer than this will be used
static char accel_msg[MAX_SIZE]; // the string that can be read or written

static ssize_t device_read (struct file *, char *, size_t, loff_t *);

static struct file_operations fops = {
.owner = THIS_MODULE,
.read = device_read,
};
/* Prototypes for functions used to access physical memory addresses */

volatile int * SPI_ptr ;//= (int *) ADC_BASE;		// ADC port address


// Called when a process reads from device, if it is ready. If not, sends the last data available
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset){

	size_t bytes;
	unsigned long not_sent;
	char hello_msg[MAX_SIZE]={""}; // the string that can be read 
	char p;	// the value of the sensor	
     alt_u16 adc_result;
     alt_u16 v_adc_result[100];
     alt_u16 status=0x00;
	 int i;
	

		 for(i=0;i<8;i++){

	     while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_ptr) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK)); //write control and sequence register
	     IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_ptr, 0x5f90); //read the convert result channel 0
	       // 0x5f90  = 0101 1111 1001 0000
	       //      WRITE SEQ -- ADD2 ADD1 ADD0 PM1 PM0 SHADOW -- RANGE CODING -- -- -- --
	       //        0    1   0   1    1    1   1   1    1     0    0     1    0  0  0  0

	       // wait tx done
	     status=0x00;
	     while (!(status & ALTERA_AVALON_SPI_STATUS_TMT_MSK)){
	          status = IORD_ALTERA_AVALON_SPI_STATUS(SPI_ptr);
	     }

	    while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_ptr) & ALTERA_AVALON_SPI_STATUS_RRDY_MSK));
	    adc_result = IORD_ALTERA_AVALON_SPI_RXDATA(SPI_ptr);

	    v_adc_result[i] = adc_result;

    }//end for
	

	sprintf(hello_msg,"%d,%d,%d,",v_adc_result[0],v_adc_result[1],v_adc_result[2]);

	bytes = strlen (hello_msg);
		
	not_sent=copy_to_user (buffer, &hello_msg[*offset], bytes);

	return 1;
} 


void SPI_init(void){
	
		//initial the spi controller
		IOWR_ALTERA_AVALON_SPI_SLAVE_SEL(SPI_ptr, 0x1); //select device 0 (Mask 0x0001)
		IOWR_ALTERA_AVALON_SPI_CONTROL(SPI_ptr, 0x00);  //reset the control reg
		IOWR_ALTERA_AVALON_SPI_STATUS(SPI_ptr, 0x00);   // Clear the Status Reg
		//initial the adc after power supply
	 
	   //tx two cycle dummy bits
        while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_ptr) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
		IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_ptr, 0xffff); //the first cycle dummy bits

	    while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_ptr) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK));
	    IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_ptr, 0xffff); //the second cycle dummy bits

	    // SEQ Bit = 1, SHADOW Bit = 1
	    while(!(IORD_ALTERA_AVALON_SPI_STATUS(SPI_ptr) & ALTERA_AVALON_SPI_STATUS_TRDY_MSK)); //write control and sequence register
	    IOWR_ALTERA_AVALON_SPI_TXDATA(SPI_ptr, 0xdf90); //datasheet figure 13,config the control reg, channel 0-7
	       // 0xdf90  = 1101 1111 1001 0000
	       //      WRITE SEQ -- ADD2 ADD1 ADD0 PM1 PM0 SHADOW -- RANGE CODING -- -- -- --
	       //        1    1   0   1    1    1   1   1    1     0    0     1    0  0  0  0
		
	
}

/***********************Funcion de entrada al modulo**************************************************/

static int __init project_init(void)
{
			
		// initialize the dev_t, cdev, and class data structures
		int err = 0;
		void *LW_virtual;          // used to map physical addresses for the light-weight bridge
		printk(KERN_INFO "Ini SPI!\n");
		/* Get a device number. Get one minor number (0) */
		if ((err = alloc_chrdev_region (&dev_no, 0, 1, DEVICE_NAME)) < 0) {
			printk (KERN_ERR "accel: alloc_chrdev_region() error %d\n", err);
		return err;
		}

		accel_class = class_create (THIS_MODULE, DEVICE_NAME);
		// Allocate and initialize the char device
		accel_cdev = cdev_alloc ();
		accel_cdev->ops = &fops;
		accel_cdev->owner = THIS_MODULE;
		// Add the character device to the kernel
		if ((err = cdev_add (accel_cdev, dev_no, 1)) < 0) {
			printk (KERN_ERR "accel: cdev_add() error %d\n", err);
		return err;
		}
		device_create (accel_class, NULL, dev_no, NULL, DEVICE_NAME );
		
		printk( "Probando el modulo SPI!\n");
		
		// Set virtual address pointer to I/O port
		LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
		SPI_ptr = (unsigned int *) (LW_virtual + ADC_SPI_BASE);
		
		//Inicialización llamando  a las funciones de configuracion/inicializacion de los diferentes elementos
		SPI_init();

	return 0;
}

/***********************Funcion de salida al modulo**************************************************/

static void __exit project_cleanup(void)
{
	//Apagar/parar los diferentes elementos

	*SPI_ptr = 0;
	
	device_destroy (accel_class, dev_no);
	cdev_del (accel_cdev);
	class_destroy (accel_class);
	unregister_chrdev_region (dev_no, 1);
	
}

module_init(project_init);
module_exit(project_cleanup);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Borja Gorriz & Edurne Sagasta");
MODULE_DESCRIPTION("Lectura y creación de PWM");