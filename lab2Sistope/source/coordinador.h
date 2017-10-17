#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int countLines(char *fileName, int lineSize)	//Función que cuenta las lineas del archivo de entrada
{
	FILE *file;
	file = fopen(fileName, "r");
	int count = 0;
	char line[lineSize+2];
	int countAux = 0;
	char c;
	while(feof(file)==0)
	{
		c = fgetc(file);
		if(countAux==lineSize) //cuenta de acuerdo al tamaño de la linea para evitar problemas como contar lineas en blanco
		{
			c = fgetc(file);
			countAux = 0;
			count++;
		}
		countAux++;
	}
	fclose(file);
	return count;
}


int forkComparador(char *i, int c, char *p, int n, int l, int w)		// Función que forkea un comparador
{
	char caux[12];		// Se transforman los int a char para que funcionen con execl
	char naux[12];
	char laux[12];
	char waux[12];
	sprintf(caux, "%d", c);
	sprintf(naux, "%d", n);
	sprintf(laux, "%d", l);
	sprintf(waux, "%d", w);

	int pid = fork();
	if(pid==0) 
	{
		execl("./comparador", "./comparador","-i",i,"-c",caux,"-p",p,"-n",naux,"-l",laux, "-w", waux, (const char *)NULL);
		return 0;
	} else if (pid > 0) 
	{
    	//wait(NULL); // Si se usa este wait se espera a que termine cada proceso cada vez que se crea uno
	} 
	else 
	{
		// No se pudo forkear comparador
		return 0;
	}
	return 1;
}



int forkComparadores(char *i, char *p, int n, int nProcesos)		//Función que forkea todos los comparadores
{
	int lines = countLines(i, n);		//Se cuentan las líneas del archivo
	int actualLine = 0;			//Línea actual

	if(nProcesos>lines)			// Error si el número de procesos es mayor al número de lineas
	{
		printf("ERROR: Número de procesos mayor que el número de líneas\n");
		return 0;
	}

	if(lines%nProcesos==0) 		//Todos los procesos trabajan el mismo número de líneas
	{
		int creados = 0;
		int actualLine = 0;
		while(creados < nProcesos)
		{
			if (forkComparador(i, actualLine, p, n, lines/nProcesos, creados))	//Si fork es exitoso
			{
				actualLine = actualLine + lines/nProcesos;
				creados++;
				
			}
			else return 0; //Falla un fork, no se pudieron forkear todos los procesos
		}
		if(creados>1)	printf("Se crearon %i procesos comparadores que trabajan %i línea(s) cada uno\n", creados, lines/nProcesos);
		if(creados==1)	printf("Se creó 1 proceso comparador que trabaja las %i líneas\n", lines/nProcesos);
		return 1;

	}
	else			//El último proceso trabaja distinto número de líneas
	{

		int creados = 0;
		while(creados < nProcesos-1)
		{
			if (forkComparador(i, actualLine, p, n, lines/nProcesos, creados))	//Si fork es exitoso
			{
				actualLine = actualLine + lines/nProcesos;
				creados++;
			}
			else return 0;	//Falla un fork, No se pudieron forkear todos los procesos

		}

		if (forkComparador(i, actualLine, p, n, (lines/nProcesos)+(lines%nProcesos), creados)) 
			{
				actualLine = actualLine + (lines/nProcesos)+(lines%nProcesos);
				creados++;
				printf("Se crearon %i procesos comparadores donde:\n", creados);
				printf("Los primeros %i trabajan %i línea(s)\n", creados-1, lines/nProcesos);
				printf("El último trabaja %i línea(s)\n", (lines/nProcesos)+(lines%nProcesos));
				return 1;
			}
		else return 0;		//Falla un fork, No se pudieron forkear todos los procesos
	}

	return 0; 	//No se pudieron forkear todos los procesos
}