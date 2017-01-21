#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <Windows.h>

const enum BOARD_ELEMENTS { OBSTACLE, FOOD, EMPTY, SNAKE };
const enum DIRECTION {UP, DOWN, LEFT, RIGHT};
const enum GAME_OVER_MENU {RETRY_SAME, RETRY_DIFFERENT, QUIT};

class Board;
class Snake;

void run(Board board, Snake snake, float difficulty);
void updateSnake(std::vector<short>& posx, std::vector<short>& posy, const short nextX, const short nextY, bool ateFood);
void validateOption(char& input);

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

	bool hasFoodTile() {
		for (short i = 1; i < BOARD_HEIGHT-1; i++) {
			for (short j = 1; j < BOARD_WIDTH-1; j++) {
				if (board.at(i).at(j) == FOOD) {
					return true;
				}
			}
		}
		return false;
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

	void createFood() { //TODO: handle error if board width or height are <= 1; fix bug where food sometimes does not spawn
		srand(time(NULL));
		createFoodOnce();
		//if( rand() % 10 + 1 > 8 ){ //20% chance to create food twice
			//createFoodOnce();
		//}
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
		board.at(tempy).at(tempx) = EMPTY; 	//turn previous tail tile into empty
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
				while (!hasFoodTile()) { 
					createFood(); 
				}
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
	short retry = RETRY_DIFFERENT, BOARD_WIDTH, BOARD_HEIGHT;;
	float difficulty;
	std::cout << "Welcome to the Snake game. Use the arrow keys to play." << std::endl;
	while(retry != QUIT){
		if (retry == RETRY_DIFFERENT) {
			std::cout << "Select board size. Enter 1 for small, 2 for medium or 3 for large" << std::endl;
			char size;
			std::cin >> size;
			validateOption(size);
			switch (size) {
			case 49: //'1'
				BOARD_WIDTH = 14;
				BOARD_HEIGHT = 7;
				break;
			case 50: //'2'
				BOARD_WIDTH = 20;
				BOARD_HEIGHT = 10;
				break;
			case 51: //'3'
				BOARD_WIDTH = 26;
				BOARD_HEIGHT = 13;
				break;
			}

			std::cout << "Select game difficulty. Enter 1 for easy, 2 for medium or 3 for hard" << std::endl;
			char diff_char;
			std::cin >> diff_char;
			validateOption(diff_char);
			switch (diff_char) {
			case 49: //'1'
				difficulty = 0.3;
				break;
			case 50: //'2'
				difficulty = 0.2;
				break;
			case 51: //'3'
				difficulty = 0.1;
				break;
			}
		}

		Board board = Board(BOARD_WIDTH, BOARD_HEIGHT);
		Snake snake = Snake(BOARD_WIDTH, BOARD_HEIGHT);

		run(board, snake, difficulty);

		std::cout << "Game over. Try again? Enter 1 to keep your settings, 2 to change board size/difficulty, or 3 to quit" << std::endl;
		char end_char = NULL;
		std::cin >> end_char;
		validateOption(end_char);
		switch (end_char) {
		case 49: //'1'
			retry = RETRY_SAME;
			break;
		case 50: //'2'
			retry = RETRY_DIFFERENT;
			break;
		case 51: //'3'
			retry = QUIT;
			break;
		}
	}
	//srand(time(NULL));
	
	return 0;
}

void validateOption(char& input) {
	while (input != '1' && input != '2' && input != '3') {
		std::cout << "Invalid option. Press 1, 2 or 3 to choose" << std::endl;
		std::cin >> input;
	}
}

void run(Board board, Snake snake, float difficulty) {
	double startTime, endTime;
	startTime = clock();
	endTime = 0;
	board.moveSnake(RIGHT, snake.posx, snake.posy);
	board.showBoard();
	while (board.alive) {
		if ((endTime - startTime)/(double)CLOCKS_PER_SEC >= difficulty) {
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