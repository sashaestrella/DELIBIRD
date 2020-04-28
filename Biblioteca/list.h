#ifndef LIST_H_
#define LIST_H_

#include "stdbool.h"
#include "../../../tp-2020-1c-BOMP/Biblioteca/node.h"


	typedef struct {
		t_link_element *head;
		int elements_count;
	} t_list;

	t_list * list_create();

	void list_destroy(t_list *);
	void list_destroy_and_destroy_elements(t_list*, void(*element_destroyer)(void*));

	int list_add(t_list *, void *element);
	void list_add_in_index(t_list *, int index, void *element);

	void *list_get(t_list *, int index);

	void *list_replace(t_list*, int index, void* element);
	void list_replace_and_destroy_element(t_list*, int index, void* element, void(*element_destroyer)(void*));

	void *list_remove(t_list *, int index);
	void list_remove_and_destroy_element(t_list *, int index, void(*element_destroyer)(void*));

	void *list_remove_by_condition(t_list *, bool(*condition)(void*));
	void list_remove_and_destroy_element_by_condition(t_list *, bool(*condition)(void*), void(*element_destroyer)(void*));

	void list_clean(t_list *);
	void list_clean_and_destroy_elements(t_list *self, void(*element_destroyer)(void*));

	void list_iterate(t_list *, void(*closure)(void*));
	void *list_find(t_list *, bool(*closure)(void*));

	int list_size(t_list *);
	int list_is_empty(t_list *);

#endif /*LIST_H_*/

