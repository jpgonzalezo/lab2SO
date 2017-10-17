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

//Función que rellena los valores nulos del tablero con caracteres aleatorios
//Entrada: Tablero y dimensiones
//Salida: Tablero modificado
char **fillTablero(char **tablero, int N, int M)
{
	int i, j;
	for (i = 0; i <N; ++i){
		for (j = 0; j < M; ++j){
			if(tablero[i][j]=='0')
			{
				int cMin = 'a';
				int cMax = 'a' + 25;
				int c = rand() % (cMax + 1 - cMin) + cMin;
				tablero[i][j] = c;
			}
		}
	}
	return tablero;
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


//Descripción: Función auxiliar a insertar palabra que reemplaza los caracteres en el tablero
//Entrada: la palabra a insertar, el tablero donde se inserta y la coordenada (x,y) desde donde se comienza a insertar la palabra
//Salida: no posee salida
void insertarAuxiliar(char *palabra, char **tablero, int posX, int posY){
	int largoPalabra=strlen(palabra);
	int i;
	for (i = 0; i < largoPalabra; ++i){
		tablero[posX][posY+i]=palabra[i];
	}
}

//Descripcion: Función que impreme un tablero
//Entrada: el tablero a imprimir , la cantidad de filas y columnas que posee y una bandera que identifica en caso de que se quiera mostrar o no
//Salida: no posee salida
void printTablero(char **tablero, int N, int M, int bandera)
{
	int i,j;
	if (bandera==1)
	{
		for (i = 0; i < N; ++i){
			for (j = 0; j < M; ++j){
				printf("%c", tablero[i][j]);	
			}
			printf("\n");
		}
	}
}

//Descripcion: funcion que escribe un tablero de sopa de letra en un archivo de salida
//Entrada: el tablero que se desea imprimir , la cantidad de filas y columnas que posee el tablero y el nombre del archivo de salida
//Salida: no posee retorno
void printTableroArchivo(char **tablero,int N,int M,char *salida){
	int i,j;
	if (salida !=NULL){
		FILE *archivoSalida;
		archivoSalida=fopen(salida,"w");
		fprintf(archivoSalida, "SOPA DE LETRAS\n");
		fprintf(archivoSalida, "========================\n");
		fprintf(archivoSalida, "\n");
		fprintf(archivoSalida, "\n");
		for (i = 0; i < N; ++i){
			for (j = 0; j < M; ++j){
				fprintf(archivoSalida,"%c", tablero[i][j]);	
			}
			fprintf(archivoSalida,"\n");
		}
		fclose(archivoSalida);
	}
}

//Descripcion: funcion que se encarga de crear las hebras que insertaran las palabras desde un archivo de texto
//Entrada: un arreglo de hebras donde se guardaran las creadas, el numero de hebras, el tablero donde trabajaran las hebras,
// la cantidad de filas y columnas que posee el tablero el nombre del archivo de texto que posee las palabras, 
//la cantidad de palabras que posee el archivo de texto y una bandera que identifica si los resultados son mostrados por consola
//Salida: no posee retorno 
void crearHebras(pthread_t threads[], int numeroHebras, char **tablero, int N, int M, char *file, int cantidadPalabras,int bandera)
{
	
	hebra *thread_data;
	int i = 0;
	int impar=0;
	int palabrasPorHebra=0;
	int palabrasArchivo= countLines(file, 100);

	//Asignación memoria mutex global
	mutex=(pthread_mutex_t **)malloc(sizeof(pthread_mutex_t*)*N);
	for (int i = 0; i <N; ++i){
		mutex[i]=(pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*M);
		for (int j = 0; j < M; ++j){

			pthread_mutex_init(&mutex[i][j], NULL);			
		}
	}

	//Necesario para leer el archivo para asignar palabras
	FILE *archivoTexto;
	archivoTexto = fopen(file, "r");
	char line[100];

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
		printf("ERROR: El número de hebras es mayor que el número de palabras.\n");
	}

	//caso contrario se deben guardar las palabras para la hebra
	else{
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
				thread_data->palabra[j]=(char*)malloc(sizeof(char)*100);
				fgets(line, 100,archivoTexto);
				line[strcspn(line, "\n")] = 0;
				strMayus(line);		//Se pasa la palabra a mayúsculas
				strcpy(thread_data->palabra[j], line);	
				int posX=rand() % N;
				int posY=rand()	% M;
				thread_data->coordenadas[j].posX=posX;
				thread_data->coordenadas[j].posY=posY;

			}

			//printf("guarde la palabra2: %s\n", thread_data->palabra[0]);
			thread_data->tablero = tablero;
			thread_data->N = N;
			thread_data->M = M;
			thread_data->bandera=bandera;
			thread_data->cantidadPalabras=palabrasPorHebra;
			
			if (bandera==1){
				printf("Hebra con id: %d\n", (int )thread_data->tid);
				printf("Se le asignan las palabras:\n");
				for (int z = 0; z < palabrasPorHebra; ++z){
					printf("%d - %d: %s\n",z,(int)thread_data->palabra[z][2], thread_data->palabra[z] );
				}
			}

			//pthread_mutex_init(&thread_data->mutexHilo, NULL);
			thread_data->mutexHilo = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)*palabrasPorHebra);
			pthread_create(&threads[i], NULL, ubicar, (void *) thread_data);
			i++;
		}		
	}

	fclose(archivoTexto);

}

//Descripcion: función que permite que se espere la ejecucion de un conjunto de hebras
//Entrada: una lista de las hebras que se desean esperar y el numero de hebras totales
//Salida: no posee retorno
void waitHebras(pthread_t threads[], int numeroHebras)
{
	int i = 0;
	while(i < numeroHebras)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

//Descripcion: funcion que inserta una palabra en un tablero
//Entrada: la palabra que se desea insertar, el tablero donde se inserta
//la pos x y pos y desde donde se comienza a insertar la palbra, la cantidad de filas y columnas que posee el tablero
//Salida: 1 en caso de que la palabra se haya insertado correctamente, 0 en caso contrario
int insertarPalabra(char *palabra, char** tablero, int posX, int posY, int N, int M)
{
	if(validarPosicionInicial(palabra, tablero, posX, posY, N, M))
	{
		insertarAuxiliar(palabra, tablero, posX, posY);
		return 1;	//Se pudo insertar palabra
	}

	return 0;		//No se pudo insertar palabra
}

//Descripcion: función que convierte un char a su contraparte en mayuscula
//Entrada: char a convertir
//Salida: no posee retorno
void strMayus(char *str)
{
	int i = 0;
	while(str[i]) {
		str[i] = toupper(str[i]);
		if(str[i]==241) str[i]=209;
		i++;
	}
}

//Descripcion: funcion que ubica las palabras dentro de un tablero
//Entrada: argumentos que en este caso es una lista de hebras
//Salida: no posee retorno
void *ubicar(void *arg)
{
	int w;
	hebra *thread_data = (hebra *) arg;
	if (thread_data->bandera==1){
		//printf("\nHebra %d entró a SC\n", (int) thread_data->tid);
	}
	for (w = 0; w < thread_data->cantidadPalabras ; ++w){
		thread_data->mutexHilo[w] = mutex[thread_data->coordenadas[w].posX][thread_data->coordenadas[w].posY];
		while(validarPosicionInicial(thread_data->palabra[w], thread_data->tablero, thread_data->coordenadas[w].posX, thread_data->coordenadas[w].posY, thread_data->N, thread_data->M)==0)
		{
			thread_data->coordenadas[w].posX= rand() % thread_data->N;
			thread_data->coordenadas[w].posY=rand() % thread_data->M;
			thread_data->mutexHilo[w] = mutex[thread_data->coordenadas[w].posX][thread_data->coordenadas[w].posY];
			if(pthread_mutex_trylock(&thread_data->mutexHilo[w])==0 && validarPosicionInicial(thread_data->palabra[w], thread_data->tablero, thread_data->coordenadas[w].posX, thread_data->coordenadas[w].posY, thread_data->N, thread_data->M)==1) break;
		}

		pthread_mutex_trylock(&thread_data->mutexHilo[w]);
		insertarAuxiliar(thread_data->palabra[w], thread_data->tablero, thread_data->coordenadas[w].posX, thread_data->coordenadas[w].posY);
		pthread_mutex_unlock(&thread_data->mutexHilo[w]);

		if (thread_data->bandera==1)
		{
			printf("\nHebra %d insertará la palabra: %s \n",(int) thread_data->tid, thread_data->palabra[w]);
			for(int i=0; i<thread_data->N; i++) printf("-");
			printf("\n");
			printTablero(thread_data->tablero, thread_data->N, thread_data->M,thread_data->bandera);
		}
		
	}
	
	free(thread_data);		//se libera mem
}

//Descripcion: fución que permite a un hebra entrar a SC
//Entrada: el mutex global
//Salida: no posee retorno
void enterSC(pthread_mutex_t mutex)
{
	//while(pthread_mutex_trylock(&mutex)!=0)
	printf("%d\n", pthread_mutex_lock(&mutex));
}

//Descripcion: funcion que permite liberar SC
//Entrada: mutex global
//Salida: no posee retorno
void exitSC(pthread_mutex_t mutex)
{
	pthread_mutex_unlock(&mutex);
	printf("salí de la SC");
}

//Descripcion: funcion que permite contar la cantidad de lineas que posee escrita un archivo 
//Entrada: el nombre del archivo y tamaño que posee la linea
//Salida: la cantidad de lineas que posee el archivo 
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