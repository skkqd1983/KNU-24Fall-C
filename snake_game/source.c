#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h> // �ܼ� ����� �Լ� ����

#define COUT GetStdHandle(STD_OUTPUT_HANDLE) // ǥ�� ��� ����̽�
#define Map_X 40 // �� Xũ��
#define Map_Y 20 // �� Yũ��

/* �ܼ� ���� �ڵ� */
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

int score = 0; // ���� ���ھ� ���� ����
int gameFlag = 0; // ���� ���� ���� ���� ����
int gameEnd = 0; // ���� ���� ���� ����

int XY[2] = { 40, 10 }; // �� ��ǥ ����
int direction = Left; // ���� ����
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

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������� �Լ��� ���Ǵ� ������ 

void setcursortype(CURSOR_TYPE c) { //Ŀ������� �Լ� 
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

void mouseXY(int x, int y) { // �ܼ� ���콺 ��ġ ���� �Լ� 
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(COUT, pos);
}

void main() {
	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR); //Ŀ�� ���� 
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

	for (i = 0; i < Map_X; i++) { // õ��� �ٴ� �Է�
		mapOrigin[0][i] = 1; // 1�� õ��, �ٴ�
		mapOrigin[Map_Y-1][i] = Ceilling;
	}

	for (i = 1; i < Map_Y-1; i++) { // ���� �� �Է�
		mapOrigin[i][0] = 2; // 2�� ��
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
					printf("��");
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
		char key = getch(); // getch()�� Ű������ �ϳ� Ű�� �Է� ���� �� �ְ� �ϴ� �Լ�

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

	Sleep(200); // ������

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


