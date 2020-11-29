
// PREPROCESOS.C
#include "preprocesos.h"

#include <string.h>
#include <stdlib.h>


//variables im caracter


// FILTRO BASURA
void Limpiar (unsigned char ini_fil,unsigned char fin_fil,unsigned char ini_col,unsigned char fin_col,unsigned char *imagen,unsigned char *columnas)
{
	int i,j;

	for (i=ini_fil;i<fin_fil;i++)
		for (j=ini_col;j<fin_col;j++) *(imagen+(i*(*columnas))+j)=0;
}

void EliminarBasura (unsigned char *imagenBasura, unsigned char* filas, unsigned char* columnas)
{
	unsigned char i,j,col_25,fil_25;

	col_25 = (unsigned char)((float)(*columnas)*0.25+0.5);
	fil_25 = (unsigned char)((float)(*filas)*0.15+0.5);
	
	//Limpiar Izquierda
	for (j=col_25;j>0;j--)
	{	
		for (i=0;i<(*filas);i++) if (*(imagenBasura+(i*(*columnas))+j)==1) break;
		if (i==(*filas)) break;
	}
	if (j>=0) Limpiar(0,(*filas),0,j,imagenBasura,columnas);

	//Limpiar Derecha
	for (j=(*filas)-col_25;j<(*filas);j++)
	{	
		for (i=0;i<(*filas);i++) if (*(imagenBasura+(i*(*columnas))+j)==1) break;
		if (i==(*filas)) break;
	}
	if (j<(*filas)) Limpiar(0,(*filas),j,(*filas),imagenBasura,columnas);
	
	//Limpiar Superior
	for (i=fil_25;i>0;i--)
	{	
		for (j=0;j<(*filas);j++) if (*(imagenBasura+(i*(*columnas))+j)==1) break;
		if (j==(*filas)) break;
	}
	if (i>=0) Limpiar(0,i,0,(*filas),imagenBasura,columnas);	
	
	//Limpiar Inferior
	for (i=(*filas)-fil_25;i<(*filas);i++)
	{	
		for (j=0;j<(*filas);j++) if (*(imagenBasura+(i*(*columnas))+j)==1) break;
		if (j==(*filas)) break;
	}
	if (i<(*filas)) Limpiar(i,(*filas),0,(*filas),imagenBasura,columnas);
}


//FILTRO GIRAR
void Girar(unsigned char **imagenGirarPtr, unsigned char* filas, unsigned char* columnas)
{
  unsigned char i,j,valor;
  float xi,yi,xiyi,xi2,nn,mediax,mediay,pendiente,h,difer; //parametros Giro
  unsigned char* wim; //auxiliar
  wim=(unsigned char *)malloc((*filas)*(*columnas)*sizeof(unsigned char *));
  for(i=0;i<(*filas);i++){
    for(j=0;j<(*columnas);j++){
        *(wim+(i*(*columnas))+j)=0;
    }
  }
  
  xi=yi=xiyi=xi2=nn=(float)0.0;//parametros de la recta de regresion

  // Calculo de parametros de giro (para el calculo de la recta de regresion) 
  for(i=0;i<(*filas);i++)
    for(j=0;j<(*columnas);j++)
    {	
        if (*((*imagenGirarPtr)+(i*(*columnas))+j)==1)
		{        
			xi+=(i+1);
			yi+=(j+1);
			xiyi+=(i+1)*(j+1);
			xi2+=(i+1)*(i+1);
			nn++;
		}
    }
  mediax=xi/nn;
  mediay=yi/nn;
  pendiente=((xiyi/nn)-(mediax*mediay))/((xi2/nn)-(mediax*mediax));
  for(i=0;i<(*filas);i++)
  {
	//valor previsto de la funcion
    h=mediay+pendiente*((float)(i+1)-mediax);
    // girar en funcion del valor previsto 
    difer=h-mediay;
    for (j=0;j<(*columnas);j++)
    {
      if(*((*imagenGirarPtr)+(i*(*columnas))+j)==1)
      {  
        valor=(int)((float)j-difer);
        if(valor<0) valor=0;
        else
          if(valor>=(*columnas)) valor=(*columnas)-1;
        *(wim+(i*(*columnas))+valor)=1;
      }
    }
  }
  //copiar a (*imagenGirarPtr)
  free(*imagenGirarPtr);
  (*imagenGirarPtr)=wim;
}

// AJUSTAR IM DEL DIGITO
void AjustarIM(unsigned char **imagenAjustarPtr, unsigned char* filas, unsigned char* columnas)
{
	unsigned char fili,filf,coli,colf;
	unsigned char i,j,fin;
	
	//establecer los límites de la imagen
	fin=0;
	for (i=0;i<(*filas);i++)
	{
		for (j=0;j<(*columnas);j++) if (*((*imagenAjustarPtr)+(i*(*columnas))+j)==1) {fin=1; break;}
		if (fin) {fili=i; break;}
	}
	fin=0;
	for (i=(*filas)-1;i>=0;i--)
	{
		for (j=0;j<(*columnas);j++) if (*((*imagenAjustarPtr)+(i*(*columnas))+j)==1) {fin=1; break;}
		if (fin) {filf=i; break;}
	}
	fin=0;
	for (i=0;i<(*columnas);i++)
	{
		for (j=0;j<(*filas);j++) if (*((*imagenAjustarPtr)+(j*(*columnas))+i)==1) {fin=1; break;}
		if (fin) {coli=i; break;}
	}
	fin=0;
	for (i=(*columnas)-1;i>=0;i--)
	{
		for (j=0;j<(*filas);j++) if (*((*imagenAjustarPtr)+(j*(*columnas))+i)==1) {fin=1; break;}
		if (fin) {colf=i; break;}
	}

	//copiar la imagen ajustada
	unsigned char* wim; //auxiliar
    unsigned char wfilas=filf-fili+1;
    unsigned char wcolumnas=colf-coli+1;
    wim=(unsigned char *)malloc(wfilas*wcolumnas*sizeof(unsigned char));
    for(i=0;i<wfilas;i++){
        for(j=0;j<wcolumnas;j++){
            *(wim+(i*wcolumnas)+j)=0;
        }
    }
	for (i=fili;i<=filf;i++){
		for (j=coli;j<=colf;j++){
            *(wim+((i-fili)*wcolumnas)+(j-coli))=*((*imagenAjustarPtr)+(i*(*columnas))+j);
        }
    }
    free(*imagenAjustarPtr);
    (*filas)=wfilas;
	(*columnas)=wcolumnas;
    (*imagenAjustarPtr)=wim;
}

/*ANADIR LAS NUEVAS FUNCIONES DE PREPROCESO */
