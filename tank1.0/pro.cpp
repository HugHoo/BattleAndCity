#include <stdio.h>
#include <conio.h>
#include "public.h"
#include "wall.h"
#include "tank.h"
#include "tank_enermy_list.h"
#include "home.h"
#include "bullet.h"
#include "bullet_list.h"
#include "map_maker.h"
#include "tank_enermy.h"
#include "tank_enermy_list.h"
#include "index.h"
#include "map_list.h"

bool initial_console(char* pszWindowTitle, int nX, int nY);
void index_ui(short c_n);
Status change_page(short index);
bool start_game_page();
bool operation_page();
bool make_map_page();

int main()
{
	initial_console("坦克大战", 120, 50);

	system("color 73");

	//设置种子
	srand(time(0));

	index_ui(1);

}

void index_ui(short c_n)
{
	Pos corner = { 10, 19 };
	print_title(corner);

	short row_offset, column_offset;
	row_offset = 8;
	column_offset = 8;
	print_index_option(corner, row_offset, column_offset);

	Pos choice_pos = corner;
	choice_pos.row += row_offset + (c_n - 1) * 2;
	choice_pos.column += column_offset - 2;
	print_choice(choice_pos);

	gotoXY(console_corner);

	short game_quit = 0;
	//选项的序号
	short choice_num = c_n;
	while (!game_quit)
	{
		char ch;
		ch = _getch();

		switch (ch)
		{
		case 'w':
			change_choice(&choice_pos, -1, &choice_num);
			break;
		case 's':
			change_choice(&choice_pos, 1, &choice_num);
			break;
		case '\r':
			game_quit = change_page(choice_num);
			break;
		default:
			break;
		}
	}

	exit(0);
}

Status change_page(short index)
{
	CLS;
	switch (index)
	{
	case 1:
		start_game_page();
		CLS;
		index_ui(1);
		return 0;
		break;
	case 2:
		operation_page();
		CLS;
		index_ui(2);
		return 0;
		break;
	case 3:
		make_map_page();
		CLS;
		index_ui(3);
		return 0;
		break;
	case 4:
		return 1;
		break;
	default:
		break;
	}

	return 1;
}

bool start_game_page()
{
	Pos corner = { 18, 27 };
	short tmtl = TANK_MOVE_TIME_LINE;
	char * difficulty[2] = {
		"困难模式",
		"抽风模式"
	};
	short diff_status = menu(difficulty, 2, corner);
	if (diff_status == 2)
		tmtl -= 3;
	CLS;

	//初始化wall
	Wall wall;
	initial_wall(&wall);

	//初始化home
	Home home;
	initial_home(&home);
	fix_home_on_wall(&home, &wall);

	//*********************new
	corner = { 18, 27 };
	char * map_option[2] = {
		"随机地图",
		"本地地图"
	};
	short map_status = menu(map_option, 2, corner);

	Map_list map_list;
	initial_map_list(&map_list);
	if (map_status == 1)
	{
		CLS;
		//****暂时使用FILE_NAME中存储的地图
		FILE * fp = fopen(FILE_NAME, "r+");
		Wall map_temp;
		while (fread(&map_temp, sizeof(Wall), 1, fp) == 1)
		{
			map_push_back(&map_temp, &map_list);
		}
		//debug_board_map_list(&map_list);

		short rand_index = rand() % map_list.len;
		wall = map_at(rand_index, &map_list)->data;
	}
	else
	{
		CLS;
		FILE * fp = fopen(FILE_NAME, "r+");
		Wall map_temp;
		while (fread(&map_temp, sizeof(Wall), 1, fp) == 1)
		{
			map_push_back(&map_temp, &map_list);
		}
		Pos corner = { 16, 25 };
		short menu_status = menu_map(&map_list, corner);
		/*printf("%d", menu_status);
		system("pause");*/
		wall = map_at(menu_status - 1, &map_list)->data;
	}
	CLS;

	/*FILE * fp = fopen(FILE_NAME, "r+");
	fread(&wall, sizeof(Wall), 1, fp);*/

	print_wall(&wall);

	//打印自定义rectangle
	//map_1(&wall);

	//初始化tank
	Tank tk = initial_tank_own(&wall);
	print_tank_own(&tk);

	Bullet_list bullet_list;
	initial_bullet_list(&bullet_list);

	//初始化tank_list
	Tank_enermy_list tank_enermy_list;
	initial_tank_list(&tank_enermy_list);

	//bullet tmp
	Bullet temp;

	long failed = 0;
	unsigned long bullet_time_line = 0;
	unsigned long tank_time_line = 0;
	unsigned long tank_move_time_line = 0;
	unsigned long tank_enermy_time_line = 0;
	unsigned short enermy_num = 12;

	Tank_move_model move_model;
	initial_tank_move_model(&move_model);

	info_board_value(3, enermy_num);
	info_board_health(5, tk.health);

	while (failed < 1)
	{
		//tank_own时间线

		if (tank_time_line <= 0)
		{
			if (_kbhit())
			{
				tank_time_line = TANK_TIME_LINE;

				char ch = _getch();

				switch (ch)
				{
				case 'w':
					if (direction(&tk) == up_dir && move_success_own(&tk, &wall, up_dir))
					{
						move_own(&tk, up_dir, &wall);
					}
					else
					{
						turn_own(&tk, up_dir);
						/*if (move_success_own(&tk, &wall, up_dir))
						move_own(&tk, up_dir, &wall);*/
					}
					break;
				case 's':
					if (direction(&tk) == down_dir && move_success_own(&tk, &wall, down_dir))
					{
						move_own(&tk, down_dir, &wall);
					}
					else
					{
						turn_own(&tk, down_dir);
						/*if (move_success_own(&tk, &wall, down_dir))
						move_own(&tk, down_dir, &wall);*/
					}
					break;
				case 'a':
					if (direction(&tk) == left_dir && move_success_own(&tk, &wall, left_dir))
					{
						move_own(&tk, left_dir, &wall);
					}
					else
					{
						turn_own(&tk, left_dir);
						/*if (move_success_own(&tk, &wall, left_dir))
						move_own(&tk, left_dir, &wall);*/
					}
					break;
				case 'd':
					if (direction(&tk) == right_dir && move_success_own(&tk, &wall, right_dir))
					{
						move_own(&tk, right_dir, &wall);
					}
					else
					{
						turn_own(&tk, right_dir);
						/*if (move_success_own(&tk, &wall, right_dir))
						move_own(&tk, right_dir, &wall);*/
					}
					break;
				case 'j':
					//先初始化一个bullet，然后在另一个时间线上操作bullet
					initial_bullet(&temp, own, tk.head, direction(&tk));
					bullet_push_back(&temp, &bullet_list);

					break;
				default:
					break;
				}

				//debug_board_tank(&tk);

			}
		}
		else
		{
			--tank_time_line;
		}

		//tank_enermy initial时间线
		if (0 == ++tank_enermy_time_line % TANK_ENERMY_TIME_LINE)
		{
			/*
			生成tank_enermy
			如果tank_list的Len为0，随机初始化1个或2个
			如果tank_list的len为1，随机初始化1个或2个，初始化2个概率降低
			如果tank_list的len为2，随机初始化1个或2个，初始化2个概率降低
			如果tank_list的len为3，随机初始化1个，初始化1个概率降低
			如果tank_list的len为4，不初始化
			*/
			long ini_chance;
			//用于同时初始化两个Tank_enermy
			Tank_enermy te_tmp_arr[2];
			//用于初始化一个Tank_enermy
			Tank_enermy te_tmp;

			switch (tank_enermy_list.len)
			{
			case 0:
				ini_chance = rand() % 10;
				if (ini_chance < 5 && enermy_num >= 2)
				{
					//初始化2个
					initial_tank_enermy_2(te_tmp_arr, &wall);
					for (short i = 0; i < 2; i++)
					{
						if (initial_success(te_tmp_arr[i].center, &wall))
						{
							fix_tank_center_on_wall(te_tmp_arr[i].center, &wall);
							tank_push_back(&te_tmp_arr[i], &tank_enermy_list);
							print_tank_enermy(&te_tmp_arr[i]);
						}
					}
				}
				else if (ini_chance < 8 && enermy_num > 0)
				{
					//初始化1个
					initial_tank_enermy(&te_tmp, &wall);

					if (initial_success(te_tmp.center, &wall))
					{
						fix_tank_center_on_wall(te_tmp.center, &wall);
						tank_push_back(&te_tmp, &tank_enermy_list);
						print_tank_enermy(&te_tmp);
					}
				}
				else
				{
					break;
				}

				break;
			case 1:
				ini_chance = rand() % 10;
				if (ini_chance < 5 && enermy_num > 1)
				{
					//初始化1个
					initial_tank_enermy(&te_tmp, &wall);

					if (initial_success(te_tmp.center, &wall))
					{
						fix_tank_center_on_wall(te_tmp.center, &wall);
						tank_push_back(&te_tmp, &tank_enermy_list);
						print_tank_enermy(&te_tmp);
					}
				}
				else if (ini_chance < 8 && enermy_num > 2)
				{
					//初始化2个
					initial_tank_enermy_2(te_tmp_arr, &wall);
					for (short i = 0; i < 2; i++)
					{
						if (initial_success(te_tmp_arr[i].center, &wall))
						{
							fix_tank_center_on_wall(te_tmp_arr[i].center, &wall);
							tank_push_back(&te_tmp_arr[i], &tank_enermy_list);
							print_tank_enermy(&te_tmp_arr[i]);
						}
					}
				}
				else
				{
					break;
				}

				break;
			case 2:
				ini_chance = rand() % 10;
				if (ini_chance < 3 && enermy_num > 2)
				{
					//初始化1个
					initial_tank_enermy(&te_tmp, &wall);

					if (initial_success(te_tmp.center, &wall))
					{
						fix_tank_center_on_wall(te_tmp.center, &wall);
						tank_push_back(&te_tmp, &tank_enermy_list);
						print_tank_enermy(&te_tmp);
					}
				}
				else if (ini_chance < 5 && enermy_num > 3)
				{
					//初始化2个
					initial_tank_enermy_2(te_tmp_arr, &wall);
					for (short i = 0; i < 2; i++)
					{
						if (initial_success(te_tmp_arr[i].center, &wall))
						{
							fix_tank_center_on_wall(te_tmp_arr[i].center, &wall);
							tank_push_back(&te_tmp_arr[i], &tank_enermy_list);
							print_tank_enermy(&te_tmp_arr[i]);
						}
					}
				}
				else
				{
					break;
				}

				break;
			case 3:
				ini_chance = rand() % 10;
				if (ini_chance < 3 && enermy_num > 3)
				{
					//初始化1个
					initial_tank_enermy(&te_tmp, &wall);

					if (initial_success(te_tmp.center, &wall))
					{
						fix_tank_center_on_wall(te_tmp.center, &wall);
						tank_push_back(&te_tmp, &tank_enermy_list);
						print_tank_enermy(&te_tmp);
					}
				}
				else
				{
					break;
				}

				break;
			case 4:
				break;
			default:
				break;
			}

			//debug_board_tank_list(&tank_enermy_list);

		}

		//tank_enermy move时间线
		if ((0 == ++tank_move_time_line % tmtl) && (!is_tank_list_empty(&tank_enermy_list)))
		{
			Tank_node * h_tmp = tank_enermy_list.head;

			Dir d_tmp = direction_enermy(&h_tmp->data);

			while (h_tmp != tank_enermy_list.tail)
			{
				//if (check_model(&h_tmp->data.tank_move_model))
				if (0)
				{
					Tank_move_model * m_tmp = &h_tmp->data.tank_move_model;

					d_tmp = m_tmp->dir;

					if (m_tmp->turn_model == 1)
					{
						m_tmp->turn_model = 0;
						turn_enermy(&h_tmp->data, d_tmp);
					}
					else if (m_tmp->move_model == 1)
					{
						if (move_success_enermy(&h_tmp->data, &wall, d_tmp))
						{
							move_enermy(&h_tmp->data, d_tmp, &wall);
							m_tmp->move_model = 0;

							/*if (rand() % 10 < 7)
							{
							m_tmp->move_model = 1;
							}*/
						}
						/*else
						{
						h_tmp->data.tank_move_model.move_model = 1;
						Dir d_tmp = change_dir(direction_enermy(&h_tmp->data));
						h_tmp->data.tank_move_model.turn_model = 1;
						h_tmp->data.tank_move_model.dir = d_tmp;
						}*/
					}
					else
					{
						initial_bullet(&temp, enermy, h_tmp->data.head, d_tmp);
						bullet_push_back(&temp, &bullet_list);


						/*if (rand() % 6 < 2)
						h_tmp->data.tank_move_model.bullet_model = 1;*/
					}
				}
				else
				{
					short act_chance = rand() % 12;
					//**********TEST
					//goto_debug_board_with_value(1, act_chance);

					if (act_chance < 3)
					{
						initial_bullet(&temp, enermy, h_tmp->data.head, d_tmp);
						bullet_push_back(&temp, &bullet_list);

						if (rand() % 2 == 0)
							h_tmp->data.tank_move_model.bullet_model = 1;

					}
					else if (act_chance < 6)
					{
						if (move_success_enermy(&h_tmp->data, &wall, d_tmp))
							move_enermy(&h_tmp->data, d_tmp, &wall);
						else
						{
							h_tmp->data.tank_move_model.move_model = 1;
							Dir d_tmp = change_dir(direction_enermy(&h_tmp->data));
							h_tmp->data.tank_move_model.turn_model = 1;
							h_tmp->data.tank_move_model.dir = d_tmp;
						}
					}
					else if (act_chance < 9)
					{
						d_tmp = rand_dir();
						turn_enermy(&h_tmp->data, d_tmp);
					}
					else
						continue;
				}

				h_tmp = h_tmp->next;
			}
		}

		//debug_board_tank_list(&tank_enermy_list);

		//bullet时间线
		if (0 == ++bullet_time_line % BULLET_TIME_LINE)
		{
			//debug_board_bullet_list(&bullet_list);

			if (!is_bullet_list_empty(&bullet_list))
			{
				for (int i = 0; i < bullet_list.len; i++)
				{
					Tank_die td_tmp;
					td_tmp.index = -1;
					short bullet_status = move_bullet(&bullet_at(i, &bullet_list)->bullet_data, &wall, &tk, &tank_enermy_list, &td_tmp);

					if (bullet_status != SUCCESS)
					{
						bullet_delete_by_index(i, &bullet_list);
					}

					if (bullet_status == TANK_ENERMY_DIE)
					{
						//*******************暂时只有消失效果，稍后添加死掉的样式
						const Tank_enermy * te_tmp = &tank_at_const(td_tmp.index, &tank_enermy_list)->data;
						print_space_enermy(te_tmp);
						eli_tank_center_on_wall(te_tmp->center, &wall);
						tank_delete_by_index(td_tmp.index, &tank_enermy_list);

						--enermy_num;
						info_board_value(3, enermy_num);

						if (enermy_num <= 0)
						{
							info_board_str(18, "YOU WIN ! (Enter)");
							getchar();
							failed = 1;
						}
					}
					else if (bullet_status == TANK_OWN_DIE)
					{
						tk.health--;
						short health_tmp = tk.health;

						if (tk.health <= 0)
						{
							info_board_str(18, "GAME OVER");
							gotoXY(console_corner);
							Sleep(2000);
							failed = 1;
						}
						else
						{
							print_space_own(&tk);
							eli_tank_center_on_wall(tk.center, &wall);

							tk = initial_tank_own(&wall);
							tk.health = health_tmp;
							print_tank_own(&tk);
							info_board_health(5, tk.health);
						}
					}
					else if (bullet_status == BREAK_MY_HEART)
					{
						//printf("hahahah");
						info_board_str(18, "GAME OVER");
						gotoXY(console_corner);
						Sleep(2000);
						failed = 1;
					}
				}
			}
		}

		Sleep(8);
	}

	return 1;
}

bool operation_page()
{
	Pos corner = { 12, 23 };
	gotoXY(corner);
	printf("操作说明");
	
	corner.row += 3;
	gotoXY(corner);
	printf("	移动	w s a d");

	corner.row += 3;
	gotoXY(corner);
	printf("	射击	j 键");

	corner.row += 5;
	gotoXY(corner);
	printf("返回(Enter)");

	getchar();
	return 1;
}

bool make_map_page()
{
	//初始化wall
	Wall wall;
	initial_wall(&wall);
	//初始化home
	Home home;
	initial_home(&home);
	fix_home_on_wall(&home, &wall);
	//打印wall
	print_wall(&wall);
	//编辑地图
	short map_maker_status = map_maker_index(&wall);
	//输入地图名称
	CLS;
	Pos input_corner = { 18, 23 };
	gotoXY(input_corner);
	printf("地图名: ");
	scanf("%s", wall.map_name);
	CLS;
	//存储地图
	save_map(&wall);
	CLS;
	input_corner = { 18, 27 };
	gotoXY(input_corner);
	printf("保存成功");
	Sleep(1500);
	CLS;
	//system("pause");

	return 1;
}

bool initial_console(char* pszWindowTitle, int nX, int nY)
{
	// 设置控制台标题
	SetConsoleTitleA(pszWindowTitle);

	HANDLE hStdIn, hStdOut;
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = GetLargestConsoleWindowSize(hStdOut);        // 获取最大控制台窗口大小
	COORD BufferSize = { pos.X + 1, pos.Y + 1 };
	if (!SetConsoleScreenBufferSize(hStdOut, BufferSize))  // 设置控制台缓冲区大小
	{
		// 设置控制台窗口缓冲区失败
		printf("N 设置窗口缓冲区出错(%d,%d) %d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return false;
	}
	SMALL_RECT srctWindow = { 0, 0, nX, nY };
	if (!SetConsoleWindowInfo(hStdOut, true, &srctWindow))    // 设置控制台窗口大小
	{
		printf("N 设置窗口大小出错 %d\n", GetLastError());
		return false;
	}
	COORD Buffer = { nX + 1, nY + 1 };
	if (!SetConsoleScreenBufferSize(hStdOut, Buffer))     // 设置控制台缓冲区大小
	{
		// 设置控制台窗口缓冲区失败
		printf("N 设置窗口缓冲区出错(%d,%d) %d\n", BufferSize.X, BufferSize.Y, GetLastError());
		return false;
	}
	return true;
}