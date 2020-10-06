#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#define LISTA_VACIA 99

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

}
