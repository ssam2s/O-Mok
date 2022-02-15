#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#pragma warning(disable:4996)

#define KEY_UP (256 + 72)
#define KEY_DOWN (256 + 80)
#define KEY_LEFT (256 + 75)
#define KEY_RIGHT (256 + 77)
#define KEY_ESC 27
#define KEY_SPACE 32
#define MAX 19*2

int GetKey(void);
void Draw_Board(int(*black)[MAX / 2], int(*white)[MAX / 2]);
void GotoXY(int x, int y);
void Draw_cur(int x, int y, int(*black)[MAX / 2], int(*white)[MAX / 2]);
int Win(int x, int y, int(*black)[MAX / 2], int(*white)[MAX / 2]);

struct current_axis
{
	int x;
	int y;
}cur_axis;

int main(void)
{
	system("mode CON COLS=25 LINES =60");

	CONSOLE_CURSOR_INFO ci = { 100, FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
	ci.bVisible = FALSE;

	int black_stone[MAX][MAX / 2];
	int white_stone[MAX][MAX / 2];
	int order = 0;					// 0 = 흑 순서, 1 = 백 순서
	Draw_Board(black_stone, white_stone);

	GotoXY(42, 1);
	printf("< 오목 게임 >");
	GotoXY(40, 3);
	printf("방향키를 눌러 이동");
	GotoXY(39, 5);
	printf("⊙ : 해당 위치 착수");
	GotoXY(39, 6);
	printf("○ : 흑");
	GotoXY(39, 7);
	printf("● : 백");
	GotoXY(12, 19);
	printf("Space Bar : 착수");
	GotoXY(12, 21);
	printf("현재 차례 : ○");

	cur_axis.x = MAX / 2 - 1;
	cur_axis.y = 9;
	GotoXY(cur_axis.x, cur_axis.y);
	printf("⊙");

	while (1)
	{
		while (1)
		{
			if (_kbhit())
			{
				int key = GetKey();

				if (key == KEY_SPACE)
				{
					if (order == 0)
					{
						if (white_stone[cur_axis.x][cur_axis.y] == 1 || black_stone[cur_axis.x][cur_axis.y] == 1)
						{
							break;
						}
						else
						{
							black_stone[cur_axis.x][cur_axis.y] = 1;
							GotoXY(cur_axis.x, cur_axis.y);
							printf("○");
							if (Win(cur_axis.x, cur_axis.y, black_stone, white_stone) == 1)
							{
								GotoXY(12, 20);
								printf("※ 흑 승리 ※");
								return 0;
							}
							GotoXY(12, 21);
							printf("현재 차례 : ●");
							order = 1;
							break;
						}
					}

					if (order == 1)
					{
						if (white_stone[cur_axis.x][cur_axis.y] == 1 || black_stone[cur_axis.x][cur_axis.y] == 1)
						{
							break;
						}
						else
						{
							white_stone[cur_axis.x][cur_axis.y] = 1;
							GotoXY(cur_axis.x, cur_axis.y);
							printf("●");
							if (Win(cur_axis.x, cur_axis.y, black_stone, white_stone) == 1)
							{
								GotoXY(12, 20);
								printf("※ 백 승리 ※");
								return 0;
							}
							GotoXY(12, 21);
							printf("현재 차례 : ○");
							order = 0;
							break;
						}
					}
				}
				else if (key == KEY_LEFT)
				{
					if (cur_axis.x == 0)
						break;
					cur_axis.x -= 2;
					Draw_cur(cur_axis.x, cur_axis.y, black_stone, white_stone);
				}
				else if (key == KEY_RIGHT)
				{
					if (cur_axis.x == MAX - 2)
						break;
					cur_axis.x += 2;
					Draw_cur(cur_axis.x, cur_axis.y, black_stone, white_stone);
				}
				else if (key == KEY_UP)
				{
					if (cur_axis.y == 0)
						break;
					cur_axis.y -= 1;
					Draw_cur(cur_axis.x, cur_axis.y, black_stone, white_stone);
				}
				else if (key == KEY_DOWN)
				{
					if (cur_axis.y == MAX / 2 - 1)
						break;
					cur_axis.y += 1;
					Draw_cur(cur_axis.x, cur_axis.y, black_stone, white_stone);
				}
				else if (key == KEY_ESC)
				{
					GotoXY(12, 22);
					printf("※ 게임 종료 ※");
					return 0;
				}
			}
		}
	}
}

void GotoXY(int x, int y)			// COORD 구조체 변수를 통해 이동할 위치 설정
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int GetKey(void)
{
	int ch = _getch();

	if (ch == 0 || ch == 224)		// 방향키의 경우 0 또는 224의 값이 먼저 입력됨
		ch = 256 + _getch();		// 그 다음에 해당 방향키에 따라 72(Up), 
									// 80(Down), 75(Left), 77(Right) 값이 입력됨
	return ch;
}

void Draw_cur(int x, int y, int(*black)[MAX / 2], int(*white)[MAX / 2])
{
	GotoXY(x, y);
	Draw_Board(black, white);
	GotoXY(x, y);
	printf("⊙");
}

void Draw_Board(int(*black)[MAX / 2], int(*white)[MAX / 2])
{
	for (int i = 0; i < MAX / 2; i++)
	{
		if (i == 0)
		{
			GotoXY(0, 0);
			printf("┌─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┬─┐");
		}
		else if (i == MAX / 2 - 1)
		{
			printf("\n└─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┘");
		}
		else
		{
			GotoXY(0, i);
			printf("├─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┤");
		}

	}

	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX / 2; j++)
		{
			if (black[i][j] == 1 && white[i][j] != 1)
			{
				GotoXY(i, j);
				printf("○");
			}

			if (white[i][j] == 1 && black[i][j] != 1)
			{
				GotoXY(i, j);
				printf("●");
			}
		}
	}
}

int Win(int x, int y, int(*black)[MAX / 2], int(*white)[MAX / 2])
{
	int count = 0;

	for (int i = 0; i < MAX / 2; i++)			// 흑 가로
	{
		for (int j = 0; j < MAX; j++)
		{
			if (black[j][i] == 1)
			{
				count++;
			}
			else if (black[j][i] == 0 || white[j][i] == 1)
			{
				count = 0;
			}

			if (count == 5)
			{
				return 1;
			}
		}
		count = 0;
	}

	for (int i = 0; i < MAX; i++)				// 흑 세로
	{
		for (int j = 0; j < MAX / 2; j++)
		{
			if (black[i][j] == 1)
			{
				count++;
			}
			else if (black[i][j] == 0 || white[i][j] == 1)
			{
				count = 0;
			}

			if (count == 5)
			{
				return 1;
			}
		}
		count = 0;
	}

	for (int i = 0; i < MAX; i++)				// 흑 대각선
	{
		for (int j = 0; j < MAX / 2; j++)
		{
			if (black[i][j] == 1)
			{
				if (black[i + 2][j + 1] == 1 && black[i + 4][j + 2] == 1 && black[i + 6][j + 3] == 1 && black[i + 8][j + 4] == 1)
				{
					return 1;
				}
				else if (black[i - 2][j + 1] == 1 && black[i - 4][j + 2] == 1 && black[i - 6][j + 3] == 1 && black[i - 8][j + 4] == 1)
				{
					return 1;
				}
			}
		}
	}

	for (int i = 0; i < MAX / 2; i++)			// 백 가로
	{
		for (int j = 0; j < MAX; j++)
		{
			if (white[j][i] == 1)
			{
				count++;
			}
			else if (white[j][i] == 0 || black[j][i] == 1)
			{
				count = 0;
			}

			if (count == 5)
			{
				return 1;
			}
		}
		count = 0;
	}

	for (int i = 0; i < MAX; i++)				// 백 세로
	{
		for (int j = 0; j < MAX / 2; j++)
		{
			if (white[i][j] == 1)
			{
				count++;
			}
			else if (white[i][j] == 0 || black[i][j] == 1)
			{
				count = 0;
			}

			if (count == 5)
			{
				return 1;
			}
		}
		count = 0;
	}

	for (int i = 0; i < MAX; i++)				// 백 대각선
	{
		for (int j = 0; j < MAX / 2; j++)
		{
			if (white[i][j] == 1)
			{
				if (white[i + 2][j + 1] == 1 && white[i + 4][j + 2] == 1 && white[i + 6][j + 3] == 1 && white[i + 8][j + 4] == 1)
				{
					return 1;
				}
				else if (white[i - 2][j + 1] == 1 && white[i - 4][j + 2] == 1 && white[i - 6][j + 3] == 1 && white[i - 8][j + 4] == 1)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}