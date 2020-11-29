#define n_filas 2
#define n_columnas 16

extern char Ventana_LCD [n_filas][n_columnas];
extern unsigned char Ventana_Datos [8][19] __attribute__((space(dma)));
extern const char Texto_1[];

extern unsigned char respuesta[9];
extern unsigned int Flag_trama_ok;
void Copia_FLASH_Ventana_LCD(const char *texto);
extern int flag_osc_80, flag_CAD1, leds_counter;

extern long int ADCValue,ADCValue2,angulo;
