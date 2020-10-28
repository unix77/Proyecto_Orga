#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MAXIMO_FACTOR_DE_CARGA 0.75
#define FACTOR_DE_CARGA(cantidad_elementos,buckets)   ((cantidad_elementos+1)/buckets)
#define LISTA_VACIA 99
typedef enum { false, true } bool;


void (*fEliminarCGlobal)(void *);
void (*fEliminarVGlobal)(void *);


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
    int maximo = MAX(1,ci);
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

static void fEliminarEntrada (tElemento elemento){
    printf("Adentro de eliminar entrada\n");
    tEntrada entrada = (tEntrada) elemento;
    printf("Atroden de eliminarEntrada AAAAA\n");

    fEliminarCGlobal(entrada->clave);
    printf("Atroden de eliminarEntrada BBBBB\n");

    fEliminarVGlobal(entrada->valor);
    printf("Atroden de eliminarEntrada CCCCC\n");

    free(entrada);
    printf("END de fEliminar Entrada\n");

}

static tPosicion buscar_Elemento(tLista lista, tClave c, tMapeo m){
    printf("Adentro de buscar elemento\n");
    tPosicion posicion = l_primera(lista);
    tPosicion pos_fin = l_fin(lista);
    tEntrada elem = NULL;
    tClave clave_recuperada;
    int encontre = 0;// en caso de que no haya encontrado la clave ingresada por parametro

    int size = l_longitud(lista);
    //int cantidad = 1; // CAMBIE, ERA cantidad = 0;

    printf("CLAVE:::::::::::::::::::::::::::: %s \n",c);
    if(size != 0){
        while(posicion != pos_fin && encontre == 0){
            clave_recuperada = ((tEntrada) l_recuperar(lista, posicion))->clave;// agarro la entrada de esa pos, y recupero su clave

            printf("La clave recuperada es {%s}\n", (char*) clave_recuperada);
            printf("Las palabras son %i\n",m->comparador(clave_recuperada,c));

            if(m->comparador(clave_recuperada,c) != 0){
                posicion = l_siguiente(lista, posicion);
            }
            else{
                encontre = 1;
            }
        }
    }
    if(encontre == 0){
        posicion = NULL;
    }
    return posicion;
}


/*
static tPosicion buscar_Elemento(tLista lista, tClave c, tMapeo m){
    printf("Adentro de buscar elemento\n");


    tPosicion posicion = l_primera(lista);
    tEntrada elem = NULL;
    tClave clave_recuperada;

    int size = l_longitud(lista);
    int cantidad = 1; // CAMBIE, ERA cantidad = 0;

    printf("CLAVE:::::::::::::::::::::::::::: %s \n",c);
    if(size!=0){

        clave_recuperada = ((tEntrada) l_recuperar(lista, posicion))->clave;// agarro la entrada de esa pos, y recupero su clave

        printf("La clave recuperada es {%s}\n", (char*) clave_recuperada);


        printf("Las palabras son %i\n",m->comparador(clave_recuperada,c));

        while((m->comparador(clave_recuperada,c) != 0) && (cantidad != size)){
            printf("INSIDE FJSDAKFJASDKFDSA \n");
            //elem = (tEntrada) l_siguiente(lista, posicion);
            elem = l_recuperar(lista , l_siguiente(lista , posicion));
            clave_recuperada = elem->clave;
            cantidad++;
        }

        if((m->comparador(clave_recuperada,c) != 0)){
            posicion = NULL;
        }
    }
    return posicion;
}
*/
static int primo (int numero){
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

static int proximo_primo(int numero){
    bool es_primo = false;
    int numero_a_probar = numero;
    while(!es_primo){
        numero_a_probar++;
        es_primo = primo(numero_a_probar);
    }
    return numero_a_probar;
}


//----------------------------------------------------------------
void print_listt(tLista l, int i){
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
//----------------------------------------------------------------

void fEliminarNada(tElemento e){

}

static void rehash(tMapeo mapeo, void (*fEliminar)(tElemento)){
    int nueva_dimension = proximo_primo(mapeo->longitud_tabla*2+1);
    int tamano_anterior = mapeo->longitud_tabla;
    tLista * arreglo_anterior = mapeo->tabla_hash;
    tLista  lista_anterior;
    tPosicion  pos_original;
    tPosicion pos_fin = NULL;
    tEntrada entrada;
    int indice;
    //Creo nuevo arreglo
    mapeo->tabla_hash = malloc(sizeof (tLista) * nueva_dimension);
    for(int i = 0; i<nueva_dimension; i++){
        //Inicializamos el nuevo arreglo
        tLista nueva_lista = NULL;
        crear_lista(&nueva_lista);
        mapeo->tabla_hash[i] = nueva_lista;
    }
    mapeo->longitud_tabla = nueva_dimension;

   for(int i = 0; i<tamano_anterior; i++){
       lista_anterior = arreglo_anterior[i];
       //Por cada bucket del arreglo anterior se verifica ese bucket pasando toda la información a su nuevo hash.
       if(l_longitud(lista_anterior)>0){
            print_listt(lista_anterior, 90);
            pos_original = l_primera(lista_anterior);//apunto al centinela
            pos_fin = l_fin(lista_anterior);
            //Mientras la lista tenga elementos

           while(pos_original != pos_fin){
               //Lo agregamos
               entrada = l_recuperar(lista_anterior,pos_original);
               indice = mapeo->hash_code(entrada->clave)% (mapeo->longitud_tabla);

               printf("Posicion es  %i \n", indice);


               printf("LONGITUD %i \n",l_longitud(mapeo->tabla_hash[indice]));
                //Se guarda en un auxiliar el siguiente en caso de que tenga

               l_insertar(mapeo->tabla_hash[indice],l_primera(mapeo->tabla_hash[indice]),entrada);

               pos_original = l_siguiente(lista_anterior,pos_original);

           }
           print_listt(mapeo->tabla_hash[indice], 90);
       }
       printf("Libero la memoria ocupada por la lista vieja\n");
       //l_destruir(&lista_anterior,fEliminar);
   }
   printf("EXIT DEL REHASH\n");
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
    int indice = (m->hash_code(c)) % (m->longitud_tabla);
    printf("Adentro de insertar indice es %i \n",indice);
    tPosicion posAux = NULL;
    tEntrada entradaAux = NULL;

    tLista * listaAux = (m->tabla_hash); //recupero EL ARREGLO, que es lo mismo que recuperar el tablaHash[0], alias el puntero a la primer lista

    int factor_actual = FACTOR_DE_CARGA(m->cantidad_elementos, m->longitud_tabla);
    //printf("Factor actual de carga  %i \n", factor_actual);

    tEntrada nueva_entrada;

    tValor valor_result = NULL;

    tLista listaActual = *(listaAux+indice);//con aritmetica de punteros, incremento el puntero"listaAux" para que me apunte a la lista que quiero manipular

    //printf("La longitud de la lista, antes de insertar es %i\n", l_longitud(listaActual));

    if(l_longitud(listaActual) == 0){
        //printf("Caso de una lista vacia\n");
        if(factor_actual>MAXIMO_FACTOR_DE_CARGA){
            printf("REHASH CON LISTA VACIA\n");
            rehash(m,&fEliminarNada);
            m_insertar(m, c, v);
        }
        else{
            nueva_entrada = crear_entrada(c, v);
            printf("Creamos una nueva entrada\n");

            l_insertar(listaActual, l_primera(listaActual),nueva_entrada);
            printf("Acabamos de insertar en la lista\n");
            printf("La cantidad de elementos de la lista, luego de insertar es %i\n", l_longitud(listaActual));

            m->cantidad_elementos++;
            printf("La cantidad de elementos del mapeo, luego de insertar es %i\n", m->cantidad_elementos);
        }
    }else{
        printf("Caso de lista no vacia\n");
        posAux = buscar_Elemento(listaActual, c, m); // recupero la posicion de la entrada, cuya clave es = c, que ya existia, o no en el mapeo
        if(posAux != NULL){ //existe una entrada con esa clave en el mapeo
            entradaAux = l_recuperar(listaActual, posAux);
            valor_result = entradaAux->valor; //guardo en esa ubicacion de memoria, el valor vi
            entradaAux->valor = v; // cambio la clave vieja, por la recibida por parametro
        }else {
            if(factor_actual>MAXIMO_FACTOR_DE_CARGA){
                printf("REHASH lista NO VACIA JJJJJJJJJJJJJJJJJJJJJJJJJJJJ\n");
                rehash(m,&fEliminarNada);
                m_insertar(m,c,v);
            }
            else{
                nueva_entrada = crear_entrada(c, v);
                l_insertar(listaActual, l_primera(listaActual),nueva_entrada);
                m->cantidad_elementos++;
                printf("EEEEE\n");
            }
        }
    }
    printf ("END Insertar\n");
    return valor_result;
}


//CONSULTAR PORQUE NO DEBE SER STATIC
void fEliminarContenedor(void * entrada){
    printf("Inicio de Eliminar Contenedor\n");
    tEntrada entrada_a_eliminar = (tEntrada) entrada;
    free(entrada_a_eliminar);
}

//----------------------------------------------------------------------------------
/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void*), void (*fEliminarV)(void*)){
    printf("Inicio de elimnar de mapeo\n");

    int indice = m->hash_code(c) % (m->longitud_tabla);

    tLista lista_actual = m->tabla_hash[indice];

    printf("Obtuve la lista\n");
    printf("La longitud de la lista es %i \n", l_longitud(lista_actual));

    tPosicion posicion = buscar_Elemento(lista_actual,c,m);
    printf("Obtuve la posicion en la que esta la entrada\n");

    tEntrada entrada;
    if(posicion!=NULL){
        entrada = (tEntrada) l_recuperar(lista_actual, posicion);
        printf("Recupere la entrada de la posicion\n");

        printf("su clave es %s\n", (char*)entrada->clave);
        printf("su valor es %s\n", (char*)entrada->valor);

        fEliminarC(entrada->clave);
        fEliminarV(entrada->valor);

        entrada->clave = NULL;
        entrada->valor = NULL;

        l_eliminar(lista_actual,posicion, fEliminarContenedor);
        m->cantidad_elementos--;
    }
}



/*
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
*/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

    printf("Inicio de destruir MAP\n");
    tLista lista_actual = NULL;

    fEliminarCGlobal = fEliminarC;
    fEliminarVGlobal = fEliminarV;

    int longitud = (*m)->longitud_tabla;
    printf("La longitud del mapeo es %i \n",longitud);

    printf("AAAAAAAAAAAAAAAAAAAAAA\n");

    for(int i=0; i< longitud;i++){
        printf("BBBBBBBBBBBBBBB\n");
        lista_actual = (*m)->tabla_hash[i];
        printf("La longitud de la lista actual es %i \n", l_longitud(lista_actual));
        printf("CCCCCCCCCCCCCCCC\n");

        l_destruir(&lista_actual, &fEliminarEntrada);
        printf("DDDDDDDDDDDDDDDDD\n");
    }
    printf("Sali del for de destruir mapeo \n");
    free((*m)->tabla_hash);
    free(*m);

}


/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/

tValor m_recuperar(tMapeo m , tClave c){
    int indice = m->hash_code(c) % (m->longitud_tabla);
    printf("Indice en RECUPERAR %i \n", indice);
    tLista lista_actual = m->tabla_hash[indice];
    tValor valor = NULL;
    tPosicion posicion;
    tEntrada result;

    printf("La palabra en RECUPERAAAAAAAAAAAR es %s \n", (char*)c);

    if(l_longitud(lista_actual)!=0){
        posicion = buscar_Elemento(lista_actual,c,m);
        if(posicion!=NULL){
            result = ((tEntrada) l_recuperar(lista_actual, buscar_Elemento(lista_actual,c,m)));// no convendria que buscar_elem, devuelva una tEntrada?
            valor = result->valor;
        }
    }
    return valor;
}
