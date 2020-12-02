#include "abb.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

typedef struct nodo_abb {
    struct nodo_abb *izq;
    struct nodo_abb *der;
    const char *clave;
    void *dato;
} nodo_abb_t;

struct abb {
    nodo_abb_t* raiz;
    size_t cant;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir;
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* abb = malloc(sizeof(abb_t));

    if (!abb) return NULL;

    abb->cant = 0;
    abb->cmp = cmp;
    abb->destruir = destruir_dato;

    return abb;
}

nodo_abb_t* nodo_abb_crear(const char *clave, void *dato) {
    nodo_abb_t* nodo_abb = malloc(sizeof(nodo_abb_t));

    if (!nodo_abb) return NULL;

    nodo_abb->clave = clave;
    nodo_abb->dato = dato;
    nodo_abb->der = NULL;
    nodo_abb->izq = NULL;

    return nodo_abb;

}

void _abb_guardar(abb_comparar_clave_t cmp ,nodo_abb_t *nodo_act, nodo_abb_t *nodo_nuevo) {
    if (!nodo_act) return;

    if (cmp(nodo_act->clave, nodo_nuevo->clave) > 0) {
        if (nodo_act->der) _abb_guardar(cmp, nodo_act->der, nodo_nuevo);
        else nodo_act->der = nodo_nuevo;
    }
    else {
        if (nodo_act->izq) _abb_guardar(cmp, nodo_act->izq, nodo_nuevo);
        else nodo_act->izq = nodo_nuevo;
    }
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    // SI LO PODEMOS OBTENER

    const char* copia_clave = strdup(clave);
    
    if (!copia_clave) return false;

    nodo_abb_t* nodo_nuevo = nodo_abb_crear(copia_clave, dato);

    if (!arbol->raiz) arbol->raiz = nodo_nuevo;
    else _abb_guardar(arbol->cmp , arbol->raiz, nodo_nuevo);
    
    arbol->cant++;
    return true;
}

void* _abb_obtener (const nodo_abb_t *nodo_act, const char *clave, abb_comparar_clave_t cmp) {
    if (!nodo_act) return NULL;

    if (cmp(nodo_act->clave, clave) == 0) return nodo_act->dato;

    if (cmp(nodo_act->clave, clave) > 0) {
        return _abb_obtener(nodo_act->der, clave, cmp);
    }
    else return _abb_obtener(nodo_act->izq, clave, cmp);
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    return _abb_obtener(arbol->raiz, clave, arbol->cmp);
}

size_t abb_cantidad(abb_t *arbol) {
    return arbol->cant;
}

bool _abb_pertenece (const nodo_abb_t *nodo_act, const char *clave, abb_comparar_clave_t cmp) {
    if (!nodo_act) return false;

    if (cmp(nodo_act->clave, clave) == 0) return true;

    if (cmp(nodo_act->clave, clave) > 0) {
        return _abb_pertenece(nodo_act->der, clave, cmp);
    }
    else return _abb_pertenece(nodo_act->izq, clave, cmp);
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    return _abb_pertenece(arbol->raiz, clave, arbol->cmp);
}

void* _abb_borrar(nodo_abb_t *nodo_padre, nodo_abb_t *nodo_hijo, const char *clave, abb_comparar_clave_t cmp) {
    if (!nodo_hijo) return NULL;

    if (cmp(nodo_hijo->clave, clave) == 0) {
        if (!nodo_hijo->izq && !nodo_hijo->der) {
            if (nodo_padre && nodo_padre->izq &&  cmp(nodo_padre->izq->clave, nodo_hijo->clave) == 0) nodo_padre->izq = NULL;
            else if (nodo_padre && nodo_padre->der && cmp(nodo_padre->der->clave, nodo_hijo->clave) == 0) nodo_padre->der = NULL;
            void* valor_hijo = nodo_hijo->dato;
            free(nodo_hijo);
            return valor_hijo;
        }

        if ((!nodo_hijo->izq && nodo_hijo->der) || (!nodo_hijo->der && nodo_hijo->izq)) {
            if (nodo_hijo->izq) {
                if (nodo_padre && nodo_padre->izq && cmp(nodo_padre->izq->clave, nodo_hijo->clave) == 0) nodo_padre->izq = nodo_hijo->izq;
                else if (nodo_padre && nodo_padre->der && cmp(nodo_padre->der->clave, nodo_hijo->clave) == 0) nodo_padre->der = nodo_hijo->izq;
                void* valor_hijo = nodo_hijo->dato;
                free(nodo_hijo);
                return valor_hijo;
            }
            
            if (nodo_hijo->der) {
                if (nodo_padre && nodo_padre->izq && cmp(nodo_padre->izq->clave, nodo_hijo->clave) == 0) nodo_padre->izq = nodo_hijo->der;
                else if (nodo_padre && nodo_padre->der && cmp(nodo_padre->der->clave, nodo_hijo->clave) == 0) nodo_padre->der = nodo_hijo->der;
                void* valor_hijo = nodo_hijo->dato;
                free(nodo_hijo);
                return valor_hijo;
            }
        }

        //if (nodo_hijo->izq && nodo_hijo->der)


    }
    else if (cmp(nodo_hijo->clave, clave) > 0) return _abb_borrar(nodo_hijo ,nodo_hijo->der, clave, cmp);
    else return _abb_borrar(nodo_hijo, nodo_hijo->izq, clave, cmp);
    return NULL;



}

void *abb_borrar(abb_t *arbol, const char *clave) {
    if (!abb_pertenece(arbol, clave)) return NULL;
    
    arbol->cant--;
    return _abb_borrar(NULL, arbol->raiz, clave, arbol->cmp);
}