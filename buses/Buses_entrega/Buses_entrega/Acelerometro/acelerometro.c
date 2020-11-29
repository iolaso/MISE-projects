#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include <linux/init.h>     
#include <linux/interrupt.h>
#include <linux/timer.h>

#include <linux/timex.h>
#include <linux/random.h>

#include "./address_map_arm.h"
#include "./interrupt_ID.h"
#include "ADXL345.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Iñgo Olaso & Edurne Sagasta");
MODULE_DESCRIPTION("Acelerometro");


// Declare global variables needed to use the accelerometer
volatile int * I2C0_ptr; // virtual address for I2C communication
volatile int * SYSMGR_ptr; // virtual address for System Manager communication
#define SUCCESS 0
#define DEVICE_NAME "accel"
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


void * LW_virtual;													
void *HPS_TIMER2_virtual;											
void *HPS_MUX_virtual;         										
void *HPS_I2C0_virtual;    													
volatile int *I2C0_CON_ptr;
volatile int *I2C0_ptr;
volatile int *I2C0_TAR_ptr; 					
volatile int *I2C0_DATA_CMD_ptr ; 		
volatile int *I2C0_FS_SCL_HCNT_ptr ; 
volatile int *I2C0_FS_SCL_LCNT_ptr; 	
volatile int *I2C0_CLR_INTR_ptr; 			
volatile int *I2C0_ENABLE_ptr; 			
volatile int *I2C0_TXFLR_ptr ; 				
volatile int *I2C0_RXFLR_ptr;  				
volatile int *I2C0_ENABLE_STATUS_ptr; 
volatile int *SYSMGR_GENERALIO7_ptr;  
volatile int *SYSMGR_GENERALIO8_ptr;  
volatile int *SYSMGR_I2C0USEFPGA_ptr;

int numero[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};		//Tabla con los hexadecimales correspondientes a los numeros 0-9
int i=1,j=0,flagPWM_X=0,flagI=0, flagPWM_Y=0;							//Flags globales
int c_alto_X, c_bajo_X;													//Variables globales interfaz para LECTURA_X y PWM_X
int c_alto_Y, c_bajo_Y;													//Variables globales interfaz para LECTURA_Y y PWM_Y




void config_INICIAL (void);


// Functions needed to read/write registers in the ADXL345 device

// Read value from internal register at address
void ADXL345_REG_READ(uint8_t address, uint8_t *value){
	
	// Send reg address (+0x400 to send START signal)
	*(I2C0_DATA_CMD_ptr) = address + 0x400;
	// Send read signal
	*I2C0_DATA_CMD_ptr = 0x100;

	 // Read the response (first wait until RX buffer contains data)
	 while (*I2C0_RXFLR_ptr == 0){}
	 *value = *I2C0_DATA_CMD_ptr;
}

// Write value to internal register at address
void ADXL345_REG_WRITE(uint8_t address, uint8_t value){

 // Send reg address (+0x400 to send START signal)
 *I2C0_DATA_CMD_ptr = address + 0x400;

 // Send value
 *I2C0_DATA_CMD_ptr = value;
}

// Read multiple consecutive internal registers
void ADXL345_REG_MULTI_READ(uint8_t address, uint8_t values[], uint8_t len){
	 // Send reg address (+0x400 to send START signal)
	*I2C0_DATA_CMD_ptr = address + 0x400;
	 // Send read signal len times
	int i;
	for (i=0;i<len;i++)
		 *I2C0_DATA_CMD_ptr = 0x100;

		 // Read the bytes
		 int nth_byte=0;
			 while (len){
				 if ((*I2C0_RXFLR_ptr) > 0){
					 values[nth_byte] = *I2C0_DATA_CMD_ptr;
					 nth_byte++;
					 len--;
			 }
		 }
}


// Read acceleration data of all three axes
void ADXL345_XYZ_Read(int16_t szData16[3]){
	uint8_t szData8[6];
	ADXL345_REG_MULTI_READ(0x32, (uint8_t *)&szData8, sizeof(szData8));
	szData16[0] = (szData8[1] << 8) | szData8[0];
	szData16[1] = (szData8[3] << 8) | szData8[2];
	szData16[2] = (szData8[5] << 8) | szData8[4];
}

// Return true if there is new data
char ADXL345_IsDataReady(void){
char bReady = false;
uint8_t data8;

	ADXL345_REG_READ(ADXL345_REG_INT_SOURCE,&data8);
	printk("1 %x\r\n",data8);
	

	if ((data8 & XL345_DOUBLETAP) == XL345_DOUBLETAP){
		bReady = 3;
	}
	else if ((data8 & XL345_SINGLETAP) == XL345_SINGLETAP){
		bReady = 2;
	}
	else if ((data8 & XL345_ACTIVITY) == XL345_ACTIVITY){
		bReady = true;
		}
	
	return bReady;
}

int on(int16_t *XYZ,char *result){
	
	int16_t mg_per_lsb = 4,count= 0;

			result[0]=ADXL345_IsDataReady();
			
			if (result[0]!=0){
				count++;
				ADXL345_XYZ_Read(XYZ);

				return 1;
			}
return 0;
}


/* Code to initialize the accelerometer driver */
static int __init start_accel(void){


// initialize the dev_t, cdev, and class data structures
int err = 0;
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
	strcpy (accel_msg, "Hello from accel\n");

	
	printk(KERN_INFO "Que pasa dani!\n");
	
	//Inicialización llamando  a las funciones de configuracion/inicializacion de los diferentes elementos
	config_INICIAL ();

	
	return 0;
}



static void __exit stop_accel(void)
{

	device_destroy (accel_class, dev_no);
	cdev_del (accel_cdev);
	class_destroy (accel_class);
	unregister_chrdev_region (dev_no, 1);

}


// Called when a process reads from device, if it is ready. If not, sends the last data available
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset){
	
	size_t bytes;
	unsigned long not_sent;
	char hello_msg[MAX_SIZE]={""}; // the string that can be read 
	int16_t XYZ[3];
	char sensor[1];
	
	int n = on(XYZ,sensor);
	if (n == 0) return 0;

	sprintf(hello_msg,"%d,%d,%d,%d,",XYZ[0],XYZ[1],XYZ[2],sensor[0]);
		
	bytes = strlen (hello_msg);
		
	not_sent=copy_to_user (buffer, &hello_msg[*offset], bytes);
	
	return 1;
}



module_init(start_accel);
module_exit(stop_accel);

/****************************************************************************/
/***********************Funciones de inicialización**************************************************/

 void Pinmux_Config(void){ 
	  *SYSMGR_I2C0USEFPGA_ptr = 0; 
	  *SYSMGR_GENERALIO7_ptr = 1; 
      *SYSMGR_GENERALIO8_ptr = 1; 
 } 
/****************************************************************************************************************************************/
void I2C0_Init(void){

	// Abort any ongoing transmits and disable I2C0. 
	*I2C0_ENABLE_ptr = 2; 

	// Wait until I2C0 is disabled 
	while(((*I2C0_ENABLE_STATUS_ptr)&0x1) == 1){} 

	// Configure the config reg with the desired setting (act as 
	 // a master, use 7bit addressing, fast mode (400kb/s)). 
	 *I2C0_CON_ptr = 0x65; 
	 
	 // Set target address (disable special commands, use 7bit addressing) 
	 *I2C0_TAR_ptr = 0x53; 
	 
	 // Set SCL high/low counts (Assuming default 100MHZ clock input to I2C0 Controller). 
	 // The minimum SCL high period is 0.6us, and the minimum SCL low period is 1.3us, 
	 // However, the combined period must be 2.5us or greater, so add 0.3us to each. 
	 *I2C0_FS_SCL_HCNT_ptr = 60 + 30; // 0.6us + 0.3us 
	 *I2C0_FS_SCL_LCNT_ptr = 130 + 30; // 1.3us + 0.3us 
	 
	 // Enable the controller 
	 *I2C0_ENABLE_ptr = 1; 
	 
	 // Wait until controller is powered on 
	 while(((*I2C0_ENABLE_STATUS_ptr)&0x1) == 0){} 
 }



// Initialize the ADXL345 chip
void ADXL345_Init(void){
	 // +- 16g range, full resolution
	ADXL345_REG_WRITE(ADXL345_REG_DATA_FORMAT, XL345_RANGE_16G | XL345_FULL_RESOLUTION);
	 // Output Data Rate: 200Hz
	ADXL345_REG_WRITE(ADXL345_REG_BW_RATE, XL345_RATE_200);
	
	

	 // The DATA_READY bit is not reliable. It is updated at a much higher rate than the Data Rate
	 // Use the Activity and Inactivity interrupts as indicators for new data.
	 ADXL345_REG_WRITE(ADXL345_REG_THRESH_ACT, 0x04); //activity threshold
	 ADXL345_REG_WRITE(ADXL345_REG_THRESH_INACT, 0x02); //inactivity threshold
	 ADXL345_REG_WRITE(ADXL345_REG_TIME_INACT, 0x02); //time for inactivity
	 ADXL345_REG_WRITE(ADXL345_REG_ACT_INACT_CTL, 0xFF); //Enables AC coupling for thresholds
	 ADXL345_REG_WRITE(ADXL345_REG_INT_ENABLE, XL345_ACTIVITY | XL345_INACTIVITY |0x60); //enable interrupts
	 
	 //sINGEL TAP//
	 ADXL345_REG_WRITE(XL345_THRESH_TAP, 0x32); //inactivity threshold
	 ADXL345_REG_WRITE(XL345_DUR_TAP, 0x0F); //time for inactivity
	 ADXL345_REG_WRITE(XL345_LATENT_TAP, 0x50); //Enables AC coupling for thresholds 
	 ADXL345_REG_WRITE(XL345_WINDOW_TAP, 0xC8); //enable interrupts
 	 ADXL345_REG_WRITE(XL345_AXES_TAP, 0x01); //enable interrupts
	 ADXL345_REG_WRITE(XL345_ACT_TAP, 0x01); //enable interrupts


	 // stop measure
	 ADXL345_REG_WRITE(ADXL345_REG_POWER_CTL, XL345_STANDBY);

	 // start measure
	 ADXL345_REG_WRITE(ADXL345_REG_POWER_CTL, XL345_MEASURE);
}


void config_INICIAL (void){


	//Creacion de las direcciones virtuales para los periféricos
	LW_virtual = ioremap_nocache (LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
	HPS_MUX_virtual = ioremap_nocache (HPS_MUX , HPS_MUX_SPAN);
	HPS_I2C0_virtual = ioremap_nocache (HPS_I2C0 , HPS_I2C0_SPAN);

	//Mapeo de los perifericos en memoria usuario	
	I2C0_CON_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_CON);//--
	I2C0_TAR_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_TAR);//--
	I2C0_DATA_CMD_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_DATA_CMD);
	I2C0_FS_SCL_HCNT_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_FS_SCL_HCNT);//--
	I2C0_FS_SCL_LCNT_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_FS_SCL_LCNT);//--
	I2C0_CLR_INTR_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_CLR_INTR);
	I2C0_ENABLE_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_ENABLE); //--
	I2C0_TXFLR_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_TXFLR);
	I2C0_RXFLR_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_RXFLR);
	I2C0_ENABLE_STATUS_ptr =  (unsigned int *)(HPS_I2C0_virtual + I2C0_ENABLE_STATUS);//--
	
	
	SYSMGR_GENERALIO7_ptr  =  (unsigned int *)(HPS_MUX_virtual + SYSMGR_GENERALIO7);
    SYSMGR_GENERALIO8_ptr  =  (unsigned int *)(HPS_MUX_virtual + SYSMGR_GENERALIO8);
    SYSMGR_I2C0USEFPGA_ptr =  (unsigned int *)(HPS_MUX_virtual + SYSMGR_I2C0USEFPGA);
	
	//Inicialización de variables globales

	
	printk("1\n");
	Pinmux_Config();
	I2C0_Init();

	
	uint8_t devid;
	ADXL345_REG_READ(0x00, &devid);
	printk("devid= 0x%x \n", devid);
	// Correct Device ID
	
	
	if (devid == 0xE5){
		ADXL345_Init();
		//on();
		} else {
		printk("Incorrect device ID\n");
	}
	
}
