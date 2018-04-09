#ifndef PUBLIC_H_

#define PUBLIC_H_

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ABS(x) ((x) >= 0 ? (x) : -(x))					//获取绝对值

#define ENDL printf("\n");								
#define CLS system("cls");
#define FILE_NAME "map.dat"								//地图存储文件位置

#define BULLET_TIME_LINE 3								//子弹时间线
#define TANK_ENERMY_TIME_LINE 300						//敌方坦克时间线
#define TANK_MOVE_TIME_LINE 8							//敌方坦克移动时间线
#define TANK_TIME_LINE 2								//坦克时间线

#define SQUARE "■"										//坦克身体
#define SPACE "  "								
	
#define TITLE "■"										//TANK标题
#define INDEX_CHOICE "■"								//选项形状

#define WALL_ROW 41										//墙壁行数
#define WALL_COLUMN 41									//墙壁列数		

#define HOME_ROW 6										//家园行数
#define HOME_COLUMN 7									//家园列数

#define WALL_META_ROW 3									//墙壁元列数
#define WALL_META_COLUMN 3								//墙壁元列数

#define FAIL 0
#define SUCCESS 1
#define DIE 2
#define TANK_OWN_DIE 3									//坦克死去状态
#define TANK_ENERMY_DIE 4								//敌方坦克死去状态
#define BREAK_MY_HEART 5								//家园被攻破状态
typedef short Status;

//定义坐标结构
struct Pos
{
	int row;
	int column;
};

//左下角角落
Pos console_corner = { 38, 1 };

//************************************ tank public data

//tank方向的enum
enum Dir
{
	up_dir,
	down_dir,
	left_dir,
	right_dir,
	error_dir
};

//定义tank类型
enum tank_type
{
	own,
	enermy
};

#define TANK_SQUARE_NUM 7		//坦克身体由7个元素构成

//定义玩家操作的坦克结构
struct Tank
{
	Pos center;
	Pos head;
	Pos body[TANK_SQUARE_NUM];
	short health;
};

//********************************** wall public data

#define NO_WALL "  "					//空地形状
#define HARDEST_WALL "■"				//墙壁形状
#define HARD_WALL "■"					//坚石形状
#define BRICKS "▓"						//砖墙形状
#define HEART "○"						//家园形状

//墙类型结构
enum Wall_type
{
	no_wall,
	hardest_wall,
	hard_wall,
	bricks,									//板砖
	heart,
	tank
};

//墙结构
struct Wall
{
	char map_name[30];
	Wall_type wall[WALL_ROW][WALL_COLUMN];
	int row_offset;
	int column_offset;
};

//将tank类型fix到wall上
Status fix_tank_center_on_wall(Pos center, Wall * w)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			w->wall[center.row + i][center.column + j] = tank;
		}
	}

	return SUCCESS;
}

//将tank类型从wall上elimate掉
Status eli_tank_center_on_wall(Pos center, Wall * w)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			w->wall[center.row + i][center.column + j] = no_wall;
		}
	}

	return SUCCESS;
}

//********************************** bullet public data

//子弹类型结构
enum bullet_type
{
	normal
};

//子弹结构
//之前bullet_type声明的变量名为type,tank_type声明的变量名为host，
struct Bullet
{
	bullet_type bullet_type;
	tank_type type;
	Pos center;
	Dir dir;
};

//********************************** tank_enermy public data

//敌人等级
enum enermy_level
{
	first_level,
	second_level,
	third_level
};

//敌方坦克移动模式
struct Tank_move_model
{
	short bullet_model;
	short move_model;
	short turn_model;
	Dir dir;
};

//敌方坦克结构
struct Tank_enermy
{
	Pos center;
	Pos head;
	Pos body[TANK_SQUARE_NUM];
	short health;
	tank_type type;
	enermy_level level;
	Tank_move_model tank_move_model;
};

//判断敌方坦克死去的标记结构
struct Tank_die
{
	short index;
};

//********************************** tank_enermy_list public data

#define TANK_ENERMY_MAX_LEN 4				//敌方坦克最大同屏数量

typedef Tank_enermy tank_data_type;

//敌方坦克链表结点结构
struct Tank_node
{
	tank_data_type data;
	Tank_node * front;
	Tank_node * next;
};

//敌方坦克链表
struct Tank_enermy_list
{
	Tank_node * head;
	Tank_node * tail;		//tail作为超尾
	unsigned short len;
};

//返回坦克链表中第index个坦克结点
const Tank_node * tank_at_const(short index, const Tank_enermy_list * tel)
{
	//****************之前写的tel->len == TANK_ENERMY_MAX_LEN 和 tel->len >= TANK_ENERMY_LEN
	//**************啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊!!!
	if (tel->len == 0)
		return NULL;

	const Tank_node * h_tmp = tel->head;

	short i = 0;
	while (i < index)
	{
		h_tmp = h_tmp->next;
		++i;
	}

	return h_tmp;
}

//************************************** map_list public data

typedef Wall map_data_type;

//地图结点
struct Map_node
{
	map_data_type data;
	Map_node * front;
	Map_node * next;
};

//地图链表
struct Map_list
{
	Map_node * head;
	Map_node * tail;
	unsigned short len;
};

//返回在地图链表中第index个地图结构
Map_node * map_at(short index, Map_list * ml)
{
	if (ml->len == 0)
		return FAIL;

	Map_node * h_tmp = ml->head;

	short i = 0;
	while (i < index)
	{
		h_tmp = h_tmp->next;
		++i;
	}

	return h_tmp;
}

//*********************************** public function

//获取控制台句柄
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//移动到坐标点
void gotoXY(const Pos p)
{
	COORD temp;
	temp.X = p.column * 2;				//别忘了*2
	temp.Y = p.row;
	SetConsoleCursorPosition(hConsole, temp);
}

//移动到调试板
void goto_debug_board(int i)
{
	Pos temp;
	temp.row = i;
	temp.column = WALL_COLUMN + 2;
	gotoXY(temp);
}

//功能类似同上，加了个value
void goto_debug_board_with_value(int i, short value)
{
	Pos temp;
	temp.row = i;
	temp.column = WALL_COLUMN + 1;
	gotoXY(temp);
	printf("%d           ", value);
}

//随机产生方向
Dir rand_dir()
{
	short dir_chance = rand() % 12;
	if (dir_chance < 2)
		return up_dir;
	else if (dir_chance < 5)
		return down_dir;
	else if (dir_chance < 8)
		return left_dir;

	return right_dir;
}

//改变非d得其他方向
Dir change_dir(Dir d)
{
	short dir_chance = rand() % 9;
	switch (d)
	{
	case up_dir:
		if (dir_chance < 3)
			return right_dir;
		else if (dir_chance < 6)
			return down_dir;
		else if (dir_chance < 9)
			return left_dir;
		break;
	case down_dir:
		if (dir_chance < 3)
			return right_dir;
		else if (dir_chance < 6)
			return up_dir;
		else if (dir_chance < 9)
			return left_dir;
		break;
	case left_dir:
		if (dir_chance < 3)
			return up_dir;
		else if (dir_chance < 5)
			return right_dir;
		else if (dir_chance < 9)
			return down_dir;
		break;
	case right_dir:
		if (dir_chance < 3)
			return up_dir;
		else if (dir_chance < 6)
			return down_dir;
		else if (dir_chance < 9)
			return left_dir;
		break;
	}
}

//批量生成菜单函数，只需三个参数生成动态菜单
short menu(char * option[], short option_num, Pos corner)
{
	Pos cor_tmp = corner;
	for (int i = 0; i < option_num; i++)
	{
		gotoXY(cor_tmp);
		printf("%s", option[i]);

		cor_tmp.row += 2;
	}

	Pos choice_corner = corner;
	choice_corner.column -= 2;
	gotoXY(choice_corner);
	printf(INDEX_CHOICE);

	gotoXY(console_corner);

	short choice_num = 1;
	while (1)
	{
		char ch = _getch();
		switch (ch)
		{
		case 'w':
			--choice_num;
			if (choice_num <= 0)
			{
				choice_num = option_num;
				gotoXY(choice_corner);
				printf(SPACE);
				choice_corner.row += 2 * (option_num - 1);
				gotoXY(choice_corner);
				printf(INDEX_CHOICE);

				gotoXY(console_corner);
			}
			else
			{
				gotoXY(choice_corner);
				printf(SPACE);
				choice_corner.row -= 2;
				gotoXY(choice_corner);
				printf(INDEX_CHOICE);

				gotoXY(console_corner);
			}
			break;
		case 's':
			++choice_num;
			if (choice_num > option_num)
			{
				choice_num = 1;
				gotoXY(choice_corner);
				printf(SPACE);
				choice_corner.row -= 2 * (option_num - 1);
				gotoXY(choice_corner);
				printf(INDEX_CHOICE);

				gotoXY(console_corner);
			}
			else
			{
				gotoXY(choice_corner);
				printf(SPACE);
				choice_corner.row += 2;
				gotoXY(choice_corner);
				printf(INDEX_CHOICE);

				gotoXY(console_corner);
			}
			break;
		case '\r':
			return choice_num;
			break;
		}
	}

	return 0;
}

//为map_list定制的菜单生成器
int menu_map(Map_list * ml, Pos p)
{
	int len = 5;
	int page_now = 1;
	int choice_num = 1;

	while (1)
	{
		Pos p_tmp = p;
		p_tmp.row += 2 * len + 1;
		//p_tmp.column -= 1;
		gotoXY(p_tmp);
		printf("上一页(a)   下一页(d)");
		p_tmp.row = p.row - 3;
		//p_tmp.column -= 1;
		gotoXY(p_tmp);
		printf("第%d页   共%d页", page_now, (ml->len - 1) / len + 1);
		p_tmp = p;

		int map_index = (page_now - 1) * len + 1;
		choice_num = map_index;

		int page_now_len;
		if (map_index + len - 1 > ml->len)
			page_now_len = ml->len;
		else
			page_now_len = len + map_index - 1;

		for (int i = map_index; i <= page_now_len; i++)
		{
			gotoXY(p_tmp);
			printf("%s", map_at(i - 1, ml)->data.map_name);
			p_tmp.row += 2;
		}

		Pos choice_corner = p;
		choice_corner.column -= 2;
		gotoXY(choice_corner);
		printf(INDEX_CHOICE);

		gotoXY(console_corner);

		bool change_page = 0;
		while (!change_page)
		{
			char ch = _getch();
			switch (ch)
			{
			case 'w':
				gotoXY(choice_corner);
				printf(SPACE);
				--choice_num;
				if (choice_num < map_index)
				{
					choice_num = map_index + len - 1;
					choice_corner.row += 2 * (len - 1);
				}
				else
					choice_corner.row -= 2;
				gotoXY(choice_corner);
				printf(INDEX_CHOICE);
				gotoXY(console_corner);
				break;
			case 's':
				gotoXY(choice_corner);
				printf(SPACE);
				++choice_num;
				if (choice_num > map_index + len - 1)
				{
					choice_num = map_index;
					choice_corner.row -= 2 * (len - 1);
				}
				else
					choice_corner.row += 2;
				gotoXY(choice_corner);
				printf(INDEX_CHOICE);
				gotoXY(console_corner);
				break;
			case 'a':
				if (page_now != 1)
				{
					CLS;
					--page_now;
					change_page = 1;
				}
				break;
			case 'd':
				if (page_now != (ml->len - 1) / len + 1)
				{
					CLS;
					++page_now;
					change_page = 1;
				}
				break;
			case '\r':
				return choice_num;
				break;
			default:
				break;
			}
		}

	}

	return 0;
}

void info_board_value(int i, int value)
{
	goto_debug_board(i);
	printf("敌人剩余： %d	", value);
}

void info_board_health(int i, int value)
{
	goto_debug_board(i);
	printf("生命数： %d	", value);
}

void info_board_str(int i, char * str)
{
	goto_debug_board(i);
	printf("%s	", str);
}

#endif