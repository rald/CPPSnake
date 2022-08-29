#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int LEN_MAX = 100;
int len = 1;
int x[LEN_MAX], y[LEN_MAX];
int xi = 1, yi = 0;
int px, py;
int key;
int rows, cols;
int fx, fy;

bool quit = false;
bool moving = true;

int kbhit(void)
{
	int ch = getch();

	if (ch != ERR)
	{
		ungetch(ch);
		return 1;
	}
	else
	{
		return 0;
	}
}

bool food()
{
	int nx = rand() % (cols-1);
	int ny = rand() % (rows-2);

	bool again;

	fx = nx;
	fy = ny;

	do
	{
		again = false;
		for (int i = 0; i < len; i++)
		{
			if (fx == x[i] && fy == y[i])
			{
				fx++;
				if (fx > cols-1)
				{
					fx = 0;
					fy++;
					if (fy > rows-2)
					{
						fy = 0;
					}
				}

				if (fx == nx && fy == ny)
				{
					return false;
				}

				again = true;
				break;
			}
		}
	} while (again);

	mvaddch(fy, fx, '0');
	refresh();

	return true;
}

int main(void)
{
	srand(time(NULL));

	initscr();
	raw();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);

	getmaxyx(stdscr, rows, cols);
	x[0] = 1;
	y[0] = 1;

	if (food())
	{
		while (!quit)
		{
			getmaxyx(stdscr, rows, cols);

			move(y[0], x[0]);
			addch('X');
			refresh();

			usleep(100000);

			if (moving)
			{
				if(len>0) {
					move(y[len - 1], x[len - 1]);
					addch(' ');
					refresh();
				}
			}


			if (x[0] == fx && y[0] == fy)
			{
				if (len < LEN_MAX)
				{
					len++;
					if (!food())
					{
						quit = true;
						continue;
					}
				}
			}

			if (!moving)
			{
				int n = 0;
				for (int i = 0; i < len; i++)
				{
					if (x[0] == x[i] && y[0] - 1 == y[i])
						n++;
					if (x[0] == x[i] && y[0] + 1 == y[i])
						n++;
					if (x[0] - 1 == x[i] && y[0] == y[i])
						n++;
					if (x[0] + 1 == x[i] && y[0] == y[i])
						n++;
				}
				if (n == 4)
				{
					quit = true;
					continue;
				}
			}

			if (kbhit())
			{
				key = getch();

				switch (key)
				{
				case 27:
					quit = true;
					continue;
					break;
				case 'w':
					if (yi != 1)
					{
						xi = 0;
						yi = -1;
					}
					break;
				case 's':
					if (yi != -1)
					{
						xi = 0;
						yi = 1;
					}
					break;
				case 'a':
					if (xi != 1)
					{
						xi = -1;
						yi = 0;
					}
					break;
				case 'd':
					if (xi != -1)
					{
						xi = 1;
						yi = 0;
					}
					break;
				}
			}

			moving = true;
			for (int i = 0; i < len; i++)
			{
				if (x[0] + xi == x[i] && y[0] + yi == y[i])
				{
					moving = false;
				}
			}

			if (moving)
			{
				for (int i = len - 1; i > 0; i--)
				{
					x[i] = x[i - 1];
					y[i] = y[i - 1];
				}

				x[0] += xi;
				y[0] += yi;
			}

			if (x[0] < 0)
				x[0] = cols-1;
				
			if (x[0] > cols-1)
				x[0] = 0;
				
			if (y[0] < 0)
				y[0] = rows - 2;
				
			if (y[0] > rows - 2)
				y[0] = 0;
		}
	}

	mvprintw(rows - 1, 1, "Game Over");
	refresh();
	curs_set(1);
	while (getch() != 27)
		;

	endwin();

	return 0;
}
