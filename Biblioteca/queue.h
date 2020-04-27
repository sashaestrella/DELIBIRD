#ifndef QUEUE_H_
#define QUEUE_H_

	#include "../../../tp-2020-1c-BOMP/Biblioteca/list.h"

	typedef struct {
		t_list* elements;
	} t_queue;

	t_queue *queue_create();
	void queue_destroy(t_queue *);

	void queue_push(t_queue *, void *element);
	void *queue_pop(t_queue *);
	void *queue_peek(t_queue *);
	void queue_clean(t_queue *);

	int queue_size(t_queue *);
	int queue_is_empty(t_queue *);

#endif /*QUEUE_H_*/
