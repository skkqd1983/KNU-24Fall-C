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

int score = 0;															// ���� ���ھ� ���� ����
int gameFlag = 0;														// ���� ���� ���� ���� ����
int gameEnd = 0;														// ���� ���� ���� ����

int XY[2] = { 20, 10 };													// ���� ���� ��ǥ ����(�ӽ� ������ ���߿� �����ϸ鼭 �����ص� ������ ��)
int direction = Left;													// ���� ����
int mapOrigin[Map_Y][Map_X];											// ������ ��
int mapCopy[Map_Y][Map_X];												// ���� ���纻(���� �� ���¸� ����)
int foodX = 0, foodY = 0;												// ������ ��ġ ����

int RW = 0, LW = 0;

void map_reset();														// ���� ���۽� �� �⺻ ���� �Լ�
void draw_map();														// ���� ��Ұ� ����� �� �ݿ��ϴ� �Լ�	
void input_game();														// Ű������ �Է����� ���� ������ �ο��ϴ� �Լ�
void snake_move();														// ���� �������� �����ϴ� �Լ�
void add_body();														// ���� ���� �ø��� �Լ�
void food_spawn();														// ���� ���� �� ���ȯ�ϴ� �Լ�

struct NODE* head;

struct NODE {															//�⺻ ��� ����
	int X;
	int Y;
	int Move;
	struct NODE* before_link;
	struct NODE* next_link;
};

struct NODE* last_node() {												// ������ ��� Ȯ��
	struct NODE* cur = head;
	while (cur->next_link != NULL) {
		cur = cur->next_link;
	}
	return cur;
}

struct NODE* create_node(int X, int Y, int Move) {						// ��� ����
	struct NODE* last = last_node(); // ������ ��� ����
	struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));
	new_node->X = X;
	new_node->Y = Y;
	new_node->Move = Move;
	new_node->before_link = last;
	new_node->next_link = NULL;
	return new_node;
}

void insert_node_last(struct NODE* new_node) {							// �������� ��� �߰�
	struct NODE* last = last_node();
	last->next_link = new_node;
}

void snake_move_point() {												// ��� Move�� ����
	int move_data;
	struct NODE* cur = last_node();
	struct NODE* before_cur = cur->before_link;
	while (before_cur != head) {										// before_link�� head��� �� �Ӹ��̱⿡ ���� Move�� ����
		cur->Move = before_cur->Move;
		cur = cur->before_link;
		before_cur = before_cur->before_link;
	}

}

int delete_node() {														// ��� ���� ����
	while (last_node() != NULL) {
		struct NODE* cur = last_node();
		free(cur);
	}
	return 0;
}


typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;		// Ŀ������� �Լ��� ���Ǵ� ������ 

void setcursortype(CURSOR_TYPE c) {										// Ŀ������� �Լ� 
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

void mouseXY(int x, int y) {											// �ܼ� ���콺 ��ġ ���� �Լ� 
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(COUT, pos);
}

void main() {
	head = (struct NODE*)malloc(sizeof(struct NODE));
	head->next_link = NULL;

	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR);											// Ŀ�� ���� 
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


	for (i = 0; i < 3; i++) {
		insert_node_last(create_node(20 + i, 10, Left));
	}

	struct NODE* cur = head;
	while (cur->next_link != NULL) {									// ���� ������ Left�� ����
		cur = cur->next_link;
		cur->Move = Left;
	}

	for (i = 0; i < Map_X; i++) {
		for (j = 0; j < Map_Y; j++) {
			mapOrigin[j][i] = 0;
			mapCopy[j][i] = 100;
		}
	}

	for (i = 0; i < Map_X; i++) { // õ��� �ٴ� �Է�
		mapOrigin[0][i] = 1; // 1�� õ��, �ٴ�
		mapOrigin[Map_Y - 1][i] = Ceilling;
	}

	for (i = 1; i < Map_Y - 1; i++) { // ���� �� �Է�
		mapOrigin[i][0] = 2; // 2�� ��
		mapOrigin[i][Map_X - 1] = Wall;
	}

	for (i = 0; i < 3; i++) {
		if (i == 0) {
			mapOrigin[XY[1] + 10][XY[0]] = -1;
		}
		else {
			mapOrigin[XY[1]][XY[0] + i] = -2;
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

		if (gameEnd != 1 && (key == 'a' || key == 'A') && direction != Right) { // 1
			direction = Left;
		}
		else if (gameEnd != 1 && (key == 'd' || key == 'D') && direction != Left) { // 2
			direction = Right;
		}
		else if (gameEnd != 1 && (key == 'w' || key == 'W') && direction != Down) { // 3
			direction = Up;
		}
		else if (gameEnd != 1 && (key == 's' || key == 'S') && direction != Up) { // 4
			direction = Down;
		}
		else if (key == '\r') {

		}
	}
}

void snake_move() {
	int i;

	Sleep(200); // ������

	struct NODE* cur = head->next_link;
	snake_move_point(); //�� �̵� ���� ����


	if (direction == Left) {
		cur->Move = Left;
	}
	else if (direction == Right) {
		cur->Move = Right;
	}
	else if (direction == Up) {
		cur->Move = Up;
	}
	else if (direction == Down) {
		cur->Move = Down;
	}


	while (cur != NULL) { // �ʿ��� ���� �ڸ� ����, �� body��ǥ ����

		mapOrigin[cur->Y][cur->X] = Empty;

		switch (cur->Move) {
		case Left:
			cur->X -= 1;
			break;
		case Right:
			cur->X += 1;
			break;
		case Up:
			cur->Y -= 1;
			break;
		case Down:
			cur->Y += 1;
			break;
		}

		if (mapOrigin[cur->Y][cur->X] == Wall) { // ���� �̵� �� ���� ������ ��ǥ�� �ݴ������� �̵�
			if (cur->X == Map_X-1) {
				cur->X = 1;
				RW += 1;
			}
			else if (cur->X == 0) {
				cur->X = Map_X - 2;
				LW += 1;
			}
		}
		else if (mapOrigin[cur->Y][cur->X] == Ceilling) {
			if (cur->Move == Up) {
				cur->Y = Map_Y - 2;
			}
			else {
				cur->Y = 1;
			}
		}

		if (cur == head->next_link) {
			if (mapOrigin[cur->Y][cur->X] == Food) { // ���� �̵��� �ڸ��� 'Food'�� ���� ��� body�߰�, 'Food'������
				score += 1;
				add_body();
				food_spawn();
			}
			mapOrigin[cur->Y][cur->X] = Snake_Head;
		}
		else {
			mapOrigin[cur->Y][cur->X] = Snake_Body;
		}
		cur = cur->next_link; // cur�� ���� body���� ����
	}

	draw_map();
	mouseXY(0, 21);

	cur = head->next_link; // cur�� head->next_link�� �ʱ�ȭ
	while (cur != NULL) {
		printf("(%d, %d) ", cur->X, cur->Y);
		cur = cur->next_link;
	}
	printf("\n");

	printf("Food : %d, %d\n", foodX, foodY);

	cur = head->next_link; // cur�� head->next_link�� �ʱ�ȭ
	while (cur != NULL) {
		printf("%d, ", cur->Move);
		cur = cur->next_link;
	}
	printf("\n");
	printf("Score : %d\n", score);
	printf("���� �� : %d | ������ �� : %d", LW, RW);

}

void add_body() {
	struct NODE* cur = last_node();
	switch (cur->Move) {
	case Left:
		insert_node_last(create_node(cur->X + 1, cur->Y, Left));
		break;
	case Right:
		insert_node_last(create_node(cur->X - 1, cur->Y, Right));
		break;
	case Up:
		insert_node_last(create_node(cur->X, cur->Y + 1, Up));
		break;
	case Down:
		insert_node_last(create_node(cur->X, cur->Y - 1, Down));
		break;
	}
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


