////////////////////////////////////////////////////
//                                                //
// SERGIO AGUILERA RAMÍREZ                        //
//                                                //
// Simulacion con incremento de tiempo fijo       //
//                                                //
////////////////////////////////////////////////////


#include <chrono>
#include <iostream>
#include <cmath>

using namespace std;


/****** INICIALIZACIÓN DE LOS DATOS ******/

float infinito = 10e30;
int atendidos = 0;
int reloj = 0;
int encola = 0;
int tultsuc;
int total_a_atender;
int medida;

double acum_cola = 0.0;
double inicio_ocio = 0.0;
double ocio;
double tlleg, tserv;

bool libre = true;
bool ocupado = false;
bool servidor = libre;



/* Genera el tiempo de llegada de un cliente */
float generallegada(float tlleg){
  	float u = (float) random(); // o también rand() en lugar de random()
  	u = (float) (u/(RAND_MAX+1.0)); //RAND_MAX es una constante del sistema
  	float val = ceil((-tlleg*log(1-u)));
  	float result = 1.0;
  	if(val != 0){
  		result = val;
  	}
  	return result;
}

/* Genera tiempo de servicio */
float generaservicio(float tserv){
  	float u = (float) random(); // o también rand() en lugar de random()
  	u = (float) (u/(RAND_MAX+1.0)); //RAND_MAX es una constante del sistema
  	float val = ceil((-tserv*log(1-u)));
  	float result = 1.0;
  	if(val != 0){
  		result = val;
  	}
  	return result;
}



int main(int argc, char *argv[]){
	// Argumentos
  if (argc == 1){
		tlleg = 0.15;
  		tserv = 0.1;
  		total_a_atender = 100;
    	medida = 0;
	}
	else if (argc == 5){
		sscanf(argv[1], "%lf", &tlleg);
		sscanf(argv[2], "%lf", &tserv);
		sscanf(argv[3], "%d", &total_a_atender);
    	sscanf(argv[4], "%d", &medida);
	}
	else{
		cout << "Error Formato: < tlleg  tserv  total_a_atender  medida(0-> horas, 1-> minutos, 2->segundos, 3-> medias horas) >" << endl;
    	exit(-1);
	}
	
	// Modificación de la unidad de medida de tiempo
	if(medida == 1){ // minutos
    	tlleg = tlleg * 60;
	    tserv = tserv * 60; 
	}
	else if(medida == 2){ // segundos
	    tlleg = tlleg * 3600;
	    tserv = tserv * 3600; 
	}
	else if(medida == 3){ // medias horas
	    tlleg = tlleg / 0.5;
	    tserv = tserv / 0.5; 
	}

  	// Inicializamos el generador de datos
  	srand(time(NULL));

  	// Generamos el tiempo de llegada y establecemos el tiempo de salida a infinito
  	int tiempo_llegada = reloj + generallegada(tlleg);
  	int tiempo_salida = infinito;

  	/****** SIMULACIÓN ******/
	unsigned tinicio, tfin;
  	tinicio = clock();

  	while (atendidos < total_a_atender) //simularemos hasta que hayamos atendido a cierto
   	{                                  //número de clientes, total_a_atender

	    if (reloj == tiempo_llegada) //si estamos en el instante en que llega alguien
	    {
	      	tiempo_llegada = reloj + generallegada(tlleg); //determinamos cuando llegará
	                                                           //el siguiente cliente
	      	if (servidor == libre) //si el servidor está ocioso
	      	{ 
	        	servidor = ocupado; //deja de estarlo
	        	tiempo_salida = reloj + generaservicio(tserv); //determinamos cuando saldrá
	                                                           //ese cliente que acaba de llegar
	        	ocio += reloj - inicio_ocio; //acumulamos el ocio hasta este momento
	      	}
	      	else //el servidor está ocupado
	      	{  
	        	acum_cola += (reloj - tultsuc) * encola; //acumulamos el número de clientes en
	                                               //cola por el tiempo en que han estado en cola
	        	tultsuc = reloj; //para saber en qué momento cambió el tamaño de la cola, en
	                                   //este caso aumentó en uno más
	        	encola ++;  //hay un cliente más en cola
	      	}
	    }
	    if (reloj == tiempo_salida) //si estamos en el instante en que se va alguien
	    {
	      	atendidos ++; //se ha atendido a un cliente más
	      	if (encola > 0) //si quedan clientes en cola
	      	{
	        	acum_cola += (reloj - tultsuc) * encola; // acumulamos el número de clientes en
	                                               //cola por el tiempo en que han estado en cola
	        	tultsuc = reloj; //para saber en qué momento cambió el tamaño de la cola, en
	                                   //este caso disminuyó en uno
	        	encola  --; //hay un cliente menos en cola
	        	tiempo_salida = reloj + generaservicio(tserv); //determinamos cuando saldrá
	                                                         //ese cliente que acaba de entrar
	      	}
	      	else //no quedan clientes en cola
	      	{  
	        	servidor = libre; //el servidor se queda ocioso por falta de clientes
	        	inicio_ocio = reloj; //marcamos cuando empieza a estar ocioso
	        	tiempo_salida = infinito; //nadie puede salir puesto que nadie hay
	      	}
	    }
	    reloj ++;   //el tiempo se incrementa en una unidad
	}

	tfin = clock();
    
	float porcent_ocio = ocio*100/reloj; //calculamos el porcentaje de tiempo de ocio del servidor
	cout << endl << "Porcentaje de tiempo de ocio: " << porcent_ocio << endl;
	float media_encola = acum_cola/reloj; //calculamos el número medio de clientes en cola
	cout << "Num medio de clientes en cola: " << media_encola << endl;


	if(tserv<tlleg){
		float p = tserv/tlleg;
		float Q = pow(p,2)/(1.0-p);
		float PTO = 100.0 * (1.0-p);
		cout << endl << "P = " << p; 
		cout << endl << "Q = " << Q;	
		cout << endl << "PTO = " << PTO;
	}

	cout << endl << endl << "Tiempo de ejecucion: " << (double(tfin-tinicio)/CLOCKS_PER_SEC) << " seg." << endl;
}