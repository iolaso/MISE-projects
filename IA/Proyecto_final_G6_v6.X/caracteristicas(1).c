
// CARACTERISTICAS.C

#include <string.h>
#include <stdlib.h>

#include "caracteristicas.h"

//caracterstica zonning
int Saca_Vec (unsigned char *v_menor,unsigned char *v_mayor,unsigned char *v_cuanto,unsigned char menor,unsigned char mayor)
{
  unsigned char ind_menor,ind_mayor;
  unsigned char llevo_menor,llevo_mayor;
  unsigned char queda_menor,queda_mayor;
  unsigned char consumo_total,i;
 
  ind_menor=ind_mayor=v_menor[0]=v_mayor[0]=0;
  v_cuanto[0]=llevo_menor=llevo_mayor=menor;
  consumo_total=menor*mayor-menor;
 
  for (i=1;consumo_total!=0;i++)
  {
    if (llevo_menor==mayor)
    {
      ind_menor++;
      llevo_menor=0;
    }
    if (llevo_mayor==menor)
    {
      ind_mayor++;
      llevo_mayor=0;
    }
    v_menor[i]=ind_menor;
    v_mayor[i]=ind_mayor;
    queda_menor=mayor-llevo_menor;
    queda_mayor=menor-llevo_mayor;
    if (queda_menor>queda_mayor) v_cuanto[i]=queda_mayor;
    else v_cuanto[i]=queda_menor;
    llevo_menor+=v_cuanto[i];
    llevo_mayor+=v_cuanto[i];
    consumo_total-=v_cuanto[i];
  }
  return (i);
}

void Zoning (unsigned char *filZoning,unsigned char *colZoning,unsigned char* filas,unsigned char* columnas, unsigned char* clase,unsigned char *preCaracteristicas,unsigned char **postCaracteristicas)
{
  unsigned char v_menor1[120],v_mayor1[120], v_cuanto1[120];
  unsigned char v_menor2[120],v_mayor2[120], v_cuanto2[120];
  unsigned short **peque1;
  unsigned char *vcarac; 
  unsigned char i,dim1,dim2,j;  
  
  vcarac=(unsigned char*) malloc((*filZoning)*(*colZoning)*sizeof(unsigned char));
  peque1=(unsigned short**) malloc((*filZoning)*sizeof(unsigned short*));
  for (i=0;i<(*filZoning);i++) peque1[i]=(unsigned short*)malloc((*colZoning)*sizeof(unsigned short));
  for (i=0;i<(*filZoning);i++)
	  for (j=0;j<(*colZoning);j++) peque1[i][j]=0;

  if ((*filas)<(*filZoning))
    dim1=Saca_Vec(v_menor1,v_mayor1,v_cuanto1,(*filas),(*filZoning));
  else  
    dim1=Saca_Vec(v_menor1,v_mayor1,v_cuanto1,(*filZoning),(*filas));

  if ((*columnas)<(*colZoning))
    dim2=Saca_Vec(v_menor2,v_mayor2,v_cuanto2,(*columnas),(*colZoning));
  else  
    dim2=Saca_Vec(v_menor2,v_mayor2,v_cuanto2,(*colZoning),(*columnas));

  if(((*filas)>=(*filZoning))&&((*columnas)>=(*colZoning)))
  {
    for (i=0;i<dim1;i++)
      for (j=0;j<dim2;j++)
        if ((*(preCaracteristicas+(v_mayor1[i]*(*columnas))+v_mayor2[j]))==1)
            peque1[v_menor1[i]][v_menor2[j]]+=(unsigned short)(v_cuanto1[i]*v_cuanto2[j]);
  }              
  else
    if(((*filas)>=(*filZoning))&&((*columnas)<(*colZoning)))
    {
      for (i=0;i<dim1;i++)
        for (j=0;j<dim2;j++)
        if ((*(preCaracteristicas+(v_mayor1[i]*(*columnas))+v_menor2[j]))==1)
           peque1[v_menor1[i]][v_mayor2[j]]+=(unsigned short)(v_cuanto1[i]*v_cuanto2[j]);
	}              
    else
      if(((*filas)<(*filZoning))&&((*columnas)>=(*colZoning)))
      {
        for (i=0;i<dim1;i++)
          for (j=0;j<dim2;j++)
            if ((*(preCaracteristicas+(v_menor1[i]*(*columnas))+v_mayor2[j]))==1)
             peque1[v_mayor1[i]][v_menor2[j]]+=(unsigned short)(v_cuanto1[i]*v_cuanto2[j]);
	  }              
      else
        for (i=0;i<dim1;i++)
          for (j=0;j<dim2;j++)
            if ((*(preCaracteristicas+(v_mayor1[i]*(*columnas))+v_menor2[j]))==1)
             peque1[v_mayor1[i]][v_mayor2[j]]+=(unsigned short)(v_cuanto1[i]*v_cuanto2[j]);

  int cf,cc;
  if((*filas)>(*filZoning)) cf=(*filas);
  else cf=(*filZoning);
  if((*columnas)>(*colZoning)) cc=(*columnas);
  else cc=(*colZoning);
  
            
  for (i=0;i<(*filZoning);i++) 
    for (j=0;j<(*colZoning);j++){
        vcarac[i*(*colZoning)+j] = (unsigned char)100*peque1[i][j]/(cf*cc);
    }
  // volcar el zoning al fichero fdcar
  (*postCaracteristicas)=vcarac;
  (*postCaracteristicas)[(*filZoning)*(*colZoning)] = *clase;

  for (i=0;i<(*filZoning);i++) free(peque1[i]);
  free(peque1);
  
}  

/* ANADIR LAS NUEVAS FUNCIONES DE CARACTERISTICAS */


