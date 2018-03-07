#ifndef BULLET_H_

#define BULLED_H_

/*
	bullet.h声明子弹的结构以及对子弹的各种操作方法
*/

#include "public.h"
//#include "wall.h"
#define NORMAL "●"

void print_space_bullet(const Bullet * b);
void print_bullet(const Bullet * b);
void print_tank_head(const Pos p);
Status bullet_hit_tank(const Bullet * b, const Tank * tk, const Tank_enermy_list * tel, Tank_die * td);
void debug_board_bullet_shoot_tank(const Tank_enermy_list * tel);

Bullet * initial_bullet(Bullet * b, tank_type tt, Pos p, Dir d)
{
	b->bullet_type = normal;
	b->type = tt;
	b->center = p;
	b->dir = d;

	/*switch (d)
	{
	case up_dir:
		b->center.row--;
		break;
	case down_dir:
		b->center.row++;
		break;
	case left_dir:
		b->center.column--;
		break;
	case right_dir:
		b->center.column++;
	}*/

	return b;
}

short move_bullet(Bullet * b, Wall * w, const Tank * tk_own, const Tank_enermy_list * tk_enermy_list, Tank_die * td)
{
	Bullet b_tmp = *b;
	switch (b->dir)
	{
	case up_dir:
		b_tmp.center.row--;
		break;
	case down_dir:
		b_tmp.center.row++;
		break;
	case left_dir:
		b_tmp.center.column--;
		break;
	case right_dir:
		b_tmp.center.column++;
		break;
	default:
		return FAIL;
	}

	//******************* 对bullet处理
	bool is_die = 0;

	print_space_bullet(b);

	//对print_space_bullet覆盖掉的tank的head填补
	if (b->type == own)
	{
		if (tk_own->head.row == b->center.row && tk_own->head.column == b->center.column)
		{
			Pos h_tmp = tk_own->head;
			print_tank_head(h_tmp);
		}
	}
	else if (enermy == b->type && !is_tank_list_empty(tk_enermy_list))
	{
		const Tank_node * h_tmp = tk_enermy_list->head;

		while (h_tmp != tk_enermy_list->tail)
		{
			if (h_tmp->data.head.row == b->center.row && h_tmp->data.head.column == b->center.column)
			{
				Pos hd = h_tmp->data.head;
				print_tank_head(hd);
			}
			h_tmp = h_tmp->next;
		}
	}

	Wall_type w_tmp = w->wall[b_tmp.center.row][b_tmp.center.column];

	//********************曾近将&b_tmp写成b，然后de了半天bug...... -_-
	Status got_shot = bullet_hit_tank(&b_tmp, tk_own, tk_enermy_list, td);

	if (got_shot)
	{
		//printf("got_shot");
		return got_shot;
		is_die = 1;
	}
	else if (w_tmp == no_wall)
	{
		print_bullet(&b_tmp);
		*b = b_tmp;

		return SUCCESS;
	}
	else
	{
		//bullet屎了
		is_die = 1;
	}

	//****************** 对wall处理

	if (b->dir == up_dir || b->dir == down_dir)
	{
		switch (w->wall[b_tmp.center.row][b_tmp.center.column])
		{
		case hardest_wall:
			break;
		case hard_wall:
			break;
		case bricks:
			for (int i = -1; i <= 1; i++)
			{
				if (w->wall[b_tmp.center.row][b_tmp.center.column + i] == bricks)
				{
					Bullet temp = b_tmp;
					temp.center.column += i;
					w->wall[b_tmp.center.row][b_tmp.center.column + i] = no_wall;
					print_space_bullet(&temp);
				}
			}
			break;
		case heart:
			return BREAK_MY_HEART;
		}
	}
	else
	{
		switch (w->wall[b_tmp.center.row][b_tmp.center.column])
		{
		case hardest_wall:
			break;
		case hard_wall:
			break;
		case bricks:
			for (int i = -1; i <= 1; i++)
			{
				if (w->wall[b_tmp.center.row + i][b_tmp.center.column] == bricks)
				{
					Bullet temp = b_tmp;
					temp.center.row += i;
					w->wall[b_tmp.center.row + i][b_tmp.center.column] = no_wall;
					print_space_bullet(&temp);
				}
			}
			break;
		case heart:
			return BREAK_MY_HEART;
		}
	}

	if (is_die)
		return DIE;

	return FAIL;
}

Status bullet_hit_tank(const Bullet * b, const Tank * tk, const Tank_enermy_list * tel, Tank_die * td)
{
	const Tank_enermy * te_tmp;

	switch (b->type)
	{
	case enermy:
		if (ABS(b->center.row - tk->center.row) <= 1 && ABS(b->center.column - tk->center.column) <= 1)
			return TANK_OWN_DIE;
		break;
	case own:
		//debug_board_bullet_shoot_tank(tel);
		//printf("own");
		for (int i = 0; i < tel->len; i++)
		{
			te_tmp = &tank_at_const(i, tel)->data;

			if (ABS(b->center.row - te_tmp->center.row) <= 1 && ABS(b->center.column - te_tmp->center.column) <= 1)
			{
				td->index = i;
				return TANK_ENERMY_DIE;
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

void debug_board_bullet_shoot_tank(const Tank_enermy_list * tel)
{
	short i_tmp = 13;
	goto_debug_board(i_tmp);
	printf("tank_enermy_list: ");
	goto_debug_board(i_tmp + 1);
	if (!is_tank_list_empty(tel))
	{
		for (int i = 0; i < tel->len; i++)
		{
			goto_debug_board(i_tmp + 2 + i);
			const tank_data_type * t_tmp = &tank_at_const(i, tel)->data;
			printf("%d: row: %d, column: %d", i + 1, t_tmp->center.row, t_tmp->center.column);
		}
	}
}

void print_space_bullet(const Bullet * b)
{
	gotoXY(b->center);
	printf(SPACE);
}

void print_tank_head(const Pos p)
{
	gotoXY(p);
	printf(SQUARE);
}

void print_bullet(const Bullet * b)
{
	gotoXY(b->center);
	printf(NORMAL);
}

#endif