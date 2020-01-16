////////////////////////////////////////////////////
//                                                //
// SERGIO AGUILERA RAMÍREZ                        //
//                                                //
// Simulacion con incremento de tiempo variable   //
//                                                //
////////////////////////////////////////////////////


#include <chrono>
#include <iostream>
#include <cmath>

using namespace std;
using namespace std::chrono;

/****** INICIALIZACIÓN DE LOS DATOS ******/

float infinito = 10e30;
int atendidos = 0;
int encola = 0;
int tultsuc;
int medida;
int total_a_atender;

double acum_cola = 0.0;
double inicio_ocio = 0.0;
double ocio;
double tlleg, tserv;

bool libre = true;
bool ocupado = false;
bool servidor = libre;

float reloj = 0;


float generallegada(float tlleg){
  float u = (float) random(); // o también rand() en lugar de random()
  u = (float) (u/(RAND_MAX+1.0)); //RAND_MAX es una constante del sistema
  return (-tlleg*log(1-u));
}

float generaservicio(float tserv){
  float u = (float) random(); // o también rand() en lugar de random()
  u = (float) (u/(RAND_MAX+1.0)); //RAND_MAX es una constante del sistema
  return (-tserv*log(1-u));
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
  float tiempo_llegada = reloj + generallegada(tlleg);
  float tiempo_salida = infinito;

	/****** SIMULACIÓN ******/
	unsigned tinicio, tfin;
  tinicio = clock();

  while (atendidos < total_a_atender)
  {
    reloj = min(tiempo_llegada,tiempo_salida); //una función que calcula el mínimo
    if (reloj == tiempo_llegada)
    {
      tiempo_llegada = reloj + generallegada(tlleg);
      if (servidor == libre)
      {
        servidor = ocupado;
        tiempo_salida = reloj + generaservicio(tserv);
        ocio += reloj - inicio_ocio;
      }
      else {
        acum_cola += (reloj - tultsuc) * encola;
        tultsuc = reloj;
        encola ++;
      }
    }
    if (reloj == tiempo_salida)
    {
      atendidos ++;
      if (encola > 0)
      {
        acum_cola += (reloj - tultsuc) * encola;
        tultsuc = reloj;
        encola  --;
        tiempo_salida = reloj + generaservicio(tserv);
      }
      else {
        servidor = libre;
        inicio_ocio = reloj;
        tiempo_salida = infinito;
      }
    }
  }

  tfin = clock();
  cout << "Tiempo de ejecucion: " << (double(tfin-tinicio)/CLOCKS_PER_SEC) << endl;
	
	float porcent_ocio = ocio*100/reloj; //calculamos el porcentaje de tiempo de ocio del servidor
	cout << "Porcentaje de tiempo de ocio: " << porcent_ocio << endl;
	float media_encola = acum_cola/reloj; //calculamos el número medio de clientes en cola
	cout << "Num medio de clientes en cola: " << media_encola << endl;

  if(tserv<tlleg){
    float p = tserv/tlleg;
    float Q = pow(p,2)/(1.0-p);
    float PTO = 100.0 * (1.0-p);
    printf("\np = %f", p);
    printf("\nQ = %f", Q);
    printf("\nPTO = %f\n\n",PTO);
  }
}