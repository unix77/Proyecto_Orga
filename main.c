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
int mStringHashDJB2(void * p){//antes era int mStringHashDJB2(void *p)
    int g = 31;
    int longitud = strlen(p);
    int result = 0;
    char * string = p;

    for(int i = 0 ; i < longitud ; i++){
        result = g * result + string[i];
    }

    return result;
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

void fEliminarC(tClave c){
    printf("Adentro de fEliminarC\n");
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
/*
void crear_CV(tClave * c, tValor * v){
    printf("Creando clave y valor\n");

    //creo 2 variables que se destruiran una vez que termine su funcionamiento esta funcion
    char key [50];
    int * value = NULL;

    //reservo memoria para la clave y el valor
    *c = (char*) malloc(sizeof(char)*50);
    *v = (int*) malloc(sizeof(int));


    printf("Ingrese una clave \n");
    scanf("%s ",key);
    printf("Key guarda %s \n", key);

    //strcpy(*c,key);//guardo en la memoria dinamica, el contenido de los strings temporales (key, value)


    printf("Ingrese un valor \n");
    scanf("%i ",value);
    *v = value;

    printf("Fin de la creacion de clave y valor\n");
}*/

void registrar(tClave * c, tValor * v){
    char * key = NULL;
    int * value = NULL;

    key = (char*)malloc(sizeof(char)*50);
    value = (int*)malloc(sizeof(int));

    printf("Ingrese una clave\n");
    scanf("%s", key);

    /*
    printf("Ingrese un valor\n");
    scanf("%i", value);
    */

    *value = 1;

    *c = key;
    *v = value;

    printf("Fin de registrar\n");
}



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
/*
    printf("Creo una nuevo mapeo\n");
    tMapeo new_map;
    crear_mapeo(&new_map, 11, &mStringHashDJB2, &mStringComparador);

    printf("Longitud de la tabla hash es %i\n", (new_map->longitud_tabla));
    char * clave;
    char * valor;

    clave = (char*) malloc(sizeof(char)*50);
    valor = (char*) malloc(sizeof(char)*50);

    //*clave = "hola";
    //*valor = "chau";

    strcpy(clave,"hola");
    strcpy(valor,"chau");


    m_insertar(new_map, clave, valor);

    printf("Imprimo el mapeo despues de insertar\n");
    print_map(new_map);


    strcpy(clave,"asd");
    strcpy(valor,"qwe");

    m_insertar(new_map, clave, valor);

    printf("Imprimo el mapeo despues de insertar\n");
    print_map(new_map);


    printf("valor con malloc es:  %s\n", (char*)m_recuperar(new_map,clave));

    printf("Longitud del mapeo es %i\n", (new_map->cantidad_elementos));

    printf("La clave afuera, ANTES DE ELIMINAR es %s \n", (char*)clave);

    printf("Su hash code, ANTES DE ELIMINAR es %i \n", mStringHashDJB2(clave)%11);
    m_eliminar(new_map, clave, &fEliminarC, &fEliminarV);

    printf("Longitud del mapeo despues de eliminar es %i\n", (new_map->cantidad_elementos));

    printf("Imprimo el mapeo despues de ELIMINAR\n");
    print_map(new_map);

     m_eliminar(new_map, "hola", &fEliminarC, &fEliminarV);

     printf("Imprimo el mapeo despues de ELIMINAR\n");
    print_map(new_map);
*/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Creo una nuevo mapeo\n");
    tMapeo map;
    crear_mapeo(&map, 11, &mStringHashDJB2, &mStringComparador);

    print_map(map);

    printf("Longitud de la tabla hash es %i\n", (map->longitud_tabla));
    void * key;
    void * value;

    registrar(&key, &value);
    //print_map(map);

    printf("*c es %s \n", (char*)key);
    printf("*v es %i \n",*(int*)value);

    m_insertar(map, key, value);

    registrar(&key, &value);
    //print_map(map);


    printf("*c es %s \n", (char*)key);
    printf("*v es %i \n",*(int*)value);

    m_insertar(map, key, value);

    print_map(map);

    /*
    m_eliminar(map, "vane", &fEliminarC, &fEliminarV);

    print_map(map);
    */

    m_destruir(&map, &fEliminarC, &fEliminarV);

    print_map(map);



}
