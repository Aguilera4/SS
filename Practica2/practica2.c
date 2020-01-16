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

int x, y, veces;
int mejor_s = -999;
int demanda;
int s, i , num_s, distribucion;

double mejor_ganancia = 0.0;
double mejor_desviacion = 0.0;
int ganancia = 0;
double gananciaesperada = 0.0;
double desviaciont = 0.0;
double sum = 0.0;
double sum2 = 0.0;

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
  	if(argc == 1){
    	veces=100;
    	x = 10;
    	y = 1;
   	}
  	else if(argc != 6){
        printf("\nFormato Argumentos -> <x> <y> <num_s> <veces> <distribucion>\n");
   		exit(1);
  	}
  	else{
    	sscanf(argv[1],"%d",&x);
    	sscanf(argv[2],"%d",&y);
    	sscanf(argv[3],"%d",&num_s);
    	sscanf(argv[4],"%d",&veces);
    	sscanf(argv[5],"%d",&distribucion);
  	}

	srand(time(NULL)); //Inicializa el generador de numeros pseudoaleatorios

  // Seleccionamos distribución
	float* tablabdemanda;
  	if(distribucion == 0){
  		tablabdemanda = construye_prop_a(num_s); 
  	}else if(distribucion == 1){
  		tablabdemanda = construye_prop_b(num_s); 
  	}else if(distribucion == 2){
  		tablabdemanda = construye_prop_c(num_s); 
  	}else{
  		printf("\nFalta distribucion\n");
   		exit(1);
  	}

  // blucle principal
	for (s = 0; s <= num_s; s++){
		sum=0.0; 
		sum2=0.0;
    // Periodo de 'veces' como dias simulados
		for (i=0; i<veces; i++){
		  demanda=genera_demanda(tablabdemanda,num_s);
		  if (s>demanda){
		  	ganancia=demanda*x-(s-demanda)*y;
		  }else{
		  	ganancia=s*x;
		  } 
		  sum+=ganancia;
		  sum2+=ganancia*ganancia;
		}

		gananciaesperada=sum/veces;
		desviaciont=sqrt((sum2-veces*gananciaesperada*gananciaesperada)/(veces-1));

		printf("%d	",s);
		printf("%d	",x);
		printf("%d	",y);
		printf("%d	",veces);
		printf("%.4f	",gananciaesperada);
		printf("%.4f\n",desviaciont);
    
		if (mejor_ganancia < gananciaesperada){
			mejor_ganancia = gananciaesperada;
			mejor_s = s;
			mejor_desviacion = desviaciont;
		}
	}

  // Mostramos los mejores resultados obtenidos
	printf("\nMejor s: %d", mejor_s );
	printf("\nMejor Ganancia esperada: %.4f",mejor_ganancia);
	printf("\nnMejor Desviacion: %.4f", mejor_desviacion);
	printf("\n\n");

}

