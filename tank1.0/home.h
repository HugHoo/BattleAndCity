#ifndef TANK_HOME_H_

#define TANK_HOME_H_

#include "public.h"
//#include "wall.h"

struct Home
{
	Pos center;
	int row;
	int column;
};

Home * initial_home(Home * h)
{
	h->center.row = WALL_ROW - 1;
	h->center.column = WALL_COLUMN / 2;
	h->row = HOME_ROW;
	h->column = HOME_COLUMN;

	return h;
}

void print_home(const Home * h, Wall * w)
{
	Pos temp;
	temp.row = h->center.row - HOME_ROW + 1;
	temp.column = h->center.column - HOME_COLUMN / 2;
	for (int i = 0; i < HOME_ROW; i++)
	{
		int row_clmn = temp.column;
		for (int j = 0; j < HOME_COLUMN; j++)
		{
			gotoXY(temp);
			printf(SQUARE);
			temp.column++;
		}
		temp.column = row_clmn;
		temp.row++;
	}
}

void fix_home_on_wall(const Home * h, Wall * w)
{
	Pos temp;
	temp.row = h->center.row - HOME_ROW + 1;
	temp.column = h->center.column - HOME_COLUMN / 2;

	for (int i = 0; i < HOME_COLUMN; i++)
	{
		w->wall[temp.row][temp.column + i] = bricks;
		w->wall[temp.row + 1][temp.column + i] = bricks;
	}

	for (int i = 0; i < HOME_ROW - 1; i++)
	{
		w->wall[temp.row + i][temp.column] = bricks;
		w->wall[temp.row + i][temp.column + 1] = bricks;
		w->wall[temp.row + i][temp.column + HOME_COLUMN - 2] = bricks;
		w->wall[temp.row + i][temp.column + HOME_COLUMN - 1] = bricks;
	}

	for (int i = 2; i < HOME_ROW - 1; i++)
	{
		for (int j = 2; j < HOME_COLUMN - 2; j++)
		{
			w->wall[temp.row + i][temp.column + j] = heart;
		}
	}
}

#endif