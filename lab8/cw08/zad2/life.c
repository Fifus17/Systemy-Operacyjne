#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode
	int no_threads = 300;

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;

	init_grid(foreground);
	init_threads(foreground, background, no_threads);

	while (true)
	{
		draw_grid(foreground);

		// Step simulation
		update_grid(foreground, background, no_threads);
		usleep(500 * 1000);
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
