#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"
#define LISTA_VACIA 99
#define ERROR_APERTURA_ARCHIVO -1
#define ERROR_PARAMETROS -2

/**
 * Funcion mStringHashDJB2: es la funcion hash mas eficiente para manejar hash abierto, cuyos valores son strings.
 * Param: un puntero a void (el cual en nuestro caso es un puntero a un string/arreglo de caracteres)
 */
int mStringHashDJB2(void * p){
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
 * Funcion mStringComparador
 * Param: recibe 2 punteros a void, los cuales sabemos que en realdiad apuntan a listas de caracteres
 * Return: un entero -> 0 si los strings son iguales
 *                   -> -1 en caso contrario
*/
int mStringComparador(void *e1, void *e2){
   return (strcmp(e1,e2));
}

void fEliminar(tElemento e){
    free(e);
}

void fEliminarC(tClave c){
    free(c);
}

void fEliminarV(tValor v){
    free(v);
}

void print_list(tLista l, int i){
    if(l == NULL){
        printf("La lista es invalida\n");
        exit(LISTA_VACIA);
    }
    printf("Lista %i:\n", i);
    int longitud_lista = l_longitud(l);
    if(longitud_lista != 0){
        tPosicion first = l_primera(l);
        tPosicion last = l_fin(l);
        tEntrada entrada;
        while(first != last){
            entrada = (tEntrada)l_recuperar(l, first);
            printf("clave %s , valor %i \n",(char*)entrada->clave, *(int*)entrada->valor);
            first = l_siguiente(l, first);
        }
    }
    else{
        printf("La lista esta vacia\n");
    }
    printf("\n");
}

void print_map(tMapeo m){
    printf("Mapeo :\n");
    tLista * tabla = m->tabla_hash;
    int size_tabla = m->longitud_tabla;
    tLista lista_actual = NULL;
    for(int i = 0; i< size_tabla ; i++){
        lista_actual = tabla[i];
        print_list(lista_actual,i);
    }
    printf("FIN DE MAPEO\n");
}


int incrementar(tValor v){
    int resultado = *((int *)v)+1;
    return (resultado);
}

static tMapeo leer_archivo(char ruta[],tClave * c, tValor * v){
     tMapeo mapeo;
     char * palabra = NULL;
     int * valor = NULL;
     tValor valor_recuperado = NULL;
     crear_mapeo(&mapeo,3,&mStringHashDJB2, &mStringComparador);
     FILE * file = fopen(ruta, "r");


     if (file) {
        while (!feof(file)){
            palabra = (char*)malloc(sizeof(char)*50);
            valor = (int*)malloc(sizeof(int));
            fscanf(file, "%s",palabra);
            valor_recuperado = m_recuperar(mapeo,palabra);

            if(valor_recuperado != NULL){
                *valor = incrementar(valor_recuperado);
            }else{
                *valor = 1;
            }
            *c = palabra;
            *v = valor;

            m_insertar(mapeo,*c,*v);
            if(valor_recuperado != NULL){
                free(palabra);
                free(valor_recuperado);
            }
        }
        fclose(file);
    }else{
        printf("Ocurrió un error al intentar abrir el archivo parametrizado");
        exit(ERROR_APERTURA_ARCHIVO);
    }
    return mapeo;
}

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


int main(int argc, char *argv []){
    if(argc==2){
        //Punteros encargados de guardar todo en la memoria dinamica
        void * key;
        void * value;
        int indice_menu = 0;
        tMapeo mapeo = leer_archivo(argv[1], &key, &value);
        print_map(mapeo);
        do{
            printf("\nCantidad de apariciones ........... 1\n");
            printf("Salir ............................. 2\n");
            scanf(" %i",&indice_menu);
            if(indice_menu==1){
                recuperar_palabras(mapeo);
            }
        }while(indice_menu == 1);
    }else{
        printf("Error ante la invocación del programa");
        exit(ERROR_PARAMETROS);
    }
    return 0;
}
