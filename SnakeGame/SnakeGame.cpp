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

class Board;
class Snake;

void run(Board board, Snake snake);
void updateSnake(std::vector<short>& posx, std::vector<short>& posy, const short nextX, const short nextY, bool ateFood);

class Board {
	short BOARD_WIDTH;
	short BOARD_HEIGHT;
	std::vector<std::vector<short>> board;

private:
	void createFoodOnce() {
		srand(time(NULL));
		short xpos = rand() % (BOARD_WIDTH - 1);
		short ypos = rand() % (BOARD_HEIGHT - 1);
		while (board.at(ypos).at(xpos) == OBSTACLE || board.at(ypos).at(xpos) == SNAKE) {
			xpos = rand() % (BOARD_WIDTH - 1);
			ypos = rand() % (BOARD_HEIGHT - 1);
		}
		board.at(ypos).at(xpos) = FOOD;
	}

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

	void createFood() { //TODO: handle error if width or height are <= 1; fix bug where food sometimes does not spawn
		srand(time(NULL));
		createFoodOnce();
		if( rand() % 10 + 1 > 8 ){ //20% chance to create food twice
			createFoodOnce();
		}
	}

	void updateBoard(std::vector<short>& posx, std::vector<short>& posy) {
		short x, y, tempx, tempy;

		//store previous snake tail position
		tempx = posx.back();
		tempy = posy.back();

		//update board tiles
		for (int i = 0; i < posx.size(); i++) {
			x = posx.at(i);
			y = posy.at(i);
			board.at(y).at(x) = SNAKE;
		}
		board.at(tempy).at(tempx) = EMPTY; 	//turn last snake tile into empty
	}

	//TODO: fix seemingly harmless bug where snake is 1 tile smaller than what it should be.
	void moveSnake(short direction, std::vector<short>& posx, std::vector<short>& posy) {
		short next_posx, next_posy;
		bool ateFood = false;
		
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
			ateFood = false;
			updateSnake(posx, posy, next_posx, next_posy, ateFood);
			updateBoard(posx, posy);
			}
			else if (nextTile == FOOD) { //move entire body and increase it by 1
				ateFood = true;
				updateSnake(posx, posy, next_posx, next_posy, ateFood);
				createFood();
				updateBoard(posx, posy);
			}
	}

	void showBoard() {
		system("cls");
		short current_tile;
		for (short i = 0; i < BOARD_HEIGHT; i++) {
			for (short j = 0; j < BOARD_WIDTH; j++) {
				current_tile = board.at(i).at(j);
				if (current_tile == EMPTY) { 
					std::cout << " ";
				}
				else {
					std::cout << current_tile;
				}
			}
			std::cout << std::endl;
		}
	}
};

class Snake {
	short initial_size, direction, BOARD_WIDTH, BOARD_HEIGHT;

public:
	std::vector<short> posx;
	std::vector<short> posy; //the board class mutates these vectors
	short current_dir;

	Snake(const short board_width, const short board_height) {
		BOARD_WIDTH = board_width;
		BOARD_HEIGHT = board_height;
		current_dir = RIGHT;
		initializeSnake();
	}

	void initializeSnake() {
		direction = RIGHT;
		initial_size = 3;
		short currx = BOARD_WIDTH / 2, curry = BOARD_HEIGHT / 2;
		posx.push_back(currx);
		posy.push_back(curry);
		
		for (short i = 1; i < initial_size; i++) {
			currx -= 1;
			posx.push_back(currx);
			posy.push_back(curry);
		}
	}
};

int main()
{
	//srand(time(NULL));
	std::cout << "Welcome to the Snake game" << std::endl;
	std::cout << "Select board size. Enter 1 for small, 2 for medium or 3 for large" << std::endl;
	char size;
	std::cin >> size;
	short BOARD_WIDTH;
	short BOARD_HEIGHT;
	while (size != '1' && size != '2' && size != '3') {
		std::cout << "Invalid option. Press 1, 2 or 3 to choose" << std::endl;
		std::cin >> size;
		std::cout << size << std::endl;
	}
	switch (size) {
		case 49: //'1'
			BOARD_WIDTH = 20;
			BOARD_HEIGHT = 10;
			break;
		case 50: //'2'
			BOARD_WIDTH = 30;
			BOARD_HEIGHT = 15;
			break;
		case 51: //'3'
			BOARD_WIDTH = 40;
			BOARD_HEIGHT = 20;
			break;
	}

	Board board = Board(BOARD_WIDTH, BOARD_HEIGHT);
	Snake snake = Snake(BOARD_WIDTH, BOARD_HEIGHT);

	setup();
	run(board, snake);

	//std::cout << "END" << std::endl;
	//std::cin >> size;
	//while (!getchar()) {}
	return 0;
}

void run(Board board, Snake snake) {
	double startTime, endTime;
	startTime = clock();
	endTime = 0;
	board.moveSnake(RIGHT, snake.posx, snake.posy);
	board.showBoard();
	while (board.alive) {
		if ((endTime - startTime)/(double)CLOCKS_PER_SEC >= 0.3) {
			startTime = clock();
			endTime = 0;
			if (GetAsyncKeyState(VK_RIGHT) != 0 && snake.current_dir != LEFT) {
				board.moveSnake(RIGHT, snake.posx, snake.posy);
				snake.current_dir = RIGHT;
			}
			else if (GetAsyncKeyState(VK_LEFT) != 0 && snake.current_dir != RIGHT) {
				board.moveSnake(LEFT, snake.posx, snake.posy);
				snake.current_dir = LEFT;
			}
			else if (GetAsyncKeyState(VK_DOWN) != 0 && snake.current_dir != UP) {
				board.moveSnake(DOWN, snake.posx, snake.posy);
				snake.current_dir = DOWN;
			}
			else if (GetAsyncKeyState(VK_UP) != 0 && snake.current_dir != DOWN) {
				board.moveSnake(UP, snake.posx, snake.posy);
				snake.current_dir = UP;
			}
			else { //no button pressed, continue in the same direction
				board.moveSnake(snake.current_dir, snake.posx, snake.posy);
			}
			board.showBoard();
		}
		endTime = clock();
	}
}

void updateSnake(std::vector<short>& posx, std::vector<short>& posy, const short nextX, const short nextY, bool ateFood) {
	short previousx, previousy, next_posx = nextX, next_posy = nextY;

	if (ateFood) {
		posx.push_back(posx.back());
		posy.push_back(posy.back());
	}

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
}