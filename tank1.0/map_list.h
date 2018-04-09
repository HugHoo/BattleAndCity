#ifndef MAP_LIST_H_

#define MAP_LIST_H_

#include "public.h"

Map_list * initial_map_list(Map_list * ml)
{
	ml->head = NULL;
	ml->tail = new Map_node;
	ml->tail->front = NULL;
	ml->tail->next = NULL;
	ml->len = 0;

	return ml;
}

Status is_map_list_empty(const Map_list * ml)
{
	return !ml->len;
}

Status map_push_back(const map_data_type * mdt, Map_list * ml)
{
	Map_node * temp = new Map_node;
	temp->data = *mdt;
	temp->front = NULL;
	temp->next = NULL;

	if (is_map_list_empty(ml))
	{
		ml->head = temp;
		ml->head->front = NULL;

		ml->head->next = ml->tail;
		ml->tail->front = ml->head;

		++ml->len;
	}
	else
	{
		Map_node * tail_front = ml->tail->front;
		tail_front->next = temp;
		temp->front = tail_front;

		temp->next = ml->tail;
		ml->tail->front = temp;

		++ml->len;
	}

	return SUCCESS;
}

void debug_board_map_list(Map_list * ml)
{
	short i_tmp = 6;
	goto_debug_board(i_tmp);
	printf("map_list: ");
	goto_debug_board(i_tmp + 1);

	if (!is_map_list_empty(ml))
	{
		for (int i = 0; i < ml->len; i++)
		{
			goto_debug_board(i_tmp + i + 2);
			const map_data_type * tmp = &map_at(i, ml)->data;
			printf("%d : %s", i + 1, tmp->map_name);
		}
	}
}

#endif