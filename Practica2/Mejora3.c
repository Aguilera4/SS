//////////////////////////////////////////////////
//
//    SERGIO AGUILERA RAMÍREZ ; PRACTICA 2
//
//////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

///// variables globales

int demanda, iteraciones, distribucion;

////////////// CÓDIGO GENERADOR //////////////
double uniforme(){ //Genera un numero uniformemente distribuido en el
//intervalo [0,1) a partir de uno de los generadores
//disponibles en C. Lo utiliza el generador de demanda
  int t = random();
  double f = ((double)RAND_MAX+1.0);

  return (double)t/f;
}


float* construye_prop_a(int n){ //Construye la tabla de busqueda de
//tamaño n para la distribucion de
//la demanda del apartado (a).
  int i;
  float* temp;
  if ((temp = (float*) malloc(n*sizeof(float))) == NULL){
    fputs("Error reservando memoria para generador uniforme\n",stderr);
    exit(1);
  }
  temp[0] = 1.0/n;
  for (i=1;i<n;i++){
    temp[i] = temp[i-1]+1.0/n;
  }

  return temp;
}


float* construye_prop_b(int n){ //Construye la tabla de busqueda de
//tamaño n para la distribucion de
//la demanda del apartado (b).
  int i, max;
  float* temp;
  if ((temp = (float*) malloc(n*sizeof(float))) == NULL){
    fputs("Error reservando memoria para generador proporcional\n",stderr);
    exit(1);
  }
  max = (n/2)*(n+1);
  temp[0] = n*1.0/max;
  for (i=1;i<n;i++){
    temp[i] = temp[i-1]+(float)(n-i)/max;
  }
  return temp;
}


float* construye_prop_c(int n){ //Construye la tabla de busqueda de
//tamaño n para la distribucion de
//la demanda del apartado (c).
  int i, max;
  float* temp;
  if ((temp = (float*) malloc(n*sizeof(float))) == NULL){
    fputs("Error reservando memoria para generador triangular\n",stderr);
    exit(1);
  }
  max = n*n/4;
  temp[0] = 0.0;
  for (i=1;i<(n/2);i++){
    temp[i] = temp[i-1]+(float)i/max;
  }
  for (i=(n/2);i<n;i++){
    temp[i] = temp[i-1]+(float)(n-i)/max;
  }
  return temp;  
}

// Mejora demanda para el caso A
int genera_demanda_mejora(float* tabla,int tama){ 
  double u = uniforme();
  int i = ceil((double)u*tama);
  return i;
}

int genera_demanda(float* tabla,int tama){ // Genera un valor de la
// distribucion de la demanda codificada en tabla, por el
// metodo de tablas de busqueda.
// tama es el tamaño de la tabla, 100 en nuestro caso particular
  int i;
  double u = uniforme();
  i = 0;
  while((i<tama) && (u>=tabla[i])){
    i++;
  }
  return i;
}

////////////// CÓDIGO PRACTICA //////////////


/* Programa principal */
int main(int argc, char *argv[]){
    if(argc != 2){
        printf("\nFormato Argumentos -> <iteraciones>\n");
      exit(1);
    }
    else{
      sscanf(argv[1],"%d",&iteraciones);
    }


  srand(time(NULL)); //Inicializa el generador de numeros pseudoaleatorios
  float* tablabdemanda;

  clock_t t_ini, t_fin;
  double secs;
  tablabdemanda = construye_prop_a(iteraciones);   
  t_ini = clock();
  int j;
  for(j=0; j<iteraciones; j++){
      demanda = genera_demanda_mejora(tablabdemanda,iteraciones);
  }
  t_fin = clock();
  secs = (float) (t_fin - t_ini) / CLOCKS_PER_SEC;
  printf("Dist. A): %.2lf milisegundos\n", secs * 1000.0 );

}


