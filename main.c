#include <stdio.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>

uint8_t sudoku[9][9] = {
	{0, 0, 6, 3, 0, 7, 1, 0, 0},
	{0, 0, 0, 0, 4, 2, 0, 7, 0},
	{3, 0, 0, 0, 0, 0, 2, 0, 0},
	{6, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 3, 0, 8, 6, 0, 0, 0, 0},
	{7, 8, 2, 0, 0, 0, 0, 3, 4},
	{0, 9, 0, 1, 0, 0, 0, 0, 0},
	{2, 0, 7, 0, 0, 0, 0, 1, 0},
	{0, 4, 1, 0, 2, 8, 0, 6, 3}
};

struct termios saved_attr, new_attr;

bool playing = true;

int cur_x = 0;
int cur_y = 0;

void termios_setup(void) {
	tcgetattr(STDIN_FILENO, &saved_attr);
	new_attr = saved_attr;

	new_attr.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
}

void termios_cleanup(void) {
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attr);
}

void process_input(void) {
	switch(getchar()) {
		case 'q': playing = false; break;
	}
}

void print_board(void) {
	printf("\n");
	for(int y = 0; y < 9; ++y) {
		for(int x = 0; x < 9; ++x) {
			uint8_t c = sudoku[y][x];
			printf(" %c ", c == 0 ? ' ' : (c + '0'));
			if(x == 2 || x == 5) printf("|");
		}
		printf("\n");
		if(y == 2 || y == 5) printf(" --------+---------+--------\n");
	}
	printf("\n");

	printf("\033[13A\033[12D"); // move terminal cursor 13 up and 12 left
}

int main(void) {
	termios_setup();

	while(playing) {
		process_input();
		print_board();
	}

	termios_cleanup();
	printf("\033[0;0H\033[2J"); // clear screen
}