/******************************************************************************
** Name      : Hypotenuse
** Copyright : Author
**
** Compiler  : TBD
** Target    : PC
**
** Description :
**    TBD
**
** $Author: $    TBD
** $RCSfile: $
**
** $Log: $
**
******************************************************************************/

/******************************************************************************
** Include
*/
#include <stdio.h>
#include <math.h>
#include <float.h>

/******************************************************************************
** Defines
*/
#define TRUE  ((bool32_t) 1u)
#define FALSE ((bool32_t) 0u)
#define FP32_POS_INFINITY ((float) INFINITY)
#define FP32_NEG_INFINITY ((float) - INFINITY)


/******************************************************************************
** Globals
*/

/******************************************************************************
** New Types Definitions
*/
#ifndef _TYPEDEFS_
#define _TYPEDEFS_

#ifndef void_t
#define void_t      void
#endif /* void_t */


/* Word32 definitions (32 bits) */
#ifndef uint32_t
typedef unsigned int       uint32_t;
#endif /* uint32_t */

/* IEEE single-precision 32-bit format */
#ifndef float_t
typedef float              float_t;
#endif /* float_t */

/* Boolean type */
#ifndef bool32_t
typedef uint32_t          bool32_t;
#endif /* bool32_t */

#endif  /* _TYPEDEFS_ */

/******************************************************************************
** Prototypes
*/
float_t hypotenuse(float_t f32_c_1, float_t f32_c_2);
bool32_t hypotenuse__unit_test(void_t);


/******************************************************************************
**  Name:    get_sqrt_base_dec
**
**  Purpose: Estimar la raiz de un numero decimal en base(10). 
**  Se tiene que cumplir que x*((20*p)+x)<=c.Siendo c el valor sobre el que calculamos la raiz.
**
**  Input Parameters:
**     *x, Puntero la variable sobre la que se itera para calular la raiz mas proxima.
**     *p, Puntero la variable sobre la que carga el resto de la operacion anterior.
**     *value, Puntero la variable que contiene el numero sobre el que se quiere calcular la raiz cuadrada.
**
**  Input parameter restrictions
**     TBD
**
**
******************************************************************************/

void get_sqrt_base_dec(int *x, int *p, int *value){

		int c= *value,resul;
		while (1){																			/*Abrimos un bucles en el que se van a realizar las iteraciones  	*/
																							/* necesarias hasta obtener el resultado.						    */
			 resul = (*x)*(20*(*p)+(*x));													/*-------------------------x*((20*p)+x)<=c--------------------------*/			
			if (resul==c){																	/*En el caso de que el resultado sea igual a c, salimos del bucle 	*/
																							/* a continuación la ultima x que hemos usado sera bufferizada.		*/
				break;																		/**********************mysqrt()**************************************/
			}																				/********************************************************************/
			else if (resul>c){																/*En el caso de que el resultado sea mayor c, carga en x el valor	*/
																							/*anterior (restandole 1) y salimos del bucle.***********************/
				(*x)--;																		/********************************************************************/
				break;//sale del bucle con el valor anterior								/**********************mysqrt()**************************************/
																							/********************************************************************/
			}else (*x)++;																	/*Mientras el resultado sea menor, seguiremos incrementando x.*******/

		}
}

/******************************************************************************
**  Name:    get_Buffer
**
**  Purpose: Divide el numero en decimales (formados por dos numeros) y los bufferiza.
**	Ademas realiza un conteo de la cantidad de numeros obtenidos como enteros y punto decimales.
**
**  Input Parameters:
**     *numero, Puntero al numero en formato float. 
**     *Buffer_Enteros, Array que almacena el numero entero de dos en dos. Ejem: 123--> 01 23
**     *num_Enteros, Numero de posiciones +1 del array Buffer_Enteros usados.
**     *Buffer_Decimales, Array que almacena el numero decimal de dos en dos. Ejem: 0.123--> 12 30
**     *num_Decimales, Numero de posiciones +1 del array Buffer_Decimales usados.
**
**  Input parameter restrictions
**     TBD
**
**
******************************************************************************/
void  get_Buffer(float *numero,int *Buffer_Enteros, int *num_Enteros, int *Buffer_Decimales, int *num_Decimales){

int parte_entera=0;
long i=0;
double parte_decimal=0;

	parte_entera = *numero/1;
	parte_decimal = *numero-parte_entera;

	    //Bufferizacion de enteros
	    for(i=1;i<1000000000;i=i*100){																									/*Bufferiza la parte entera del numero, cada posicion contiene dos 		*/
																																		/*digitos.																*/
	        if((parte_entera/i)>=1){																									/*Ejem: 123--> 01 23. 													*/
																																		/*Poisicón 0 y 1 respectivamente.										*/
	           Buffer_Enteros[(*num_Enteros)++]= (parte_entera/(i))-((parte_entera/(i))/(100))*100;										/*	num_Enteros: Contiene el numero de posiciones guardadas +1.			*/
	        }																															/************************************************************************/
	    }																																/************************************************************************/
																																							
	    //Bufferizacion de decimales																											
	    int val = parte_decimal;																										/*Bufferiza la parte decimla del numero, cada posicion contiene dos 		*/
	    for(i=1;i<1000000000;i=i*100){																									/*digitos.																	*/
																																		/*Ejem: 0,123--> 12 30. 													*/
	        val=parte_decimal*(i);																										/*Poisicón 0 y 1 respectivamente.											*/
	        val = (parte_decimal*(i))-(val/(100))*100;																					/*	num_Decimales: Contiene el numero de posiciones guardadas +1.			*/
																																		/****************************************************************************/
	        if(val>=1){																													/************************************************************/
	           Buffer_Decimales[(*num_Decimales)++]=  val;					/******************************Observaciónes*********************************************************************/													
	        }																/* Cada vez que le sometes a la variable val a una operacion de desplazamiento como es la multiplicacion *10	*/															
	    }																	/*	se observava como aparecian numeros aleatorios en las nuevas posiciones del numero  						*/															
}																			/*	Ejemplo:	356.654320; 3566,543207																			*/															
																			/*	Para eliminar el ruido primero divido por 100 y luego multiplico											*/
																			/****************************************************************************************************************/


/******************************************************************************
**  Name:    get_sqrt_base_dec
**
**  Purpose: Calculo de la raiz cuadrada meidante el metodo decimal en base(10). 
**
**  Input Parameters:
**     numero, El numero sobre el que se quiere calcular la raiz cuadrada.
**
**  Input parameter restrictions
**     TBD
**
**  Bases del calculo de la raiz cuadrad mediante el metodo decimal en base (10).
**  Ejemplo: Calculo de la raiz cuadrada de 152.2756.
**		________________
**     /
**   \/  01 52.27 56
**
**       01                   1*1 <= 1 < 2*2                 x = 1
**       01                     y = x*x = 1*1 = 1
**       00 52                22*2 <= 52 < 23*3              x = 2
**       00 44                  y = (20+x)*x = 22*2 = 44
**          08 27             243*3 <= 827 < 244*4           x = 3
**          07 29               y = (240+x)*x = 243*3 = 729
**             98 56          2464*4 <= 9856 < 2465*5        x = 4
**             98 56            y = (2460+x)*x = 2464*4 = 9856
**             00 00          Algorithm terminates: Answer is 12.34
**
******************************************************************************/

float mysqrt(float numero){

    static int Buffer_Enteros[100]={0,1,2,3},i,num_Enteros,Buffer_Decimales[100],num_Decimales=0, resultado_Enteros[30],resto,resultado_Decimales[30];
    static double resultado;
    int x=0,p=0;

    resultado=0;
    i = 0;
    num_Enteros = 0;
    num_Decimales=0;
    resto = 0;

    if (numero == FP32_POS_INFINITY) return FP32_POS_INFINITY;
    else if (numero == FP32_NEG_INFINITY) return FP32_NEG_INFINITY;

    get_Buffer(&numero,Buffer_Enteros, &num_Enteros, Buffer_Decimales, &num_Decimales);

		///////////////////////////////
        //Calculo de la raiz cuadrada//
		///////////////////////////////
		
        //1. Raiz de los enteros
        int counter=0,value;
       while(num_Enteros!=0){																							


    	x=0;																								/********************************************************************/
    	value = Buffer_Enteros[num_Enteros-1];																/*Recorre el Buffer de numeros enteros y calcula la raiz de cada uno*/
    	if (resto > 0) 	value = resto*100 + Buffer_Enteros[num_Enteros-1];                  				/*Para cada caso actualiza el valor de "p".							*/
        get_sqrt_base_dec(&x, &p, &value);                                                                  /********************************************************************/ 
        resultado_Enteros[counter]=x;                                                                            
        counter++;                                                                                                        
                                                                                                                                
        resto= value - (x*(20*p+x));                                                                             
        p=p*10+x;

        num_Enteros--;
       }

       for (i=0;i<counter;i++){																				/************************************************************************/
                                                                                                            /*El resultado obtenido esta almacendo en un array.						*/
    	   resultado = resultado*10 + resultado_Enteros[i];                                              	/*	Carga el resultado desde un array, sobre un float llamado resultado	*/
       }                                                                                                    /************************************************************************/ 



       counter=0;
       int dec_counter=0;
       //2.Calculo decimal
       if(num_Decimales>0){

    	   while(dec_counter<=num_Decimales){

																											/************************************************************************/			
    		x=0;                                                                                            /*En el caso de que el numero tenga decimales repetimos el mismo 		*/ 
    		value = Buffer_Decimales[dec_counter];                                                          /*proceso sobre el array de decimales.									*/ 
    		if (resto > 0) 	value = resto*100 + Buffer_Decimales[dec_counter];                				/************************************************************************/ 
    	    get_sqrt_base_dec(&x, &p, &value);
    	    resultado_Decimales[counter]=x;
    	    counter++;

    	    resto= value - (x*(20*p+x));
    	    p=p*10+x;

    	    dec_counter++;
    	   }

       }

       int Scounter=0;
    	if (resto > 0){

    		while(Scounter < 7){

																					/*********************************************************************************************************/
    	  			x=0;                                                         	/*Repetimos la operaion suponiendo el caso peor. Es decir que el numero no tuviese decimales.			 */ 
    	  			if (resto > 0) 	value = resto*100;            					/*Repetimos la operacion 7 veces ya que el float solo usa 6 y el 7 puede influir sobre el redondeo del 6.*/
    	  		    get_sqrt_base_dec(&x, &p, &value);          					/*********************************************************************************************************/  
    	  		    resultado_Decimales[counter]=x;
    	  		    counter++;
    	  		    Scounter++;

    	  		    resto= value - (x*(20*p+x));
    	  		    if (resto==0) break;
    	  		    p=p*10+x;

    	  		    num_Enteros--;

    	  	   }
    	   }

		   float bridge; //La variable bridege la uso para generar el decimal antes de sumarlo al resultado.
    	   int u=10;
			for (i=0;i<counter;i++){								/************************************************************************/			
                                                                    /*El resultado obtenido esta almacendo en un array.						*/
    		   bridge=resultado_Decimales[i];                   	/*	Carga el resultado desde un array, sobre un float llamado resultado	*/ 
    		   bridge = bridge / u;                                 /************************************************************************/ 

    		   resultado = resultado + bridge;

    		   u*=10;
    	   }

       return resultado;
}

/******************************************************************************
**  Name:    hypotenuse
**
**  Purpose: Hypotenuse calculation for right-angled triangle
**
**  Input Parameters:
**     f32_c_1, Triangle side c1
**     f32_c_2, Triangle side c2
**
**  Input parameter restrictions
**     TBD
**
**  Return Parameter: Hypotenuse
**
******************************************************************************/

float_t hypotenuse(float_t f32_c_1, float_t f32_c_2)
{
	float_t f32_result;

	/*
	*| 1. Defensive programming
	*/
	if (isnan(f32_c_1) || isnan(f32_c_2))
	{
		f32_result = NAN;
	}
	else if (isinf(f32_c_1) || isinf(f32_c_2))
	{
		f32_result = FP32_POS_INFINITY;
	}
	else
	{
		/*
		*| 2. Hypotenuse calculation
		*/
		/* TBD */
		/* 2.1. Default implementation */

		//f32_result = sqrtf((f32_c_1 * f32_c_1) + (f32_c_2 * f32_c_2));

		
		/*Decimal (base 10) implementation*/	
		
		printf(" entrada %f",((f32_c_1 * f32_c_1) + (f32_c_2 * f32_c_2)));
		f32_result = mysqrt((f32_c_1 * f32_c_1) + (f32_c_2 * f32_c_2));
		printf(" resultado %f",f32_result);


	}

	/* TBD */
	return f32_result;
}

/******************************************************************************
**  Name:    hypotenuse__unit_test
**
**  Purpose: Unit Test of <Hypotenuse calculation for right-angled triangle>
**
**  Input Parameters:
**     None
**
**  Return Parameter: Boolean (TRUE == success)
**
******************************************************************************/
bool32_t hypotenuse__unit_test(void_t)
{
	bool32_t b32_test = TRUE;

	/*
	*| 1. Default Unit Test
	*/
	/* 1.1. NAN with other combination */
	b32_test &= isnan(hypotenuse(NAN, NAN));
	b32_test &= isnan(hypotenuse(NAN, FP32_POS_INFINITY));
	b32_test &= isnan(hypotenuse(FP32_POS_INFINITY,  NAN));
	b32_test &= isnan(hypotenuse(NAN, FP32_POS_INFINITY));
	b32_test &= isnan(hypotenuse(FP32_NEG_INFINITY, NAN));
	b32_test &= isnan(hypotenuse(NAN, 0.0f));
	b32_test &= isnan(hypotenuse(0.0f, NAN));
	b32_test &= isnan(hypotenuse(NAN, 1.0f));
	b32_test &= isnan(hypotenuse(1.0f, NAN));
	b32_test &= isnan(hypotenuse(NAN, -1.0f));
	b32_test &= isnan(hypotenuse(-1.0f, NAN));

	/* 1.2. INFINITE with other combination, except for NAN */
	b32_test &= isinf(hypotenuse(FP32_POS_INFINITY, FP32_POS_INFINITY));
	b32_test &= isinf(hypotenuse(0.0f, FP32_POS_INFINITY));
	b32_test &= isinf(hypotenuse(FP32_POS_INFINITY, 0.0f));
	b32_test &= isinf(hypotenuse(FP32_NEG_INFINITY, FP32_NEG_INFINITY));
	b32_test &= isinf(hypotenuse(FP32_NEG_INFINITY, FP32_POS_INFINITY));
	b32_test &= isinf(hypotenuse(0.0f, FP32_NEG_INFINITY));
	b32_test &= isinf(hypotenuse(FP32_NEG_INFINITY, 0.0f));

	/* 1.3. Not-special values */
	b32_test &= (hypotenuse(0.0f, 0.0f) == hypotf(0.0f, 0.0f));
	b32_test &= (hypotenuse(1.0f, 1.0f) == hypotf(1.0f, 1.0f));
	b32_test &= (hypotenuse(-1.0f, 1.0f) == hypotf(-1.0f, 1.0f));
	b32_test &= (hypotenuse(1.0f, -1.0f) == hypotf(1.0f, -1.0f));
	b32_test &= (hypotenuse(-1.0f, -1.0f) == hypotf(-1.0f, -1.0f));

	/* 1.4. Maximum values */
	b32_test &= (hypotenuse(FLT_MAX, 0.0f) == hypotf(FLT_MAX, 0.0f)) || isinf(hypotenuse(FLT_MAX, 0.0f));
	b32_test &= (hypotenuse(FLT_MIN, 0.0f) == hypotf(FLT_MIN, 0.0f)) || (hypotenuse(FLT_MIN, 0.0f) == 0.0f);

	/*
	*| Extended Unit Test
	*/

	b32_test &= (hypotenuse(FP32_NEG_INFINITY, 381.5460f) == hypotf(FP32_NEG_INFINITY, 381.5460f));
	b32_test &= (hypotenuse(381.5460f, FP32_NEG_INFINITY) == hypotf(381.5460f, FP32_NEG_INFINITY));
	b32_test &= (hypotenuse(9999, 1.0f) == hypotf(9999, 1.0f));
	b32_test &= (hypotenuse(1.0f, 9999) == hypotf(1.0f,9999));
	b32_test &= (hypotenuse(0.123456789546f, -1.0f) == hypotf(0.123456789546f, -1.0f));
	b32_test &= (hypotenuse(-1.0f, 0.123456789546f) == hypotf(-1.0f, 0.123456789546f));

	/* TBD */

	return b32_test;
}

/******************************************************************************
**  Name:    main
**
**  Purpose: Program main
**
**  Input Parameters:
**     None
**
**  Return Parameter: None
**
******************************************************************************/
int main(void_t)
{
	bool32_t b_result = hypotenuse__unit_test();
	printf("\n Hypotenuse Unit Test result = %s", (b_result) ? "OK" : "NOK");
}


/*
** End of $RCSfile: $
******************************************************************************/
