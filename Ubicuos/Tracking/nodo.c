#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/leds.h"
#include "random.h"
#include "node-id.h"
#include <stdio.h>
#include "powertrace.h"

/*-------------------------------------------------------*/
PROCESS(nodo, "Nodo");
AUTOSTART_PROCESSES(&nodo);
/*-------------------------------------------------------*/



struct multihop_Message {
	int beaconId;
	int nodeType;
	int salto;
};



//Almacenar baliza
struct beacon {
	char beaconId;
	int rssi;
	char type;
};
//Array 3 balizas
struct beacon_3 {
	struct beacon beacons[3];
	int n_balizas;
	int my_id;
	unsigned long tiempo;
	int destino_id;
} beacon_Temp_3 ={.n_balizas=0,.my_id=-1,.tiempo=0,.destino_id=0};

char id_nodo=-1;             // To store the Node Id

//Variables multihop
char id_enlace=-1;
char salto_enlace=-1;
int  rssi_enlace=-88;
//-------------------

unsigned long sec;
//-------------------

static void print_beacon( struct beacon Baliza){

	printf("ID: %d, Type: %d, Rssi: %d\n", Baliza.beaconId, Baliza.type, Baliza.rssi);	

}

#define MAX_ITEMS    20
typedef struct Queue
{
    int     first;
    int     last;
    int     validItems;
    struct beacon_3     data[MAX_ITEMS];
};
struct Queue circularQueue_t; //Histórico balizas

void initializeQueue(struct Queue *theQueue);

int isEmpty(struct Queue *theQueue);

int putItem(struct Queue *theQueue, struct beacon_3 theItemValue);

int getItem(struct Queue *theQueue, struct beacon_3 *theItemValue);

void printQueue(struct Queue *theQueue);



void initializeQueue(struct Queue *theQueue)
{
	
    theQueue->validItems  =  0;
    theQueue->first       =  0;
    theQueue->last        =  0;
    
}

int isEmpty(struct Queue *theQueue)
{
    if(theQueue->validItems==0)
        return(1);
    else
        return(0);
}

int putItem(struct Queue *theQueue, struct beacon_3 theItemValue)
{
    if(theQueue->validItems>=MAX_ITEMS)
    {
        //~ printf("The queue is full\n");
        //~ printf("You cannot add items\n");
        return(-1);
    }
    else
    {
        theQueue->validItems++;
        theQueue->data[theQueue->last] = theItemValue;
        theQueue->last = (theQueue->last+1)%MAX_ITEMS;
        return 0;
    }
}

int getItem(struct Queue *theQueue, struct beacon_3 *theItemValue)
{
    if(isEmpty(theQueue))
    {
        //~ printf("isempty\n");
        return(-1);
    }
    else
    {
        *theItemValue=theQueue->data[theQueue->first];
        theQueue->first=(theQueue->first+1)%MAX_ITEMS;
        theQueue->validItems--;
        return(0);
    }
}

void printQueue(struct Queue *theQueue)
{
	printf("//////////////Cola historico/////////////\n");
    int aux, aux1;
    aux  = theQueue->first;
    aux1 = theQueue->validItems;
	//~ printf("Elementos validos: %d\n",aux1);

    while(aux1>0)
    {
        int i=0;
        for(i;i<3;i++)
			print_beacon(theQueue->data[aux].beacons[i]);
			printf("       -------       \n");
        aux=(aux+1)%MAX_ITEMS;
        aux1--;
    }
	//~ printf("/////////////////////////////////////////\n");

    return;
}

/*----------------------------------------*/

static int Beacon_Comp(struct beacon Baliza)
{
	int i=0;
	
	for(i; i<beacon_Temp_3.n_balizas; i++){
		if (beacon_Temp_3.beacons[i].beaconId == Baliza.beaconId){  //Mismo ID
				
			if (beacon_Temp_3.beacons[i].rssi < Baliza.rssi)
				beacon_Temp_3.beacons[i]=Baliza;
				
			return 0;	
		}
	}
	
	if (beacon_Temp_3.n_balizas < 3) {
		beacon_Temp_3.beacons[beacon_Temp_3.n_balizas] = Baliza;  // buffer no completo
		beacon_Temp_3.n_balizas++;
		return 0;
	}
	
	for(i=0; i<3; i++) {

		if (beacon_Temp_3.beacons[i].rssi < Baliza.rssi){  // Mejor rssi
			beacon_Temp_3.beacons[i]=Baliza;
			return 0;
		}
	}
	
	return 1;
	
}

static void Enlace_comp(struct multihop_Message *msg_content,int rssi){
	

	if(salto_enlace==-1){
		id_enlace=msg_content->beaconId;
		salto_enlace=msg_content->salto;
		rssi_enlace=rssi;
			printf("%d --> %d\n",id_nodo,id_enlace);
		
	}else if(msg_content->salto < salto_enlace){
		
		salto_enlace=msg_content->salto;
		id_enlace=msg_content->beaconId;
		rssi_enlace=rssi;
			printf("%d --> %d\n",id_nodo,id_enlace);
		
	}else if ( msg_content->salto == salto_enlace){
		
		if(rssi>rssi_enlace){
			salto_enlace=msg_content->salto;
			id_enlace=msg_content->beaconId;
			rssi_enlace=rssi;
			printf("%d --> %d\n",id_nodo,id_enlace);

		}	
	}
	
}
/*---------------------------------------*/

static void msg_recv(struct abc_conn *c) {
	
	struct multihop_Message *msg_content = packetbuf_dataptr();
	struct beacon Baliza;
	
	Baliza.beaconId=msg_content->beaconId;
	Baliza.type=msg_content->nodeType;
	Baliza.rssi=(int) packetbuf_attr(PACKETBUF_ATTR_RSSI);
	//~ print_beacon(Baliza);
	
	if(Baliza.type==1)
		Beacon_Comp(Baliza);
	
	Enlace_comp(msg_content,(int) packetbuf_attr(PACKETBUF_ATTR_RSSI));

}

static const struct abc_callbacks abc_call_multihop = {msg_recv};
static const struct abc_callbacks abc_call_historico;

static struct abc_conn abc_Multihop;
static struct abc_conn abc_Historico;

/*------------------------------------------------------*/


PROCESS_THREAD(nodo, ev, data)
{
	//Variables timers
    static struct beacon_3 Beacon_3_sink;
	static struct etimer timer_Beacon_Register,timer_Beacon_Hist_To_Sink, timer_Enlace_reset, timer_desfase;

	//powertrace_start(CLOCK_SECOND * 1);

  
	PROCESS_EXITHANDLER(abc_close(&abc_Historico));
	PROCESS_EXITHANDLER(abc_close(&abc_Multihop));

	PROCESS_BEGIN();
	id_nodo = (int) node_id;
	
	    /* Esperar desfase*/
	etimer_set(&timer_desfase, CLOCK_SECOND * random_rand()/4369/*(65535/15)*/ );

	PROCESS_WAIT_EVENT();
	while(ev != PROCESS_EVENT_TIMER);
	abc_open(&abc_Multihop, 126, &abc_call_multihop);
	abc_open(&abc_Historico, 127, &abc_call_historico);
		
	initializeQueue(&circularQueue_t);
	int i;
	for(i=0;i<3;i++){
		beacon_Temp_3.beacons[i].beaconId=-1;
		beacon_Temp_3.beacons[i].rssi=-1;
		beacon_Temp_3.beacons[i].type=-1;
	}
	etimer_set(&timer_Beacon_Register, (CLOCK_SECOND*60 ));
	etimer_set(&timer_Beacon_Hist_To_Sink, (CLOCK_SECOND));
	etimer_set(&timer_Enlace_reset,(CLOCK_SECOND)*180);
	
	
	  while(1) {
		  
		sec=clock_seconds();
		PROCESS_WAIT_EVENT();
		
		if(ev == PROCESS_EVENT_TIMER) {
			
			if(data == &timer_Beacon_Register){  // Timer guardado en histórico de mejores balizas (3)
				
				putItem(&circularQueue_t,beacon_Temp_3);
				for(i=0;i<3;i++){
					beacon_Temp_3.beacons[i].beaconId=-1;
					beacon_Temp_3.beacons[i].rssi=-1;
					beacon_Temp_3.beacons[i].type=-1;
				}
				
				beacon_Temp_3.n_balizas=0;
				
				//~ printQueue(&circularQueue_t);
				etimer_reset(&timer_Beacon_Register);

			}else if (data == & timer_Beacon_Hist_To_Sink){ // Timer envío de histórico a sink
				if(id_enlace!=-1){
					
					if(!isEmpty(&circularQueue_t)){
						printf("Enviado balizas...\n");
							getItem(&circularQueue_t,&Beacon_3_sink);
							Beacon_3_sink.my_id=id_nodo;
							Beacon_3_sink.tiempo=sec;
							Beacon_3_sink.destino_id=id_enlace;
							//~ printf("--> Enviando datos a baliza: %d\n",id_enlace);
							packetbuf_copyfrom(&Beacon_3_sink, sizeof(struct beacon_3));
							abc_send(&abc_Historico);
					}
				}
				etimer_reset(&timer_Beacon_Hist_To_Sink);
				
			} else if (data == &timer_Enlace_reset){
				
				id_enlace=-1;
				salto_enlace=-1;
				rssi_enlace=-88;
				//~ printf("Reset de enlace: %d, salto %d , rssi %d\n",id_enlace, salto_enlace, rssi_enlace);
				etimer_reset(&timer_Enlace_reset);


			}
		}
	}

  	

  

  PROCESS_END();
}



