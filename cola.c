#include "cola.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct nodo{
    void* dato;
    struct nodo* prox;
}nodo_t;


nodo_t* crear_nodo(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = valor;
    nodo->prox = NULL;

    return nodo;
}

struct cola {
    nodo_t* prim;   
    nodo_t* ult;
};


cola_t *cola_crear(void){

    cola_t *cola = malloc(sizeof(cola_t));

    if (!cola) return NULL;

    cola->prim = NULL;
    cola->ult = NULL; 
    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
    while (cola->prim){
        void* dato = cola_desencolar(cola);
        if(destruir_dato) destruir_dato(dato);
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
    return (cola->prim  == NULL && cola->ult == NULL);
}

bool cola_encolar(cola_t *cola, void *valor){
    nodo_t* nodo = crear_nodo(valor);
    if (!nodo) return false;

    if (!cola->prim){
        cola->prim = nodo;
        cola->ult = nodo;
    }else{
        cola->ult->prox = nodo;
        cola->ult = nodo;
    }
    return true;
}

void *cola_ver_primero(const cola_t *cola){
    if (cola_esta_vacia(cola)) return NULL;
    return cola->prim->dato;

}

void *cola_desencolar(cola_t *cola){
    if (cola_esta_vacia(cola)) return NULL;
    void* devolver = cola->prim->dato;
    nodo_t* proximo = cola->prim->prox;
    free(cola->prim);
    cola->prim = proximo;
    if (!cola->prim) cola->ult = NULL;
    return devolver;
}