#ifndef TANK_H_

#define TANK_H_

#include <Windows.h>
#include "public.h"
//#include "wall.h"
#include <stdio.h>

Tank initial_tank_own(Wall * w)
{
	Pos center_tmp;
	center_tmp.row = WALL_ROW - 4;
	center_tmp.column = WALL_COLUMN / 2  - HOME_COLUMN / 2 - 3;

	Tank tk_tmp = {
		center_tmp,
		{ center_tmp.row - 1, center_tmp.column },
		{
			center_tmp,
			{ center_tmp.row - 1, center_tmp.column },
			{ center_tmp.row, center_tmp.column - 1 },
			{ center_tmp.row, center_tmp.column + 1 },
			{ center_tmp.row + 1, center_tmp.column - 1 },
			{ center_tmp.row + 1, center_tmp.column },
			{ center_tmp.row + 1, center_tmp.column + 1 }
		},
		3
	};

	fix_tank_center_on_wall(tk_tmp.center, w);

	return tk_tmp;
}

Dir direction(const Tank * tk)
{
	if (tk->head.row - tk->center.row == -1)
		return up_dir;
	else if (tk->head.row - tk->center.row == 1)
		return down_dir;
	else if (tk->head.column - tk->center.column == -1)
		return left_dir;
	else if (tk->head.column - tk->center.column == 1)
		return right_dir;
	return error_dir;
}

void print_tank_own(const Tank * tk)
{
	for (int i = 0; i < TANK_SQUARE_NUM; i++)
	{
		gotoXY(tk->body[i]);
		printf(SQUARE);
	}
}

void print_space_own(const Tank * tk)
{
	for (int i = 0; i < TANK_SQUARE_NUM; i++)
	{
		gotoXY(tk->body[i]);
		printf(SPACE);
	}
}

void turn_own(Tank * tk, Dir d)
{
	Tank tk_tmp = *tk;
	switch (d)
	{
	case up_dir:
		tk_tmp.head = tk->center;
		tk_tmp.head.row--;

		for (int i = 0; i < TANK_SQUARE_NUM; i++)
		{
			tk_tmp.body[i] = tk->center;
			if (i >= 4)
				tk_tmp.body[i].row++;
		}

		tk_tmp.body[0].row--;
		tk_tmp.body[1].column--;
		tk_tmp.body[3].column++;
		tk_tmp.body[4].column--;
		tk_tmp.body[6].column++;
		print_space_own(tk);
		print_tank_own(&tk_tmp);
		
		break;
	case down_dir:
		tk_tmp.head = tk->center;
		tk_tmp.head.row++;

		for (int i = 0; i < TANK_SQUARE_NUM; i++)
		{
			tk_tmp.body[i] = tk->center;
			if (i >= 4)
				tk_tmp.body[i].row--;
		}

		tk_tmp.body[0].row++;
		tk_tmp.body[1].column++;
		tk_tmp.body[3].column--;
		tk_tmp.body[4].column++;
		tk_tmp.body[6].column--;
		print_space_own(tk);
		print_tank_own(&tk_tmp);

		break;
	case left_dir:
		tk_tmp.head = tk->center;
		tk_tmp.head.column--;

		for (int i = 0; i < TANK_SQUARE_NUM; i++)
		{
			tk_tmp.body[i] = tk->center;
			if (i >= 4)
				tk_tmp.body[i].column++;
		}

		tk_tmp.body[0].column--;
		tk_tmp.body[1].row--;
		tk_tmp.body[3].row++;
		tk_tmp.body[4].row--;
		tk_tmp.body[6].row++;
		print_space_own(tk);
		print_tank_own(&tk_tmp);

		break;
	case right_dir:
		tk_tmp.head = tk->center;
		tk_tmp.head.column++;

		for (int i = 0; i < TANK_SQUARE_NUM; i++)
		{
			tk_tmp.body[i] = tk->center;
			if (i >= 4)
				tk_tmp.body[i].column--;
		}

		tk_tmp.body[0].column++;
		tk_tmp.body[1].row++;
		tk_tmp.body[3].row--;
		tk_tmp.body[4].row++;
		tk_tmp.body[6].row--;
		print_space_own(tk);
		print_tank_own(&tk_tmp);

		break;
	default:
		break;
	}

	*tk = tk_tmp;
}

void move_own(Tank * tk, Dir d, Wall * w)
{
	eli_tank_center_on_wall(tk->center, w);

	struct Tank tk_tmp = *tk;
	switch (d)
	{
	case up_dir:
		tk_tmp.center.row--;
		tk_tmp.head.row--;
		for (int i = 0; i < TANK_SQUARE_NUM; i++)
			tk_tmp.body[i].row--;
		print_space_own(tk);
		print_tank_own(&tk_tmp);

		break;
	case down_dir:
		tk_tmp.center.row++;
		tk_tmp.head.row++;
		for (int i = 0; i < TANK_SQUARE_NUM; i++)
			tk_tmp.body[i].row++;
		print_space_own(tk);
		print_tank_own(&tk_tmp);

		break;
	case left_dir:
		tk_tmp.center.column--;
		tk_tmp.head.column--;
		for (int i = 0; i < TANK_SQUARE_NUM; i++)
			tk_tmp.body[i].column--;
		print_space_own(tk);
		print_tank_own(&tk_tmp);

		break;
	case right_dir:
		tk_tmp.center.column++;
		tk_tmp.head.column++;
		for (int i = 0; i < TANK_SQUARE_NUM; i++)
			tk_tmp.body[i].column++;
		print_space_own(tk);
		print_tank_own(&tk_tmp);

		break;
	}

	*tk = tk_tmp;

	fix_tank_center_on_wall(tk->center, w);
}

void debug_board_tank(const Tank * tk)
{
	goto_debug_board(1);
	printf("center:");
	goto_debug_board(2);
	printf("x = %d, y = %d", tk->center.column, tk->center.row);
	goto_debug_board(3);
	printf("head:");
	goto_debug_board(4);
	printf("x = %d, y = %d", tk->head.column, tk->center.row);
	goto_debug_board(5);
}

bool move_success_own(const Tank * tk, const Wall * wall, Dir d)
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

bool initial_success(Pos center, Wall * w)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (w->wall[center.row + i][center.column + j] == tank)
				return 0;
		}
	}
	return 1;
}

#endif