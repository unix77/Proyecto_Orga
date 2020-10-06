#ifndef MAPEO_H_INCLUDED
#define MAPEO_H_INCLUDED

#include "lista.h"

// Macros con códigos de finalización
#define MAP_ERROR_MEMORIA           100

// Structs para la manipulación de un mapeo.
struct entrada{
    void * clave;
    void * valor;
};

struct mapeo{
    unsigned int longitud_tabla;
    unsigned int cantidad_elementos;
    int (*hash_code)(void *);
    int (*comparador)(void *, void *);
    tLista * tabla_hash;
};

typedef void * tClave;
typedef void * tValor;
typedef struct entrada * tEntrada;
typedef struct mapeo * tMapeo;

// ---------------------------------------------------------------------------------------------
// Definición de prototipos y documentación de las funciones.
// Obs.: la tabla hash debe mantener control del factor de carga en todo momento.
// Obs.: el factor de carga máximo permitido equivale al 75% de la longitud de la tabla.
// ---------------------------------------------------------------------------------------------

/**
 Inicializa un mapeo vacío, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves será computado mediante la función fHash.
 A todo efecto, la comparación de claves se realizará mediante la función fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
extern void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *));

/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no existía en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
extern tValor m_insertar(tMapeo m, tClave c, tValor v);

/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
extern void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *));

/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
extern void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *));

/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/
extern tValor m_recuperar(tMapeo m, tClave c);

#endif // MAPEO_H_INCLUDED
