#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <Windows.h>

void setup() {

}

const enum BOARD_ELEMENTS { OBSTACLE, FOOD, EMPTY, SNAKE };
const enum DIRECTION {UP, DOWN, LEFT, RIGHT};

class Board {
	short BOARD_WIDTH;
	short BOARD_HEIGHT;
	std::vector<std::vector<short>> board;

public:
	bool alive;

	Board(const short width, const short height) {
		alive = true;
		BOARD_WIDTH = width;
		BOARD_HEIGHT = height;
		board.resize(BOARD_HEIGHT, std::vector<short>(BOARD_WIDTH));
		initializeBoard();
	}

	void initializeBoard() {
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
		createFood();
	}

	void createFood() {
		short xpos = rand() % (BOARD_WIDTH - 1);
		short ypos = rand() % (BOARD_HEIGHT - 1);
		while (board.at(ypos).at(xpos) == OBSTACLE || board.at(ypos).at(xpos) == SNAKE) {
			xpos = rand() % (BOARD_WIDTH - 1);
			ypos = rand() % (BOARD_HEIGHT - 1);
		}
		board.at(ypos).at(xpos) = FOOD;
	}

	void moveSnake(short direction, std::vector<short>& posx, std::vector<short>& posy) {
		short next_posx, next_posy;
		
		if (direction == RIGHT) {
			next_posx = posx.front() + 1;
			next_posy = posy.front();
		}
		else if (direction == LEFT) {
			next_posx = posx.front() - 1;
			next_posy = posy.front();
		}
		else if (direction == UP) {
			next_posx = posx.front();
			next_posy = posy.front() - 1;
		}
		else {
			next_posx = posx.front();
			next_posy = posy.front() + 1;
		}

		short nextTile = board.at(next_posy).at(next_posx);
		if (nextTile == OBSTACLE || nextTile == SNAKE) {
			alive = false;
		}
		if (nextTile == EMPTY) { //move entire body
			short x, y, previousx, previousy, tempx, tempy;
			//change board tiles
			tempx = posx.back();
			tempy = posy.back();
			for (int i = 0; i < posx.size(); i++) {
				x = posx.at(i);
				y = posy.at(i);
				board.at(y).at(x) = SNAKE;
			}
			board.at(tempy).at(tempx) = EMPTY; 	//turn last snake tile into empty

			//move snake
			previousx = posx.at(0);
			previousy = posy.at(0);
			posx.at(0) = next_posx;
			posy.at(0) = next_posy;
			for (int j = 1; j < posx.size(); j++) {
				next_posx = previousx;
				next_posy = previousy;
				previousx = posx.at(j);
				previousy = posy.at(j);
				posx.at(j) = next_posx;
				posy.at(j) = next_posy;
			}
			//append new head at next location, probably not efficient
			//posx.insert(posx.begin(),next_posx,1);
			//posy.insert(posy.begin(), next_posy, 1);
			//remove previous tail
			//posx.pop_back();
			//posy.pop_back();
		}
		else if (nextTile == FOOD) {
			//move entire body and increase it by 1
		}
		
		
		

	}

	void showBoard() {
		system("cls");
		for (short i = 0; i < BOARD_HEIGHT; i++) {
			for (short j = 0; j < BOARD_WIDTH; j++) {
				std::cout << board.at(i).at(j);
			}
			std::cout << std::endl;
		}
	}

};

class Snake {
	short size, direction, BOARD_WIDTH, BOARD_HEIGHT;

public:
	std::vector<short> posx;
	std::vector<short> posy; //the board class mutates these vectors

	Snake(const short board_width, const short board_height) {
		BOARD_WIDTH = board_width;
		BOARD_HEIGHT = board_height;
		initializeSnake();
	}

	void initializeSnake() {
		direction = RIGHT;
		size = 3;
		short currx = BOARD_WIDTH / 2, curry = BOARD_HEIGHT / 2;
		posx.push_back(currx);
		posy.push_back(curry);
		
		for (short i = 1; i < size; i++) {
			currx -= 1;
			posx.push_back(currx);
			posy.push_back(curry);
		}
	}

	void changeDirection() {

	}
};

void run(Board board, Snake snake);

int main()
{
	short BOARD_WIDTH = 10;
	short BOARD_HEIGHT = 10; //use width and height greater than ~ 4 to avoid issues for now
	srand(time(NULL));

	Board board = Board(BOARD_WIDTH, BOARD_HEIGHT);
	Snake snake = Snake(BOARD_WIDTH, BOARD_HEIGHT);

	//TODO: make movement impossible from left to right, up to down, etc to stop snake suicide

	setup();
	run(board, snake);

	std::cout << "END" << std::endl;
	while (!getchar()) {}
	return 0;
}

void run(Board board, Snake snake) {

	while (board.alive) {
		//getKeyPress()
		board.moveSnake(RIGHT, snake.posx, snake.posy);
		board.showBoard();
		Sleep(2000);
	}
}