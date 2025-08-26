// -------------------------> IMPORTANTE LEER <-------------------------
// NOTA: compilar con C99; poner en las opciones del compilador: -std=c99
// NOTA: el programa no verifica si una id es única

// Codigo realizado por: Benjamin Larenas como colaborador
// Este código fue realizado como apoyo para la tarea de Kevin Saavedra con fines educativos

#include <stdio.h>
#include <windows.h> // para system("cls"); este codigo ya no es estandar

#define MAX_ELEMENTS 4

// codigos de error
// usados cuando la pila o la cola se quedan sin espacio para almacenar más datos
#define QUEUE_NO_SPACE -1
#define STACK_NO_SPACE -1

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

// -------------------------> funciones de pila <-------------------------
// agregar elemento
int StackPush(stack_t* stack, const unsigned int id) {
	if (stack->index == MAX_ELEMENTS)
		return STACK_NO_SPACE;
	
	stack->stack[stack->index] = id;
	++stack->index;
	
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
	// si el indice final llega al final del array, hacer que vuelva al inicio
	if (queue->final == MAX_ELEMENTS)
		queue->final = 0;
	
	if (queue->queue[queue->final] != 0)
		return QUEUE_NO_SPACE;
	
	queue->queue[queue->final] = id;
	++queue->final;
	
	return 1;
}

// quitar elemento
void QueueDequeue(queue_t* queue) {
	// si el indice final llega al final del array, hacer que vuelva al inicio
	if (queue->frente == MAX_ELEMENTS) {
		queue->frente = 0;
		return;
	}
	
	if (queue->queue[queue->frente] == 0)
		return;
	
	queue->queue[queue->frente] = 0;
	++queue->frente;
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
			continue;
		printf("Proceso ejecutado: %d\n", queue_procwaiting->queue[queue_procwaiting->frente]);
		QueueDequeue(queue_procwaiting);
		++total;
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
	
	while(stack_procsuspended->stack[stack_procsuspended->index-1]) {
		if (QueueEnqueue(queue_procwaiting, stack_procsuspended->stack[stack_procsuspended->index-1]) != QUEUE_NO_SPACE)
			StackPop(stack_procsuspended);
		else {
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
		if (QueueEnqueue(queue_procwaiting, queue_procblocked->queue[queue_procblocked->frente]))
		QueueDequeue(queue_procblocked);
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
		printf("Procesos en espera:   ");
		QueuePrint(&queue_procwaiting);
				
		printf("Procesos suspendidos: ");
		StackPrint(&stack_procsuspended);
				
		printf("Procesos bloqueados:  ");
		QueuePrint(&queue_procblocked);
		
		// prompt
		putchar('>');
		
		// procesar input
		if ((scanf("%d", &option) != 1) || (option < 0) || (option > 7)) {
			system("cls");
			printf("Entrada incorrecta\n");
			while(getchar() != '\n') {} // limpiar stdin
			continue;
		}
		
		// ejecutar opciones
		
		// salir del programa
		if (option == 0) break;
		
		system("cls");
		
		switch(option) {
			case 1: // ejecutar procesos
				ExecuteProcess(&queue_procwaiting);
			break;
			
			case 2: // agregar proceso
				while(1) {
					printf("Ingrese id del proceso\n");
					putchar('>');
					
					// verificar entrada
					if ((scanf("%d", &processid) != 1) || (processid < 1)) {
						printf("Entrada incorrecta\n\n");
						while(getchar() != '\n') {} // limpiar stdin
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
			
			case 4:
				ActivateBlockedProcesses(&queue_procblocked, &queue_procwaiting);
			break;
		}
	}
}