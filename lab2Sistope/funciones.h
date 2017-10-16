typedef struct{

	int posX;
	int posY;
}coordenada;

typedef struct {
     pthread_t 			tid;
     char 				**palabra;
     char 				**tablero;
     coordenada 		*coordenadas;
     int 				N;
     int 				M;
     int  				cantidadPalabras;
     pthread_mutex_t	mutexHilo;
}hebra;

pthread_mutex_t mutex;	//mutex global, solo para testear.


char **crearTableroDinamico(int N, int M);
int validarPosicionInicial(char *palabra, char **tablero, int posicionX, int posicionY, int N, int M);
void insertarAuxiliar(char *palabra, char **tablero, int posX, int posY);
void printTablero(char **tablero, int N, int M);
void crearHebras(pthread_t threads[], int numeroHebras, char **tablero, int N, int M,char *file, int cantidadPalabras);
void *threadTest(void *arg);
void waitHebras(pthread_t threads[], int numeroHebras);
int insertarPalabra(char *palabra, char** tablero, int posX, int posY, int N, int M);
void *ubicar(void *arg);
int countLines(char *fileName, int lineSize);
void enterSC(pthread_mutex_t mutex);
void exitSC(pthread_mutex_t mutex);

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
void crearHebras(pthread_t threads[], int numeroHebras, char **tablero, int N, int M, char *file, int cantidadPalabras)
{
	
	hebra *thread_data;
	int i = 0;
	int impar=0;
	int palabrasPorHebra=0;
	int palabrasArchivo= countLines(file, 100);

	//Necesario para leer el archivo para asignar palabras
	FILE *archivoTexto;
	archivoTexto = fopen(file, "r");
	char line[9];

	//Todas las hebras tienen la misma cantidad de palabras para insertar
	if (cantidadPalabras%numeroHebras==0){
		palabrasPorHebra=cantidadPalabras/numeroHebras;
	}
	//La cantidad de palabras es impar
	else{
		impar=1;
		palabrasPorHebra=cantidadPalabras/numeroHebras;
	}


	//Si el numero de hebras es mayor que la cantidad de palabras existentes entonces hay un error
	if (numeroHebras>cantidadPalabras || palabrasArchivo!=cantidadPalabras){
		printf("Error: EL numero de hebras es mayor que la cantidad de palabras\n");
	}

	//caso contrario se deben guardar las palabras para la hebra
	else{
		printf("\n");
		printf("\n");
		while(i < numeroHebras){
			int j;
			thread_data = malloc(sizeof(hebra));
			thread_data->tid = i;

			int contadorPalabras=0;

			if (impar==1 && i==numeroHebras-1){
				palabrasPorHebra=palabrasPorHebra+1;
			}

			thread_data->coordenadas=(coordenada*)malloc(sizeof(char)*palabrasPorHebra);
			thread_data->palabra=(char**)malloc(sizeof(char*)*palabrasPorHebra);
			for (j = 0; j < palabrasPorHebra; ++j)
			{
				thread_data->palabra[j]=(char*)malloc(sizeof(char)*9);
				fgets(line, 9,archivoTexto);
				strcpy(thread_data->palabra[j], line);
				//printf("guarde la palabra %d: %s\n",j,thread_data->palabra[j]);
				int posX=rand() % N-1;
				int posY=rand()	% M-1;
				int validador= validarPosicionInicial(thread_data->palabra[j], tablero, posX, posY, N, M);
				while(validador==0){
					int posX=rand() % N-1;
					int posY=rand()	% M-1;
					validador= validarPosicionInicial(thread_data->palabra[j], tablero, posX, posY, N, M);
				}

				thread_data->coordenadas[j].posX=posX;
				thread_data->coordenadas[j].posY=posY;

			}

			//printf("guarde la palabra2: %s\n", thread_data->palabra[0]);
			thread_data->tablero = tablero;
			thread_data->N = N;
			thread_data->M = M;
			thread_data->cantidadPalabras=palabrasPorHebra;
			
			
			printf("Soy la hebra: %d\n", (int )thread_data->tid);
			printf("tengo que colocar las palabras:\n");
			for (int z = 0; z < palabrasPorHebra; ++z){
				printf("%d: %s\n",z,thread_data->palabra[z] );
			}
			printf("\n");
			printf("\n");
			pthread_mutex_init(&thread_data->mutexHilo, NULL);
			pthread_create(&threads[i], NULL, ubicar, (void *) thread_data);
			i++;
		}		
	}

	fclose(archivoTexto);

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
		printf("posicion valida (%d,%d) para palabra %s\n", posX,posY,palabra);
		insertarAuxiliar(palabra, tablero, posX, posY);
		return 1;	//Se pudo insertar palabra
	}

	return 0;		//No se pudo insertar palabra
}

//
void *ubicar(void *arg)
{
	int w;
	hebra *thread_data = (hebra *) arg;
	//printf("Hola, soy la hebra %d \n", (int) thread_data->tid);
	//pthread_mutex_lock(&mutex);
	pthread_mutex_lock(&mutex);
	printf("Hebra %d entró a SC\n", (int) thread_data->tid);
	for (w = 0; w < thread_data->cantidadPalabras ; ++w){
		//insertarAuxiliar(thread_data->palabra[w], thread_data->tablero, thread_data->coordenadas[w].posX,
		//thread_data->coordenadas[w].posY);
		insertarPalabra(thread_data->palabra[w], thread_data->tablero, thread_data->coordenadas[w].posX,
						thread_data->coordenadas[w].posY, thread_data->N, thread_data->M);
		printf("inserte la palabra: %s \n",thread_data->palabra[w]);
		printTablero(thread_data->tablero, thread_data->N, thread_data->M);
	}
	//pthread_mutex_unlock(&mutex);
	
	free(thread_data);		//se libera mem
	pthread_mutex_unlock(&mutex);
}

void enterSC(pthread_mutex_t mutex)
{
	//while(pthread_mutex_trylock(&mutex)!=0)
	printf("%d\n", pthread_mutex_lock(&mutex));
}

void exitSC(pthread_mutex_t mutex)
{
	pthread_mutex_unlock(&mutex);
	printf("salí de la SC");
}

int countLines(char *fileName, int lineSize)	//Función que cuenta las lineas del archivo de entrada
{
	FILE *file;
	file = fopen(fileName, "r");
	int count = 0;
	char line[lineSize];
	char c;
	while(feof(file)==0)
	{
		fgets(line, 100,file);
		count++;
	}
	fclose(file);
	return count-1;
}