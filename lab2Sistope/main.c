#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char **crearTableroDinamico(int N, int M);
int validarPosicionInicial(char *palabra, char **tablero, int posicionX, int posicionY, int N, int M);
void insertarAuxiliar(char *palabra, char **tablero, int posX, int posY);
void printTablero(char **tablero, int N, int M);

int main(int argc, char **argv){
	
	char *file = NULL;
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
				file = optarg;
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
					fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
				else if (isprint (optopt))
				  	fprintf (stderr, "Opción desconocida `-%c'.\n", optopt);
				else
				  	fprintf (stderr,"Opción con caracter desconocido `\\x%x'.\n",optopt);
					return 1;
				default:
				abort ();
		}
	}

	//Prueba de que los argumentos fueron guardados correctamente 
	printf("numeroHebras: %d\n",numeroHebras);
	printf("cantidadPalabras: %d\n", cantidadPalabras );
	printf("filas: %d\n",N);
	printf("columnas: %d\n",M);
	//crear hebras a partir de la cantidad de hebras y asignar las palabras del archivo de texto 


	//creación del tablero a partir de las dimensiones de entrada
	tablero = crearTableroDinamico(N,M);
	printTablero(tablero, N, M);
}


//Descripción: Función que crea el tablero base en el cual se irán insertando las palabras por las hebras
//Entradas: la cantidad de filas (N) y la cantidad de columnas (M) que debe poseer el tablero
//Salida: un tablero de dimensiones NXM inicializados por caracteres por defectos  como caso valido, y NULL en caso contrario
char **crearTableroDinamico(int N, int M){
	char **tablero=NULL;
	int i, j;
	if (N>0 && M>0){
		tablero=(char **)malloc(sizeof(char*)*N);
		for (i = 0; i <N ; ++i){
			tablero[i]=(char*)malloc(sizeof(char)*M);
		}

		for (i = 0; i <N; ++i){
			for (j = 0; j < M; ++j){
				tablero[i][j]='0';			
			}
		}

		return tablero;
	}

	else{
		return NULL;
	}
}

//Desricpion: Función que se encarga de validar si la posicion inicial es valida para poder inserar la palabra
//Entrada: La palabra que se desea insertar, el tablero donde se inserta la palabra, la posicion X donde se comienza a insertar la palabra, la posicion Y donde se comienza a insertar la palabra
//la cantidad de filas que posee el tablero, la cantidad de columnas que posee el tablero
int validarPosicionInicial(char *palabra, char **tablero, int posicionX, int posicionY, int N, int M){
	int largoPalabra=strlen(palabra);
	//contador que lleva "conteo" de los espacios validos para insertar la palabra
	int contador=0;
	int validador=0;
	int i;
	if (posicionX<N && posicionY<M){
		for (i = 0; i <largoPalabra; ++i){
			if (tablero[posicionX][posicionY+i]=='0'){
				contador++;
			}
			else{
				break;
			}
		}
	}
	if (contador==largoPalabra){
		validador=1;
	}
	return validador;
}


//FUNCION AUXILIAR PARA TENER LA IDEA DE INSERTAR UNA PALABRA SIN IMPLEMENTACION DE HEBRAS 
void insertarAuxiliar(char *palabra, char **tablero, int posX, int posY){
	int largoPalabra=strlen(palabra);
	int i;
	for (i = 0; i < largoPalabra; ++i){
		tablero[posX][posY+i]=palabra[i];
	}
}

void printTablero(char **tablero, int N, int M)
{
	int i,j;
	for (i = 0; i < N; ++i){
		for (j = 0; j < M; ++j){
			printf("%c", tablero[i][j]);	
		}
		printf("\n");
	}
}