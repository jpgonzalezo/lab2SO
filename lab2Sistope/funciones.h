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
     int 				bandera;
     pthread_mutex_t	*mutexHilo;
}hebra;


pthread_mutex_t **mutex;	//Inicialización mutex


char **crearTableroDinamico(int N, int M);
int validarPosicionInicial(char *palabra, char **tablero, int posicionX, int posicionY, int N, int M);
void insertarAuxiliar(char *palabra, char **tablero, int posX, int posY);
void printTablero(char **tablero, int N, int M, int bandera);
void printTableroArchivo(char **tablero,int N,int M,char *salida);
void crearHebras(pthread_t threads[], int numeroHebras, char **tablero, int N, int M,char *file, int cantidadPalabras, int bandera);
void *threadTest(void *arg);
void waitHebras(pthread_t threads[], int numeroHebras);
int insertarPalabra(char *palabra, char** tablero, int posX, int posY, int N, int M);
void *ubicar(void *arg);
int countLines(char *fileName, int lineSize);
void enterSC(pthread_mutex_t mutex);
void exitSC(pthread_mutex_t mutex);
void strMayus(char *str);