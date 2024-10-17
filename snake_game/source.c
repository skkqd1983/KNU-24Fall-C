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

int score = 0;															// 게임 스코어 관련 변수
int gameFlag = 0;														// 게임 시작 여부 관련 변수
int gameEnd = 0;														// 게임 종료 여부 변수

int XY[2] = { 20, 10 };													// 최초 시작 좌표 변수(임시 변수라 나중에 수정하면서 삭제해도 괜찮을 듯)
int direction = Left;													// 뱀의 방향
int mapOrigin[Map_Y][Map_X];											// 실질적 맵
int mapCopy[Map_Y][Map_X];												// 맵의 복사본(변경 전 행태를 가짐)
int foodX = 0, foodY = 0;												// 음식의 위치 변수

int RW = 0, LW = 0;

void map_reset();														// 최초 시작시 맵 기본 설정 함수
void draw_map();														// 맵의 요소가 변경될 시 반영하는 함수	
void input_game();														// 키보드의 입력으로 뱀의 방향을 부여하는 함수
void snake_move();														// 뱀의 움직임을 제어하는 함수
void add_body();														// 뱀의 몸을 늘리는 함수
void food_spawn();														// 음식 섭취 시 재소환하는 함수

struct NODE* head;

struct NODE {															//기본 노드 선언
	int X;
	int Y;
	int Move;
	struct NODE* before_link;
	struct NODE* next_link;
};

struct NODE* last_node() {												// 마지막 노드 확인
	struct NODE* cur = head;
	while (cur->next_link != NULL) {
		cur = cur->next_link;
	}
	return cur;
}

struct NODE* create_node(int X, int Y, int Move) {						// 노드 생성
	struct NODE* last = last_node(); // 마지막 노드 저장
	struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));
	new_node->X = X;
	new_node->Y = Y;
	new_node->Move = Move;
	new_node->before_link = last;
	new_node->next_link = NULL;
	return new_node;
}

void insert_node_last(struct NODE* new_node) {							// 마지막에 노드 추가
	struct NODE* last = last_node();
	last->next_link = new_node;
}

void snake_move_point() {												// 노드 Move값 이전
	int move_data;
	struct NODE* cur = last_node();
	struct NODE* before_cur = cur->before_link;
	while (before_cur != head) {										// before_link가 head라면 뱀 머리이기에 따로 Move값 지정
		cur->Move = before_cur->Move;
		cur = cur->before_link;
		before_cur = before_cur->before_link;
	}

}

int delete_node() {														// 노드 전부 삭제
	while (last_node() != NULL) {
		struct NODE* cur = last_node();
		free(cur);
	}
	return 0;
}


typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;		// 커서숨기는 함수에 사용되는 열거형 

void setcursortype(CURSOR_TYPE c) {										// 커서숨기는 함수 
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

void mouseXY(int x, int y) {											// 콘솔 마우스 위치 조정 함수 
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(COUT, pos);
}

void main() {
	head = (struct NODE*)malloc(sizeof(struct NODE));
	head->next_link = NULL;

	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR);											// 커서 없앰 
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
	while (cur->next_link != NULL) {									// 최초 방향을 Left로 지정
		cur = cur->next_link;
		cur->Move = Left;
	}

	for (i = 0; i < Map_X; i++) {
		for (j = 0; j < Map_Y; j++) {
			mapOrigin[j][i] = 0;
			mapCopy[j][i] = 100;
		}
	}

	for (i = 0; i < Map_X; i++) { // 천장과 바닥 입력
		mapOrigin[0][i] = 1; // 1은 천장, 바닥
		mapOrigin[Map_Y - 1][i] = Ceilling;
	}

	for (i = 1; i < Map_Y - 1; i++) { // 양쪽 벽 입력
		mapOrigin[i][0] = 2; // 2은 벽
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

	Sleep(200); // 딜레이

	struct NODE* cur = head->next_link;
	snake_move_point(); //뱀 이동 방향 이전


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


	while (cur != NULL) { // 맵에서 원래 자리 제거, 각 body좌표 수정

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

		if (mapOrigin[cur->Y][cur->X] == Wall) { // 뱀이 이동 중 벽과 만나면 좌표를 반대편으로 이동
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
			if (mapOrigin[cur->Y][cur->X] == Food) { // 뱀이 이동할 자리에 'Food'가 있을 경우 body추가, 'Food'리스폰
				score += 1;
				add_body();
				food_spawn();
			}
			mapOrigin[cur->Y][cur->X] = Snake_Head;
		}
		else {
			mapOrigin[cur->Y][cur->X] = Snake_Body;
		}
		cur = cur->next_link; // cur를 다음 body으로 이전
	}

	draw_map();
	mouseXY(0, 21);

	cur = head->next_link; // cur를 head->next_link로 초기화
	while (cur != NULL) {
		printf("(%d, %d) ", cur->X, cur->Y);
		cur = cur->next_link;
	}
	printf("\n");

	printf("Food : %d, %d\n", foodX, foodY);

	cur = head->next_link; // cur를 head->next_link로 초기화
	while (cur != NULL) {
		printf("%d, ", cur->Move);
		cur = cur->next_link;
	}
	printf("\n");
	printf("Score : %d\n", score);
	printf("왼쪽 벽 : %d | 오른쪽 벽 : %d", LW, RW);

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


