#include "abb.h"
#include "testing.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include <stdbool.h>

static void prueba_crear_abb_vacio()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_iterar_abb_vacio()
{
    abb_t* abb = abb_crear(strcmp, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}


static void prueba_abb_insertar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "A", *valor1 = "guau";
    char *clave2 = "B", *valor2 = "miau";
    char *clave3 = "C", *valor3 = "mu";

    //Inserta 1 valor y luego lo borra
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0); 
    //Inserta otros 2 valores y no los borra (se destruyen con el abb)
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));
    
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void prueba_abb_reemplazar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir()
{
    abb_t* abb = abb_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

static void prueba_abb_borrar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave1));
    print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave2));
    print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_clave_vacia()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_valor_null()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

static void prueba_abb_iterar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    /* Inserta 3 valores */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Primer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Segundo valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Tercer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    abb_iter_in_avanzar(iter);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    /* Vuelve a tratar de avanzar, por las dudas */
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

bool mostrar(const char* clave, void* valor, void* extra){
    if(extra && *(int*)extra == 0) return false;
    printf("%s\n", clave);
    if (extra) *(int*)extra -=1;
    return true;
}

static void pruebas_abb_iterador_interno(){

    abb_t* abb = abb_crear(strcmp, NULL);

    char* claves[] = {"A","B","C","D","E", "F"};
    char *valores[] = {"guau", "miau", "mu"};

    abb_guardar(abb, claves[4], valores[0]);
    abb_guardar(abb, claves[0], valores[0]);
    abb_guardar(abb, claves[3], valores[0]);
    abb_guardar(abb, claves[5], valores[0]);
    abb_guardar(abb, claves[2], valores[0]);
    abb_guardar(abb, claves[1], valores[0]);

    abb_in_order(abb, mostrar,NULL);
    abb_destruir(abb);
}

static void pruebas_abb_iterador_interno_con_condicion_de_corte(){

    abb_t* abb = abb_crear(strcmp, NULL);

    char* claves[] = {"A","B","C","D","E", "F"};
    char *valores[] = {"guau", "miau", "mu"};

    abb_guardar(abb, claves[4], valores[0]);
    abb_guardar(abb, claves[0], valores[0]);
    abb_guardar(abb, claves[3], valores[0]);
    abb_guardar(abb, claves[5], valores[0]);
    abb_guardar(abb, claves[2], valores[0]);
    abb_guardar(abb, claves[1], valores[0]);
    int extra = 3;
    abb_in_order(abb, mostrar,&extra);
    abb_destruir(abb);
}


static void prueba_abb_volumen(int num){
    hash_t* hash_numeros = hash_crear(NULL);
    abb_t* abb = abb_crear(strcmp, NULL);
    for(int i =0; i<num;){
        int valor = rand();
        char valor_string[10];
        sprintf(valor_string, "%d", valor);
        int dato = rand();
        char dato_string[10];
        sprintf(dato_string, "%d", dato);
        if(!hash_pertenece(hash_numeros, valor_string)){
            hash_guardar(hash_numeros, valor_string, dato_string);
            abb_guardar(abb, valor_string, dato_string);
            i++;
        }
    }
    print_test("Prueba abb la cantidad de elementos correctos", abb_cantidad(abb) == num);
    bool es_correcto = true;
    hash_iter_t* iterador_hash = hash_iter_crear(hash_numeros);
    while(!hash_iter_al_final(iterador_hash)){
        const char* clave = hash_iter_ver_actual(iterador_hash);
        es_correcto = abb_pertenece(abb,clave);
        if(!es_correcto) break;
        char* valor_hash = hash_obtener(hash_numeros, clave);
        char* valor_abb = abb_obtener(abb,clave);
        if(strcmp(valor_hash, valor_abb)!=0){
            es_correcto = false;
            break;
        }
        hash_iter_avanzar(iterador_hash);
    }
    print_test("Prueba que el abb tiene todos las claves y valores correspondientes", es_correcto);
    hash_iter_destruir(iterador_hash);

    hash_iter_t* iterador_hash_dos = hash_iter_crear(hash_numeros);
    for(int i = 0; i<20;i++){
        const char* clave = hash_iter_ver_actual(iterador_hash_dos);
        char* valor_abb = abb_borrar(abb, clave);
        char* valor_hash = hash_borrar(hash_numeros, clave);
        if(strcmp(valor_abb,valor_hash) !=0){
            es_correcto = false;
            break;
        }
        hash_iter_avanzar(iterador_hash_dos);

    }
    hash_iter_destruir(iterador_hash_dos);
    print_test("Al borrar los elementos los datos eran los correctos", es_correcto);
    print_test("Prueba abb la cantidad de elementos correctos luego de borrar 20 elementos", abb_cantidad(abb) == num-20);

    hash_iter_t* iterador_hash_tres = hash_iter_crear(hash_numeros);
    while(!hash_iter_al_final(iterador_hash_tres)){
        const char* clave = hash_iter_ver_actual(iterador_hash_tres);
        es_correcto = abb_pertenece(abb,clave);
        if(!es_correcto) break;
        char* valor_hash = hash_obtener(hash_numeros, clave);
        char* valor_abb = abb_obtener(abb,clave);
        if(strcmp(valor_hash, valor_abb)!=0){
            es_correcto = false;
            break;
        }
        hash_iter_avanzar(iterador_hash_tres);
    }
    print_test("Prueba que el abb tiene todos las claves y valores correspondientes luego de borrar 20 elementos", es_correcto);
    hash_iter_destruir(iterador_hash_tres);

    abb_destruir(abb);
    hash_destruir(hash_numeros);

}

static void prueba_abb_iterar_volumen(int num){
    hash_t* hash_numeros = hash_crear(NULL);
    abb_t* abb = abb_crear(strcmp, NULL);
    for(int i =0; i<num;){
        int valor = rand();
        char valor_string[10];
        sprintf(valor_string, "%d", valor);
        int dato = rand();
        char dato_string[10];
        sprintf(dato_string, "%d", dato);
        if(!hash_pertenece(hash_numeros, valor_string)){
            hash_guardar(hash_numeros, valor_string, dato_string);
            abb_guardar(abb, valor_string, dato_string);
            i++;
        }
    }
    abb_iter_t* iterador_abb = abb_iter_in_crear(abb);
    bool es_correcto = true;
    while(!abb_iter_in_al_final(iterador_abb)){
        const char* clave_abb = abb_iter_in_ver_actual(iterador_abb);
        if(!hash_pertenece(hash_numeros, clave_abb)){
            es_correcto = false;
            break;
        }
        hash_borrar(hash_numeros, clave_abb);
        abb_iter_in_avanzar(iterador_abb);
    }
    if(hash_cantidad(hash_numeros) != 0) es_correcto = false;
    print_test("Prueba que el iterador del abb recorrio todos los elementos del abb", es_correcto);
    print_test("Prueba abb la cantidad de elementos correctos", abb_cantidad(abb) == num);

    abb_iter_in_destruir(iterador_abb);
    abb_destruir(abb);
    hash_destruir(hash_numeros);
}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_abb_alumno()
{
    /* Ejecuta todas las pruebas unitarias. */
    printf("\nprueba prueba_crear_abb_vacio\n\n");
    prueba_crear_abb_vacio();
    printf("\nprueba prueba_iterar_abb_vacio\n\n");
    prueba_iterar_abb_vacio();
    printf("\nprueba prueba_abb_insertar\n\n");
    prueba_abb_insertar();
    printf("\nprueba prueba_abb_reemplazar\n\n");
    prueba_abb_reemplazar();
    printf("\nprueba prueba_abb_reemplazar_con_destruir\n\n");
    prueba_abb_reemplazar_con_destruir();
    printf("\nprueba prueba_abb_borrar\n\n");
    prueba_abb_borrar();
    printf("\nprueba prueba_abb_clave_vacia\n\n");
    prueba_abb_clave_vacia();
    printf("\nprueba prueba_abb_valor_null\n\n");
    prueba_abb_valor_null();
    printf("\nprueba prueba_abb_volumen\n\n");
    prueba_abb_volumen(5000);
    printf("\nprueba prueba_abb_iterar\n\n");
    prueba_abb_iterar();
    printf("\nprueba pruebas_abb_iterador_interno\n\n");
    pruebas_abb_iterador_interno();
    printf("\nprueba pruebas_abb_iterador_interno_con_condicion_de_corte\n\n");
    pruebas_abb_iterador_interno_con_condicion_de_corte();
    printf("\nprueba prueba_abb_iterar_volumen\n\n");
    prueba_abb_iterar_volumen(5000);
}
