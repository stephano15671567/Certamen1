Certamen1
Ejecución de los Códigos
Los códigos implementados en este proyecto se ejecutan sin errores y han sido probados exhaustivamente para asegurar su correcto funcionamiento en diferentes escenarios de simulación.

Claridad y Documentación
El código está escrito de manera clara y estructurada, con comentarios que explican el propósito de las funciones y los bloques principales. Se ha seguido un estilo consistente de codificación, cumpliendo con las condiciones establecidas en las directrices de entrega.

Datos de Entrada y Salida
Los detalles sobre los datos de entrada y salida, así como su formato, están comentados dentro del código y se describen de manera más detallada en este archivo README. A continuación, se proporciona un resumen de los principales comandos, su uso y ejemplos para facilitar la ejecución de la simulación:

Comandos Principales
Crear un autómata
Comando: CREAR_AUTOMATA [nombre]
Ejemplo: CREAR_AUTOMATA auto1
Descripción: Crea un nuevo autómata con el nombre especificado.
Crear una celda dentro de un autómata
Comando: CREAR_CELDA [nombre_autómata] [nombre_celda] [alto] [ancho] [S] [E] [I] [R] [D] [V]
Ejemplo: CREAR_CELDA auto1 celda1 4 4 100 0 10 0 0 5
Descripción: Crea una celda en el autómata especificado con dimensiones y valores iniciales para los compartimientos susceptibles (S), expuestos (E), infectados (I), recuperados (R), fallecidos (D) y vacunados (V).
Conectar celdas entre autómatas
Comando: CONECTAR_AC [autómata1] [celda1] [x1] [y1] [autómata2] [celda2] [x2] [y2]
Ejemplo: CONECTAR_AC auto1 celda1 0 0 auto1 celda2 1 1
Descripción: Conecta dos celdas, permitiendo que sus casos específicos actúen como vecinos mutuos.
Establecer tasas de transición en el modelo de contagio
Comando: ESTABLECER_TASAS [beta] [sigma] [gamma] [mu] [delta] [rho]
Ejemplo: ESTABLECER_TASAS 0.3 0.1 0.05 0.02 0.01 0.05
Descripción: Configura las tasas de infección (beta), exposición (sigma), recuperación (gamma), mortalidad (mu), fallecidos (delta) y pérdida de inmunidad (rho) en el modelo.
Simulación del sistema
Comando: SIMULAR [n_pasos]
Ejemplo: SIMULAR 10
Descripción: Ejecuta la simulación por el número de pasos indicado, actualizando el estado de los compartimientos en cada paso.
Permitir o restringir el movimiento entre celdas
Comando: PERMITIR_MOVIMIENTO / RESTRINGIR_MOVIMIENTO
Descripción: PERMITIR_MOVIMIENTO permite el movimiento entre celdas, mientras que RESTRINGIR_MOVIMIENTO restringe el movimiento, simulando confinamiento.
Imprimir el estado de una celda o autómata
Comando: IMPRIMIR [nombre_celda/autómata]
Ejemplo: IMPRIMIR celda1
Descripción: Muestra el estado actual de una celda o autómata específico, detallando el número de individuos en cada compartimiento.
Imprimir el estado de todos los autómatas y celdas
Comando: IMPRIMIR TODOS
Descripción: Imprime el estado de todos los autómatas y celdas en el sistema, facilitando una visualización completa de la simulación.
Ejemplo Completo de Ejecución

CREAR_AUTOMATA auto1
CREAR_AUTOMATA auto2  // Creación del segundo automata antes de agregarle celdas
CREAR_CELDA auto1 celda1 4 4 100 0 10 0 0 5
CREAR_CELDA auto2 celda2 4 4 80 5 15 0 0 2
CONECTAR_AC auto1 celda1 0 0 auto2 celda2 1 1
ESTABLECER_TASAS 0.3 0.1 0.05 0.02 0.01 0.05
PERMITIR_MOVIMIENTO
SIMULAR 10
IMPRIMIR TODOS
RESTRINGIR_MOVIMIENTO
SIMULAR 5
IMPRIMIR auto1


Descripción del Ejemplo
Creación de autómatas y celdas: Se crea un autómata y se aseguran de crear ambos autómatas (auto1 y auto2) antes de intentar agregarles celdas.
Conexión de celdas: Se conectan celda1 en auto1 y celda2 en auto2 para permitir interacciones entre ellas.
Configuración de tasas: Las tasas de transición se configuran con valores específicos para controlar la propagación de la infección, exposición, recuperación y demás parámetros del modelo.
Habilitación de movimiento: Se permite el movimiento entre celdas conectadas durante los primeros 10 pasos de simulación.
Ejecución y visualización de la simulación: La simulación se ejecuta por 10 pasos y se imprime el estado de todos los autómatas y celdas.
Restricción de movimiento: El movimiento entre celdas se desactiva para simular confinamiento.
Continuación de la simulación y visualización: La simulación continúa durante 5 pasos adicionales sin movimiento, y finalmente se imprime el estado de auto1.