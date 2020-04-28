#include "../../../tp-2020-1c-BOMP/Biblioteca/queue.h"

#include <stdlib.h>


//Crea y devuelve un puntero a una cola

t_queue *queue_create() {
	t_queue* queue = malloc(sizeof(t_queue));
	t_list* elements = list_create();
	queue->elements = elements;
	return queue;
}
//Elimina todos los nodos de la cola.

void queue_clean(t_queue *self) {
	list_clean(self->elements);
}

//Elimina todos los elementos de la cola.

void queue_clean_and_destroy_elements(t_queue *self, void(*element_destroyer)(void*)) {
	list_clean_and_destroy_elements(self->elements, element_destroyer);
}

//Destruye una cola.

void queue_destroy(t_queue *self) {
	list_destroy(self->elements);
	free(self);
}

 //Destruye una cola, recibiendo como argumento el metodo encargado de liberar cada elemento de la cola.

void queue_destroy_and_destroy_elements(t_queue *self, void(*element_destroyer)(void*)) {
	list_destroy_and_destroy_elements(self->elements, element_destroyer);
	free(self);
}

//Agrega un elemento al final de la cola

void queue_push(t_queue *self, void *element) {
	list_add(self->elements, element);
}

//Quita el primer elemento de la cola

void *queue_pop(t_queue *self) {
	return list_remove(self->elements, 0);
}

//Devuelve el primer elemento de la cola sin extraerlo

void *queue_peek(t_queue *self) {
	return list_get(self->elements, 0);
}

//Devuelve la cantidad de elementos de la cola

int queue_size(t_queue* self) {
	return list_size(self->elements);
}

//Verifica si la cola esta vacía

int queue_is_empty(t_queue *self) {
	return list_is_empty(self->elements);
}


