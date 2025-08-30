// DISEÑO:
// El diseño consiste en 3 almacenamientos de procesos:
// - Procesos en espera
// - Procesos en suspension
// - Procesos bloqueados
// Cuando el usuario ingresa un proceso, se va a la cola de procesos en espera
// Si esta cola está llena, se mueve a la de suspension
// Y si la de suspension está llena, el proceso se mueve a procesos bloqueados
// Y si todos los almacenamientos de procesos están llenos, no se acepta ningun proceso más

// -------------------------> IMPORTANTE LEER <-------------------------
// NOTA: compilar con C99; poner en las opciones del compilador: -std=c99
// NOTA: el programa no verifica si una id es única

// Codigo realizado por: Benjamin Larenas como colaborador
// Este código fue realizado como apoyo para la tarea de Kevin Saavedra con fines educativos

#include <stdio.h> // libreria estandar
#include <windows.h> // para system("cls"); este codigo ya no es estandar

#define MAX_ELEMENTS 4

// codigos de error
// usados cuando la pila o la cola se quedan sin espacio para almacenar más datos
#define QUEUE_NO_SPACE -1
#define STACK_NO_SPACE -1

// DATO: MAX_ELEMENTS, QUEUE_NO_SPACE y STACK_NO_SPACE son macros (https://www.tutorialspoint.com/cprogramming/c_macros.htm)

// types
typedef struct { // pila (stack)
	int index;
	unsigned int stack[MAX_ELEMENTS];
} stack_t;

typedef struct { // cola (queue)
	int frente;
	int final;
	unsigned int queue[MAX_ELEMENTS];
} queue_t;

// typedef: https://www.tutorialspoint.com/cprogramming/c_typedef.htm
// DATO: stack_t y queue_t son structs (https://www.tutorialspoint.com/cprogramming/c_structures.htm)

// -------------------------> funciones de pila <-------------------------

// DATO: las funciones usan punteros para acceder a las structs stack_t y queue_t.
// NOTA: las funciones pueden tener tipos. Los usados en este codigo son void y int, los cuales definen el tipo
// de retorno que tiene la funcion. Por ejemplo:

// int funcion() {
// 		return 7; 	
// }
// funcion devuelve 7

// void funcion() {
// 		return; 
// } 
// la funcion no devuelve nada

// Las funciones que sean void no necesitan un return explicito. En cambio, las funciones que devuelven un valor
// deben devolver un valor sí o sí (https://www.tutorialspoint.com/cprogramming/c_functions.htm)

// Un puntero apunta a una dirección de memoria. Cuando usamos punteros en funciones, tenemos la ventaja de 
// poder modificar la variable la cual pasemos como argumento. 

// NOTA: los punteros tienen el aspecto: <tipo de dato>* <nombre>. Ese "*" significa que trabajamos con un puntero

// En StackPush(), usamos stack_t* stack (un puntero de tipo stack_t llamado stack) para modificar sus valores
// en la funcion.

// El operador -> es para acceder a los miembros de esa struct:
// stack->index
// stack->stack[]

// NOTA: const se usa para decir que un valor es constante, es decir, que no cambiará a lo largo del programa.

// agregar elemento
int StackPush(stack_t* stack, const unsigned int id) {
	// en caso de que index sea igual a MAX_ELEMENTS (elementos maximos que permite el stack[])
	// se ejecuta un return STACK_NO_SPACE (STACK_NO_SPACE es -1), esto significa que hubo un error
	// al momento de agregar un elemento
	if (stack->index == MAX_ELEMENTS)
		return STACK_NO_SPACE;
	
	// aquí se accede al mimebro stack (stack->stack[])
	// luego, se accede al index (stack->index)
	// se iguala a id
	stack->stack[stack->index] = id;
	++stack->index;
	
	// NOTA:
	// ++stack->index; es lo mismo que stack->index += 1;
	
	// si todo va bien, la funcion devuelve 1
	return 1;
}
// eliminar elemento
void StackPop(stack_t* stack) {
	const int index = stack->index-1;
	if (index >= 0) {
		stack->stack[index] = 0;
		--stack->index;
	}
}
// imprimir pila
void StackPrint(const stack_t* stack) {
	for(int i = 0; i < MAX_ELEMENTS; ++i)
		printf("[%d] ", stack->stack[i]);
	putchar('\n');
}

// -------------------------> funciones cola <-------------------------
// agregar elemento
int QueueEnqueue(queue_t* queue, const unsigned int id) {
	// si queue->final llega a MAX_ELEMENTS, hacer que vuelva desde el inicio, es decir, igualarla a 0
	if (queue->final == MAX_ELEMENTS)
		queue->final = 0;
	
	// aquí, se accede a queue (queue->queue[]) en la posición queue->final
	// significa que ya no hay mas espacio disponible
	if (queue->queue[queue->final] != 0)
		return QUEUE_NO_SPACE; // devolver QUEUE_NO_SPACE, es decir, -1
	
	// igualar la id a la queue, en el lugar del valor de queue->final
	queue->queue[queue->final] = id;
	++queue->final; // aumentar queue->final en 1
	
	return 1;
}

// quitar elemento
void QueueDequeue(queue_t* queue) {
	// si el indice final llega al final del array, hacer que vuelva al inicio
	if (queue->frente == MAX_ELEMENTS) {
		queue->frente = 0;
		return;
	}
	
	// si en queue->queue[], en la posición queue->frente es 0, retorar, es decir, salir de la funcion
	// ya que no hay ningun elemento a elminar
	if (queue->queue[queue->frente] == 0)
		return;
	
	// en queue->queue[], en la posición queue->frente, asignar 0
	queue->queue[queue->frente] = 0;
	++queue->frente; // aumentar queue->frente en 1
}

// imprimir cola
void QueuePrint(const queue_t* queue) {
	for(int i = 0; i < MAX_ELEMENTS; ++i)
		printf("[%d] ", queue->queue[i]);
	putchar('\n');
}

// -------------------------> funciones menu <-------------------------
// ejecutar procesos en espera
void ExecuteProcess(queue_t* queue_procwaiting) {
	int total = 0; // cantidad de procesos ejecutados
	
	if (queue_procwaiting->queue[queue_procwaiting->frente] == 0) {
		system("cls");
		printf("No hay procesos para ejecutar\n");
		return;
	}
	
	// ejecutar procesos
	for (int i = 0; i < MAX_ELEMENTS; ++i) {
		// si el proceso no tiene id
		if (queue_procwaiting->queue[queue_procwaiting->frente] == 0)
			continue; // continue fuerza al bucle a hacer otra iteracion
		printf("Proceso ejecutado: %d\n", queue_procwaiting->queue[queue_procwaiting->frente]);
		QueueDequeue(queue_procwaiting);
		++total; // aumentar en 1 los procesos totales ejecutados
	}
	
	// establecer posicion de frente y final de la cola a 0
	queue_procwaiting->frente = queue_procwaiting->final = 0;
	
	printf(">>> Procesos ejecutados: %d <<<\n", total);
}

// activar procesos suspendidos; que pasen a procesos en espera
void ActivateSupendedProcesses(stack_t* stack_procsuspended, queue_t* queue_procwaiting) {
	if (stack_procsuspended->stack[0] == 0) {
		printf("No hay procesos suspendidos.\n");
		return;
	}
	
	// en while, la condicion es la siguiente:
	// mientras que en stack_procsuspended->stack[], en la posicion stack_procsuspended->index-1 no sea 0, sigue ejecutando
	while(stack_procsuspended->stack[stack_procsuspended->index-1]) {
		if (QueueEnqueue(queue_procwaiting, stack_procsuspended->stack[stack_procsuspended->index-1]) != QUEUE_NO_SPACE) // agregar id a la cola de procesos en espera
			StackPop(stack_procsuspended); // eliminar procesos en pila de procesos suspendidos
		else { // si la cola de espera está llena, salir del bucle
			printf("No es posible activar procesos suspendidos, la cola de procesos en espera esta llena.\n");
			return;
		}
	}
	
	printf("Procesos suspendidos activados.\n");
}

// activar procesos bloqueados; que pasen a procesos en espera
void ActivateBlockedProcesses(queue_t* queue_procblocked, queue_t* queue_procwaiting) {
	if (queue_procblocked->queue[queue_procblocked->frente] == 0) {
		printf("No hay procesos bloqueados\n");
		return;
	}
	
	// ejecutar procesos
	for (int i = 0; i < MAX_ELEMENTS; ++i) {
		// agregar proceso bloqueado a proceso en espera, siempre y cuando haya espacio
		if (QueueEnqueue(queue_procwaiting, queue_procblocked->queue[queue_procblocked->frente]))
			QueueDequeue(queue_procblocked); // quitar procesos bloqueado
	}
	
	// establecer posicion de frente y final de la cola a 0
	queue_procblocked->frente = queue_procblocked->final = 0;
}

int main(void) {
	queue_t queue_procwaiting   = {0}; // procesos en espera
	stack_t stack_procsuspended = {0}; // procesos suspendidos
	queue_t queue_procblocked   = {0}; // procesos bloqueados
	
	int option, processid;
	option = processid = 0;
	
	// NOTA:
	// option y processid se usa para la entrada del usuario con scanf() (https://www.tutorialspoint.com/c_standard_library/c_function_scanf.htm)
	
	// menu
	while(1) {
		// imprimir opciones del menu
		printf("\t=== Gestor de procesos ===\n\n");
		printf("0. Salir\n\n");
		
		printf("1. Ejecutar procesos en espera\n\n");
		
		printf("2. Agregar proceso\n\n");
		
		printf("3. Activar procesos en suspension\n");
		printf("4. Activar procesos bloqueados\n\n");
		
		// imprimir procesos
		
		// NOTA: como puedes observar, en la función QueuePrint(), se manda el siguiente argumento: &queue_procwaiting
		// el & significa la dirección de memoria de queue_procwaiting, significa que QueuePrint() acepta 
		// solo punteros
		
		printf("Procesos en espera:   ");
		QueuePrint(&queue_procwaiting);
				
		printf("Procesos suspendidos: ");
		StackPrint(&stack_procsuspended);
				
		printf("Procesos bloqueados:  ");
		QueuePrint(&queue_procblocked);
		
		// prompt
		putchar('>');
		
		// procesar input
		if ((scanf("%d", &option)) || (option < 0) || (option > 7)) {
			system("cls");
			printf("Entrada incorrecta\n");
			// si el usuario ingresa texto, la siguiente linea limpia la entrada
			while(getchar() != '\n') {}
			continue; // siguiente iteracion
		}
		
		// ejecutar opciones
		
		// salir del programa
		if (option == 0) break;
		
		system("cls");
		
		// NOTA: switch (https://www.tutorialspoint.com/cprogramming/switch_statement_in_c.htm)
		switch(option) {
			case 1: // ejecutar procesos
				ExecuteProcess(&queue_procwaiting);
			break;
			
			case 2: // agregar proceso
				while(1) {
					printf("Ingrese id del proceso\n");
					putchar('>');
					
					// verificar entrada
					if ((scanf("%d", &processid)) || (processid < 1)) { // processid < 1, quiere decir que processid debe ser > 1
						printf("Entrada incorrecta\n\n");
						while(getchar() != '\n') {} // limpiar entrada
					} else // salir de bucle
						break;
				}
				system("cls");
				
				// agregar a procesos en espera
				// intentar agregar proceso en la cola de espera
				if (QueueEnqueue(&queue_procwaiting, processid) == QUEUE_NO_SPACE) {
					printf("El ultimo proceso fue suspendido\n");
					
					// intentar agregar proceso a la pila de procesos suspendidos
					if (StackPush(&stack_procsuspended, processid) == STACK_NO_SPACE) {
						printf("La pila de procesos suspendidos esta llena, el ultimo proceso fue bloqueado\n");
						
						// intentar agregar proceso a la cola de procesos bloqueados
						if (QueueEnqueue(&queue_procblocked, processid) == QUEUE_NO_SPACE)
							printf("La cola de procesos bloqueados esta llena, ya no se aceptan mas procesos\n");
					}
				}
			break;
			
			case 3: // procesos de suspension a procesos en espera
				ActivateSupendedProcesses(&stack_procsuspended, &queue_procwaiting);
			break;
			
			case 4: // procesos bloqueados a procesos en espera
				ActivateBlockedProcesses(&queue_procblocked, &queue_procwaiting);
			break;
		}
	}
}
