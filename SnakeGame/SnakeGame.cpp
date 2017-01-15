#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

void setup() {

}

void run() {

}

const enum BOARD_ELEMENTS { OBSTACLE, FOOD, EMPTY, SNAKE };

class Board {
	const short BOARD_WIDTH = 60;
	const short BOARD_HEIGHT = 30;
	std::vector<std::vector<short>> board;

public:
	Board() {
		board.resize(BOARD_HEIGHT, std::vector<short>(BOARD_WIDTH));
		for (short i = 0; i < BOARD_HEIGHT; i++) {
			for (short j = 0; j < BOARD_WIDTH; j++) {
				if (i == 0 || i == (BOARD_HEIGHT - 1) || j == 0 || j == (BOARD_WIDTH - 1)) {
					board.at(i).at(j) = OBSTACLE;
				}
				else {
					board.at(i).at(j) = EMPTY;
				}
			}
		}
	}

	void showBoard() {
		for (short i = 0; i < BOARD_HEIGHT; i++) {
			for (short j = 0; j < BOARD_WIDTH; j++) {
				std::cout << board.at(i).at(j);
			}
			std::cout << std::endl;
		}
	}

};

class Snake {
	short size;

	void move() {

	}

	void initialize() {

	}
};

int main()
{
	Board board = Board();
	Snake snake = Snake();

	setup();
	run();

	board.showBoard();

	while (!getchar()) {}
	return 0;
}
