#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#pragma comment(lib,"winmm.lib")

#define ROW 20
#define COL 20
#define NUM 50
#define SIZE 20

int count = 0; //click number
int map[ROW+2][COL+2]; //initial 0
IMAGE img[12];

void GameInt() 
{
	//random root
	srand((unsigned int)time(NULL));

	//initial array
	for (int i = 0; i < ROW+2; i++)
	{
		for (int j = 0; j < COL+2; j++)
		{
			map[i][j] = 0;
		}
	}

	//put mine in map and use -1 for mine
	int n = 0;
	while (n < NUM) 
	{
		int r = rand() % ROW+1;
		int c = rand() % COL+1;

		if (map[r][c] == 0)
		{
			map[r][c] = -1;
			n++;
		}
	}

	//full in
	for (int i = 1; i <= ROW; i++)
	{
		for (int j = 1; j <= COL; j++)
		{
			if (map[i][j] != -1)
			{
				for (int m = i - 1; m <= i + 1; m++)
				{
					for (int n = j - 1; n <= j + 1; n++)
					{
						if (map[m][n] == -1)
						{
							map[i][j]++;
						}
					}
				}
			}
		}
	}

	//put secrect
	for (int i = 1; i < ROW+1; i++)
	{
		for (int j = 1; j < COL+1; j++)
		{
			map[i][j] += 20;
		}
	}
}

void DrawGame() //print all of elements in 2-array
{
	for (int i = 1; i < ROW+1; i++)
	{
		for (int j = 1; j < COL+1; j++)
		{
			printf("%2d ", map[i][j]);
			if (map[i][j] == -1) //mine
			{
				putimage((i - 1) * SIZE, (j - 1) * SIZE, &img[9]);
			}
			else if (map[i][j] >= 0 && map[i][j] <= 8) //number pic
			{
				putimage((i - 1) * SIZE, (j - 1) * SIZE, &img[map[i][j]]);
			}
			else if (map[i][j] >= 19 && map[i][j] <= 28) //blank pic
			{
				putimage((i - 1) * SIZE, (j - 1) * SIZE, &img[10]);
			}
			else if (map[i][j] > 30) //mark pic
			{
				putimage((i - 1) * SIZE, (j - 1) * SIZE, &img[11]);
			}
		}
		printf("\n");
	}
}

void OpenZero(int r, int c)
{
	map[r][c] -= 20;
	count++;

	for (int m = r - 1; m <= r + 1; m++)
	{
		for (int n = c - 1; n <= c + 1; n++)
		{
			if ((m >= 1 && m <= ROW) && (n >= 1 && n <= COL)) //make sure in game area
			{
				if (map[m][n] >= 19 && map[m][n] <= 28) //make sure blank area
				{
					if (map[m][n] != 20)
					{
						map[m][n] -= 20;
						count++;
					}
					else
					{
						OpenZero(m, n);
					}
				}
			}
		}
	}
}

int PlayGame()
{
	MOUSEMSG msg = { 0 }; //define a mouse message
	int r, c;

	while (1)
	{
		msg = GetMouseMsg();

		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN: //flip blank pic 19-28
			r = msg.x / SIZE + 1;
			c = msg.y / SIZE + 1;

			if (map[r][c] >= 19 && map[r][c] <= 28) //blank
			{
				if (map[r][c] == 20) //0
				{
					OpenZero(r,c);
				}
				else
				{
					map[r][c] -= 20; //-1-8
					count++;
				}
			}
			return map[r][c];
			break;
		case WM_RBUTTONDOWN: //mark blank pic or cancell mark
			r = msg.x / SIZE + 1;
			c = msg.y / SIZE + 1;

			if (map[r][c] >= 19 && map[r][c] <= 28) //mark
			{
				map[r][c] += 50; //>30
			}
			else if (map[r][c] > 30) //cancell
			{
				map[r][c] -= 50;
			}
			return map[r][c];
			break;
		}
	}
}


int main()
{
	HWND hwnd = initgraph(ROW * SIZE, COL * SIZE, SHOWCONSOLE);

	mciSendString(L"open music.mp3 alias music", 0, 0, 0);
	mciSendString(L"play music repeat", 0, 0, 0);

	loadimage(&img[0], L"0.jpg", SIZE, SIZE);
	loadimage(&img[1], L"1.jpg", SIZE, SIZE);
	loadimage(&img[2], L"2.jpg", SIZE, SIZE);
	loadimage(&img[3], L"3.jpg", SIZE, SIZE);
	loadimage(&img[4], L"4.jpg", SIZE, SIZE);
	loadimage(&img[5], L"5.jpg", SIZE, SIZE);
	loadimage(&img[6], L"6.jpg", SIZE, SIZE);
	loadimage(&img[7], L"7.jpg", SIZE, SIZE);
	loadimage(&img[8], L"8.jpg", SIZE, SIZE);
	loadimage(&img[9], L"9.jpg", SIZE, SIZE);
	loadimage(&img[10], L"10.jpg", SIZE, SIZE);
	loadimage(&img[11], L"11.jpg", SIZE, SIZE);

	GameInt();

	while (1)
	{
		DrawGame();
		if (PlayGame() == -1)
		{
			DrawGame();
			MessageBox(hwnd, L"You lose!",L"",MB_OK);
			break;
		}

		if (ROW * COL - NUM == count)
		{
			DrawGame();
			MessageBox(hwnd, L"Nice! You Win!", L"", MB_OK);
			break;
		}
	}

	while (1);
	return 0;
}
