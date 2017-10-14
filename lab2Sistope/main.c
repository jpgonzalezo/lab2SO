#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


struct Thread {
     pthread_t 			tid;
     char 				*palabra;
     char 				**tablero;
     int 				posX;
     int 				posY;
     int 				N;
     int 				M;
     pthread_mutex_t	mutexHilo;
};

pthread_mutex_t mutex;	//mutex global, solo para testear.




char **crearTableroDinamico(int N, int M);
int validarPosicionInicial(char *palabra, char **tablero, int posicionX, int posicionY, int N, int M);
void insertarAuxiliar(char *palabra, char **tablero, int posX, int posY);
void printTablero(char **tablero, int N, int M);
void crearHebras(pthread_t threads[], int numeroHebras, char **tablero, int N, int M);
void *threadTest(void *arg);
void waitHebras(pthread_t threads[], int numeroHebras);
int insertarPalabra(char *palabra, char** tablero, int posX, int posY, int N, int M);
void *ubicar(void *arg);

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
	srand(time(NULL));		//seed para usar rand aleatorio


	//creación del tablero a partir de las dimensiones de entrada
	tablero = crearTableroDinamico(N,M);
	//printTablero(tablero, N, M);

	//Se crean las hebras
	pthread_t threads[numeroHebras];
	crearHebras(threads, numeroHebras, tablero, N, M);






	//El hilo main espera que terminen las hebras
	waitHebras(threads, numeroHebras);
	printf("Hebras terminaron\n");

	//insertarPalabra("MANZANA", tablero, 2, 3, N, M);
	//insertarPalabra("HOLA", tablero, 2, 1, N, M);
	//printf("--------------------\n");
	printTablero(tablero, N, M);
	


	return 0;
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

//Función que se encarga de validar si la posicion inicial es valida para poder inserar la palabra
//Entrada: La palabra que se desea insertar, el tablero donde se inserta la palabra, la posicion X donde se comienza a insertar la palabra,
//la posicion Y donde se comienza a insertar la palabra
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

//Función que imprime la sopa de letras
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

//Función que crea las hebras
void crearHebras(pthread_t threads[], int numeroHebras, char **tablero, int N, int M)
{
	char palabra[4];
	strcpy(palabra, "HOLA");
	struct Thread *thread_data;
	int i = 0;
	while(i < numeroHebras)
	{
		thread_data = malloc(sizeof(struct Thread));
		thread_data->tid = i;
		thread_data->palabra = palabra;
		thread_data->tablero = tablero;
		thread_data->posX = rand() % N-1;
		thread_data->posY = rand() % M-1;
		thread_data->N = N;
		thread_data->M = M;
		pthread_mutex_init(&thread_data->mutexHilo, NULL);

		pthread_create(&threads[i], NULL, ubicar, (void *) thread_data);
		i++;
	}
}

//Función que espera que terminen las hebras hijas
void waitHebras(pthread_t threads[], int numeroHebras)
{
	int i = 0;
	while(i < numeroHebras)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

//Función que inserta una palabra en la sopa de letras
int insertarPalabra(char *palabra, char** tablero, int posX, int posY, int N, int M)
{
	if(validarPosicionInicial(palabra, tablero, posX, posY, N, M))
	{
		insertarAuxiliar(palabra, tablero, posX, posY);
		return 1;	//Se pudo insertar palabra
	}

	return 0;		//No se pudo insertar palabra
}

//
void *ubicar(void *arg)
{
	struct Thread *thread_data = (struct Thread *) arg;
	//sleep(2);
	printf("Hola, soy la hebra %d\n", (int) thread_data->tid);
	pthread_mutex_lock(&mutex);
	printf("Hebra %d entró a SC\n", (int) thread_data->tid);
	while(1)
	{
		if(insertarPalabra(	thread_data->palabra, thread_data->tablero, thread_data->posX,
							thread_data->posY, thread_data->N, thread_data->M))
		{
			pthread_mutex_unlock(&mutex);
			break;		//Si se logra insertar la palabra se termina el while.
		}
		thread_data->posX = rand() % (thread_data->N)-1;	//Se intenta con otra posición random
		thread_data->posY = rand() % (thread_data->M)-1;
	}

	free(thread_data);		//se libera mem
}