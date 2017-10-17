#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "comparador.h"

int main (int argc, char **argv)
{

	char *file = NULL;
	int cursor = 0;
	char *search = NULL;
	int lineSize = 0;
	int linesToRead = 0;
	int numeroProceso = 0;
	int index;
	int c;

	opterr = 0;

		while ((c = getopt (argc, argv, "i:c:p:n:l:w:")) != -1)	//i: archivo, c: cursor(línea), p: cadena a buscar,
	  														//n: n° caracteres en cada línea, l: n° líneas a trabajar
		switch (c)
		{
		case 'i':
			file = optarg;
			break;
		case 'c':
			cursor = atoi(optarg);
			break;
		case 'p':
			search = optarg;
			break;
		case 'n':
			lineSize = atoi(optarg);
			break;
		case 'l':
			linesToRead = atoi(optarg);
			break;
		case 'w':
			numeroProceso = atoi(optarg);
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

    char **lines;
    lines = linesToCompare(file, cursor, linesToRead, lineSize+1);

    int i = 0;
    char guinBajo[]="_";
   	char nombre[]="rp";
    char pidx[5];
    sprintf(pidx,"%d",numeroProceso);
    strcat(nombre,guinBajo);
    strcat(nombre,search);
    strcat(nombre,guinBajo);
    strcat(nombre,pidx);
    strcat(nombre,".txt");
    FILE *archivoNuevo;
 	archivoNuevo=fopen(nombre,"w");
    fclose(archivoNuevo);

    while(i<linesToRead)
    {
    
    	FILE *archivoSalida;

   
    	archivoSalida=fopen(nombre,"a");
        int aux = findString(lines[i], search);
        if(aux==1) {
        	fprintf(archivoSalida, "%s\tSI\n", lines[i]);	//Se imprime resultado en archivo salida comparador
        }
        if(aux==0) {
        	fprintf(archivoSalida, "%s\tNO\n", lines[i]);	//Se imprime resultado en archivo salida comparador
        }i++;

        fclose(archivoSalida);
    }


    return 0;
}