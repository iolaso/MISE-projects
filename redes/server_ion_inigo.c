#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include<unistd.h>
#include <sys/mman.h>
#include "../address_map_arm.h"


#define TRUE 1
#define FALSE 0
/* Prototypes for functions used to access physical memory addresses */
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);


 // Prototipos de funciones auxiliares
void buffer_Params(char *buf, char *command,int *value1,int *value2) ;
int  siete_segmentos(int *value1,int *borrado);
int leds(int *value1);

#define PUERTO 2018
#define TAMBUF 1024

int main()

{
        struct sockaddr_in dir;
        static char buf[TAMBUF];
		static char buffer[TAMBUF];
        struct sockaddr_in  dir_cli;
        int length = sizeof dir_cli;
        int leidos;
        int borrado=0;

        static char command[6];
        static int value1[1];
        static int value2[1];
        // Declaración de variables
        //char *COMANDOS[] = {.............};
        //char *RESPUESTAS[] = {...........};

        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        memset(&dir,0,sizeof(dir));
        memset(&dir_cli,0,sizeof(dir_cli));

        dir.sin_family = AF_INET;
        dir.sin_addr.s_addr = htonl(INADDR_ANY);
        dir.sin_port = htons(PUERTO);

        int resp = bind(sock, (struct sockaddr *)&dir, sizeof dir);
		

        length = sizeof(struct sockaddr);

        while(1)
        {



                leidos = 0;
                strcpy(buf,"");//memset(buf,"",TAMBUF);
                value1[0]=0;
                value2[0]=0;
				
                leidos = recvfrom(sock, buf, 1024,0,(struct sockaddr *)&dir_cli, &length) ;

                buf[leidos]='\0';
				sprintf(buffer,"%s",buf);
                buffer_Params(buffer, command,value1, value2) ;
                 if(strncmp(command,"CLEAR",5) == 0){
                        value1[0]=0;
                        borrado = 1;
                        siete_segmentos(value1, &borrado);
                        borrado = 0;
                        leds(value1);//CLEAR /*BORRA TODO-MASCARA DE CEROS*/
                        sprintf(buf, "%s","OK:0,0");
                 }
                else if(strncmp(command,"LEARN",5) == 0){
                        siete_segmentos(value1, &borrado);
                        leds(value1);
                   sprintf(buf, "%s","OK:0,0");
                }//LEARN /*VALUE1 7SEG Y LEDS*/
				else if(strncmp(command,"TESTS",5) == 0){
                        if (value1[0] == value2[0]){
                        siete_segmentos(value1, &borrado);
                        leds(value1);
                         sprintf(buf, "%s","OK:0,0");
                        }
                        else sprintf(buf, "%s","ER:0,0");//CLEAR /*COMPARA DATOS Y SI ES CORRECTO 7SEG & LED´S*/
                } else sprintf (buf,"%s","ER:0,0");
                sendto(sock, buf, strlen(buf), 0, (struct sockaddr *) &dir_cli, length);

        }
        exit(0);
}


// Character to single digit int ctoi (char ch){
int ctoi (char ch){
        if (ch >= '0' && ch <= '9' ) return ch - '0';
        return -1; //error
}

// Funciones auxiliares
// Codificar número para mostrar en 7-segment displays int segmentos ( int number) {
int segmentos (int number){
   char digitos[5]={'\0'};
   int c, i;
   int SEG_HEX[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67};
        //{0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111};
   sprintf(digitos,"%d",number);
   c = 0x0;
      for (i=0;i<strlen(digitos);i++){
                c = c << 8 | SEG_HEX[ctoi(digitos[i])];
   }
   return c;
}




int siete_segmentos(int *value1,int *borrado){

   volatile int *BCD_ptr; // virtual address pointer to red LEDs
   int LEDR_Value; // GA: red LEDs Value
   int resul;
   int fd = -1; // used to open /dev/mem for access to physical addresses
   void *LW_virtual; // used to map physical addresses for the light-weight bridge

   if(*borrado == 0){
   resul = segmentos (value1[0]);
   }else{ resul = 0;}

   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical (fd)) == -1)
      return (-1);
   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   BCD_ptr = (unsigned int *) (LW_virtual + HEX3_HEX0_BASE);
  
   // Add 1 to the I/O register
   *BCD_ptr = resul;

   unmap_physical (LW_virtual, LW_BRIDGE_SPAN); // release the physical-memory mapping
   close_physical (fd); // close /dev/mem
   return 0;
}

int leds(int *value1){

   volatile int * LEDR_ptr; // virtual address pointer to red LEDs
   int LEDR_Value; // GA: red LEDs Value

   int fd = -1; // used to open /dev/mem for access to physical addresses
   void *LW_virtual; // used to map physical addresses for the light-weight bridge

   // Create virtual memory access to the FPGA light-weight bridge
   if ((fd = open_physical (fd)) == -1)
      return (-1);

   if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
      return (-1);

   // Set virtual address pointer to I/O port
   LEDR_ptr = (unsigned int *) (LW_virtual + LEDR_BASE);

   // Add 1 to the I/O register
   *LEDR_ptr = value1[0];

   unmap_physical (LW_virtual, LW_BRIDGE_SPAN); // release the physical-memory mapping 
     close_physical (fd); // close /dev/mem
   return 0;
}


void buffer_Params(char *buf, char *command,int *value1, int *value2) {
    char str[80] = "TESTS:12,24";
        const char s[2] = ":";
        const char s2[2] = ",";
    char compare=0;
   char value = 0;
   char *token=0;
   char *Comando=0;
   char *CMD_compare=0;


   /* get the first token */
   Comando = strtok(buf, s);


   sprintf(command, "%s",Comando);

   if(strncmp(command,"CLEAR",5) != 0){

    token = strtok(NULL, s2);

        value1[0] = atoi(token); 
	        if(strncmp(command,"TESTS",5) == 0){
				token = strtok(NULL, s2);
                value2[0] = atoi(token);
        }
   }
}
// Open /dev/mem, if not already done, to give access to physical addresses int open_physical (int fd) {
int open_physical (int fd){
   if (fd == -1)
      if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1)
      {
         printf ("ERROR: could not open \"/dev/mem\"...\n");
         return (-1);
      }
   return fd;
}
// Close /dev/mem to give access to physical addresses void close_physical (int fd) {
void close_physical (int fd){
        close (fd);
}
void* map_physical(int fd, unsigned int base, unsigned int span) {
   void *virtual_base;
   // Get a mapping from physical addresses to virtual addresses
   virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
   if (virtual_base == MAP_FAILED)
   {
      printf ("ERROR: mmap() failed...\n");
      close (fd);
      return (NULL);
   }
   return virtual_base;
}
/*
 * Close the previously-opened virtual address mapping
 */
 int unmap_physical(void * virtual_base, unsigned int span) {
   if (munmap (virtual_base, span) != 0)
   {
      printf ("ERROR: munmap() failed...\n");
      return (-1);
   }
   return 0;	
   }