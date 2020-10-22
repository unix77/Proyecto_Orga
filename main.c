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
    /*
    unsigned long hash = 5381;
    int c;
    while ((c = (*string)++)){//le asigno a c, string[0] al inicio, y voy incrementando
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c
    }
    */
    return ((int)strlen(p));
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
/*
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
*/
int main()
{

    /*
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
*/
    //------------------------------------------------------------
    //Map tester
    printf("Creo una nuevo mapeo\n");
    tMapeo new_map;
    crear_mapeo(&new_map, 11, &mStringHashDJB2, &mStringComparador);

    printf("Longitud de la tabla hash es %i\n", (new_map->longitud_tabla));
    /*
    char z[100] = "asd";
    //printf("El valor con hashDJB2 es %i\n" , (hashDJB2(z) % (new_map->longitud_tabla)));


    printf("El valor con mStringHashCode es %i\n" , mStringHashDJB2(z));
    char * a = "asd";
    char * b = "asd";
    printf("Los 2 strings son %i\n",mStringComparador(a,b));
*/
/*
    printf("Insertamos 3 entradas\n");

    char i1 = '1';
    char i2 = '2';
    char i3 = '3';


    char * clave = "cardo";
    char clave2 = "asd";
    char * clave3 = "dk";

    char * vv1 = "wqewq";
    char vv2 = "dim";
    char * vv3 = "poi";


    /*
    m_insertar(new_map,clave1,vv1);
    printf("valor 1:  %s\n", (char*)m_recuperar(new_map,"cardo"));





    m_insertar(new_map,&clave2,&vv2);
    printf("valor 2:  %s\n", (char*)m_recuperar(new_map,"asd"));

    m_insertar(new_map,vv3,clave3);

    printf("El size del mapeo es %i\n", sizeof(*new_map));
    printf("Longitud del map es %i\n", (new_map->longitud_tabla));

    tValor valor1 = m_recuperar(new_map,clave1);

    printf("valor 1:  %s\n", (char*)(valor1));
    tValor valor2 = m_recuperar(new_map,clave2);

    printf("valor 2:  %s\n",(char*)(valor2));
    tValor valor3 = m_recuperar(new_map,clave3);

    printf("valor 13:  %s\n", (char*)(valor3));
*/
    /** cada vez q inserto, tengo q hacer malloc
    declaro un puntero a char y otro a int
    */

    char * clave;
    char * valor;

    clave = (char*) malloc(sizeof(char)*4);
    valor = (char*) malloc(sizeof(float)*4);

    //*clave = "hola";
    //*valor = "chau";

    strcpy(clave,"hola");
    strcpy(valor,"chau");

    m_insertar(new_map, clave, valor);

    printf("valor con malloc es:  %s\n", (char*)m_recuperar(new_map,"hola"));

    strcpy(clave,"asd");
    strcpy(valor,"qwe");

    m_insertar(new_map, clave, valor);

    printf("valor con malloc es:  %s\n", (char*)m_recuperar(new_map,"asd"));




}
