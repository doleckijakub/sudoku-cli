#include <stdio.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>

#define PLAIN_NUMBER_MASK 63
#define USER_PLACED_MASK 64

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
	char c = getchar();
	switch(c) {
		case 'q': playing = false; break;

		case 'w': goto move_up;
		case 's': goto move_down;
		case 'd': goto move_right;
		case 'a': goto move_left;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':	
		case '9': {
			if(sudoku[cur_y][cur_x] == 0 || (sudoku[cur_y][cur_x] & USER_PLACED_MASK) != 0) { // cell is empty or number was placed by the user
				sudoku[cur_y][cur_x] = USER_PLACED_MASK | (c - '0');
			} else { // user attempts to override the initial board
				// TODO: prompt the user that they cannot alter that cell
			}
		} break;

		case '0':
		case ' ': {
			if((sudoku[cur_y][cur_x] & USER_PLACED_MASK) != 0) { // number was placed by the user
				sudoku[cur_y][cur_x] = 0;
			}
		} break;

		case '\033': { // ESC
			if(getchar() != '[') return;
			switch(getchar()) {
				case 'A': goto move_up;
				case 'B': goto move_down;
				case 'C': goto move_right;
				case 'D': goto move_left;
			}
		} break;
	}

	return;

	move_up: if(--cur_y < 0) cur_y = 0; return;
	move_down: if(++cur_y > 8) cur_y = 8; return;
	move_right: if(++cur_x > 8) cur_x = 8; return;
	move_left: if(--cur_x < 0) cur_x = 0; return;
}

#define ANSI_RESET "\033[0m"
#define ANSI_CYAN "\033[0;36m"

void print_cell(int x, int y) {
	uint8_t c = sudoku[y][x];
	bool is_cursor = cur_x == x && cur_y == y;
	printf("%c%s%c%s%c",
		" >"[is_cursor], // if cursor is on cell, show the left cursor part
		(sudoku[y][x] & USER_PLACED_MASK) != 0 ? ANSI_CYAN : ANSI_RESET, // if current number was placed by the user, set text color to cyan
		c == 0 ? ' ' : (PLAIN_NUMBER_MASK & c + '0'), // print the number or space if empty
		ANSI_RESET, // reset the text color (doesn't hurt if it was already reset before)
		" <"[is_cursor] // if cursor is on cell, show the right cursor part
	);
}

void print_board(void) {
	printf("\n");
	for(int y = 0; y < 9; ++y) {
		for(int x = 0; x < 9; ++x) {
			print_cell(x, y);
			
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
		print_board();
		process_input();
	}

	termios_cleanup();
	printf("\033[0;0H\033[2J"); // clear screen
}