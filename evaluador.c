#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lista.h"
#include "mapeo.h"
#define ERROR_APERTURA_ARCHIVO -1
#define ERROR_PARAMETROS -2

/**
 * Calcula y retorna el codigo hash del valor parametrizado.
 */
int hash_string(void * p){
    int g = 31;
    int longitud = strlen(p);
    int resultado = 0;
    char * string = p;
    for(int i = 0 ; i < longitud ; i++){
        resultado = g * resultado + string[i];
    }
    return resultado;
}

/**
 * Funcion mStringComparador, utilizada por el mapeo, que compara 2 cadenas de caracteres
 * Param: recibe 2 punteros a void, que apuntan a cadenas de caracteres,
 * Return: -> 0 si los strings son iguales
*/
int mStringComparador(void *e1, void *e2){
   return (strcmp(e1,e2));
}

/**
 * Libera la memoria del elemento parametrizado.
 */
void fEliminar(tElemento e){
    free(e);
    e = NULL;
}

/**
 * Libera la memoria de la clave parametrizada.
 */
void fEliminarC(tClave c){
    free(c);
    c = NULL;
}

/**
 * Libera la memoria del valor parametrizado.
 */
void fEliminarV(tValor v){
    free(v);
    v = NULL;
}

/**
 * Incrementa en 1 el valor parametrizado.
 */
static int incrementar(tValor v){
    int resultado = *((int *)v)+1;
    return (resultado);
}

/**
 * Se encarga de eliminar cualquier caracter que no sea una letra, de la primer y ultima posicion de la palabra.
 */
static void limpiar_palabra(char palabra[]) {
    int longitud = strlen(palabra);
    char * aux = palabra+1;

    if(isalpha(palabra[0])==0){
        strcpy(palabra, aux);
    }

    longitud = strlen(palabra);
    if(isalpha(palabra[longitud-1])==0){
        palabra[longitud-1] = '\0';
    }
}

/**
 * Crea y retorna un mapeo, generado a partir de un archivo cuya ruta fue parametrizada.
 * Param: recibe 2 punteros por parametro, que representan la clave y valor a insertar.
 */
static tMapeo leer_archivo(char ruta[],tClave * c, tValor * v){
     //se utilizaran los punteros c y v para realizar la insercion en el mapeo
     tMapeo mapeo;
     //se crean punteros auxiliares para facilitar el manejo de memoria dinamica
     char * palabra = NULL;
     int * valor = NULL;
     //se encarga de reservar memoria para la palabra leida desde el archivo
     char * palabra_leida = NULL;
     palabra_leida = (char*)malloc(sizeof(char)*150);

     tValor valor_recuperado = NULL;

     crear_mapeo(&mapeo,11,&hash_string, &mStringComparador);
     FILE * file = fopen(ruta, "r");


     if (file) {
        while(!ferror(file) && fscanf(file, "%s",palabra_leida) != EOF){
            limpiar_palabra(palabra_leida);
            //se reserva memoria para la palabra y el valor a insertar (en caso de que sea necesario)
            palabra = (char*)malloc(sizeof(char)*strlen(palabra_leida));
            valor = (int*)malloc(sizeof(int));
            //copia la "palabra_leida", en "palabra" sin los caracteres sobrantes
            strcpy(palabra ,palabra_leida);
            valor_recuperado = m_recuperar(mapeo,palabra);
            if(valor_recuperado != NULL){
                *valor = incrementar(valor_recuperado);
            }else{
                *valor = 1;
            }
            *c = palabra;
            *v = valor;

            m_insertar(mapeo,*c,*v);
            //en caso de que una entrada con la misma clave ya existia en el mapeo, se libera la memoria innecesaria
            if(valor_recuperado != NULL){
                free(palabra);
                free(valor_recuperado);
            }
        }
        fclose(file);
    }else{
        printf("Ocurrio un error al intentar abrir el archivo parametrizado");
        exit(ERROR_APERTURA_ARCHIVO);
    }
    //se libera memoria reservada por la palabra_leida (una vez que se leyo todo el archivo)
    free(palabra_leida);
    return mapeo;
}

/**
 * A partir del mapeo parametrizado, muestra la cantidad de apariciones de la palabra leida por consola.
 */
static void recuperar_palabras(tMapeo mapeo){
    char palabra[50];
    tValor cantidad_de_apariciones;
    tClave clave;
    printf("Ingrese la palabra que desea consultar: \n");
    scanf(" %s",palabra);
    clave = &palabra;

    cantidad_de_apariciones = m_recuperar(mapeo,clave);
    if(cantidad_de_apariciones == NULL){
        printf("La palabra %s no aparece\n",palabra);
    }else{
        printf("La palabra %s aparece %i veces\n",palabra,*(int*)cantidad_de_apariciones);
    }
}

/**
 * Maneja el menu del programa .
 */
int main(int argc, char *argv []){
    if(argc==2){
        //Punteros encargados de guardar todo en la memoria dinamica
        void * key;
        void * value;

        int indice_menu = 0;
        tMapeo mapeo = leer_archivo(argv[1], &key, &value);

        do{
            printf("\nCantidad de apariciones ........... 1\n");
            printf("Salir ............................. 2\n");
            scanf(" %i",&indice_menu);
            if(indice_menu==1){
                recuperar_palabras(mapeo);
            }
            else if (indice_menu == 2){
                m_destruir(&mapeo,fEliminarC,fEliminarV);
                printf("El mapeo ha sido destruido\n");
            }
            else{
                printf("Elija una de las opciones en pantalla\n");
            }
        }while(indice_menu != 2);

    }else{
        printf("Error ante la invocacion del programa");
        exit(ERROR_PARAMETROS);
    }
    return 0;
}
