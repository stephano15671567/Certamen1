#include "cell.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stddef.h>

#define TAMANO_INICIAL_CONEXION 10

// Tablas globales
Conexion **tabla_conexiones = NULL;
int num_conexiones = 0;
Celda **tabla_celdas = NULL;
int num_celdas = 0;
Automata **tabla_automatas = NULL;
int num_automatas = 0;

// Variables globales para las tasas
double beta = 0.3;   // Tasa de infección (S->E)
double sigma = 0.1;  // Tasa de exposición (E->I)
double gamma_ = 0.05; // Tasa de recuperación (I->R)
double mu = 0.02;     // Tasa de mortalidad (I->Removidos)
double delta = 0.01;  // Tasa de mortalidad para modelos como SEIRD
double rho = 0.05;    // Tasa de pérdida de inmunidad (R->S para SEIRV)
int permitir_movimiento = 1; // Por defecto, se permite el movimiento

// Crear un caso con coordenadas y estados iniciales
Caso *crear_caso(int x, int y, int S_inicial, int E_inicial, int I_inicial, int R_inicial, int D_inicial, int V_inicial)
{
    Caso *nuevo_caso = (Caso *)malloc(sizeof(Caso));
    if (nuevo_caso == NULL) {
        perror("Error al asignar memoria para nuevo_caso");
        exit(EXIT_FAILURE);
    }
    nuevo_caso->x = x;
    nuevo_caso->y = y;
    nuevo_caso->S = S_inicial;
    nuevo_caso->E = E_inicial;
    nuevo_caso->I = I_inicial;
    nuevo_caso->R = R_inicial;
    nuevo_caso->D = D_inicial;  // Inicializar Fallecidos
    nuevo_caso->V = V_inicial;  // Inicializar Vacunados
    nuevo_caso->vecinos = NULL;
    nuevo_caso->num_vecinos = 0;
    return nuevo_caso;
}

// Crear una celda con dimensiones y estados iniciales para los casos
Celda *crear_celda(char *nombre, int altura, int anchura, int S_inicial, int E_inicial, int I_inicial, int R_inicial, int D_inicial, int V_inicial)
{
    Celda *nueva_celda = (Celda *)malloc(sizeof(Celda));
    if (nueva_celda == NULL) {
        perror("Error al asignar memoria para nueva_celda");
        exit(EXIT_FAILURE);
    }
    nueva_celda->nombre = strdup(nombre);
    nueva_celda->altura = altura;
    nueva_celda->anchura = anchura;

    // Asignar memoria para los casos
    nueva_celda->casos = (Caso **)malloc(altura * anchura * sizeof(Caso *));
    if (nueva_celda->casos == NULL) {
        perror("Error al asignar memoria para casos en crear_celda");
        exit(EXIT_FAILURE);
    }

    // Inicializar cada caso con las coordenadas y los estados iniciales
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < anchura; j++)
        {
            nueva_celda->casos[i * anchura + j] = crear_caso(i, j, S_inicial, E_inicial, I_inicial, R_inicial, D_inicial, V_inicial);
        }
    }

    // Asignar vecinos a los casos
    asignar_vecinos(nueva_celda);

    // Agregar la celda a la tabla global de celdas
    agregar_celda(nueva_celda);

    return nueva_celda;
}

// Asignar vecinos a cada caso dentro de una celda (vecindad de Moore)
void asignar_vecinos(Celda *celda)
{
    for (int i = 0; i < celda->altura; i++)
    {
        for (int j = 0; j < celda->anchura; j++)
        {
            Caso *caso_actual = celda->casos[i * celda->anchura + j];
            caso_actual->vecinos = (Caso **)malloc(8 * sizeof(Caso *));
            caso_actual->num_vecinos = 0;

            // Vecindad de Moore
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (dx == 0 && dy == 0)
                        continue; // Saltar la celda actual

                    int nx = i + dx;
                    int ny = j + dy;

                    // Verificar si las coordenadas están dentro de la celda
                    if (nx >= 0 && nx < celda->altura && ny >= 0 && ny < celda->anchura)
                    {
                        Caso *caso_vecino = celda->casos[nx * celda->anchura + ny];
                        caso_actual->vecinos[caso_actual->num_vecinos++] = caso_vecino;
                    }
                }
            }
        }
    }
}

// Agregar una celda a la tabla global
void agregar_celda(Celda *nueva_celda)
{
    num_celdas++;
    tabla_celdas = (Celda **)realloc(tabla_celdas, num_celdas * sizeof(Celda *));
    if (tabla_celdas == NULL) {
        perror("Error al asignar memoria para tabla_celdas");
        exit(EXIT_FAILURE);
    }
    tabla_celdas[num_celdas - 1] = nueva_celda;
}

// Crear un autómata
Automata *crear_automata(char *nombre)
{
    Automata *nuevo_automata = (Automata *)malloc(sizeof(Automata));
    if (nuevo_automata == NULL) {
        perror("Error al asignar memoria para nuevo_automata");
        exit(EXIT_FAILURE);
    }
    nuevo_automata->nombre = strdup(nombre);
    nuevo_automata->celdas = NULL;
    nuevo_automata->num_celdas = 0;

    // Agregar el autómata a la tabla global
    agregar_automata(nuevo_automata);

    return nuevo_automata;
}

// Agregar una celda a un autómata
void agregar_celda_a_automata(Automata *automata, Celda *nueva_celda)
{
    automata->num_celdas++;
    automata->celdas = (Celda **)realloc(automata->celdas, automata->num_celdas * sizeof(Celda *));
    if (automata->celdas == NULL) {
        perror("Error al asignar memoria para celdas en agregar_celda_a_automata");
        exit(EXIT_FAILURE);
    }
    automata->celdas[automata->num_celdas - 1] = nueva_celda;
}

// Agregar un autómata a la tabla global
void agregar_automata(Automata *nuevo_automata)
{
    num_automatas++;
    tabla_automatas = (Automata **)realloc(tabla_automatas, num_automatas * sizeof(Automata *));
    if (tabla_automatas == NULL) {
        perror("Error al asignar memoria para tabla_automatas");
        exit(EXIT_FAILURE);
    }
    tabla_automatas[num_automatas - 1] = nuevo_automata;
}

// Inicializar la tabla de conexiones
void inicializar_tabla_conexiones()
{
    if (tabla_conexiones == NULL)
    {
        tabla_conexiones = (Conexion **)malloc(TAMANO_INICIAL_CONEXION * sizeof(Conexion *));
        if (tabla_conexiones == NULL) {
            perror("Error al asignar memoria para tabla_conexiones");
            exit(EXIT_FAILURE);
        }
        num_conexiones = 0;
    }
}

// Conectar celdas entre autómatas
void conectar_celdas_ac(char *nombre_automata1, char *nombre_celda1, int x1, int y1,
                        char *nombre_automata2, char *nombre_celda2, int x2, int y2)
{
    Automata *automata1 = encontrar_automata(nombre_automata1);
    Automata *automata2 = encontrar_automata(nombre_automata2);

    if (automata1 == NULL || automata2 == NULL)
    {
        printf("Error: Uno o ambos autómatas no fueron encontrados.\n");
        return;
    }

    Celda *celda1 = encontrar_celda_en_automata(automata1, nombre_celda1);
    Celda *celda2 = encontrar_celda_en_automata(automata2, nombre_celda2);

    if (celda1 == NULL || celda2 == NULL)
    {
        printf("Error: Una o ambas celdas no fueron encontradas.\n");
        return;
    }

    conectar_celdas(celda1->nombre, x1, y1, celda2->nombre, x2, y2);
}


// Conectar dos celdas y agregar los casos como vecinos mutuos
void conectar_celdas(char *nombre_celda1, int x1, int y1, char *nombre_celda2, int x2, int y2)
{
    inicializar_tabla_conexiones();

    Celda *celda1 = encontrar_celda(nombre_celda1);
    Celda *celda2 = encontrar_celda(nombre_celda2);

    if (celda1 == NULL || celda2 == NULL)
    {
        printf("Error: Una o ambas celdas no fueron encontradas.\n");
        return;
    }

    Caso *caso1 = celda1->casos[x1 * celda1->anchura + y1];
    Caso *caso2 = celda2->casos[x2 * celda2->anchura + y2];

    if (caso1 == NULL || caso2 == NULL)
    {
        printf("Error: Uno o ambos casos no pudieron ser encontrados.\n");
        return;
    }

    // Agregar los casos como vecinos mutuos
    caso1->vecinos = (Caso **)realloc(caso1->vecinos, (caso1->num_vecinos + 1) * sizeof(Caso *));
    caso1->vecinos[caso1->num_vecinos++] = caso2;

    caso2->vecinos = (Caso **)realloc(caso2->vecinos, (caso2->num_vecinos + 1) * sizeof(Caso *));
    caso2->vecinos[caso2->num_vecinos++] = caso1;

    // Registrar la conexión en la tabla de conexiones
    Conexion *nueva_conexion = (Conexion *)malloc(sizeof(Conexion));
    if (nueva_conexion == NULL) {
        perror("Error al asignar memoria para nueva_conexion");
        exit(EXIT_FAILURE);
    }
    nueva_conexion->celda1 = celda1;
    nueva_conexion->caso1 = caso1;
    nueva_conexion->celda2 = celda2;
    nueva_conexion->caso2 = caso2;

    tabla_conexiones[num_conexiones] = nueva_conexion;
    num_conexiones++;

    printf("Casos (%d, %d) de %s y (%d, %d) de %s están ahora conectados.\n",
           x1, y1, celda1->nombre, x2, y2, celda2->nombre);
}

// Encontrar una celda por nombre
Celda *encontrar_celda(char *nombre)
{
    for (int i = 0; i < num_celdas; i++)
    {
        if (strcmp(tabla_celdas[i]->nombre, nombre) == 0)
        {
            return tabla_celdas[i];
        }
    }
    return NULL;
}

// Encontrar un autómata por nombre
Automata *encontrar_automata(char *nombre)
{
    for (int i = 0; i < num_automatas; i++)
    {
        if (strcmp(tabla_automatas[i]->nombre, nombre) == 0)
        {
            return tabla_automatas[i];
        }
    }
    return NULL;
}

// Encontrar una celda dentro de un autómata
Celda *encontrar_celda_en_automata(Automata *automata, char *nombre_celda)
{
    for (int i = 0; i < automata->num_celdas; i++)
    {
        if (strcmp(automata->celdas[i]->nombre, nombre_celda) == 0)
        {
            return automata->celdas[i];
        }
    }
    return NULL;
}

// Imprimir todas las celdas y sus casos
void imprimir_celdas()
{
    printf("Lista de celdas:\n");
    for (int i = 0; i < num_celdas; i++)
    {
        printf("Celda %s: %d x %d\n", tabla_celdas[i]->nombre, tabla_celdas[i]->altura, tabla_celdas[i]->anchura);
        for (int j = 0; j < tabla_celdas[i]->altura; j++)
        {
            for (int k = 0; k < tabla_celdas[i]->anchura; k++)
            {
                Caso *caso_actual = tabla_celdas[i]->casos[j * tabla_celdas[i]->anchura + k];
                printf("  Caso (%d, %d) -> S=%.2f, E=%.2f, I=%.2f, R=%.2f, D=%.2f, V=%.2f\n",
                       caso_actual->x, caso_actual->y,
                       caso_actual->S, caso_actual->E, caso_actual->I, caso_actual->R,
                       caso_actual->D, caso_actual->V);
            }
        }
        imprimir_celda(tabla_celdas[i]);
    }
}

#include <stddef.h> // Necesario para offsetof

void imprimir_celda(Celda *celda) {
    printf("Celda %s:\n", celda->nombre);

    // Nombres de los compartimentos y offsets en la estructura Caso
    const char *compartimentos[] = {"S", "E", "I", "R", "D", "V"};
    size_t offsets[] = {
        offsetof(Caso, S),
        offsetof(Caso, E),
        offsetof(Caso, I),
        offsetof(Caso, R),
        offsetof(Caso, D),
        offsetof(Caso, V)
    };
    int num_compartimentos = sizeof(compartimentos) / sizeof(compartimentos[0]);

    for (int j = 0; j < celda->altura; j++) {
        // Imprimir la parte superior de las casillas
        for (int k = 0; k < celda->anchura; k++)
            printf("+-------+  ");
        printf("\n");

        // Imprimir las coordenadas de las casillas
        for (int k = 0; k < celda->anchura; k++) {
            Caso *caso_actual = celda->casos[j * celda->anchura + k];
            printf("| (%d,%d) |  ", caso_actual->x, caso_actual->y);
        }
        printf("\n");

        // Imprimir la parte inferior de las casillas
        for (int k = 0; k < celda->anchura; k++)
            printf("+-------+  ");
        printf("\n");

        // Imprimir la información de cada compartimento
        for (int c = 0; c < num_compartimentos; c++) {
            for (int k = 0; k < celda->anchura; k++) {
                Caso *caso_actual = celda->casos[j * celda->anchura + k];
                double valor = *(double *)((char *)caso_actual + offsets[c]);
                printf("| %s: %3d |  ", compartimentos[c], (int)round(valor));
            }
            printf("\n");
        }

        // Separador inferior de las casillas
        for (int k = 0; k < celda->anchura; k++)
            printf("+-------+  ");
        printf("\n\n");
    }
}



// Imprimir el estado de un autómata específico
void imprimir_estado_automata(char *nombre_automata)
{
    Automata *automata = encontrar_automata(nombre_automata);
    if (automata == NULL)
    {
        printf("Autómata %s no encontrado.\n", nombre_automata);
        return;
    }

    printf("Estado del Autómata %s:\n", automata->nombre);
    for (int i = 0; i < automata->num_celdas; i++)
    {
        imprimir_celda(automata->celdas[i]);
    }
}

// Imprimir el estado de todas las celdas y autómatas
void imprimir_todos_estados()
{
    printf("Estado de todos los autómatas y celdas:\n");
    for (int i = 0; i < num_automatas; i++)
    {
        imprimir_estado_automata(tabla_automatas[i]->nombre);
    }
}

// Imprimir todas las conexiones entre celdas
void imprimir_conexiones()
{
    printf("Lista de conexiones:\n");
    for (int i = 0; i < num_conexiones; i++)
    {
        Conexion *conn = tabla_conexiones[i];
        printf("  Conexión entre Celda %s (%d, %d) y Celda %s (%d, %d)\n",
               conn->celda1->nombre, conn->caso1->x, conn->caso1->y,
               conn->celda2->nombre, conn->caso2->x, conn->caso2->y);
    }
}

// Imprimir el estado de una celda específica
void imprimir_estado_celda(char *nombre_celda)
{
    Celda *celda = encontrar_celda(nombre_celda);
    if (celda == NULL)
    {
        printf("Celda %s no encontrada.\n", nombre_celda);
        return;
    }

    printf("Estado de la Celda %s:\n", celda->nombre);
    for (int j = 0; j < celda->altura; j++)
    {
        for (int k = 0; k < celda->anchura; k++)
        {
            Caso *caso_actual = celda->casos[j * celda->anchura + k];
            printf("  Caso (%d, %d) -> S=%.2f, E=%.2f, I=%.2f, R=%.2f, D=%.2f, V=%.2f\n",
                   caso_actual->x, caso_actual->y,
                   caso_actual->S, caso_actual->E, caso_actual->I, caso_actual->R,
                   caso_actual->D, caso_actual->V);
        }
    }
}

// Establecer nuevas tasas
void establecer_tasas(double nueva_beta, double nuevo_sigma, double nuevo_gamma, double nuevo_mu, double nuevo_delta, double nuevo_rho)
{
    beta = nueva_beta;
    sigma = nuevo_sigma;
    gamma_ = nuevo_gamma;
    mu = nuevo_mu;
    delta = nuevo_delta;
    rho = nuevo_rho;
    printf("Tasas actualizadas: beta=%.2f, sigma=%.2f, gamma=%.2f, mu=%.2f, delta=%.2f, rho=%.2f\n",
           beta, sigma, gamma_, mu, delta, rho);
}

 

void visualizar_celdas_con_todos_compartimentos() {
    printf("\nEstado actual de las celdas con casillas mostrando todos los compartimentos:\n");

    // Nombres de los compartimentos y offsets de sus valores en la estructura
    const char *compartimentos[] = {"S", "E", "I", "R", "D", "V"};
    size_t offsets[] = {offsetof(Caso, S), offsetof(Caso, E), offsetof(Caso, I),
                        offsetof(Caso, R), offsetof(Caso, D), offsetof(Caso, V)};

    // Recorrer cada celda
    for (int i = 0; i < num_celdas; i++) {
        Celda *celda = tabla_celdas[i];
        printf("Celda %s:\n", celda->nombre);

        // Recorrer cada fila
        for (int j = 0; j < celda->altura; j++) {
            // Imprimir la parte superior de las casillas
            for (int k = 0; k < celda->anchura; k++)
                printf("+-------+  ");
            printf("\n");

            // Imprimir las coordenadas de las casillas
            for (int k = 0; k < celda->anchura; k++) {
                Caso *caso_actual = celda->casos[j * celda->anchura + k];
                printf("| (%d,%d) |  ", caso_actual->x, caso_actual->y);
            }
            printf("\n");

            // Imprimir la parte inferior de las casillas
            for (int k = 0; k < celda->anchura; k++)
                printf("+-------+  ");
            printf("\n");

            // Imprimir la información de cada compartimento
            for (int c = 0; c < 6; c++) { // 6 compartimentos
                for (int k = 0; k < celda->anchura; k++) {
                    Caso *caso_actual = celda->casos[j * celda->anchura + k];
                    double valor_d = *(double *)((char *)caso_actual + offsets[c]);
                    int valor = (int)round(valor_d);
                    printf("| %s: %3d |  ", compartimentos[c], valor);
                }
                printf("\n");
            }

            // Separador inferior de las casillas
            for (int k = 0; k < celda->anchura; k++)
                printf("+-------+  ");
            printf("\n\n");
        }
    }
}

void simular(int pasos, int permitir_movimiento) {
    for (int paso = 0; paso < pasos; paso++) {
        printf("Paso %d:\n", paso + 1);
        for (int i = 0; i < num_celdas; i++) {
            Celda *celda = tabla_celdas[i];
            int num_casos = celda->altura * celda->anchura;
            Caso **nuevos_casos = (Caso **)malloc(num_casos * sizeof(Caso *));
            Caso **mapeo = (Caso **)malloc(num_casos * sizeof(Caso *));
            if (!nuevos_casos || !mapeo) {
                perror("Error al asignar memoria");
                exit(EXIT_FAILURE);
            }
            for (int idx = 0; idx < num_casos; idx++) {
                Caso *nuevo_caso = (Caso *)malloc(sizeof(Caso));
                if (!nuevo_caso) {
                    perror("Error al asignar memoria para nuevo_caso");
                    exit(EXIT_FAILURE);
                }
                *nuevo_caso = *(celda->casos[idx]); // Copiar datos
                nuevo_caso->vecinos = NULL;
                nuevos_casos[idx] = nuevo_caso;
                mapeo[idx] = nuevo_caso;
            }
            for (int idx = 0; idx < num_casos; idx++) {
                Caso *nuevo_caso = nuevos_casos[idx];
                Caso *caso_actual = celda->casos[idx];
                int num_vecinos = caso_actual->num_vecinos;
                nuevo_caso->vecinos = (Caso **)malloc(num_vecinos * sizeof(Caso *));
                if (!nuevo_caso->vecinos) {
                    perror("Error al asignar memoria para vecinos");
                    exit(EXIT_FAILURE);
                }
                for (int n = 0; n < num_vecinos; n++) {
                    Caso *vecino_viejo = caso_actual->vecinos[n];
                    int indice_vecino = -1;
                    for (int j = 0; j < num_casos; j++) {
                        if (celda->casos[j] == vecino_viejo) {
                            indice_vecino = j;
                            break;
                        }
                    }
                    nuevo_caso->vecinos[n] = (indice_vecino != -1) ? mapeo[indice_vecino] : vecino_viejo;
                }
            }
            for (int idx = 0; idx < num_casos; idx++) {
                Caso *nuevo_caso = nuevos_casos[idx];
                double N = nuevo_caso->S + nuevo_caso->E + nuevo_caso->I + nuevo_caso->R + nuevo_caso->D + nuevo_caso->V;
                if (N == 0) N = 1;
                double nuevos_expuestos = beta * nuevo_caso->S * nuevo_caso->I / N;
                double nuevos_infectados = sigma * nuevo_caso->E;
                double nuevos_recuperados = gamma_ * nuevo_caso->I;
                double nuevas_muertes = delta * nuevo_caso->I;
                double nuevos_vacunados = rho * nuevo_caso->R;
                nuevo_caso->S -= nuevos_expuestos;
                nuevo_caso->E += nuevos_expuestos - nuevos_infectados;
                nuevo_caso->I += nuevos_infectados - nuevos_recuperados - nuevas_muertes;
                nuevo_caso->R += nuevos_recuperados;
                nuevo_caso->D += nuevas_muertes;
                nuevo_caso->V += nuevos_vacunados;
                if (nuevo_caso->S < 0) nuevo_caso->S = 0;
                if (nuevo_caso->E < 0) nuevo_caso->E = 0;
                if (nuevo_caso->I < 0) nuevo_caso->I = 0;
                if (nuevo_caso->R < 0) nuevo_caso->R = 0;
                if (nuevo_caso->D < 0) nuevo_caso->D = 0;
                if (nuevo_caso->V < 0) nuevo_caso->V = 0;
                if (permitir_movimiento) {
                    double infectados_mov = nuevo_caso->I * 0.01;
                    nuevo_caso->I -= infectados_mov;
                    int num_vecinos = nuevo_caso->num_vecinos;
                    if (num_vecinos > 0) {
                        double infectados_por_vecino = infectados_mov / num_vecinos;
                        for (int n = 0; n < num_vecinos; n++) {
                            Caso *vecino = nuevo_caso->vecinos[n];
                            vecino->I += infectados_por_vecino;
                            printf("Movidos %.2f infectados de Celda %s (%d,%d) al vecino (%d,%d)\n",
                                   infectados_por_vecino, celda->nombre, nuevo_caso->x, nuevo_caso->y,
                                   vecino->x, vecino->y);
                        }
                    }
                }
                nuevo_caso->S = (int)round(nuevo_caso->S);
                nuevo_caso->E = (int)round(nuevo_caso->E);
                nuevo_caso->I = (int)round(nuevo_caso->I);
                nuevo_caso->R = (int)round(nuevo_caso->R);
                nuevo_caso->D = (int)round(nuevo_caso->D);
                nuevo_caso->V = (int)round(nuevo_caso->V);
                printf("Celda %s (%d,%d): S=%d, E=%d, I=%d, R=%d, D=%d, V=%d\n",
                       celda->nombre, nuevo_caso->x, nuevo_caso->y,
                       (int)nuevo_caso->S, (int)nuevo_caso->E, (int)nuevo_caso->I,
                       (int)nuevo_caso->R, (int)nuevo_caso->D, (int)nuevo_caso->V);
            }
            for (int idx = 0; idx < num_casos; idx++) {
                free(celda->casos[idx]);
                celda->casos[idx] = nuevos_casos[idx];
            }
            free(nuevos_casos);
            free(mapeo);
        }
        visualizar_celdas_con_todos_compartimentos();
    }
}
