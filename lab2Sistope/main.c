#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include "funciones.h"
#include "funciones.c"

//pthread_mutex_t mutex;	//mutex global, solo para testear.


int main(int argc, char **argv){
	
	char *nameArchivo = NULL;
	int numeroHebras = 0;
	int cantidadPalabras = 0;
	int N = 0;
	int M = 0;
	char *salida=NULL;
	int bandera=0;
	int c;

	char **tablero=NULL;

	opterr = 0;
	//i: archivo, h: numero de hebras, c: cantidad de palabras,
	//n: ancho matriz, m:largo matriz s: nombreArchivoSalida d: bandera
	while ((c = getopt (argc, argv, "i:h:c:n:m:s:d:")) != -1){

		switch (c){
			case 'i':
				nameArchivo = optarg;
				break;
			case 'h':
				numeroHebras = atoi(optarg);
				break;
			case 'c':
				cantidadPalabras = atoi(optarg);
				break;
			case 'n':
				N = atoi(optarg);
				break;
			case 'm':
				M = atoi(optarg);
				break;
			case 's':
				salida = optarg;
				break;
			case 'd':
				bandera = atoi(optarg);
				break;
			case '?':
				if (optopt == 'c')
					printf ( "Opción -%c requiere un argumento.\n", optopt);
				else if (isprint (optopt))
				  	printf ( "Opción desconocida `-%c'.\n", optopt);
				else
				  	printf ("Opción con caracter desconocido `\\x%x'.\n",optopt);
					return 1;
				default:
				abort ();
		}
	}

	//Prueba de que los argumentos fueron guardados correctamente
	if (bandera==1)
	{
		printf("numeroHebras: %d\n",numeroHebras);
		printf("cantidadPalabras: %d\n", cantidadPalabras );
		printf("filas: %d\n",N);
		printf("columnas: %d\n",M);
	}


	srand((unsigned)time(NULL));		//seed para usar rand aleatorio


	//creación del tablero a partir de las dimensiones de entrada
	tablero = crearTableroDinamico(N,M);
	//printTablero(tablero, N, M);




	//Se crean las hebras
	pthread_t threads[numeroHebras];
	crearHebras(threads, numeroHebras, tablero, N, M, nameArchivo, cantidadPalabras, bandera);




	//El hilo main espera que terminen las hebras
	if (numeroHebras<cantidadPalabras){
		waitHebras(threads, numeroHebras);
	}


	printf("\n");
	printf("\n");
	//printTablero(tablero, N, M,bandera);
	printTableroArchivo(tablero,N,M,salida);
	


	return 0;
}














