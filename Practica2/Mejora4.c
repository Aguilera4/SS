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

int demanda;

///// variables globales

// Búsqueda del valor i en el array, si esta contenido devuelve true, en caso
// contrario false
bool buscarEnArray(long int* array, long int i, int m){
  bool repetido = false;
  int j;
  for(j=0; j<m; j++){
      if(array[j] == i){
        repetido = true; 
      }
    }
    return repetido;
}


// Genera la demanda mediante aritmetia entera
int genera_demanda_arit_entera(int m,int a,int c){ // 
  long int x;
  x = 10  ;
  long int* array;
  if ((array = (long int*) malloc(2*m*sizeof(long int))) == NULL){
    fputs("Error reservando memoria para generador proporcional\n",stderr);
    exit(1);
  }
  bool repetido = false;
  int contador = 0;
  while(repetido == false){
    array[contador] = x;
    x = (a*array[contador]+c)%m;
    repetido = buscarEnArray(array,x,contador);
    contador++;
  }
  return contador;
}

// genera la demanda mediante aritmetica real artesanal
int genera_demanda_arit_real_artesanal(int m,int a,int c){ // 
  float x;
  x = 10;
  long int* array;
  if ((array = (long int*) malloc(2*m*sizeof(long int))) == NULL){
    fputs("Error reservando memoria para generador proporcional\n",stderr);
    exit(1);
  }
  bool repetido = false;
  int contador = 0;
  while(repetido == false){
    array[contador] = x;
    x = (a*(float)array[contador]+c)/m;
    x = (x-(long int)x)*m;
    repetido = buscarEnArray(array,x,contador);
    contador++;
  }
  return contador;    
}



// genera la demanda mediante aritmetica real artesanal corregida
int genera_demanda_arit_real_artesanal_corregida(int m,int a,int c){ // 
  float x;
  x = 10;
  long int* array;
  if ((array = (long int*) malloc(2*m*sizeof(long int))) == NULL){
    fputs("Error reservando memoria para generador proporcional\n",stderr);
    exit(1);
  }
  bool repetido = false;
  int contador = 0;
  while(repetido == false){
    array[contador] = x;
    x = (a*(float)array[contador]+c)/m;
    x = (x-(long int)x)*m;
    x = (long int) (x+0.5);
    repetido = buscarEnArray(array,x,contador);
    contador++;
  }
  return contador;    
}

// genera la demanda mediante aritmetica real con fmod
int genera_demanda_arit_real_fmod(int m,int a,int c){ // 
  double x;
  x = 10;
  long int* array;
  if ((array = (long int*) malloc(2*m*sizeof(long int))) == NULL){
    fputs("Error reservando memoria para generador proporcional\n",stderr);
    exit(1);
  }
  bool repetido = false;
  int contador = 0;
  while(repetido == false){
      array[contador] = x;
      x =(long int) fmod((a*array[contador]+c),m);
      repetido = buscarEnArray(array,x,contador);
      contador++;
    }
  return contador;
}


////////////// CÓDIGO PRACTICA //////////////


/* Programa principal */
int main(int argc, char *argv[]){
    int a1, a2, c, m;
    a1 = 2061;
    a2 = 2060;
    c = 4321;
    m = 10000;

  //******************** xn+1 = (2061xn + 4321) mod m ********************
printf("\n** xn+1 = (2061xn + 4321) mod m **\n");


int periodo = genera_demanda_arit_entera(m,a1,c);
printf("\nPeriodo (Aritmetica entera): %d ",periodo);


int periodo1 = genera_demanda_arit_real_artesanal(m,a1,c);
printf("\nPeriodo (Aritmetica real artesanal): %d ",periodo1);

int periodo2 = genera_demanda_arit_real_artesanal_corregida(m,a1,c);
printf("\nPeriodo (Aritmetica real artesanal corregida): %d ",periodo2);



int periodo3 = genera_demanda_arit_real_fmod(m,a1,c);
printf("\nPeriodo (Aritmetica real fmod): %d \n\n",periodo3);


  //******************** xn+1 = (2060xn + 4321) mod m ********************
printf("\n** xn+1 = (2060xn + 4321) mod m **\n");

int periodo4 = genera_demanda_arit_entera(m,a2,c);
printf("\nPeriodo (Aritmetica entera): %d ",periodo4);

int periodo5 = genera_demanda_arit_real_artesanal(m,a2,c);
printf("\nPeriodo (Aritmetica real artesanal): %d ",periodo5);


int periodo6 = genera_demanda_arit_real_artesanal_corregida(m,a2,c);
printf("\nPeriodo (Aritmetica real artesanal corregida): %d ",periodo6);

int periodo7 = genera_demanda_arit_real_fmod(m,a2,c);
printf("\nPeriodo (Aritmetica real fmod): %d \n\n",periodo7);    

}


