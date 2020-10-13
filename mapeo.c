#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MAXIMO_FACTOR_DE_CARGA 0.75
#define FACTOR_DE_CARGA(cantidad_elementos,buckets)   ((cantidad_elementos+1)/buckets)

/**
ACOTACIONES GENERALES PARA LA IMPLEMENTACION DE MAPEO.C

--------------------------------------
es un mapeo de <string,int>
--------------------------------------
Para minimizar la cantidad de colisiones, la cantidad de listas (elementos del arreglo) deberia ser un numero primo (ej 11 : [0...10], etc).
Esto provoca que al superar el factor de carga y tener que reacomodar los elementos del mapeo, la cantidad de listas debe ser :
(numero primo(size viejo del mapeo) * 2 + 1), y a partir de ese valor, buscar el primer numero primo siguiente.
Siguiendo el ejemplo del 11, 11*2 = 22 + 1 = 23(controlamos que sea primo, si no lo es, buscamos sumando de a 1).
Todo eso lo va a hacer una nueva funcion "void mReacomodar(tMapeo m)", creo que debe ser un procedimiento, que va a estar en el main.c
--------------------------------------
hashcode() nos hace caer en un indice "i" del arreglo (x ej la 3)-> luego comparo usando la funcion comparador() elemento a elemento(string x string)
hasta llegar a encontrar el mismo string que quiero insertar ya presente en la lista(colision), o en caso de llegar al final, va a insertar en esa lista.
(en ese caso ese String no estaba en la lista).

*/


// ---------------------------------------------------------------------------------------------
// Definici�n de prototipos y documentaci�n de las funciones.
// Obs.: la tabla hash debe mantener control del factor de carga en todo momento.
// Obs.: el factor de carga m�ximo permitido equivale al 75% de la longitud de la tabla.
// ---------------------------------------------------------------------------------------------

/**
 Inicializa un mapeo vac�o, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves ser� computado mediante la funci�n fHash.
 A todo efecto, la comparaci�n de claves se realizar� mediante la funci�n fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    int maximo = MAX(10,ci);
    *m = (tMapeo) malloc(sizeof(struct mapeo));
    if(m == NULL){
		exit(MAP_ERROR_MEMORIA);
    }
	tLista mapeo[maximo];
	for (int i = 0; i<maximo; i++){
	    crear_lista(&mapeo[i]);
	}
	(*m)->tabla_hash = mapeo;
	(*m)->longitud_tabla = maximo;
	(*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
}

/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no exist�a en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
 ---------------------------------------------------------------------------------------------
 le pasamos a mStringHashDJB2 el puntero "c", y nos retorna un entero "x".
 Ingresamos al indice del arreglo de la TablaHashp[x].
 "factorDeCarga"(haciendo cant elemns +1/cant total de espacios), si nos retorna
        !=0(osea que no se excedio aun el 75%)
            hacer malloc del struct entrada insertamos la entrada en la lista !!! pensar como se deberia hacer de la manera mas eficiente
            auementamos en 1 la cant de elems del mapeo
            retornamos NULL.
        0 (osea que se excedio el factor de carga)
            hacemos rehash
 Si la lista que alli se enuentra, esta vacia
    hacemos la verificacion del factor de carga con la funcion

Si no esta vacia, vemos si esa clave esta presente en la lista de ese TablaHash[x]
    si esta presente
        recuperamos el valor de esa entrada y lo reemplazamos
        retornamos ese valor obtenido
    si no esta presente
        verificamos el factor de carga, si nos da
            >=75
                rehash
                insertar() recursivo ahora con el nuevo mapeo
 el

**/

tValor m_insertar(tMapeo m, tClave c, tValor v){
    int indice = m->hash_code(c);
    tLista * listaActual = m->tabla_hash[indice];
    if(l_longitud(listaActual) == 0){
        if(FACTOR_DE_CARGA(m->cantidad_elementos,m->longitud_tabla)>MAXIMO_FACTOR_DE_CARGA){
            return rehash(m, c, v);
        }
        else{
            tEntrada nueva_entrada = (tEntrada) malloc(sizeof(struct entrada));
            if(nueva_entrada == NULL){
	            exit(MAP_ERROR_MEMORIA);
            }
            nueva_entrada->clave = c;
            nueva_entrada->valor = v;
            l_insertar(listaActual, l_primera(listaActual), nueva_entrada);
            return NULL;
        }
    }else{

    }
}

}













//----------------------------------------------------------------------------------
/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

}

/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

}

/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/
tValor m_recuperar(tMapeo m, tClave c){
    return 0;
}
