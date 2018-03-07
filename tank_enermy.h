#ifndef TANK_ENERMY_H_

#define TANK_ENERMY_H_

#include "public.h"

void print_space_enermy(const Tank_enermy * te);
void print_tank_enermy(const Tank_enermy * te);
void initial_tank_move_model(Tank_move_model * tmm);

void initial_tank_enermy(Tank_enermy * te, Wall * w)
{
	Pos c_tmp;
	c_tmp.row = 2;
	short ini_pos_chance = rand() % 9;
	if (ini_pos_chance < 3)
	{
		c_tmp.column = 2;
	}
	else if (ini_pos_chance < 6)
	{
		c_tmp.column = WALL_COLUMN / 2;
	}
	else
	{
		c_tmp.column = WALL_COLUMN - 3;
	}

	te->center = c_tmp;
	te->head = c_tmp;
	te->head.row++;

	te->body[0] = te->center;
	te->body[1] = te->head;
	te->body[2] = te->center; te->body[2].column--;
	te->body[3] = te->center; te->body[3].column++;
	te->body[4] = te->center; te->body[4].row--; te->body[4].column--;
	te->body[5] = te->center; te->body[5].row--;
	te->body[6] = te->center; te->body[6].row--; te->body[6].column++;

	te->health = 1;
	te->level = first_level;
	te->type = enermy;

	initial_tank_move_model(&te->tank_move_model);

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			w->wall[te->center.row + i][te->center.column + i] = no_wall;
		}
	}

	//fix_tank_center_on_wall(te->center, w);
}

void initial_tank_enermy_2(Tank_enermy te[2], Wall * w)
{
	initial_tank_enermy(&te[0], w);

	short plus;
	short ini_pos_chance = rand() % 10;

	switch (te[0].center.column)
	{
	case 2:
		if (ini_pos_chance < 5)
			plus = WALL_COLUMN / 2 - 2;
		else
			plus = WALL_COLUMN - 3 - 2;

		break;
	case WALL_COLUMN / 2:
		if (ini_pos_chance < 5)
			plus = 2 - WALL_COLUMN / 2;
		else
			plus = WALL_COLUMN - 3 - (WALL_COLUMN / 2);

		break;
	case WALL_COLUMN - 3:
		if (ini_pos_chance < 5)
			plus = 2 - WALL_COLUMN / 2;
		else
			plus = 2 - (WALL_COLUMN - 3);

		break;
	default:
		break;
	}

	te[1] = te[0];

	te[1].center.column += plus;
	te[1].head.column += plus;
	for (int i = 0; i < TANK_SQUARE_NUM; i++)
		te[1].body[i].column += plus;

	initial_tank_move_model(&te[1].tank_move_model);

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			w->wall[te[1].center.row + i][te[1].center.column + i] = no_wall;
		}
	}

	//fix_tank_center_on_wall(te[1].center, w);
}

Dir direction_enermy(const Tank_enermy * tk)
{
	if (-1 == tk->head.row - tk->center.row)
		return up_dir;
	else if (tk->head.row - tk->center.row == 1)
		return down_dir;
	else if (tk->head.column - tk->center.column == -1)
		return left_dir;
	else if (tk->head.column - tk->center.column == 1)
		return right_dir;
	return error_dir;
}

void turn_enermy(Tank_enermy * te, Dir d)
{
	Tank_enermy tk_tmp = *te;
	switch (d)
	{
	case up_dir:
		tk_tmp.head = te->center;
		tk_tmp.head.row--;

		for (int i = 0; i < TANK_SQUARE_NUM; i++)
		{
			tk_tmp.body[i] = te->center;
			if (i >= 4)
				tk_tmp.body[i].row++;
		}

		tk_tmp.body[0].row--;
		tk_tmp.body[1].column--;
		tk_tmp.body[3].column++;
		tk_tmp.body[4].column--;
		tk_tmp.body[6].column++;
		print_space_enermy(te);
		print_tank_enermy(&tk_tmp);

		break;
	case down_dir:
		tk_tmp.head = te->center;
		tk_tmp.head.row++;

		for (int i = 0; i < TANK_SQUARE_NUM; i++)
		{
			tk_tmp.body[i] = te->center;
			if (i >= 4)
				tk_tmp.body[i].row--;
		}

		tk_tmp.body[0].row++;
		tk_tmp.body[1].column++;
		tk_tmp.body[3].column--;
		tk_tmp.body[4].column++;
		tk_tmp.body[6].column--;
		print_space_enermy(te);
		print_tank_enermy(&tk_tmp);

		break;
	case left_dir:
		tk_tmp.head = te->center;
		tk_tmp.head.column--;

		for (int i = 0; i < TANK_SQUARE_NUM; i++)
		{
			tk_tmp.body[i] = te->center;
			if (i >= 4)
				tk_tmp.body[i].column++;
		}

		tk_tmp.body[0].column--;
		tk_tmp.body[1].row--;
		tk_tmp.body[3].row++;
		tk_tmp.body[4].row--;
		tk_tmp.body[6].row++;
		print_space_enermy(te);
		print_tank_enermy(&tk_tmp);

		break;
	case right_dir:
		tk_tmp.head = te->center;
		tk_tmp.head.column++;

		for (int i = 0; i < TANK_SQUARE_NUM; i++)
		{
			tk_tmp.body[i] = te->center;
			if (i >= 4)
				tk_tmp.body[i].column--;
		}

		tk_tmp.body[0].column++;
		tk_tmp.body[1].row++;
		tk_tmp.body[3].row--;
		tk_tmp.body[4].row++;
		tk_tmp.body[6].row--;
		print_space_enermy(te);
		print_tank_enermy(&tk_tmp);

		break;
	default:
		break;
	}

	*te = tk_tmp;
}

void move_enermy(Tank_enermy * tk, Dir d, Wall * w)
{
	eli_tank_center_on_wall(tk->center, w);

	struct Tank_enermy tk_tmp = *tk;
	switch (d)
	{
	case up_dir:
		tk_tmp.center.row--;
		tk_tmp.head.row--;
		for (int i = 0; i < TANK_SQUARE_NUM; i++)
			tk_tmp.body[i].row--;
		print_space_enermy(tk);
		print_tank_enermy(&tk_tmp);

		break;
	case down_dir:
		tk_tmp.center.row++;
		tk_tmp.head.row++;
		for (int i = 0; i < TANK_SQUARE_NUM; i++)
			tk_tmp.body[i].row++;
		print_space_enermy(tk);
		print_tank_enermy(&tk_tmp);

		break;
	case left_dir:
		tk_tmp.center.column--;
		tk_tmp.head.column--;
		for (int i = 0; i < TANK_SQUARE_NUM; i++)
			tk_tmp.body[i].column--;
		print_space_enermy(tk);
		print_tank_enermy(&tk_tmp);

		break;
	case right_dir:
		tk_tmp.center.column++;
		tk_tmp.head.column++;
		for (int i = 0; i < TANK_SQUARE_NUM; i++)
			tk_tmp.body[i].column++;
		print_space_enermy(tk);
		print_tank_enermy(&tk_tmp);

		break;
	}

	*tk = tk_tmp;

	fix_tank_center_on_wall(tk->center, w);
}

bool move_success_enermy(const Tank_enermy * tk, const Wall * wall, Dir d)
{
	switch (d)
	{
	case up_dir:
		for (int i = -1; i <= 1; i++)
		{
			if (wall->wall[tk->head.row - 1][tk->head.column + i] != no_wall)
				return 0;
		}
		return 1;
		break;
	case down_dir:
		for (int i = -1; i <= 1; i++)
		{
			if (wall->wall[tk->head.row + 1][tk->head.column + i] != no_wall)
				return 0;
		}
		return 1;
		break;
	case left_dir:
		for (int i = -1; i <= 1; i++)
		{
			if (wall->wall[tk->head.row + i][tk->head.column - 1] != no_wall)
				return 0;
		}
		return 1;
		break;
	case right_dir:
		for (int i = -1; i <= 1; i++)
		{
			if (wall->wall[tk->head.row + i][tk->head.column + 1] != no_wall)
				return 0;
		}
		return 1;
		break;
	default:
		return 0;
	}

	return 1;
}

bool initial_success(const Tank_enermy * te, const Wall * w)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (w->wall[te->center.row + i][te->center.column + j] == tank)
				return false;
		}
	}
	return true;
}

void print_tank_enermy(const Tank_enermy * te)
{
	for (int i = 0; i < TANK_SQUARE_NUM; i++)
	{
		gotoXY(te->body[i]);
		printf(SQUARE);
	}
}

void print_space_enermy(const Tank_enermy * te)
{
	for (int i = 0; i < TANK_SQUARE_NUM; i++)
	{
		gotoXY(te->body[i]);
		printf(SPACE);
	}
}

void initial_tank_move_model(Tank_move_model * tmm)
{
	tmm->bullet_model = 0;
	tmm->move_model = 0;
	tmm->turn_model = 0;
	tmm->dir = error_dir;
}

//如果需要重复动作返回1，否则返回0
bool check_model(Tank_move_model * tmm)
{
	if (tmm->bullet_model == 0 && tmm->move_model == 0 && tmm->move_model == 0)
		return 0;
	return 1;
}


#endif