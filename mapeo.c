#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"
#include "lista.h"

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MAXIMO_FACTOR_DE_CARGA 0.75
#define FACTOR_DE_CARGA(cantidad_elementos,buckets)   ((cantidad_elementos+1)/buckets)

typedef enum { false, true } bool;

void (*fEliminarCGlobal)(void *);
void (*fEliminarVGlobal)(void *);

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

static tPosicion buscar_elemento(tLista lista, tClave c, tMapeo m){
    tPosicion pos;
    tPosicion pos_fin;
    tClave clave_recuperada;
    bool encontre = false;

    int size = l_longitud(lista);
    if(size != 0){
        pos = l_primera(lista);
        pos_fin = l_fin(lista);
        while(pos != pos_fin && !encontre){
            clave_recuperada = ((tEntrada) l_recuperar(lista, pos))->clave;

            if(m->comparador(clave_recuperada,c) != 0){
                pos = l_siguiente(lista, pos);
            }
            else{
                encontre = true;
            }
        }
    }
    if(!encontre){
        pos = NULL;
    }
    return pos;
}

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

static void rehash(tMapeo mapeo, void (*fEliminar)(tElemento)){
    int nueva_dimension = proximo_primo(mapeo->longitud_tabla*2+1);
    int tamano_anterior = mapeo->longitud_tabla;
    int indice;
    tLista * arreglo_anterior = mapeo->tabla_hash;
    tLista lista_anterior;
    tPosicion pos_original;
    tPosicion pos_fin = NULL;
    tEntrada entrada;

    mapeo->tabla_hash = malloc(sizeof (tLista) * nueva_dimension);

    //Inicializamos el nuevo arreglo
    for(int i = 0; i<nueva_dimension; i++){
        tLista nueva_lista = NULL;
        crear_lista(&nueva_lista);
        mapeo->tabla_hash[i] = nueva_lista;
    }
    mapeo->longitud_tabla = nueva_dimension;

   for(int i = 0; i< tamano_anterior; i++){
       lista_anterior = arreglo_anterior[i];
       //Por cada bucket del arreglo anterior se envia toda la información a su nuevo hash.
       if(l_longitud(lista_anterior) > 0){
            pos_original = l_primera(lista_anterior);
            pos_fin = l_fin(lista_anterior);

           while(pos_original != pos_fin){
               entrada = l_recuperar(lista_anterior,pos_original);
               indice = mapeo->hash_code(entrada->clave)% (mapeo->longitud_tabla);
               l_insertar(mapeo->tabla_hash[indice],l_primera(mapeo->tabla_hash[indice]),entrada);
               pos_original = l_siguiente(lista_anterior,pos_original);
           }
       }
       l_destruir(&lista_anterior,fEliminar);
   }
}

static void fEliminarNada(){

}

tValor m_insertar(tMapeo m, tClave c, tValor v){
    int indice = (m->hash_code(c)) % (m->longitud_tabla);
    tPosicion pos_aux = NULL;
    tEntrada entrada_aux = NULL;
    //lista_aux será un puntero a lo que sería tablaHash[0]
    tLista * lista_aux = (m->tabla_hash);
    int factor_actual = FACTOR_DE_CARGA(m->cantidad_elementos, m->longitud_tabla);
    tEntrada nueva_entrada;
    tValor valor_a_retornar = NULL;
    //Con aritmetica de punteros, se ubica el puntero en la lista que indica el indice.
    tLista lista_actual = *(lista_aux+indice);
    if(l_longitud(lista_actual) == 0){
        if(factor_actual>MAXIMO_FACTOR_DE_CARGA){
            rehash(m,&fEliminarNada);
            m_insertar(m, c, v);
        }
        else{
            nueva_entrada = crear_entrada(c, v);
            l_insertar(lista_actual, l_primera(lista_actual),nueva_entrada);
            m->cantidad_elementos++;
        }
    }else{
        pos_aux = buscar_elemento(lista_actual, c, m);
        if(pos_aux != NULL){
            entrada_aux = l_recuperar(lista_actual, pos_aux);
            valor_a_retornar = entrada_aux->valor;
            entrada_aux->valor = v;
        }else {
            if(factor_actual>MAXIMO_FACTOR_DE_CARGA){
                rehash(m,&fEliminarNada);
                m_insertar(m,c,v);
            }
            else{
                nueva_entrada = crear_entrada(c, v);
                l_insertar(lista_actual, l_primera(lista_actual),nueva_entrada);
                m->cantidad_elementos++;
            }
        }
    }
    return valor_a_retornar;
}


static void fEliminarContenedor(void * entrada){
    tEntrada entrada_a_eliminar = (tEntrada) entrada;
    free(entrada_a_eliminar);
    entrada = NULL;
}


void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void*), void (*fEliminarV)(void*)){
    int indice = m->hash_code(c) % (m->longitud_tabla);
    tLista lista_actual = m->tabla_hash[indice];
    tPosicion posicion = buscar_elemento(lista_actual,c,m);
    tEntrada entrada;
    if(posicion!=NULL){
        entrada = (tEntrada) l_recuperar(lista_actual, posicion);
        fEliminarC(entrada->clave);
        fEliminarV(entrada->valor);
        entrada->clave = NULL;
        entrada->valor = NULL;
        l_eliminar(lista_actual,posicion, fEliminarContenedor);
        m->cantidad_elementos--;
    }
}

/**
A partir de los punteros globales de eliminación de clave y valor
previamente asignados se elimina una entrada.
**/
static void fEliminarEntrada (tElemento elemento){
    tEntrada entrada = (tEntrada) elemento;
    fEliminarCGlobal(entrada->clave);
    fEliminarVGlobal(entrada->valor);
    free(entrada);
    elemento = NULL;
}

void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tLista lista_actual = NULL;

    fEliminarCGlobal = fEliminarC;
    fEliminarVGlobal = fEliminarV;

    int longitud = (*m)->longitud_tabla;
    for(int i=0; i< longitud;i++){
        lista_actual = (*m)->tabla_hash[i];
        l_destruir(&lista_actual, &fEliminarEntrada);
    }
    free((*m)->tabla_hash);
    (*m)->tabla_hash = NULL;
    free(*m);
    (*m) = NULL;
}


tValor m_recuperar(tMapeo m , tClave c){
    int indice = m->hash_code(c) % (m->longitud_tabla);
    tLista lista_actual = m->tabla_hash[indice];
    tValor valor = NULL;
    tPosicion pos;
    tEntrada entrada;

    if(l_longitud(lista_actual)!=0){
        pos = buscar_elemento(lista_actual,c,m);
        if(pos!=NULL){
            entrada = ((tEntrada) l_recuperar(lista_actual, buscar_elemento(lista_actual,c,m)));
            valor = entrada->valor;
        }
    }
    return valor;
}
