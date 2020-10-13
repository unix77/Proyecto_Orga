#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"
#define LISTA_VACIA 99

/**
 * Funcion mStringHashDJB2: es la funcion hash mas eficiente para manejar hash abierto, cuyos valores son strings.
 *
 * Param: un puntero a void (el cual en nuestro caso es un puntero a un string/arreglo de caracteres)
 *
 * Return: entero entre [0...map_array_size]
 */
int mStringHashDJB2(void *p){
    char * string = p;
    printf("El string es %s \n",string);
    unsigned long hash = 5381;
    int c;
    while (c = (*string)++){//le asigno a c, string[0] al inicio, y voy incrementando
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

/**
 * Funcion mStringComparador
 *
 * Param: recibe 2 punteros a void, los cuales sabemos que en realdiad apuntan a listas de caracteres
 *
 * Return: un entero -> 0 si los strings son iguales
 *                   -> -1 en caso contrario
*/
int mStringComparador(void *e1, void *e2){
   return (strcmp(e1,e2));
}

void fEliminar(tElemento e){
    free(e);
}

void print_list(tLista l){
    if(l == NULL){
        printf("La lista esta vacia\n");
        exit(LISTA_VACIA);
    }
    tPosicion p = l_primera(l);
    int * temp;
    while(p != l_fin(l)){
        temp = l_recuperar(l, p);
        printf("%i \n",*temp);
        p = l_siguiente(l, p);
    }
    printf("Imprimi toda la lista\n");
}

int main()
{
    printf("Creo una nueva lista\n");
    tLista new_list;
    crear_lista(&new_list);

    printf("La longitud de la lista es %i \n", l_longitud(new_list));

    int primero = 4;
    int segundo = 5;

    l_insertar(new_list,l_primera(new_list),&primero);
    l_insertar(new_list,l_primera(new_list),&segundo);
    printf("La longitud de la lista es %i \n", l_longitud(new_list));

    print_list(new_list);

    //l_eliminar(new_list, l_primera(new_list),&fEliminar);
    //print_list(new_list);

    l_destruir(&new_list,&fEliminar);
    print_list(new_list);
    //------------------------------------------------------------
    //Map tester
    printf("El size del struct mapeo en memoria es %i \n", sizeof(struct mapeo));
    printf("Creo una nuevo mapeo\n");
    tMapeo new_map;
    crear_mapeo(&new_map, 11, &mStringHashDJB2, &mStringComparador);
    printf("El size del mapeo es %i\n", sizeof(*new_map));

    printf("Longitud del map es %i\n", (new_map->longitud_tabla));

    char z[100] = "asd";
    //printf("El valor con hashDJB2 es %i\n" , (hashDJB2(z) % (new_map->longitud_tabla)));


    printf("El valor con mStringHashCode es %i\n" , mStringHashDJB2(z));
    char * a = "asd";
    char * b = "asd";
    printf("Los 2 strings son %i\n",mStringComparador(a,b));

}
