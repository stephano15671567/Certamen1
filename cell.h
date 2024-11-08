#ifndef CELDA_H
#define CELDA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Para manejar strcmp y strdup

// Estructura para un "Caso"
typedef struct Caso
{
    int x, y;    // Coordenadas
    double S;    // Susceptibles
    double E;    // Expuestos
    double I;    // Infectados
    double R;    // Recuperados
    double D;    // Fallecidos
    double V;    // Vacunados
    struct Caso **vecinos; // Vecinos
    int num_vecinos;       // Número de vecinos
} Caso;

// Estructura para una "Celda"
typedef struct
{
    char *nombre;   // Nombre de la celda
    Caso **casos;   // Array de casos
    int altura;     // Altura (número de filas)
    int anchura;    // Anchura (número de columnas)
} Celda;

// Estructura para una "Conexión" entre dos celdas
typedef struct
{
    Celda *celda1;  // Primera celda
    Caso *caso1;    // Primer caso
    Celda *celda2;  // Segunda celda
    Caso *caso2;    // Segundo caso
} Conexion;

// Estructura para un "Autómata"
typedef struct
{
    char *nombre;    // Nombre del autómata
    Celda **celdas;  // Array de celdas
    int num_celdas;  // Número de celdas
} Automata;

// Tablas globales
extern Conexion **tabla_conexiones;
extern int num_conexiones;
extern Celda **tabla_celdas;
extern int num_celdas;
extern Automata **tabla_automatas;
extern int num_automatas;

// Variables globales para las tasas
extern double beta;    // Tasa de infección (S->E)
extern double sigma;   // Tasa de exposición (E->I)
extern double gamma_;  // Tasa de recuperación (I->R)
extern double mu;      // Tasa de mortalidad (I->Removidos)
extern double delta;   // Tasa de mortalidad (I->D para SEIRD)
extern double rho;     // Tasa de pérdida de inmunidad (R->S para SEIRV)
extern int permitir_movimiento; // Control de movimiento

// Declaración de funciones
Celda *crear_celda(char *nombre, int altura, int anchura, int S_inicial, int E_inicial, int I_inicial, int R_inicial, int D_inicial, int V_inicial);
Caso *crear_caso(int x, int y, int S_inicial, int E_inicial, int I_inicial, int R_inicial, int D_inicial, int V_inicial);
void asignar_vecinos(Celda *celda);
void conectar_celdas(char *nombre_celda1, int x1, int y1, char *nombre_celda2, int x2, int y2); // Agregado aquí
void conectar_celdas_ac(char *nombre_automata1, char *nombre_celda1, int x1, int y1,
                        char *nombre_automata2, char *nombre_celda2, int x2, int y2);
void agregar_celda(Celda *nueva_celda);
void agregar_celda_a_automata(Automata *automata, Celda *nueva_celda);
Automata *crear_automata(char *nombre);
void agregar_automata(Automata *nuevo_automata);

void imprimir_celda(Celda *celda);

void imprimir_celdas();
void imprimir_conexiones();
void imprimir_estado_celda(char *nombre_celda);
void imprimir_estado_automata(char *nombre_automata);
void imprimir_todos_estados();
Celda *encontrar_celda(char *nombre);
Automata *encontrar_automata(char *nombre);
Celda *encontrar_celda_en_automata(Automata *automata, char *nombre_celda);
void inicializar_tabla_conexiones();
void establecer_tasas(double nueva_beta, double nuevo_sigma, double nuevo_gamma, double nuevo_mu, double nuevo_delta, double nuevo_rho);
void simular(int pasos, int permitir_movimiento);

#endif // CELDA_H
