// ============================================================================
// Copyright (c) 2014 by Terasic Technologies Inc.
// ============================================================================
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
// ============================================================================
//
//  Terasic Technologies Inc
//  9F., No.176, Sec.2, Gongdao 5th Rd, East Dist, Hsinchu City, 30070. Taiwan
//
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// ============================================================================



#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "terasic_includes.h"
#include "gui.h"
#include "address_map_arm.h"
#include "ILI9341.h"
#include "address_map_arm.h"
#include <fcntl.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "simple_graphics.h"

//#define ID argv[4]
//#define Debug
/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);

//void GUI_VPG(alt_video_display *pDisplay, TOUCH_HANDLE *pTouch);
volatile int *ptr_LT24_CONTROLLER;
volatile int *ptr_LCD_RESET;



#define error(a) {perror(a);  FinishAll(SIGINT, 2);}


// Global defines
#define END_OF_FILE          0
#define TRY_READLINE_AGAIN  -1
#define READ_ERROR          -2

#define TIME_WAIT      ((useconds_t ) 10000)
#define TIME_1_SEC_US  ((useconds_t ) 1000000)

#define MAXBUFFER_SIZE 512

char gBuffer[MAXBUFFER_SIZE];
char gNamedFifo[256];
int flag=0;
int fd1,fd2;

// To finish the child process
pid_t child_pid = -1;

// File descriptor to read fifo data
int  fd_FiFoRead;

void FinishAll(int signum, int ExitCode) {
	  // kill child process
  if (child_pid!= -1) 
	kill(child_pid, signum);

   // wait to really finish all child process
   while((child_pid=wait(NULL))!=-1) {
		#ifdef Debug
		printf("\n Child process pid(%d) finished..\n",child_pid);
		#endif
	}

    // close open  read fifo
	close(fd_FiFoRead);
	unlink(gNamedFifo) ;
  
	close(fd1);
	close(fd2);

	#ifdef Debug
	printf("All finish...bye.\n");
	#endif
	_exit(ExitCode);
}

void send_signal(int signum){
	FinishAll(signum, 0);
}

void init_signals(void){
  signal(SIGINT, send_signal);
}

int try_read_line(int fd, char *Buff, int MaxTam)
{
   static int NumReadBytes=0;   // static counteer read one byte
   int nread;
   char Dat;
   
   nread=read(fd,&Dat, 1);	// Try read one byte
   
   switch( nread) {
   case -1: 	// This means is empty pipe or read erro// 
         if (errno == EAGAIN)   return (TRY_READLINE_AGAIN); 
		 else 	return (READ_ERROR); // READ ERROR
		 break;	// Not necessary (allways make a return)
		 
   case 0: // End of file (FIFO). Finish read ... 
	     memcpy(Buff,gBuffer,NumReadBytes); // Copy to return buffer (Buff)
		 nread = NumReadBytes;
		 NumReadBytes=0;					// Init new readline
	     return (nread);	// Return line size
		 break;	// Not necessary (allways make a return)
   		 
   default: 	//  must be equal to case 1: // // Read OK (1 byte)
	     gBuffer[NumReadBytes] = Dat;	//Add read byte to global buffer
	     NumReadBytes++;
	     if (Dat == '\n') { // End of line
	        memcpy(Buff,gBuffer,NumReadBytes); // Copy to return buffer (Buff)
		    nread = NumReadBytes;
		    NumReadBytes=0;					// Init new readline
	        return (nread);	// Return line size
	     } else  return (TRY_READLINE_AGAIN);  // Try read_line again to complete read_line
   } 
}

//void Sensor_ADC(alt_video_display *Display);
void Sensor_ADC(alt_video_display *pDisplay){
	
	int i,n=0,x1,y1,z1,T1;
	char x[1]={""},y[1]={""},z[1]={""},T[1]={""},ct1[1]={""},ct2[1]={""};
	char buff[200];
	char *token=0;
	char *Comando=0;   
	const char s[2] = ","; 
	static int counter_tap_1=0,counter_tap_2=0;
	double p;	// the value of the sensor
	
	   n=0;
	   memset( x, 0, 1 );
	   memset( y, 0, 1 );
	   memset( z, 0, 1 );
   	   memset( T, 0, 1 );

	   n= read(fd1,buff,4);		
		
	  if (n == 1 ){

		Comando = strtok(buff, s);		  
		x1 = atoi(Comando);
		sprintf (x,"%d",x1);
		token = strtok(NULL, s);
		y1 = atoi(token);  
		sprintf (y,"%d",y1);
		token = strtok(NULL, s);
		z1 = atoi(token);  
		sprintf (z,"%d",z1);
		
		char buffer[256];
		sprintf(buffer,"          ");	

		
		sprintf(buffer,"X: %.3f",2*2.5*(x1&0x0fff)/4096);
		vid_print_string_alpha(130, 40, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
		sprintf(buffer,"Y: %.3f",2*2.5*(y1&0x0fff)/4096);
		vid_print_string_alpha(130, 60, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
		sprintf(buffer,"P:%.3f",2*2.5*(z1&0x0fff)/4096);
		vid_print_string_alpha(130, 80, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);

		n=0;
	   }

	   
	  n= read(fd2,buff,4);

	  if (n == 1 ){
		
		Comando = strtok(buff, s);		  
		x1 = atoi(Comando);
		sprintf (x,"%d",x1);
		token = strtok(NULL, s);
		y1 = atoi(token);  
		sprintf (y,"%d",y1);
		token = strtok(NULL, s);
		z1 = atoi(token);  
		sprintf (z,"%d",z1);
		token = strtok(NULL, s);		
		T1 = atoi(token);
		sprintf (T,"%d",T1);
		counter_tap_1 += ((T1==2)?1:0);	
		counter_tap_2 += ((T1==3)?1:0);	
	    sprintf (ct1,"%d",counter_tap_1);
		sprintf (ct2,"%d",counter_tap_2);

		char buffer[256];

		
		sprintf(buffer,"X: %d   ",x1);
		vid_print_string_alpha(0, 40, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
		sprintf(buffer,"Y: %d   ",y1);
		vid_print_string_alpha(0, 60, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
		sprintf(buffer,"Z: %d  ",z1);
		vid_print_string_alpha(0, 80, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
		sprintf(buffer,"1Tap: %d  ",counter_tap_1);
		vid_print_string_alpha(0, 100, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
		sprintf(buffer,"2Tap: %d  ",counter_tap_2);
		vid_print_string_alpha(0, 120, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
		

		if (T1==3){p = rand() % 16777216; 
		vid_draw_box(10,210,90,300,p,tahomabold_20,pDisplay);
		}
		
		n=0;
	   }
	   
	  

}


void TrazaSensor(uint16_t Humedad, uint16_t Temperatura, uint16_t Iluminacion,alt_video_display *pDisplay) {

	 double rh=-6+125*((double)Humedad/65536);
	 double temp=-46.85+175.72*((double)Temperatura/65536);

	 
	 flag++;
	char buffer[256];
	
		
	sprintf(buffer,"Hum: %2.1f ",rh);
	//GUI_ShowInfo(pDisplay,buffer);
	vid_print_string_alpha(125, 220, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
	sprintf(buffer,"Tem: %2.1f  ",temp);
	vid_print_string_alpha(125, 240, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);
	
	//GUI_ShowInfo(pDisplay,buffer);
	sprintf(buffer,"Il: %d    ",Iluminacion);
	vid_print_string_alpha(125, 260, BLUE_24, BLACK_24, tahomabold_20, pDisplay, buffer);

}

void ini_devices(alt_video_display *pDisplay){
	
		//generar la direccion base //
		int fd = -1;               // used to open /dev/mem for access to physical addresses
		void *LW_virtual;          // used to map physical addresses for the light-weight bridge
		
	
	   if ((fd1= open("/dev/SPI_POT", O_RDONLY))== -1) {
		   fprintf(stderr, "Error al abrir el fichero /dev/SPI_POT \n");
		   _exit(1);
	   }

	   if ((fd2= open("/dev/accel", O_RDONLY))== -1) {
		   fprintf(stderr, "Error al abrir el fichero /dev/accel \n");
		   _exit(1);
	   }	   
		// Create virtual memory access to the FPGA light-weight bridge
		fd = open_physical (fd);
		if (fd == -1) {
			#ifdef Debug
			printf("Error eb open_physical %d\n", fd);
			#endif
			exit(1);		
		}
		#ifdef Debug
		printf("open_physical=%d\n", fd);
		#endif
		
			//return (-1);
		LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN);
		if (LW_virtual== NULL) {
			#ifdef Debug
			printf("Error eb map_physical\n");
			#endif
			exit(1);
		}
  
       	ptr_LT24_CONTROLLER= (unsigned int *)(LW_virtual + 0x4230);
		ptr_LCD_RESET=(unsigned int *)(LW_virtual + 0x4220);


        float f1, f2;
		
		fd = -1;               // used to open /dev/mem for access to physical addresses		
        f1=100;
        #ifdef Debug
		printf("Introduce un numero del 1 al 300: ");
		scanf("%f", & f1);
        f2= sqrt(f1);
		printf("Resultado %f  sqrt(%f)=%f \n", f1, f1, f2);
		#endif
		
		const bool bVPG = FALSE;
		#ifdef Debug
		printf("LT24 Demo!...\n");
		#endif

	   // init LCD
		LCD_Init();
		LCD_Clear(0X0000);

		pDisplay->interlace = 0;
		pDisplay->bytes_per_pixel = 2;
		pDisplay->color_depth = 16;
		pDisplay->height = SCREEN_HEIGHT;
		pDisplay->width = SCREEN_WIDTH;


		GUI2(pDisplay);
		#ifdef Debug
		printf("....LT24 Demo DONE!!\n ...Bye\n");
		#endif
}

main(int argc, char *argv[])
{
   int  child_pid;
   char Line[MAXBUFFER_SIZE];
   int  i,u, ret, finish;
   char *token;
   char buffer[255];
   char delim[2]=" ";
   char *comand;
   unsigned char result=0;
   uint16_t Humedad, Temperatura, Iluminacion,id,ID_filtro;
	alt_video_display Display;   
	static double count=0;
	
	ini_devices(&Display);
	
      if (argc != 5) {
		fprintf(stderr, "ERROR... \n\t Use: %s seriallisten /dev/ttyXXX  baudrate ID\n", argv[0]);
		_exit(1);
    }
    init_signals();	// To Kill child process when  ctr+C has been pressed 
	ID_filtro = argv[4];
   sprintf(gNamedFifo,"Named_Fifo");
   unlink(gNamedFifo);	// if it already existed
   if (mknod(gNamedFifo,S_IFIFO|0666,0)==-1) 
      error("mknod");
	#ifdef Debug  
	printf("Step 1: The %s fifo has been created \n",gNamedFifo);
	#endif
	
   switch(child_pid=fork()) {	// Create child process
      case -1:	error("fork");
      case 0:	//  child process
			#ifdef Debug		
			printf("child process created\n");		// it should be the seriallisten process
			#endif
			
			if(close(1)==-1) error("close");		// these two lines redirect the 

			if(open(gNamedFifo,O_WRONLY)!=1)		// standard output (1) to gNamedFifo
				error("open");
			
			fprintf(stderr, "Step 2: The child %s pid(%d) proces  has been created \n",argv[1], getpid());
			execlp(argv[1],argv[1], argv[2], argv[3], NULL);	// execute seriallisten /dev/ttyXXX  baudrate
			error("execlp");
	}		
	#ifdef Debug
	printf("Step 3: The %s Process(pid=%d) has been launched\n",argv[1],child_pid);
	#endif
   
	// Now we proced to read lines
	if((fd_FiFoRead = open(gNamedFifo,O_RDONLY ))==-1)	{	// open gNamedFifo to read
		error("failed to open NamedFifo\n"); 
	}
	
	#ifdef Debug
	printf("Step 4: The %s  has been opened by %s \n",gNamedFifo,argv[0]);
	#endif
	
	// error checking for fcntl 
	if (fcntl(fd_FiFoRead, F_SETFL, O_NONBLOCK) < 0)  {
		error("failed to open NamedFifo\n"); 
 	}
	#ifdef Debug
	printf("Step 5: The %s  has been seet to NONBLOKC mode by %s\n",gNamedFifo,argv[0]);
   
	printf("Step 6: Startind to read serial data....\n\n\n");
	#endif
	usleep( TIME_1_SEC_US);		// wait 1 second

      for (i=0, finish = 0;  (!finish ) ; ) {
      ret = try_read_line(fd_FiFoRead, Line, MAXBUFFER_SIZE-1);
	  
	  switch(ret) {
	     case READ_ERROR:		// problem... finish all
		    kill(child_pid, SIGKILL); // Kill child process
		    error("READ FIFO ERROR... bye...\n"); 
	        break;
	     case 0:	// End of file (fifo) Normal finish
		    finish = 1;
	        break;
	     case TRY_READLINE_AGAIN:
	      // usleep( TIME_WAIT);		// wait some time and try again
	       break;
	     default: // Some data read (process it)
	        Line[ret] = '\0';	
		    #ifdef Debug
			printf("%03d-(%2d):%s", i, strlen(Line), Line); // New line (\n) isn't necessary
			#endif
			comand=strtok(Line,delim);
			for( u=0;u<7;u++)comand=strtok(NULL,delim);
			
			//int id;
			comand=strtok(NULL,delim);
			sscanf(comand,"%x",&result);
			#ifdef Debug
			printf("\r\n");
			printf(" 0x%x",result);
			#endif
			id = (id&0x00) | result;
			
			comand=strtok(NULL,delim);
			sscanf(comand,"%x",&result);		
			#ifdef Debug
			printf(" 0x%x",result);
			#endif
			id = (id<<8) | result;
			

			if(id==ID_filtro){
			#ifdef Debug
			printf("ID %d \r\n",id);
			#endif			
				for( u=0;u<6;u++)comand=strtok(NULL,delim);
				
				//TEMP
				comand=strtok(NULL,delim);
				sscanf(comand,"%x",&result);
				#ifdef Debug
				printf("\r\n");
				printf(" 0x%x",result);
				#endif
				Temperatura = (Temperatura&0x00) | result;
				
				comand=strtok(NULL,delim);
				sscanf(comand,"%x",&result);		
				#ifdef Debug
				printf(" 0x%x",result);
				#endif
				Temperatura = (Temperatura<<8) | result;
				//HUM
				comand=strtok(NULL,delim);
				sscanf(comand,"%x",&result);
				#ifdef Debug
				printf(" 0x%x",result);
				#endif
				Humedad = (Humedad&0x00) | result;
				
				comand=strtok(NULL,delim);
				sscanf(comand,"%x",&result);		
				#ifdef Debug
				printf(" 0x%x",result);
				#endif
				Humedad = (Humedad<<8) | result;			
				
				//ILUM
				comand=strtok(NULL,delim);
				sscanf(comand,"%x",&result);
				#ifdef Debug
				printf(" 0x%x",result);
				#endif

				Iluminacion = (Iluminacion&0x00) | result;
				
				comand=strtok(NULL,delim);
				sscanf(comand,"%x",&result);		
				#ifdef Debug
				printf(" 0x%x\r\n",result);
				#endif
				Iluminacion = (Iluminacion<<8) | result;
				#ifdef Debug
				printf("Humedad 0x%x\r\n",Humedad);
				printf("Temperatura 0x%x\r\n",Temperatura);
				printf("Iluminacion 0x%x\r\n",Iluminacion);
				#endif
				TrazaSensor( Humedad,  Temperatura,  Iluminacion, &Display);
			}			
			i++;
	  }
	  if (count==2000){
		  count=0;
		 Sensor_ADC(&Display); 
	  }
	  else count++;
	  
   
   }
   
    FinishAll(SIGINT,0);

	return 0;
}
// Open /dev/mem, if not already done, to give access to physical addresses
int open_physical (int fd)
{
   if (fd == -1)
      if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1)
      {
        #ifdef Debug
		printf ("ERROR: could not open \"/dev/mem\"...\n");
		#endif
		 return (-1);
      }
   return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical (int fd)
{
   close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
   void *virtual_base;

   // Get a mapping from physical addresses to virtual addresses
   virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
   if (virtual_base == MAP_FAILED)
   {
		#ifdef Debug
		printf ("ERROR: mmap() failed...\n");
		#endif
		close (fd);
		return (NULL);
   }
   return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 */
int unmap_physical(void * virtual_base, unsigned int span)
{
   if (munmap (virtual_base, span) != 0)
   {	
		#ifdef Debug
		printf ("ERROR: munmap() failed...\n");
		#endif
		return (-1);
   }
   return 0;
}


 
