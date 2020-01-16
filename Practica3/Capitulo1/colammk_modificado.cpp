////////////////////////////////////////////////////
//                                                //
// SERGIO AGUILERA RAMÍREZ                        //
//                                                //
// Simulacion colammk modificado                  //
//                                                //
////////////////////////////////////////////////////


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iostream>
#include <vector>

using namespace std;

#define suceso_llegada 0
#define suceso_salida 1
#define suceso_finsimulacion 3
#define suceso_monitor 4

typedef struct {
	int suceso;
	float tiempo;
	float retraso;
	} suc;

list<suc> lsuc;
suc nodo;
int nada = 0;
list<float> cola;
int m; //numero de servidores
float tparada;
float tlleg;
float tserv;
float valor;

float reloj;
int libres;
int encola;
int ensistema;
int atendidos;
float acum_cola;
float acum_sistema;
float acum_ocio;
float acum_retraso;
float tultsuc_cola;
float tultsuc_ocio;
float tultsuc_sistema;
float acum_sincola;
float init_sincola;
int maximacola;
bool parar;

// Variables añadidas
int num_simulaciones;

float t_medio_espera_enCola_global;
float t_medio_estancia_enSistema_global;
float n_medio_personas_enCola_global;
float n_medio_personas_enSistema_global;
float longitud_media_colas_noVacias_global;
float porcent_medio_t_ocioServidor_global;
float longitud_media_max_cola_global;

vector<float> desviacion_t_espera_enCola_global;
vector<float> desviacion_t_estancia_enSistema_global;
vector<float> desviacion_n_personas_enCola_global;
vector<float> desviacion_n_personas_enSistema_global;
vector<float> desviacion_longitud_colas_noVacias_global;
vector<float> desviacion_porcent_t_ocioServidor_global;
vector<float> desviacion_longitud_max_cola_global;



bool compare(const suc &s1, const suc &s2)
{ return s1.tiempo < s2.tiempo; }


/* Inserta de forma ordenada un elemento en la lista de sucesos */
void insertar_lsuc(suc n)
{
  lsuc.push_back(n);
  // Mantener ordenada la lista por el tiempo de los sucesos
  lsuc.sort(compare);
}

float generador_exponencial(float media)
{
  float u;
  u = (float) random();
  u = u/(float)(RAND_MAX+1.0);
  return(-media*log(1-u));
}

float generador_uniforme(float media)
{
  return (float) (random()/((float) RAND_MAX+media));
}

// Generador de tiempos entre llegadas (exponencial)
float generallegada(float media)
{ return generador_uniforme(media); }

// Generador de tiempos de servicio (exponencial)
float generaservicio(float media)
{ return generador_uniforme(media); }


/* Procedimiento inicializacion */
void inicializacion()
{
  reloj = 0.0;
  libres = m;
  encola = 0;
  ensistema = 0;
  atendidos = 0;
  acum_cola = 0.0;
  acum_sistema = 0.0;
  acum_ocio = 0.0;
  acum_retraso = 0.0;
  tultsuc_cola = reloj;
  tultsuc_ocio = reloj;
  tultsuc_sistema = reloj;
  acum_sincola = 0.0;
  init_sincola = reloj;
  maximacola = 0;
  nodo.suceso = suceso_llegada;
  nodo.tiempo = reloj+generallegada(tlleg);
  nodo.retraso = nada;
  insertar_lsuc(nodo);
  nodo.suceso = suceso_finsimulacion;
  nodo.tiempo = reloj+tparada;
  nodo.retraso = nada;
  insertar_lsuc(nodo); //tparada es un parámetro de entrada al programa
  //Inicializar el suceso monitor si que quiere trazar alguna medida del rendimiento a lo largo del tiempo, no solo al final
  /* 
  nodo.suceso = suceso_monitor;
  nodo.tiempo = reloj+1.0;
  nodo.retraso = nada;
  insertar_lsuc(nodo);
  */
  parar = false;
}

/* Procedimiento temporizacion */
void temporizacion()
{
  nodo = lsuc.front();
  lsuc.pop_front();
  reloj = nodo.tiempo;
//  printf("\n%.3f",reloj);
}

void llegada()
{
  acum_sistema += (reloj - tultsuc_sistema) * ensistema;
  tultsuc_sistema = reloj;
  ensistema ++;
  nodo.suceso = suceso_llegada;
  nodo.tiempo = reloj+generallegada(tlleg);
  nodo.retraso = nada;
  insertar_lsuc(nodo);
  if (libres > 0)
       {
        acum_ocio += (reloj - tultsuc_ocio) * libres;
        tultsuc_ocio = reloj;
        libres --;
        nodo.suceso = suceso_salida;
        nodo.tiempo = reloj+generaservicio(tserv);
        nodo.retraso = nada;
        insertar_lsuc(nodo);
       }
  else {
        if (encola == 0) acum_sincola += reloj - init_sincola;
        acum_cola += (reloj - tultsuc_cola) * encola;
        tultsuc_cola = reloj;
        encola ++;
        if (encola > maximacola) maximacola = encola;
        cola.push_back(reloj);
        }
}

void salida()
{
  acum_sistema += (reloj - tultsuc_sistema) * ensistema;
  tultsuc_sistema = reloj;
  ensistema --;
  atendidos ++;
  acum_retraso += nodo.retraso;
  if (encola > 0)
       {
        acum_cola += (reloj - tultsuc_cola) * encola;
        tultsuc_cola = reloj;
        encola --;
        if (encola == 0) init_sincola = reloj;
        valor = cola.front();
        cola.pop_front();
        nodo.suceso = suceso_salida;
        nodo.tiempo = reloj+generaservicio(tserv);
        nodo.retraso = reloj-valor;
        insertar_lsuc(nodo);
       }
  else {
        acum_ocio += (reloj - tultsuc_ocio) * libres;
        tultsuc_ocio = reloj;
        libres ++;
       }
}

void fin()
{
  parar = true; //para detener la simulación
                //habrá que hacer las últimas actualizaciones de algunas variables
  float retrasomedio = acum_retraso/atendidos;
  //printf("\nTiempo medio de espera en cola = %.3f",retrasomedio);
  float estanciamedia = retrasomedio + tserv;
  //printf("\nTiempo medio de estancia en el sistema = %.3f",estanciamedia);
  acum_cola += (reloj - tultsuc_cola) * encola;

  //printf("\nTiempo medio de espera en cola bis = %.3f",acum_cola/atendidos);
  //printf("\nTiempo medio de estancia en el sistema bis= %.3f",acum_cola/atendidos+tserv);


  float encolamedio = acum_cola/reloj;
  //printf("\nNumero medio de personas en cola = %.3f",encolamedio);
  acum_sistema += (reloj - tultsuc_sistema) * ensistema;
  float ensistemamedio = acum_sistema/reloj;
  //printf("\nNumero medio de personas en el sistema = %.3f",ensistemamedio);
  if (encola == 0) acum_sincola += reloj - init_sincola;
  float colasnovaciasmedio = acum_cola/(reloj - acum_sincola);
  //printf("\nLongitud media de colas no vacias = %.3f",colasnovaciasmedio);
  acum_ocio += (reloj - tultsuc_ocio) * libres;
  float porcentajemedioocio = 100*acum_ocio/(m*reloj);
  //printf("\nporcentaje medio de tiempo de ocio por servidor = %.3f",porcentajemedioocio);
  //printf("\nLongitud máxima de la cola = %d",maximacola);
  //printf("\n");

  // Sumatoria de los valores medios
  t_medio_espera_enCola_global += retrasomedio;
  t_medio_estancia_enSistema_global += estanciamedia;
  n_medio_personas_enCola_global += encolamedio;
  n_medio_personas_enSistema_global += ensistemamedio;
  longitud_media_colas_noVacias_global += colasnovaciasmedio;
  porcent_medio_t_ocioServidor_global += porcentajemedioocio;
  longitud_media_max_cola_global += maximacola;

  // Guardamos los valores medios
  desviacion_t_espera_enCola_global.push_back(retrasomedio);
  desviacion_t_estancia_enSistema_global.push_back(estanciamedia);
  desviacion_n_personas_enCola_global.push_back(encolamedio);
  desviacion_n_personas_enSistema_global.push_back(ensistemamedio);
  desviacion_longitud_colas_noVacias_global.push_back(colasnovaciasmedio);
  desviacion_porcent_t_ocioServidor_global.push_back(porcentajemedioocio);
  desviacion_longitud_max_cola_global.push_back(maximacola);
}

//Si se desea monitorizar, por ejemplo el número medio de clientes en sistema a lo largo del tiempo se puede usar el suceso monitor siguiente, que habrá que inicializar
void monitor()
{
  nodo.suceso = suceso_monitor;
  nodo.tiempo = reloj+100.0;
  nodo.retraso = nada;
  insertar_lsuc(nodo);
  cout<<"tiempo y media en sistema "<<reloj<<" "<<(acum_sistema+(reloj - tultsuc_sistema) * ensistema)/reloj<<endl;
  //printf("\n %.3f, %3f","tiempo y media en sistema",reloj,(acum_sistema+(reloj - tultsuc_sistema) * ensistema)/reloj);
}

/* Procedimiento suceso */
void suceso()
{
  switch(nodo.suceso)
  {
      case suceso_llegada: llegada(); break;
      case suceso_salida: salida(); break;
      case suceso_finsimulacion: fin(); break;
      case suceso_monitor: monitor(); break;
  }
}

float elevar(float num){
  float result = pow(num,2);
  return result;
}

int main(int argc, char *argv[])
{
  // Argumentos
  if (argc != 6)
    {
     printf("\n\nFormato Argumentos -> < numero_servidores  tiempo de parada  tlleg  tserv  num_simulaciones >\n\n");
     exit(1);
    }
  sscanf(argv[1],"%d",&m);
  sscanf(argv[2],"%f",&tparada);
  sscanf(argv[3],"%f",&tlleg);
  sscanf(argv[4],"%f",&tserv);
  sscanf(argv[5],"%d",&num_simulaciones);

  // Inicializamos el generador de datos
  srandom(time(NULL));

  int i;
  /****** SIMULACIÓN ******/
  for(i=0; i<num_simulaciones; i++){
    inicializacion();
    while (!parar)
	  {
	    temporizacion();
	    suceso();
	  }
  }

  // Calculo de los valores medios de los datos de rendimiento
  t_medio_espera_enCola_global = t_medio_espera_enCola_global/num_simulaciones ;
  t_medio_estancia_enSistema_global = t_medio_estancia_enSistema_global/num_simulaciones;
  n_medio_personas_enCola_global = n_medio_personas_enCola_global/num_simulaciones;
  n_medio_personas_enSistema_global = n_medio_personas_enSistema_global/num_simulaciones;
  longitud_media_colas_noVacias_global = longitud_media_colas_noVacias_global/num_simulaciones;
  porcent_medio_t_ocioServidor_global = porcent_medio_t_ocioServidor_global/num_simulaciones;
  longitud_media_max_cola_global = longitud_media_max_cola_global/num_simulaciones;


  // Calculo de las desviaciones típicas de los datos de rendimiento
  float val1, val2, val3, val4, val5, val6, val7;
  for(i=0; i<num_simulaciones; i++){
    val1 += elevar(desviacion_t_espera_enCola_global[i] - t_medio_espera_enCola_global);
    val2 += elevar(desviacion_t_estancia_enSistema_global[i] - t_medio_estancia_enSistema_global);
    val3 += elevar(desviacion_n_personas_enCola_global[i] - n_medio_personas_enCola_global);
    val4 += elevar(desviacion_n_personas_enSistema_global[i] - n_medio_personas_enSistema_global);
    val5 += elevar(desviacion_longitud_colas_noVacias_global[i] - longitud_media_colas_noVacias_global);
    val6 += elevar(desviacion_porcent_t_ocioServidor_global[i] - porcent_medio_t_ocioServidor_global);
    val7 += elevar(desviacion_longitud_max_cola_global[i] - longitud_media_max_cola_global);
  }

    float constante = 1.0/(num_simulaciones-1.0);

    float result1 = sqrt(constante*val1);
    float result2 = sqrt(constante*val2);
    float result3 = sqrt(constante*val3);
    float result4 = sqrt(constante*val4);
    float result5 = sqrt(constante*val5);
    float result6 = sqrt(constante*val6);
    float result7 = sqrt(constante*val7);

    /*
    printf("\n\n-- MEDIAS GLOBALES TEORICOS -- \n");

    printf("Tiempo medio de espera en cola: %.4f\n", (elevar(tserv)/(tlleg-tserv)));
    printf("Tiempo medio de estancia en sistema: %.4f\n", ((tserv*tlleg)/(tlleg-tserv)));
    printf("Número medio de personas en cola: %.4f\n", (elevar(tserv)/(tlleg*(tlleg-tserv))));
    printf("Número medio de personas en sistema: %.4f\n", (tserv/(tlleg-tserv)));
    printf("Longitud media de colas no vacías: %.4f\n", (tlleg/(tlleg-tserv)));
    printf("Porcentaje medio de tiempo de ocio por servidor: %.4f\n", ((1-(tserv/tlleg))*100));
    printf("Longitud media máxima en cola: %.4f\n", longitud_media_max_cola_global);*/

    printf("\n\n-- MEDIAS GLOBALES -- \n");

    printf("Tiempo medio de espera en cola: %.4f\n", t_medio_espera_enCola_global);
    printf("Tiempo medio de estancia en sistema: %.4f\n", t_medio_estancia_enSistema_global);
    printf("Número medio de personas en cola: %.4f\n", n_medio_personas_enCola_global);
    printf("Número medio de personas en sistema: %.4f\n", n_medio_personas_enSistema_global);
    printf("Longitud media de colas no vacías: %.4f\n", longitud_media_colas_noVacias_global);
    printf("Porcentaje medio de tiempo de ocio por servidor: %.4f\n", porcent_medio_t_ocioServidor_global);
    printf("Longitud media máxima en cola: %.4f\n", longitud_media_max_cola_global);

    printf("\n\n-- DESVIACIONES TÍPICAS -- \n");

    printf("Desviación típica del tiempo de espera en cola: %.4f\n", result1);
    printf("Desviación típica del tiempo de estancia en sistema: %.4f\n", result2);
    printf("Desviación típica del número de personas en cola: %.4f\n", result3);
    printf("Desviación típica del número de personas en sistema: %.4f\n", result4);
    printf("Desviación típica de la longitud de colas no vacías: %.4f\n", result5);
    printf("Desviación típica del porcentaje de tiempo de ocio por servidor: %.4f\n", result6);
    printf("Desviación típica de la longitud máxima en cola: %.4f\n", result7);
    
}