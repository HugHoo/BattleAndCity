#ifndef INDEX_H_

#define INDEX_H_

#include "public.h"

void print_title(Pos corner)
{
	bool title[5][21] = {
		{ 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1 },
		{ 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0 },
		{ 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0 },
		{ 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0 },
		{ 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 }
	};

	for (int i = 0; i < 5; i++)
	{
		gotoXY(corner);

		for (int j = 0; j < 21; j++)
		{
			if (title[i][j])
				printf("%c%c", 0xa8, 0x80);
				//printf(TITLE);
			else
				printf("  ");
		}
		corner.row++;
	}
}

void print_index_option(Pos corner, short row_offset, short column_offset)
{
	Pos c_tmp = corner;

	c_tmp.row += row_offset;
	c_tmp.column += column_offset;

	char * options[4] = {
		"开始游戏",
		"操作说明",
		"制作地图",
		"退出"
	};

	for (int i = 0; i < 4; i++)
	{
		gotoXY(c_tmp);
		printf("%s", options[i]);

		c_tmp.row += 2;
	}
}

void print_choice(Pos p)
{
	gotoXY(p);
	printf(INDEX_CHOICE);
}

void eli_choice(Pos p)
{
	gotoXY(p);
	printf(SPACE);
}

Pos change_choice(Pos * p, short plus, short * choice_num)
{
	eli_choice(*p);

	*choice_num += plus;

	if (*choice_num <= 0)
	{
		*choice_num = 4;
		p->row += 6;
	}
	else if (*choice_num > 4)
	{
		*choice_num = 1;
		p->row -= 6;
	}
	else
	{
		p->row += plus * 2;
	}

	print_choice(*p);

	gotoXY(console_corner);

	return *p;
}

#endif