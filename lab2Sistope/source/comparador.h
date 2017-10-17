#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int findString(char *str, char *search)     //Función que busca el string en una líea
{
    int i = 0;
    int j = 0;
    int cont = 0;
    while(i < strlen(str))
    {
        if (str[i]==search[j])
        {
            cont++;
            j++;
            if (cont == strlen(search))
            {
                return 1;
                break;
            }
        }
        else
        {
            j = 0;
            cont = 0;
        }
        i++;

    }
    return 0;

}



char **linesToCompare(char* name, int cursor, int lines, int lineSize){     //Función que retorna las líneas
                                                                            //que debe checkear el comparador
    FILE *in;
    in = fopen(name, "r");
    char linea[lineSize];
    int cont=0;
    int aux=cursor;
    int aux2=0;
    char **result=NULL;
    result=(char **)malloc(sizeof(char*)*(lines));
    for (int i = 0; i < lines; ++i){
        result[i]=(char*)malloc(sizeof(char)*100);
    }

    while(feof(in)==0){
        fgets(linea,lineSize+1,in);
        linea[strcspn(linea, "\n")] = 0;       //Para quitar el salto de línea al final de linea
        if (cont==aux){
            if (aux2<lines)
            {
                strcpy(result[aux2], linea);
                aux2++;
                aux++;
            }
        }
        cont++;
    } 
    fclose(in);

    return result;
}