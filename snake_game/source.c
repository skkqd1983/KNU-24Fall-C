#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h> // 콘솔 입출력 함수 제공

#define COUT GetStdHandle(STD_OUTPUT_HANDLE) // 표준 출력 디바이스
#define Map_X 40 // 맵 X크기
#define Map_Y 20 // 맵 Y크기

/* 콘솔 색상 코드 */
#define WHITE 7 
#define RED 4
#define BLUE 1

#define Left 1
#define Right 2
#define Up 3
#define Down 4

#define Empty 0
#define Ceilling 1
#define Wall 2
#define Food 3
#define Snake_Head -1
#define Snake_Body -2

int score = 0; // 게임 스코어 관련 변수
int gameFlag = 0; // 게임 시작 여부 관련 변수
int gameEnd = 0; // 게임 종료 여부 변수

int XY[2] = { 40, 10 }; // 현 좌표 변수
int direction = Left; // 뱀의 방향
int mapOrigin[Map_Y][Map_X];
int mapCopy[Map_Y][Map_X];
int body_count = 3;
int bodyXY[2000][3] = { {40, 10}, { 41, 10 }, {42, 10} };
int body_move[2000] = { 0, };
int foodX = 0, foodY = 0;


void map_reset();
void draw_map();
void input_game();
void snake_move();
void add_body();
void food_spawn();

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 

void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void mouseXY(int x, int y) { // 콘솔 마우스 위치 조정 함수 
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(COUT, pos);
}

void main() {
	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR); //커서 없앰 
	map_reset();
	food_spawn();
	draw_map();
	while (1) {
		input_game();
		snake_move();
	}
}

void map_reset() {
	int i, j;

	for (i = 0; i < body_count; i++) {
		body_move[i] = Left;
	}

	for (i = 0; i < Map_X; i++) {
		for (j = 0; j < Map_Y; j++) {
			mapOrigin[j][i] = 0;
			mapCopy[j][i] = 100;
		}
	}

	for (i = 0; i < Map_X; i++) { // 천장과 바닥 입력
		mapOrigin[0][i] = 1; // 1은 천장, 바닥
		mapOrigin[Map_Y-1][i] = Ceilling;
	}

	for (i = 1; i < Map_Y-1; i++) { // 양쪽 벽 입력
		mapOrigin[i][0] = 2; // 2은 벽
		mapOrigin[i][Map_X-1] = Wall;
	}

	for (i = 0; i < 3; i++) {
		if (i == 0) {
			mapOrigin[XY[1]+10][XY[0]] = -1;
		}
		else {
			mapOrigin[XY[1]][XY[0]+i] = -2;
		}
	}
}

void draw_map() {
	int i, j;
	for (i = 0; i < Map_X; i++) {
		for (j = 0; j < Map_Y; j++) {
			if (mapCopy[j][i] != mapOrigin[j][i]) {
				mouseXY(i, j);
				switch (mapOrigin[j][i])
				{
				case Snake_Head:
					printf("◇");
					break;
				case Snake_Body:
					printf("*");
					break;
				case Empty:
					printf(" ");
					break;
				case Ceilling:
					printf("-");
					break;
				case Wall:
					printf("|");
					break;
				case Food:
					printf("#");
					break;
				default:
					break;
				}
			}
		}
	}

	for (i = 0; i < Map_X; i++) {
		for (j = 0; j < Map_Y; j++) {
			mapCopy[j][i] = mapOrigin[j][i];
		}
	}
}

void input_game() {

	if (_kbhit()) {
		char key = getch(); // getch()는 키보드의 하나 키를 입력 받을 수 있게 하는 함수

		if (gameEnd != 1 && (key == 'a' || key == 'A')) {
			direction = Left;
		}
		else if (gameEnd != 1 && (key == 'd' || key == 'D')) {
			direction = Right;
		}
		else if (gameEnd != 1 && (key == 'w' || key == 'W')) {
			direction = Up;
		}
		else if (gameEnd != 1 && (key == 's' || key == 'S')) {
			direction = Down;
		}
		else if (key == '\r') {

		}
	}
}

void snake_move() {
	int i;

	Sleep(200); // 딜레이

	for (i = body_count; i >= 0; i--) {
		body_move[i] = body_move[i-1];
		if (i == body_count) {
			body_move[body_count] = 0;
		}
	}

	if (direction == Left) {
		body_move[0] = Left;
	}
	else if (direction == Right) {
		body_move[0] = Right;
	}
	else if (direction == Up) {
		body_move[0] = Up;
	}
	else if (direction == Down) {
		body_move[0] = Down;
	}

	for (i = 0; i < body_count; i++) {

		mapOrigin[bodyXY[i][1]][bodyXY[i][0]] = Empty;

		switch (body_move[i]) {
		case Left:
			bodyXY[i][0] -= 1;
			break;
		case Right:
			bodyXY[i][0] += 1;
			break;
		case Up:
			bodyXY[i][1] -= 1;
			break;
		case Down:
			bodyXY[i][1] += 1;
			break;
		}

		if (i == 0) {
			if (mapOrigin[bodyXY[i][1]][bodyXY[i][0]] == Food) {
				add_body();
				food_spawn();
			}
			mapOrigin[bodyXY[i][1]][bodyXY[i][0]] = Snake_Head;
		}
		else {
			mapOrigin[bodyXY[i][1]][bodyXY[i][0]] = Snake_Body;
		}
	}

	draw_map();
	mouseXY(0, 21);

	for (i = 0; i < body_count; i++) {
		printf("(%d, %d) ", bodyXY[i][0], bodyXY[i][1]);
	}
	printf("\n");

	printf("Food : %d, %d\n", foodX, foodY);

	for (i = 0; i < body_count; i++) {
		printf("%d, ", body_move[i]);
	}

}

void add_body() {
	body_count += 1;
	switch (body_move[body_count - 2]) {
	case Left:
		bodyXY[body_count - 1][0] = bodyXY[body_count - 2][0] + 1;
		bodyXY[body_count - 1][1] = bodyXY[body_count - 2][1];
		break;
	case Right:
		bodyXY[body_count - 1][0] = bodyXY[body_count - 2][0] - 1;
		bodyXY[body_count - 1][1] = bodyXY[body_count - 2][1];
		break;
	case Up:
		bodyXY[body_count - 1][0] = bodyXY[body_count - 2][0];
		bodyXY[body_count - 1][1] = bodyXY[body_count - 2][1] + 1;
		break;
	case Down:
		bodyXY[body_count - 1][0] = bodyXY[body_count - 2][0];
		bodyXY[body_count - 1][1] = bodyXY[body_count - 2][1] - 1;
		break;
	}
	body_move[body_count - 1] = body_move[body_count - 2];
}

void food_spawn() {
	while (1) {
		foodX = rand() % (Map_X - 2) + 1;
		foodY = rand() % (Map_Y - 2) + 1;

		if (mapOrigin[foodY][foodX] == Empty) {
			break;
		}
	}

	mapOrigin[foodY][foodX] = 3;
}


