#include <stdio.h>
#include <stdint.h>

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
}

int main(void) {
	print_board();
}