#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/leds.h"
#include "random.h"
#include "node-id.h"
#include <stdio.h>
#include "powertrace.h"
#include <stdlib.h>

/*-------------------------------------------------------*/
PROCESS(baliza, "Baliza");
AUTOSTART_PROCESSES(&baliza);
/*-------------------------------------------------------*/

struct multihop_Message {
	int beaconId;
	int nodeType;
	int salto;
};

int id_baliza=-1;             // To store the Node Id

//Variables multihop
char id_enlace=-1;
char salto_enlace=-1;
int  rssi_enlace=-88;
//------------------
int espera=0;
//~ int flag_sink_status = 0;

//Almacenar baliza
struct beacon {
	char beaconId;
	int rssi;
	char type;
};

//Array 3 balizas
//Array 3 balizas
struct beacon_3 {
	struct beacon beacons[3];
	int n_balizas;
	int my_id;
	unsigned long tiempo;
	int destino_id;
} beacon_Temp_3 ={.n_balizas=0,.my_id=-1,.tiempo=0,.destino_id=0};

struct beacon_3 Beacon_3_sink ;

static struct abc_conn abc_Historico;
static struct abc_conn abc_Multihop;

static void Enlace_comp(struct multihop_Message *baliza_multihop,int rssi){
	
	if(salto_enlace==-1){
		salto_enlace=baliza_multihop->salto;
		id_enlace=baliza_multihop->beaconId;
		rssi_enlace=rssi;
		//~ printf("%d --> %d\n",id_baliza,id_enlace);

	}else if(baliza_multihop->salto < salto_enlace){
		
		salto_enlace=baliza_multihop->salto;
		id_enlace=baliza_multihop->beaconId;
		rssi_enlace=rssi;
		//~ printf("%d --> %d\n",id_baliza,id_enlace);
		
	}else if ( baliza_multihop->salto == salto_enlace){
		
		if(rssi>rssi_enlace){
			salto_enlace=baliza_multihop->salto;
			id_enlace=baliza_multihop->beaconId;
			rssi_enlace=rssi;
			//~ printf("%d --> %d\n",id_baliza,id_enlace);

		}	
	}
	
}


static void multihop_msg_recv(struct abc_conn *c) {
	
	struct multihop_Message *msg_content = packetbuf_dataptr();
	Enlace_comp(msg_content,(int) packetbuf_attr(PACKETBUF_ATTR_RSSI));
}

static void msg_Baliza_Recv(struct abc_conn *c) {
	
	struct beacon_3 *msg_content = packetbuf_dataptr();
	
	int i =0;
	char str[80];
		
	
		if (msg_content->destino_id == id_baliza){
			
			//~ printf("Destino: %d, origen: %d\n", msg_content->destino_id, msg_content->my_id);
			for(i;i<3;i++){
				Beacon_3_sink.beacons[i] = msg_content->beacons[i];
			}	
			Beacon_3_sink.n_balizas = msg_content->n_balizas;
			Beacon_3_sink.my_id = msg_content->my_id;
			Beacon_3_sink.tiempo = msg_content->tiempo;
			Beacon_3_sink.destino_id = id_enlace;
			
			packetbuf_copyfrom(&Beacon_3_sink, sizeof(struct beacon_3));
			abc_send(&abc_Historico);
						
			}


}

static const struct abc_callbacks abc_call_multihop = {multihop_msg_recv};
static const struct abc_callbacks abc_call_historico = {msg_Baliza_Recv};







PROCESS_THREAD(baliza, ev, data)
{
	
	static struct etimer timer_desfase,timer_MultiHop_Message, timer_Reinicio_Enlace, timer_Espera_Reinicio_Enlace;
    static struct multihop_Message multihop_content;

	PROCESS_EXITHANDLER(abc_close(&abc_Multihop);)
	PROCESS_EXITHANDLER(abc_close(&abc_Historico);)

	PROCESS_BEGIN();	

	//powertrace_start(CLOCK_SECOND * 1);

	id_baliza = (int) node_id;
	
     /* Esperar desfase*/
	etimer_set(&timer_desfase, CLOCK_SECOND * random_rand()/4369/*(65535/15)*/ );

	PROCESS_WAIT_EVENT();
	while(ev != PROCESS_EVENT_TIMER);
		int i;
	for(i=0;i<3;i++){
		beacon_Temp_3.beacons[i].beaconId=-1;
		beacon_Temp_3.beacons[i].rssi=-1;
		beacon_Temp_3.beacons[i].type=-1;
	}

	//Recepción de datos del nodo
	abc_open(&abc_Historico, 127, &abc_call_historico);
	
	//Envio/Recepcion de multihop
	abc_open(&abc_Multihop, 126, &abc_call_multihop);
	
	
  	
  	etimer_set(&timer_MultiHop_Message, (CLOCK_SECOND*20));
	etimer_set(&timer_Reinicio_Enlace, CLOCK_SECOND *370);
	
	
  		printf("Baliza id: %d\n",id_baliza);
		
	  while(1) {
		PROCESS_WAIT_EVENT();
		
		if(ev == PROCESS_EVENT_TIMER) {
			
			if(data == &timer_MultiHop_Message){
				
				if (id_enlace!=-1 && espera==0){
					multihop_content.beaconId=id_baliza;
					multihop_content.salto=salto_enlace+1;
					multihop_content.nodeType=1;
					packetbuf_copyfrom(&multihop_content, sizeof(struct multihop_Message));
					abc_send(&abc_Multihop);
				}
				etimer_reset(&timer_MultiHop_Message);
				
			}else if(data==&timer_Reinicio_Enlace){
				
				id_enlace=-1;
				salto_enlace=-1;
				rssi_enlace=-88;
				espera=1;
				printf("Reiniciando enlace\n");
				etimer_set(&timer_Espera_Reinicio_Enlace, (CLOCK_SECOND)*30);

				
			}else if (data == &timer_Espera_Reinicio_Enlace){
				
				espera=0;
				etimer_reset(&timer_Reinicio_Enlace);
				printf("Ya he esperado con sink =-1\n");
				
			}

		}
		
	  }
	PROCESS_END();
}

