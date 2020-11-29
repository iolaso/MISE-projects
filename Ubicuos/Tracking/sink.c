#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/leds.h"
#include "random.h"
#include "node-id.h"
#include <stdio.h>
#include "powertrace.h"

/*-------------------------------------------------------*/
PROCESS(sink, "Sink");
AUTOSTART_PROCESSES(&sink);
/*-------------------------------------------------------*/

struct multihop_Message {
	int beaconId;
	int nodeType;
	int salto
};

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
/*---------------------------------------*/

int id_sink = -1;             // To store the Node Id
int n_salto=1;

static void msg_Sink_Recv(struct abc_conn *c) {
	
	struct beacon_3 *msg_content = packetbuf_dataptr();
	
	int i =0;
	char str[80];
	
	if (msg_content->destino_id == id_sink){

		printf("Nodo %d: tiempo %d: ",msg_content->my_id,msg_content->tiempo);
		printf("balizas ");
		printf((msg_content->beacons[0].beaconId!=-1)?"%d (%d): ":" - :",msg_content->beacons[0].beaconId,msg_content->beacons[0].rssi);
		printf((msg_content->beacons[1].beaconId!=-1)?"%d (%d): ":" - :",msg_content->beacons[1].beaconId,msg_content->beacons[1].rssi);
		printf((msg_content->beacons[2].beaconId!=-1)?"%d (%d)\n":" -\n",msg_content->beacons[2].beaconId,msg_content->beacons[2].rssi);

	}
}

/*------------------------------------------------------*/

	 




static struct abc_conn abc_Historico;
static const struct abc_callbacks abc_call_historico = {msg_Sink_Recv};

static struct abc_conn abc_Multihop;
static const struct abc_callbacks abc_call_multihop;


/*------------------------------------------------------*/



PROCESS_THREAD(sink, ev, data)
{

	static struct etimer timer_MultiHop_Message;
   
    static struct multihop_Message multihop_content;

	//powertrace_start(CLOCK_SECOND * 1);
	
	id_sink = (int) node_id;    

	PROCESS_EXITHANDLER(abc_close(&abc_Historico));
	PROCESS_EXITHANDLER(abc_close(&abc_Multihop));
	PROCESS_BEGIN();
	
	
	
	//Recepción de datos del nodo
	abc_open(&abc_Historico, 127, &abc_call_historico);
	
	//Envio de multihop
	abc_open(&abc_Multihop, 126, &abc_call_multihop);
  	etimer_set(&timer_MultiHop_Message, (CLOCK_SECOND*20));

	
	while(1) {
		PROCESS_WAIT_EVENT();

		if(ev == PROCESS_EVENT_TIMER) {

			if ( data == &timer_MultiHop_Message){
				
				multihop_content.beaconId=id_sink;
				multihop_content.salto=n_salto;
				multihop_content.nodeType=0;
				packetbuf_copyfrom(&multihop_content, sizeof(struct multihop_Message));
				abc_send(&abc_Multihop);
				etimer_reset(&timer_MultiHop_Message);
				}
		}
	}

  PROCESS_END();
}
