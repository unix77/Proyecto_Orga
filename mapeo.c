#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MAXIMO_FACTOR_DE_CARGA 0.75
#define FACTOR_DE_CARGA(cantidad_elementos,buckets)   ((cantidad_elementos+1)/buckets)
#define LISTA_VACIA 99
typedef enum { false, true } bool;


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
    tLista * mapeo = (tLista*) malloc(sizeof (tLista) * maximo);
	for (int i = 0; i<maximo; i++){
	    crear_lista(&mapeo[i]);
	}
	(*m)->tabla_hash = mapeo;
	(*m)->longitud_tabla = maximo;
	(*m)->cantidad_elementos = 0;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
}


static tEntrada crear_entrada (tClave c, tValor v){
    tEntrada nueva_entrada = (tEntrada) malloc(sizeof(struct entrada));
    if(nueva_entrada == NULL){
        exit(MAP_ERROR_MEMORIA);
    }
    nueva_entrada->clave = c;
    nueva_entrada->valor = v;
    return nueva_entrada;
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
/**TO DO: Terminar de corregir el caso en el que caemos a una lista no vacia
          falta testear el funcionamiento de toda la funcion,
*/
tValor m_insertar(tMapeo m, tClave c, tValor v){
    printf("Adentro de insertar\n");
    printf("Longitud de tabla es  %i \n", m->longitud_tabla);
    int indice = (m->hash_code(c)) % (m->longitud_tabla);
    printf("Indice obtenido %i \n", indice);

    tLista * listaAux = (m->tabla_hash); //recupero EL ARREGLO, que es lo mismo que recuperar el tablaHash[0], alias el puntero a la primer lista

    int factor_actual = FACTOR_DE_CARGA(m->cantidad_elementos, m->longitud_tabla);
    printf("Factor actual de carga  %i \n", factor_actual);
    tEntrada nueva_entrada;
    tValor valor_result = NULL;

    tLista listaActual = *(listaAux+indice);//con aritmetica de punteros, incremento el puntero"listaAux" para que me apunte a la lista que quiero manipular

    printf("La longitud de la lista, antes de insertar es %i\n", l_longitud(listaActual));

    if(l_longitud(listaActual) == 0){
        printf("Caso de una lista vacia\n");
        if(factor_actual>MAXIMO_FACTOR_DE_CARGA){
            printf("Rehash con lista vacia\n");
            //rehash
        }
        nueva_entrada = crear_entrada(c, v);
        printf("Creamos una nueva entrada\n");

        l_insertar(listaActual, l_primera(listaActual),nueva_entrada);
        printf("Acabamos de insertar en la lista\n");
        printf("La cantidad de elementos de la lista, luego de insertar es %i\n", l_longitud(listaActual));

        m->cantidad_elementos++;
        printf("La cantidad de elementos del mapeo, luego de insertar es %i\n", m->cantidad_elementos);

    }else{
        printf("Caso de lista no vacia\n");
        valor_result = m_recuperar(m,c);
        if(valor_result != NULL){ //OJO ---------------------------------------
            valor_result = v; //OJO ---------------------------------------
        }else {
            if(factor_actual>MAXIMO_FACTOR_DE_CARGA){
                //rehash
            }
            nueva_entrada = crear_entrada(c, v);
            l_insertar(listaActual, l_primera(listaActual),nueva_entrada);
            m->cantidad_elementos++;
            printf("EEEEE\n");
        }
    }
    printf ("END Insertar\n");
    return valor_result;
}

static tPosicion buscar_Elemento(tLista lista, tClave c, tMapeo m){
    bool elemento_pertenece = false;
    tPosicion posicion = l_primera(lista);
    tEntrada elem = NULL;

    int size = l_longitud(lista);
    int cantidad = 0;

    tClave clave_recuperada = ((tEntrada) l_recuperar(lista, posicion))->clave;

    // tClave clave_recuperada = (posicion->elemento)->clave; // MAL

    while((m->comparador(clave_recuperada,c) != 0) && (cantidad != size)){
        //elem = (tEntrada) l_siguiente(lista, posicion);
        elem = l_recuperar(lista , l_siguiente(lista , posicion));
        clave_recuperada = elem->clave;
        cantidad++;
    }

    if((m->comparador(clave_recuperada,c) != 0)){
        posicion = NULL;
    }

    return posicion;
}

static void rehash(tMapeo mapeo){
    int nueva_dimension = proximo_primo(mapeo->longitud_tabla*2+1);
    int tamano_anterior = mapeo->longitud_tabla;
    tLista * arreglo_anterior = mapeo->tabla_hash;
    tLista  lista_anterior;
    tLista  pos_original;
    tLista pos_aux;
    tEntrada entrada;
    int posicion;
    //Creo nuevo arreglo
    mapeo->tabla_hash = malloc(sizeof (tLista) * nueva_dimension);
    for(int i = 0; i<nueva_dimension; i++){
        //Inicializamos el nuevo arreglo
	    crear_lista(&crear_lista(mapeo->tabla_hash[i]));
    }
    m->longitud = nueva_dimension;

   for(int i = 0; i<tamano_anterior; i++){
       lista_anterior = arreglo_anterior[i];
       //Por cada bucket del arreglo anterior se verifica ese bucket pasando toda la información a su nuevo hash.
       if(l_longitud(lista_anterior)>0){
           pos_original = l_primera(lista_anterior);
            //Mientras la lista tenga elementos
           while(pos_original!=NULL){
               //Lo agregamos
               entrada = l_recuperar(lista_anterior,pos_original);
               posicion = hash_code(m->clave);
               //Se guarda en un auxiliar el siguiente en caso de que tenga
               l_insertar(mapeo->tabla_hash[posicion],l_fin(mapeo->tabla_hash[posicion]),entrada);
               if(pos_original == l_fin(lista_anterior))
                    pos_aux = NULL;
               else
                    pos_aux = l_siguiente(lista_anterior,pos_original);
                //Se remueve de la lista original
                l_eliminar(lista_anterior,pos_original);
                //Se referencia al siguiente del eliminado.
                pos_original = pos_aux;
           }
       }
   }
}

static int proximo_primo(int numero){
    bool es_primo = false;
    int numero_a_probar = numero;
    while(!es_primo){
        numero_a_probar++;
        es_primo = es_primo(numero_a_probar);
    }
    return numero_a_probar;
}

static int es_primo (int numero){
    bool es_primo = true;
    int contador = 2;
    while (es_primo && contador != numero){
        if (numero %contador == numero){
            es_primo = false;
        }
        contador++;
    }
    return es_primo;
}

//----------------------------------------------------------------------------------
/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    int indice = m->hash_code(c);
    tLista * lista_actual = m->tabla_hash[indice];
    tEntrada entrada = ((tEntrada) l_recuperar(lista_actual, buscar_Elemento(*lista_actual,c,m)));
    fEliminarC(entrada->clave);
    fEliminarV(entrada->valor);

    l_eliminar(*lista_actual,)
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

tValor m_recuperar(tMapeo m , tClave c){
    int indice = m->hash_code(c);
    tLista * lista_actual = m->tabla_hash[indice];
    tEntrada result = ((tEntrada) l_recuperar(lista_actual, buscar_Elemento(*lista_actual,c,m)));// no convendria que buscar_elem, devuelva una tEntrada?
    return result->valor;
}
