#ifndef WALL_H_

#define WALL_H_

#include "public.h"

void initial_wall(Wall * w)
{
	//初始化墙壁
	for (int i = 0; i < WALL_ROW; i++)
	{
		for (int j = 0; j < WALL_COLUMN; j++)
		{
			w->wall[i][j] = no_wall;
		}
	}
	for (int i = 0; i < WALL_ROW; i++)
	{
		w->wall[i][0] = hardest_wall;
		w->wall[i][WALL_COLUMN - 1] = hardest_wall;
	}
	for (int i = 0; i < WALL_COLUMN; i++)
	{
		w->wall[0][i] = hardest_wall;
		w->wall[WALL_ROW - 1][i] = hardest_wall;
	}

	//初始化行列偏移量
	w->row_offset = 0;
	w->column_offset = 0;
}

void print_wall(const Wall * w)
{
	for (int i = 0; i < WALL_ROW; i++)
	{
		for (int j = 0; j < WALL_COLUMN; j++)
		{
			Pos temp;
			temp.row = i + w->row_offset;
			temp.column = j + w->column_offset;
			gotoXY(temp);
			switch (w->wall[i][j])
			{
			case hardest_wall:
				printf(HARDEST_WALL);
				break;
			case hard_wall:
				printf(HARD_WALL);
				break;
			case bricks:
				//SetConsoleTextAttribute(hConsole, 4);
				printf(BRICKS);
				//SetConsoleTextAttribute(hConsole, 7);
				break;
			case heart:
				printf(HEART);
			}
		}
	}
}

#endif