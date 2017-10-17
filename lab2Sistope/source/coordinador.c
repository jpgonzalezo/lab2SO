#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "coordinador.h"




int main (int argc, char **argv)
{

	char *file = NULL;		//Archivo a leer
	int nProcesos = 0;		//N° de procesos
	int lineSize = 0;		//Tamaño de linea (cantidad de carácteres)
	char *search = NULL;	//String a buscar
	int flagShow = 0;			//Flag para mostrar o no por pantalla

	int index;
	int c;

	opterr = 0;

		while ((c = getopt (argc, argv, "i:n:c:p:d")) != -1)	//i: archivo, n: numero procesos, c: tamaño linea,
	  															//p: cadena a buscar, d: flag mostrar resultados
		switch (c)
		{
		case 'i':
			file = optarg;
			break;
		case 'n':
			nProcesos = atoi(optarg);
			break;
		case 'c':
			lineSize = atoi(optarg);
			break;
		case 'p':
			search = optarg;
			break;
		case 'd':
			flagShow = 1;
			break;
		case '?':
		if (optopt == 'c')
			fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
		else if (isprint (optopt))
		  	fprintf (stderr, "Opción desconocida `-%c'.\n", optopt);
		else
		  	fprintf (stderr,
           		"Opción con caracter desconocido `\\x%x'.\n",
				optopt);
			return 1;
		default:
		abort ();
		}

	for (index = optind; index < argc; index++)
		printf ("Argumento sin opción %s\n", argv[index]);

	FILE *in;			//Checkeando si archivo existe
	in = fopen(file, "r");
	if(in==NULL)
	{
		printf("ERROR: Archivo %s no existe\n", file);
		return 1;
	}
	fclose(in);

	if(strlen(search)<4)
	{
		printf("ERROR: Longitud de sucesión de ADN debe ser mayor a 4 (ejemplo: ATAT)\n");
		return 1;
	}





	
	if (forkComparadores(file, search, lineSize, nProcesos))
	{
		while (wait(NULL) > 0);			//Esperar a que terminen los procesos hijos.
	}
	else
	{
		while (wait(NULL) > 0);			//Esperar a que terminen los procesos hijos.
		printf("ERROR: No se pudieron forkear todos los comparadores.\n");
		return 1;	//Salida anormal
	}


	//REALIZAR ARCHIVO DE SALIDA FINAL
	FILE *salidaFinal;
	char nombreArchivoFinal[]="rc_";
	strcat(nombreArchivoFinal,search);
	strcat(nombreArchivoFinal,".txt");
	salidaFinal=fopen(nombreArchivoFinal,"w");
	if (flagShow)	printf("Resultados:\n");
	for (int i = 0; i < nProcesos; ++i){
		char nombreArchivoParcial[]="rp";
		char guion[]="_";
		char procActual[12];
    	sprintf(procActual,"%d",i);
    	strcat(nombreArchivoParcial,guion);
    	strcat(nombreArchivoParcial,search);
    	strcat(nombreArchivoParcial, guion);
    	strcat(nombreArchivoParcial, procActual);
    	strcat(nombreArchivoParcial, ".txt");


    	FILE *entrada;
    	entrada=fopen(nombreArchivoParcial,"r");
    	char linea[lineSize+7];
    	int cont=0;
    	while((feof(entrada))==0){
    		fgets(linea,lineSize+7, entrada);
    		cont++;
    	}
    	rewind(entrada);
    	for (int j = 0; j < cont-1; ++j){
    		fgets(linea,lineSize+7, entrada);			
			fprintf(salidaFinal, "%s",linea);	//se imprime en el archivo de salida
			if (flagShow)	printf("%s",linea);	//Si flag -d presente se imprime por pantalla
    	}
		fclose(entrada);
    	
	}
	fclose(salidaFinal);

	return 0;	//Todos los comparadores funcionaron correctamente

}