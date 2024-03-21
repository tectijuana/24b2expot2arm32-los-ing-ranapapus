# Uso de registros y convenciones de llamadas
<!---
https://www.canva.com/design/DAF_cWux6Oc/k8AQyKAOCLx1Y70bYZEYpQ/edit
   Para comentarios usar este bloque para documentar pendientes, secuencias, etc.
--->

![image](https://github.com/tectijuana/24b2expot2arm32-los-ing-ranapapus/assets/158226731/5af42423-06ee-423b-89fa-defca9b73378)












------

<pre>

	<p align=center>

Tecnológico Nacional de México
Instituto Tecnológico de Tijuana

Departamento de Sistemas y Computación
Ingeniería en Sistemas Computacionales

Semestre:
Febrero - Junio 2022

Materia:
Lenguajes de interfaz

Docente:
M.C. Rene Solis Reyes 

Unidad:
1

Título del trabajo:
Ejercicios ......

Estudiante:
Bañuelos Vazquez Eduardo Daniel
Garcia Cortez Kevin Eduardo
Montaño Zaragoza Marcos Ulises
Perez Luna Jaime Alonso
Velazquez Pizarro Mixcoatl Obed

	</p>


</pre>
# 3. Uso de registros y convenciones de llamadas

## Introducción 

La comunicación efectiva entre usuario y sistema es esencial para una experiencia fluida. Los mensajes del sistema, ya sean instrucciones, notificaciones de errores o información contextual, son clave para una interacción intuitiva.

Dos herramientas fundamentales para lograr esta comunicación son los registros y las convenciones de llamadas. Los registros funcionan como la memoria del sistema, almacenando información sobre las acciones del usuario y otros eventos relevantes. Por otro lado, las convenciones de llamadas establecen reglas para la estructura y el nombre de las funciones y métodos, lo que facilita la comprensión del código y mejora la legibilidad.

En este tema, exploraremos el uso de registros y convenciones de llamadas, destacando sus beneficios y las mejores prácticas para su implementación.

## Tipos de registros en ARM32

### Registros de Proposito general
R0 - R12: Se utilizan para almacenar datos y direcciones de memoria.

R13(SP): Puntero de pila, almacena la direccion de la parte superior de la pila.

R14(LR): Registro de enlace, almacena la direccion de retorno de una subtrina.

R15(PC): Contador de programa. Almacena la direccion de la siguiente instruccion que se ejecutara

### Registro de Control (CR)
CPSR: Registro de estado del programa. Contiene informacion sobre el modo de ejecucion del procesador, banderas de condicion y nivel de interrucpcion actual

SPSR: Registro estado del programa aplilado. ALmacena el valor de CPSR anter de una interrupcion o excepcion.

### Registro de bancos de registros
Permite acceder a un conjunto adicional de 32 registros a proposito general (R16 a R31).

Se utilizan para almacenar datos locales en subrutinas o para implementar diferentes contextos de ejecucion.

### Registros de coprocesador (CP)
Proporcionan acceso a la funcionalidad de coprocesadores especificos, como unidades de punto de flotante o unidades de procesamiento de señales digitales.

### Registros de sistema (SY)
Contienen informacion sobre el sistema como la indentificacion de procesador, la configuracion de la memoria y las opciones de configuracion

### Registros de depuracion (DBG)
Se utilizan para facilitar la depuracion del software. Permiten al depurador leer y escribir en los registros del procesador y la memorria

## Uso de registros para paso de argumentos
El rendimiento de las llamadas de función se puede mejorar si el sistema tiene todos los argumentos pasados en los registros. Debido a que sólo hay un número limitado de registros, con el fin de aumentar la posibilidad de tener todos los argumentos pasados en los registros, combinar varios argumentos en una clase y pasar la dirección de la clase a la función. Debido a que se está pasando una dirección, se utiliza la semántica de pasar por referencia, que puede no haber sido el caso cuando los argumentos se pasaban como variables individuales.

En ARM de 32 bits, los valores de retorno de las funciones se almacenan en un conjunto de registros específicos conocidos como registros de resultado (result registers). Estos registros son los siguientes:

r0: Valor de retorno primario.
r1-r3: Registros de retorno adicionales (para valores de retorno múltiples o extensiones de 64 bits).
r4-r12: Registros temporales utilizados para almacenamiento de propósito general.

Cuando una función devuelve un valor, generalmente se coloca en r0. Sin embargo, si el valor de retorno es más grande que 32 bits o si la función devuelve varios valores, se pueden usar otros registros de resultado.

### Convención de llamadas AAPCS. 

La Convención de llamadas AAPCS (ARM Architecture Procedure Call Standard) establece cómo se manejan los argumentos y los valores de retorno durante las llamadas a funciones en la arquitectura ARM, promoviendo la interoperabilidad entre compiladores, sistemas operativos y bibliotecas.

Aspectos clave de AAPCS:
Registros: Los primeros 4 argumentos se pasan en los registros R0 a R3. Las variables locales se asignan en la pila, y los valores de retorno se guardan en R0.

Paso de argumentos: Los valores de los argumentos se copian en registros o en la pila para el paso por valor, mientras que para el paso por referencia, se pasa la dirección del argumento en un registro.

Punteros de pila: El SP (Stack Pointer) apunta al final de la pila, y el FP (Frame Pointer) apunta al marco de la pila actual.

## Programa ejemplo
### Serie Fibonacci
```asm
.section .data
format_str: .asciz "Introduce un número: "
input_format: .asciz "%d"
fibonacci_str: .asciz "Fibonacci %d: %d\n"

.section .bss
numero: .skip 4

.section .text
.global main
.global fibonacci
.global printf
.global scanf

main:
    @ Imprimir mensaje
    LDR R0, =format_str
    BL printf

    @ Leer entrada del usuario
    LDR R0, =input_format
    LDR R1, =numero  @ Coloca el formato de entrada
    BL scanf

    @ Inicializar variables del bucle
    MOV R3, #0       @ R3 es el contador del bucle

loop:
    CMP R3, R0       @ Comprobar si hemos alcanzado el número ingresado
    BGE end_loop     @ Si hemos llegado al número, salir del bucle

    MOV R1, R3       @ R1 contiene el índice actual del bucle para fibonacci
    BL fibonacci     @ Llama a la función fibonacci
    MOV R2, R0       @ R2 contiene el valor calculado de fibonacci

    @ Imprimir resultado de fibonacci
    LDR R0, =fibonacci_str
    MOV R1, R3       @ R1 contiene el índice actual del bucle
    MOV R2, R2       @ R2 contiene el valor calculado de fibonacci
    BL printf

    ADD R3, R3, #1   @ Incrementa el contador del bucle
    B loop           @ Volver al inicio del bucle

end_loop:
    MOV R7, #1       @ syscall para exit
    SWI 0            @ causa una interrupción de software para salir

fibonacci:
    PUSH {LR}        @ Guarda el enlace de retorno
    CMP R0, #1       @ Compara n con 1
    MOVLE R0, R0     @ Si n <= 1, devuelve n
    BLE end_fibonacci

    SUB R1, R0, #1   @ Calcula fibonacci(n-1)
    BL fibonacci

    MOV R2, R0       @ Guarda fibonacci(n-1) en R2

    SUB R0, R0, #2   @ Calcula fibonacci(n-2)
    BL fibonacci

    ADD R0, R0, R2   @ Suma fibonacci(n-1) y fibonacci(n-2)

end_fibonacci:
    POP {PC}         @ Retorna de la función
```

```c
#include <stdio.h>

	extern int fibonacci(int n) {
		if (n <= 1) {
			return n;
		} else {
			return fibonacci(n - 1) + fibonacci(n - 2);
		}
	}

	int main() {
		int numero;

		printf("Introduce un número: ");
		scanf("%d", &numero);

		for (int i = 0; i < numero; i++) {
			int fib = fibonacci(i);
			printf("Fibonacci %d: %d\n", i + 1, fib);
		}

		return 0;
	}
//Codigo C con integracion
#include <stdio.h>

extern int fibonacci(int n);

int main() {
	int numero;

	printf("Introduce un número: ");
	scanf("%d", &numero);

	for (int i = 0; i < numero; i++) {
		int fib = fibonacci(i);
		printf("Fibonacci %d: %d\n", i + 1, fib);
	}

	return 0;
}


```
