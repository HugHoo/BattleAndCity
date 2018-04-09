#ifndef BULLET_LIST_H_

#define BULLET_LIST_H_	

/*
	该头文件声明了bullet的链表结构以及操作链表的方法
	另：因为无法重载[]运算符，所以构造bullet_at()方法代替
*/
#include "public.h"
//#include "bullet.h"

//暂时将int作为bullet类型，一会回来改
typedef Bullet bullet_data_type;

//定义结点
struct Bullet_node
{
	bullet_data_type bullet_data;
	Bullet_node * front;
	Bullet_node * next;
};

//定义bullet链表
struct Bullet_list
{
	Bullet_node * head;
	Bullet_node * tail;		//注:tail作为超尾
	unsigned short len;
};

//初始化bullet表
Bullet_list * initial_bullet_list(Bullet_list * bl)
{
	bl->head = NULL;
	bl->tail = new Bullet_node;
	bl->tail->front = NULL;
	bl->tail->next = NULL;
	bl->len = 0;

	return bl;
}

Status is_bullet_list_empty(const Bullet_list * bl)
{
	return !bl->len;
}

Status bullet_push_back(bullet_data_type * b, Bullet_list * bl)
{
	Bullet_node * temp = new Bullet_node;
	temp->bullet_data = *b;
	temp->front = NULL;
	temp->next = NULL;

	if (is_bullet_list_empty(bl))
	{
		bl->head = temp;
		bl->head->front = NULL;

		bl->head->next = bl->tail;
		bl->tail->front = bl->head;

		++bl->len;
	}
	else
	{
		Bullet_node * tail_front = bl->tail->front;
		tail_front->next = temp;
		temp->front = tail_front;

		temp->next = bl->tail;
		bl->tail->front = temp;

		++bl->len;
	}

	return SUCCESS;
}

Status bullet_delete_by_index(unsigned short index, Bullet_list * bl)
{
	if (is_bullet_list_empty(bl) || index >= bl->len)
	{
		return FAIL;
	}

	Bullet_node * bullet_tmp = bl->head;
	unsigned short n = 0;
	while (n < index)
	{
		bullet_tmp = bullet_tmp->next;
		++n;
	}

	Bullet_node * delete_tmp = bullet_tmp;

	if (0 == index)
	{
		bl->head = bl->head->next;
		bullet_tmp->next->front = NULL;
		--bl->len;
	}
	else
	{
		bullet_tmp->front->next = bullet_tmp->next;
		bullet_tmp->next->front = bullet_tmp->front;
		--bl->len;
	}

	delete delete_tmp;

	return SUCCESS;
}

Status bullet_delete_by_data(Bullet * b, Bullet_list * bl)
{
	if (is_bullet_list_empty(bl))
	{
		return FAIL;
	}

	Bullet_node * bullet_tmp = bl->head;
	while (&bullet_tmp->bullet_data != b)
	{
		bullet_tmp = bullet_tmp->next;
	}

	Bullet_node * delete_tmp = bullet_tmp;

	bullet_tmp->front->next = bullet_tmp->next;
	bullet_tmp->next->front = bullet_tmp->front;
	--bl->len;

	delete delete_tmp;

	return SUCCESS;
}

Bullet_node * bullet_at(unsigned short index, Bullet_list * bl)
{
	if (index >= bl->len)
	{
		return NULL;
	}

	Bullet_node * bullet_tmp = bl->head;
	unsigned short n = 0;
	while (n < index)
	{
		bullet_tmp = bullet_tmp->next;
		++n;
	}

	return bullet_tmp;
}

Status clear_bullet_list(Bullet_list * bl)
{
	if (is_bullet_list_empty(bl))
		return FAIL;

	Bullet_node * bullet_tmp = bl->head;
	while (bullet_tmp != bl->tail)
	{
		Bullet_node * delete_tmp = bullet_tmp;
		bullet_tmp = bullet_tmp->next;

		delete delete_tmp;
	}

	return SUCCESS;
}

//用于调试bullet_list
void debug_board_bullet_list(Bullet_list * bl)
{
	goto_debug_board(6);
	printf("bullet_list: ");
	goto_debug_board(7);
	if (!is_bullet_list_empty(bl))
	{
		for (int i = 8; i < 8 + bl->len; i++)
		{
			goto_debug_board(i);
			const Bullet * temp = &bullet_at(i - 8, bl)->bullet_data;
			printf("%d row:%d,clm:%d",i - 7 , temp->center.row, temp->center.column);
			//printf("list len: %d ", bl->len);
		}
	}
}

#endif