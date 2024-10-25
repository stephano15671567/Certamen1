# Certamen1

Ejecución de los Códigos
Los códigos implementados en este proyecto se ejecutan sin errores y han sido probados exhaustivamente para asegurar su correcto funcionamiento en diferentes escenarios de simulación.

Claridad y Documentación
El código está escrito de manera clara y estructurada, con comentarios que explican el propósito de las funciones y los bloques principales. Se ha seguido un estilo consistente de codificación, cumpliendo con las condiciones establecidas en las directrices de entrega.

Datos de Entrada y Salida
Los detalles sobre los datos de entrada y salida, así como su formato, están comentados dentro del código y se describen de manera más detallada en este archivo README. A continuación, se proporciona un resumen de los principales comandos, su uso y ejemplos para facilitar la ejecución de la simulación:

Comandos principales
Crear un autómata

Comando: CREATE_AUTOMATON [nombre]
Ejemplo: CREATE_AUTOMATON my_automaton
Descripción: Crea un nuevo autómata con el nombre especificado.
Crear una celda dentro de un autómata

Comando: CREATE_CELL [nombre_automaton] [nombre_celda] [alto] [ancho] [S] [E] [I] [R] [D] [V]
Ejemplo: CREATE_CELL my_automaton cell1 4 4 100 0 10 0 0 5
Descripción: Crea una celda en el autómata especificado con dimensiones y valores iniciales para los compartimientos susceptibles (S), expuestos (E), infectados (I), recuperados (R), fallecidos (D), y vacunados (V).
Conectar celdas entre autómatas

Comando: CONNECT_AC [automaton1] [celda1] [x1] [y1] [automaton2] [celda2] [x2] [y2]
Ejemplo: CONNECT_AC my_automaton cell1 0 0 my_automaton cell2 1 1
Descripción: Conecta dos celdas, permitiendo que sus casos específicos actúen como vecinos mutuos.
Establecer tasas de transición en el modelo de contagio

Comando: SET_RATES [beta] [sigma] [gamma] [mu] [delta] [rho]
Ejemplo: SET_RATES 0.3 0.1 0.05 0.02 0.01 0.05
Descripción: Configura las tasas de infección (beta), exposición (sigma), recuperación (gamma), mortalidad (mu), fallecidos (delta) y pérdida de inmunidad (rho) en el modelo.
Simulación del sistema

Comando: SIMULATE [n_pasos]
Ejemplo: SIMULATE 10
Descripción: Ejecuta la simulación por el número de pasos indicado, actualizando el estado de los compartimientos en cada paso.
Permitir o restringir el movimiento entre celdas

Comando: ALLOW_MOVEMENT / DISALLOW_MOVEMENT
Descripción: ALLOW_MOVEMENT permite el movimiento entre celdas, mientras que DISALLOW_MOVEMENT restringe el movimiento, simulando confinamiento.
Imprimir el estado de una celda o autómata

Comando: PRINT [nombre_celda/automaton]
Ejemplo: PRINT cell1
Descripción: Muestra el estado actual de una celda o autómata específico, detallando el número de individuos en cada compartimiento.
Imprimir el estado de todos los autómatas y celdas

Comando: PRINT ALL
Descripción: Imprime el estado de todos los autómatas y celdas en el sistema, facilitando una visualización completa de la simulación.

Ejemplo Completo de Ejecución
Aquí se muestra un flujo de comandos típico para iniciar una simulación completa:

CREATE_AUTOMATON my_automaton
CREATE_CELL my_automaton cell1 4 4 100 0 10 0 0 5
CREATE_CELL my_automaton cell2 4 4 80 5 15 0 0 2
CONNECT_AC my_automaton cell1 0 0 my_automaton cell2 1 1
SET_RATES 0.3 0.1 0.05 0.02 0.01 0.05
ALLOW_MOVEMENT
SIMULATE 10
PRINT ALL
DISALLOW_MOVEMENT
SIMULATE 5
PRINT my_automaton

En este ejemplo:

Se crea un autómata y dos celdas con estados iniciales diferentes.
Se conectan ambas celdas para permitir interacciones.
Se configuran las tasas de transición y se permite el movimiento entre celdas.
Se ejecuta la simulación durante 10 pasos, y se imprime el estado de todas las celdas.
Se restringe el movimiento y se continúa la simulación por otros 5 pasos, mostrando luego el estado final.