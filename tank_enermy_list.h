#ifndef TANK_ENERMY_LIST_H_

#define TANK_ENERMY_LIST_H_

#include "public.h"


Tank_enermy_list * initial_tank_list(Tank_enermy_list * tel)
{
	tel->head = NULL;
	tel->tail = new Tank_node;
	tel->tail->front = NULL;
	tel->tail->next = NULL;
	tel->len = 0;

	return tel;
}

Status is_tank_list_empty(const Tank_enermy_list * tel)
{
	return !tel->len;
}

Status is_tank_list_full(const Tank_enermy_list * tel)
{
	return tel->len == TANK_ENERMY_MAX_LEN;
}

Status tank_push_back(const tank_data_type * data, Tank_enermy_list * tel)
{
	if (is_tank_list_full(tel))
		return FAIL;

	Tank_node * temp = new Tank_node;
	temp->data = *data;
	temp->front = NULL;
	temp->next = NULL;

	if (is_tank_list_empty(tel))
	{
		tel->head = temp;
		tel->head->front = NULL;

		tel->head->next = tel->tail;
		tel->tail->front = tel->head;

		++tel->len;
	}
	else
	{
		Tank_node * tail_front = tel->tail->front;
		tail_front->next = temp;
		temp->front = tail_front;

		temp->next = tel->tail;
		tel->tail->front = temp;

		++tel->len;
	}

	return SUCCESS;
}

Tank_node * tank_at(short index, Tank_enermy_list * tel)
{
	if (is_tank_list_empty(tel))
		return NULL;

	Tank_node * h_tmp = tel->head;

	short i = 0;
	while (i < index)
	{
		h_tmp = h_tmp->next;
		++i;
	}

	return h_tmp;
}

Status tank_delete_by_index(short index, Tank_enermy_list * tel)
{
	if (is_tank_list_empty(tel) || index >= tel->len)
		return FAIL;

	Tank_node * h_tmp = tel->head;
	short i = 0;
	while (i < index)
	{
		h_tmp = h_tmp->next;
		++i;
	}

	Tank_node * delete_tmp = h_tmp;

	if (0 == index)
	{
		tel->head = delete_tmp->next;
		delete_tmp->next->front = NULL;
		--tel->len;
	}
	else
	{
		delete_tmp->front->next = delete_tmp->next;
		delete_tmp->next->front = delete_tmp->front;
		--tel->len;
	}

	delete delete_tmp;

	return SUCCESS;
}

void debug_board_tank_list(Tank_enermy_list * tel)
{
	short i_tmp = 6;
	for (int i = 0; i < 6; i++)
	{
		goto_debug_board(i_tmp + i);
		printf("                        ");
	}

	goto_debug_board(i_tmp);
	printf("tank_enermy_list: ");
	goto_debug_board(i_tmp + 1);
	if (!is_tank_list_empty(tel))
	{
		for (int i = 0; i < tel->len; i++)
		{
			goto_debug_board(i_tmp + 2 + i);
			const tank_data_type * t_tmp = &tank_at(i, tel)->data;
			printf("%d: row: %d, column: %d", i + 1, t_tmp->center.row, t_tmp->center.column);
		}
	}
}




#endif