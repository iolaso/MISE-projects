/**********************************************************************
* © 2017 LG
*
 Proyecto: MISE Micros 16bits 2017
* Fichero: CAD_NO_DMA_G1_v0.h

Grupo: G1    Autor: Borja Gorriz y Edurne Sagasta
	Fecha: 2017-11-27
*  v0
*
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/
#define TAMBUF 64

//==========================================
void Inic_CAD_DMA (void);
extern int CAD_BuffA[TAMBUF] __attribute__((space(dma)));