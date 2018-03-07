#ifndef MAP_MAKER_H_

#define MAP_MAKER_H

#include "public.h"
#define ENDL(x) goto_debug_board(x)

struct Map_maker_meta
{
	Pos center;
	Wall_type wall_type;
};

void fix_wall_meta(Pos big_corner, Wall_type wt, Wall * w);
void rectangle(Pos big_corner, short big_row, short big_column, Wall_type wt, Wall * w, bool mirror);
void rectangle_mirror(Pos big_corner, short big_row, short big_column, Wall_type wt, Wall * w);
void print_map_maker_meta(Pos p, Wall * w);
void eli_map_maker_meta(Pos p, Wall * w);
void fix_map_maker_on_wall(Pos p, Wall * w, Wall_type wt);
Status save_map(Wall * w);
static void map_maker_info_board();

Status map_maker_index(Wall * w)
{
	Map_maker_meta mmm;
	mmm.center = { 1, 1 };
	mmm.wall_type = bricks;
	/*Map_maker_meta mmm_tmp;
	mmm.center = { 1, 1 };
	mmm.wall_type = no_wall;*/
	print_map_maker_meta(mmm.center, w);

	bool map_done = 0;
	Dir offset_dir = error_dir;

	map_maker_info_board();

	while (!map_done)
	{
		char ch = _getch();
		switch (ch)
		{
		case 'w':
			//消除之前的光标
			eli_map_maker_meta(mmm.center, w);
			//对光标坐标处理
			--mmm.center.row;
			if (mmm.center.row < 1)
				mmm.center.row = 13;
			//打印新的光标
			print_map_maker_meta(mmm.center, w);
			break;
		case 's':
			eli_map_maker_meta(mmm.center, w);
			++mmm.center.row;
			if (mmm.center.row > 13)
				mmm.center.row = 1;
			print_map_maker_meta(mmm.center, w);
			break;
		case 'a':
			eli_map_maker_meta(mmm.center, w);
			--mmm.center.column;
			if (mmm.center.column < 1)
				mmm.center.column = 13;
			print_map_maker_meta(mmm.center, w);
			break;
		case 'd':
			eli_map_maker_meta(mmm.center, w);
			++mmm.center.column;
			if (mmm.center.column > 13)
				mmm.center.column = 1;
			print_map_maker_meta(mmm.center, w);
			break;
		case 'j':
			fix_map_maker_on_wall(mmm.center, w, bricks);
			switch (offset_dir)
			{
			case right_dir:
				++mmm.center.column;
				if (mmm.center.column > 13)
					mmm.center.column = 1;
				break;
			case down_dir:
				++mmm.center.row;
				if (mmm.center.row > 13)
					mmm.center.row = 1;
				break;
			case left_dir:
				--mmm.center.column;
				if (mmm.center.column < 1)
					mmm.center.column = 13;
				break;
			case up_dir:
				--mmm.center.row;
				if (mmm.center.row < 1)
					mmm.center.row = 13;
			}
			print_map_maker_meta(mmm.center, w);
			break;
		case 'k':
			fix_map_maker_on_wall(mmm.center, w, hard_wall);
			switch (offset_dir)
			{
			case right_dir:
				++mmm.center.column;
				if (mmm.center.column > 13)
					mmm.center.column = 1;
				break;
			case down_dir:
				++mmm.center.row;
				if (mmm.center.row > 13)
					mmm.center.row = 1;
				break;
			case left_dir:
				--mmm.center.column;
				if (mmm.center.column < 1)
					mmm.center.column = 13;
				break;
			case up_dir:
				--mmm.center.row;
				if (mmm.center.row < 1)
					mmm.center.row = 13;
			}
			print_map_maker_meta(mmm.center, w);
			break;
		case 'l':
			fix_map_maker_on_wall(mmm.center, w, no_wall);
			switch (offset_dir)
			{
			case right_dir:
				++mmm.center.column;
				if (mmm.center.column > 13)
					mmm.center.column = 1;
				break;
			case down_dir:
				++mmm.center.row;
				if (mmm.center.row > 13)
					mmm.center.row = 1;
				break;
			case left_dir:
				--mmm.center.column;
				if (mmm.center.column < 1)
					mmm.center.column = 13;
				break;
			case up_dir:
				--mmm.center.row;
				if (mmm.center.row < 1)
					mmm.center.row = 13;
			}
			print_map_maker_meta(mmm.center, w);
			break;
		case 'i':
			SetConsoleTextAttribute(hConsole, 115);
			offset_dir = error_dir;
			goto_debug_board(19);
			printf("自动移动：off");
			break;
		case 72: 
			SetConsoleTextAttribute(hConsole, 115); 
			offset_dir = up_dir; 
			goto_debug_board(19); 
			printf("自动移动： ↑"); 
			gotoXY(console_corner); 
			break;
		case 75: 
			SetConsoleTextAttribute(hConsole, 115); 
			offset_dir = left_dir;
			goto_debug_board(19); 
			printf("自动移动： ←"); 
			gotoXY(console_corner); 
			break;
		case 77:
			SetConsoleTextAttribute(hConsole, 115); 
			offset_dir = right_dir;
			goto_debug_board(19); 
			printf("自动移动： →"); 
			gotoXY(console_corner);
			break;
		case 80: 
			SetConsoleTextAttribute(hConsole, 115); 
			offset_dir = down_dir; goto_debug_board(19); 
			printf("自动移动： ↓"); 
			gotoXY(console_corner); 
			break;
		case 'p':
			map_done = 1;
			break;
		default:
			break;
		}
	}

	SetConsoleTextAttribute(hConsole, 115);
	return SUCCESS;
}

static void map_maker_info_board()
{
	SetConsoleTextAttribute(hConsole, 115);

	int i = 1;
	ENDL(i);
	printf("操作说明");

	ENDL(i + 3);
	printf("移动");
	ENDL(i + 5);
	printf("	w a s d");

	ENDL(i + 7);
	printf("添加砖块(k键设置空白)");
	ENDL(i + 9);
	printf("	j键  ▓▓▓");
	ENDL(i + 10);
	printf("	     ▓▓▓");
	ENDL(i + 11);
	printf("	     ▓▓▓");
	ENDL(i + 13);
	printf("	k键  ■■■");
	ENDL(i + 14);
	printf("	     ■■■");
	ENDL(i + 15);
	printf("	     ■■■");

	ENDL(i + 18);
	printf("自动移动：off");
	ENDL(i + 20);
	printf("	→键  向右");
	ENDL(i + 21);
	printf("	←键  向左");
	ENDL(i + 22);
	printf("	↑键  向上");
	ENDL(i + 23);
	printf("	↓键  向下");
	ENDL(i + 25);
	printf("	i 键  关闭自动移动");

	ENDL(i + 28);
	printf("保存地图");
	ENDL(i + 30);
	printf("	p 键");

	SetConsoleTextAttribute(hConsole, 115);
}

void fix_map_maker_on_wall(Pos p, Wall * w, Wall_type wt)
{
	Pos p_tmp;
	p_tmp.row = (p.row - 1) * WALL_META_ROW + 1;
	p_tmp.column = (p.column - 1) * WALL_META_COLUMN + 1;

	for (int i = 0; i < WALL_META_ROW; i++)
	{
		for (int j = 0; j < WALL_META_COLUMN; j++)
		{
			gotoXY(p_tmp);
			SetConsoleTextAttribute(hConsole, 115);

			Wall_type wt_tmp = wt;
			if (no_wall == wt_tmp)
				printf(NO_WALL);
			else if (bricks == wt_tmp)
				printf(BRICKS);
			else if (hard_wall == wt_tmp)
				printf(HARD_WALL);
			else if (heart == wt_tmp)
				printf(HEART);

			//fix wt on wall
			w->wall[p_tmp.row][p_tmp.column] = wt;

			p_tmp.column++;
		}
		p_tmp.column -= WALL_META_COLUMN;
		p_tmp.row++;
	}
}

void print_map_maker_meta(Pos p, Wall * w)
{
	Pos p_tmp;
	p_tmp.row = (p.row - 1) * WALL_META_ROW + 1;
	p_tmp.column = (p.column - 1) * WALL_META_COLUMN + 1;

	for (int i = 0; i < WALL_META_ROW; i++)
	{
		for (int j = 0; j < WALL_META_COLUMN; j++)
		{
			gotoXY(p_tmp);
			SetConsoleTextAttribute(hConsole, 243);

			Wall_type wt_tmp = w->wall[p_tmp.row][p_tmp.column];
			if (no_wall == wt_tmp)
				printf(NO_WALL);
			else if (bricks == wt_tmp)
				printf(BRICKS);
			else if (hard_wall == wt_tmp)
				printf(HARD_WALL);
			else if (heart == wt_tmp)
				printf(HEART);

			p_tmp.column++;
		}
		p_tmp.column -= WALL_META_COLUMN;
		p_tmp.row++;
	}
}

void eli_map_maker_meta(Pos p, Wall * w)
{
	Pos p_tmp;
	p_tmp.row = (p.row - 1) * WALL_META_ROW + 1;
	p_tmp.column = (p.column - 1) * WALL_META_COLUMN + 1;

	for (int i = 0; i < WALL_META_ROW; i++)
	{
		for (int j = 0; j < WALL_META_COLUMN; j++)
		{
			gotoXY(p_tmp);
			SetConsoleTextAttribute(hConsole, 115);

			Wall_type wt_tmp = w->wall[p_tmp.row][p_tmp.column];
			if (no_wall == wt_tmp)
				printf(NO_WALL);
			else if (bricks == wt_tmp)
				printf(BRICKS);
			else if (hard_wall == wt_tmp)
				printf(HARD_WALL);
			else if (heart == wt_tmp)
				printf(HEART);

			p_tmp.column++;
		}
		p_tmp.column -= WALL_META_COLUMN;
		p_tmp.row++;
	}
}

//存储地图
Status save_map(Wall * w)
{
	FILE * fp = fopen(FILE_NAME, "a+");
	fwrite(w, sizeof(Wall), 1, fp);
	fclose(fp);
	return 1;
}

//**************************************

void fix_wall_meta(Pos big_corner, Wall_type wt, Wall * w)
{
	Pos corner;
	corner.row = (big_corner.row - 1) * WALL_META_ROW + 1;
	corner.column = (big_corner.column - 1) * WALL_META_COLUMN + 1;

	Pos c_tmp = corner;

	for (int i = 0; i < WALL_META_ROW; i++)
	{
		for (int j = 0; j < WALL_META_COLUMN; j++)
		{
			gotoXY(c_tmp);
			switch (wt)
			{
			case hard_wall:
				//SetConsoleTextAttribute(hConsole, 7);
				printf(HARD_WALL);
				//SetConsoleTextAttribute(hConsole, 7);
				w->wall[c_tmp.row][c_tmp.column] = wt;
				break;
			case bricks:
				//SetConsoleTextAttribute(hConsole, 4);
				printf(BRICKS);
				//SetConsoleTextAttribute(hConsole, 7);
				w->wall[c_tmp.row][c_tmp.column] = wt;
				break;
			default:
				break;
			}
			c_tmp.column++;
		}
		c_tmp.row++;
		c_tmp.column = corner.column;
	}

}

void rectangle(Pos big_corner, short big_row, short big_column, Wall_type wt, Wall * w, bool mirror)
{
	Pos bc_tmp = big_corner;

	for (int i = 0; i < big_row; i++)
	{
		for (int j = 0; j < big_column; j++)
		{
			fix_wall_meta(bc_tmp, wt, w);
			bc_tmp.column++;
		}
		bc_tmp.row++;
		bc_tmp.column = big_corner.column;
	}

	if (mirror)
	{
		rectangle_mirror(big_corner, big_row, big_column, wt, w);
	}
	
}

void rectangle_mirror(Pos big_corner, short big_row, short big_column, Wall_type wt, Wall * w)
{
	Pos bc_tmp = big_corner;
	bc_tmp.column = 14 - big_corner.column;

	for (int i = 0; i < big_row; i++)
	{
		for (int j = 0; j < big_column; j++)
		{
			fix_wall_meta(bc_tmp, wt, w);
			bc_tmp.column--;
		}
		bc_tmp.row++;
		bc_tmp.column = 14 - big_corner.column;
	}
}

void map_1(Wall * wall)
{
	//五个bricks柱

	Pos big_corner = {
		2, 2
	};
	Pos big_area = {
		10, 1
	};
	rectangle(big_corner, big_area.row, big_area.column, bricks, wall, 0);

	big_corner.column += 3;
	for (int i = 0; i < 3; i++)
	{
		rectangle(big_corner, big_area.row, big_area.column, bricks, wall, 0);
		big_corner.column += 2;
	}

	big_corner.column += 1;
	rectangle(big_corner, big_area.row, big_area.column, bricks, wall, 0);

	//左右两个hard_wall块

	big_corner.row = 7;
	big_corner.column = 1;
	rectangle(big_corner, 1, 1, hard_wall, wall, 1);

	//4个 1 : 2 hard_wall块

	big_corner.row = 5;
	big_corner.column = 3;
	rectangle(big_corner, 1, 2, hard_wall, wall, 1);

	big_corner.row = 9;
	rectangle(big_corner, 1, 2, hard_wall, wall, 1);

	//中间的bricks大块

	big_corner.row = 5;
	big_corner.column = 6;
	rectangle(big_corner, 5, 1, bricks, wall, 1);

	//中间嵌入几个hard_wall

	big_corner.row = 7;
	big_corner.column = 7;
	rectangle(big_corner, 1, 1, hard_wall, wall, 0);

}

#endif