#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include "funciones.h"
#include "funciones.c"
#include <locale.h>


//pthread_mutex_t mutex;	//mutex global, solo para testear.


int main(int argc, char **argv){

	setlocale(LC_ALL, "");
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
	while ((c = getopt (argc, argv, "i:h:c:n:m:s:d")) != -1){

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
				bandera = 1;
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
		printf("-----------------------------------------------------------------------------\n");
		printf("Bienvenid@\n");
		printf("Ha activado la función de impresión de resultados por pantalla\n");
		printf("Observación: Si nota problemas de formato a medida que se modifica la sopa de letras\n");
		printf("esto se debe a que las hebras se encuentran en proceso de inserción de palabras\n");
		printf("mientras otra hebra ha mostrado por pantalla los cambios que ella ha realizado.\n");
		printf("Esto no afectará el resultado final que siempre se imprimirá correctamente\n");
		printf("tanto por consola como en el archivo de salida.\n");
		printf("-----------------------------------------------------------------------------\n");
		printf("Hebras: %d\n",numeroHebras);
		printf("Palabras: %d\n", cantidadPalabras );
		printf("Filas: %d\n",N);
		printf("Columnas: %d\n",M);
		printf("-----------------------------------------------------------------------------\n");
	}	

	//Capturando errores
	int error = 0;
	int palabrasArchivo = countLines(nameArchivo, 4096);
	int maxLen = getMaxLength(nameArchivo);	//

	//Si el numero de hebras es mayor que la cantidad de palabras existentes entonces hay un error
	if (numeroHebras>cantidadPalabras || palabrasArchivo!=cantidadPalabras)
	{
		printf("ERROR: El número de hebras es mayor que el número de palabras.\n");
		error = 1;
	}

	if(error==1) return 1;



	srand((unsigned)time(NULL));		//seed para usar rand aleatorio

	//creación del tablero a partir de las dimensiones de entrada
	tablero = crearTableroDinamico(N,M);




	//Se crean las hebras
	pthread_t threads[numeroHebras];
	crearHebras(threads, numeroHebras, tablero, N, M, nameArchivo, cantidadPalabras, bandera, maxLen);




	//El main espera que terminen las hebras
	waitHebras(threads, numeroHebras);

	//Se rellenan las posiciones nulas restantes del tablero
	tablero = fillTablero(tablero, N, M);


	//Se imprime resultado final por pantalla si se ingresó el flag -d
	if(bandera==1)
	{
		for(int i=0; i<N; i++) printf("-");
		printf("\n");
		printf("Proceso finalizado, a continuación se presenta la sopa de letras final:\n");
		printTablero(tablero, N, M,bandera);
	}

	//Se imprime el tablero en el archivo de salida
	printTableroArchivo(tablero,N,M,salida);
	


	return 0;
}














