#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h> // �ܼ� ����� �Լ� ����


#pragma region Define ���ǵ�

#define COUT GetStdHandle(STD_OUTPUT_HANDLE) // ǥ�� ��� ����̽�
#define Map_X 40 // �� Xũ��
#define Map_Y 20 // �� Yũ��

/* �ܼ� ���� �ڵ� */
#define WHITE 7 
#define RED 4
#define BLUE 1

/* ���� �̵� ���� */
#define Left 1
#define Right 2
#define Up 3
#define Down 4

/* ���� ������� */
#define Empty 0
#define Ceilling 1
#define Wall 2
#define Food 3
#define Spike_Wall 4
#define Snake_Head -1
#define Snake_Body -2
#define Enemy_Snake_Head -3
#define Enemy_Snake_Body -4

/* ���� ��� */
#define Solo_mod 1
#define Battle_mod 2

/* ���� ���̵� */
#define Easy 1
#define Normal 2
#define Hard 3

/* ���� ������ ��� */
#define Game_mod 1
#define Difficulty_level 2
#define Game_end 3

/*���� �� ����*/
#define Me 0
#define Enemy 1

#pragma endregion

#pragma region ���� ����

int best_score = 0;
int score = 0;															// ���� ���ھ� ���� ����
int gameFlag = 0;														// ���� ���� ���� ���� ����
int gameEnd = 0;														// ���� ���� ���� ����

int XY[2] = { 20, 10 };													// ���� ���� ��ǥ ����(�ӽ� ������ ���߿� �����ϸ鼭 �����ص� ������ ��)
int direction = Left;													// ���� ����
int mapOrigin[Map_Y][Map_X];											// ������ ��
int mapCopy[Map_Y][Map_X];												// ���� ���纻(���� �� ���¸� ����)
int foodX = 0, foodY = 0;												// ������ ��ġ ����
int spike_wallX = 0, spike_wallY = 0;									// ���ú��� ��ġ ����
int spike_spawn_delay = 0;													// ���ú� ��ȯ ������ ����
int is_dash = 0;														// �뽬 ��� ���� ����
int dash_limit = 30;													// �뽬 �ִ� ��뷮

int RW = 0, LW = 0; // �׽�Ʈ�� ����

int game_mod = Solo_mod;												// ���� ���
int difficulty_level = Easy;											// ������ ���̵�
int select_menu = Game_mod;												// �޴����� �������� �׸�

#pragma endregion

void map_reset();														// ���� ���۽� �� �⺻ ���� �Լ�
void draw_map();														// ���� ��Ұ� ����� �� �ݿ��ϴ� �Լ�	
void input_game();														// Ű������ �Է����� ���� ������ �ο��ϴ� �Լ�
void snake_move();														// ���� �������� �����ϴ� �Լ�
void add_body();														// ���� ���� �ø��� �Լ�
void food_spawn();														// ���� ���� �� ���ȯ�ϴ� �Լ�
void spike_wall_spawn();												// ���ú��� ���� �ð����� ��ȯ�ϴ� �Լ�
void print_game_menu();													// ���� �޴� ���
void gameStartScreen();
void gamePlayingScreen();
void inputGameMenu();
void game_reset();														// ���� ����� ���� ��Ҹ� �ʱ�ȭ

#pragma region �� ��� ���� ����ü, ���� ���� ����

struct NODE* head;
struct NODE* enemy_head;

struct NODE {															//�⺻ ��� ����
	int X;
	int Y;
	int Move;
	struct NODE* before_link;
	struct NODE* next_link;
};

struct NODE* last_node(int is_enemy) {	
											// ������ ��� Ȯ��
	struct NODE* cur = head;
	if (is_enemy == 1) {
		struct NODE* cur = enemy_head;
	}

	while (cur->next_link != NULL) {
		cur = cur->next_link;
	}
	return cur;
}

struct NODE* create_node(int X, int Y, int Move, int is_enemy) {						// ��� ����
	struct NODE* last = last_node(is_enemy); // ������ ��� ����
	struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));
	new_node->X = X;
	new_node->Y = Y;
	new_node->Move = Move;
	new_node->before_link = last;
	new_node->next_link = NULL;
	return new_node;
}

void insert_node_last(struct NODE* new_node, int is_enemy) {							// �������� ��� �߰�
	struct NODE* last = last_node(is_enemy);
	last->next_link = new_node;
}

void snake_move_point(int is_enemy) {												// ��� Move�� ����
	int move_data;
	struct NODE* cur = last_node(is_enemy);
	struct NODE* before_cur = cur->before_link;
	while (before_cur != head) {										// before_link�� head��� �� �Ӹ��̱⿡ ���� Move�� ����
		cur->Move = before_cur->Move;
		cur = cur->before_link;
		before_cur = before_cur->before_link;
	}

}

int delete_node(int is_enemy) {														// ��� ���� ����
	struct NODE* prev = head;
	struct NODE* cur = head->next_link;

	if (is_enemy == 1) {
		struct NODE* prev = enemy_head;
		struct NODE* cur = enemy_head->next_link;
	}

	while (cur != NULL) {
		prev->next_link = cur->next_link;
		free(cur);
		cur = prev->next_link;
	}
	return 0;
}

#pragma endregion

#pragma region �ܼ� Ŀ�� ����� �Լ�

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

#pragma endregion

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
	while (1) {
		if (gameFlag == 0) {
			gameEnd = 0;
			gameStartScreen();
		}
		else {
			gamePlayingScreen();
		}
	}
}

void gameStartScreen() {
	print_game_menu();

	while (gameFlag != 1) {
		inputGameMenu();
	}
}

void gamePlayingScreen() {
	system("cls");
	score = 0;

	map_reset();
	food_spawn();
	draw_map();
	while (gameFlag == 1) {
		input_game();
	}
}

void print_game_menu() {
	system("cls");
	printf("[���� ���ھ�]\n");
	printf("Best Score : %d | Last Score : %d\n\n", best_score, score);

	printf("[���� �޴�]\n");
	printf("���ַ� ���\n");
	printf("��Easy\n");
	printf("����������");
}

void inputGameMenu() {
	char key = getch(); // getch()�� Ű������ �ϳ� Ű�� �Է� ���� �� �ְ� �ϴ� �Լ�

	if (key == 'w' || key == 'W') {
		if (select_menu == Game_end) {
			select_menu = Difficulty_level;
			mouseXY(0, 5);
			printf("��\n\r��");
		}
		else if (select_menu == Difficulty_level) {
			select_menu = Game_mod;
			mouseXY(0, 4);
			printf("��\n\r��");
		}
	}
	else if (key == 's' || key == 'S') {
		if (select_menu == Game_mod) {
			select_menu = Difficulty_level;
			mouseXY(0, 4);
			printf("��\n\r��");
		}
		else if (select_menu == Difficulty_level) {
			select_menu = Game_end;
			mouseXY(0, 5);
			printf("��\n\r��");
		}
	}
	else if (key == 'd' || key == 'D') {
		if (select_menu == Game_mod) {
			mouseXY(1, 4);
			if (game_mod == Solo_mod) {
				game_mod = Battle_mod;
				printf("��Ʋ ���");
			}
			else if (game_mod == Battle_mod) {
				game_mod = Solo_mod;
				printf("�ַ� ���");
			}
		}
		else if (select_menu == Difficulty_level) {
			mouseXY(1, 5);
			if (difficulty_level == Easy) {
				difficulty_level = Normal;
				printf("Normal");
			}
			else if (difficulty_level == Normal) {
				difficulty_level = Hard;
				printf("Hard  ");
			}
			else if (difficulty_level == Hard) {
				difficulty_level = Easy;
				printf("Easy  ");
			}
		}
	}
	else if (key == 'a' || key == 'A') {
		if (select_menu == Game_mod) {
			mouseXY(1, 4);
			if (game_mod == Solo_mod) {
				game_mod = Battle_mod;
				printf("��Ʋ ���");
			}
			else if (game_mod == Battle_mod) {
				game_mod = Solo_mod;
				printf("�ַ� ���");
			}
		}
		else if (select_menu == Difficulty_level) {
			mouseXY(1, 5);
			if (difficulty_level == Easy) {
				difficulty_level = Hard;
				printf("Hard  ");
			}
			else if (difficulty_level == Normal) {
				difficulty_level = Easy;
				printf("Easy  ");
			}
			else if (difficulty_level == Hard) {
				difficulty_level = Normal;
				printf("Normal");
			}
		}
	}
	else if (key == '\r') {
		switch (select_menu) {
		case Game_mod:
			gameFlag = 1;
			break;
		case Game_end:
			mouseXY(0, 7);
			exit(0); // ���α׷��� ���� �ϴ� �Լ�
			break;
		}
	}
}

void map_reset() {
	int i, j;

	delete_node(Me);

	direction = Left;
	spike_spawn_delay = 0;

	for (i = 0; i < 3; i++) {
		insert_node_last(create_node(20 + i, 10, Left, Me), Me);				// ���� ������ Left�� ����
		if (Game_mod == Battle_mod) {											
			insert_node_last(create_node(20 + i, 17, Left, Enemy), Enemy);		// ���� ��尡 Battle����� Enemy�� �߰�
		}
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

void draw_map() {	// ���� �ʰ� ���Ͽ� �ٸ� �κ��� �ۼ�
	int i, j;
	for (i = 0; i < Map_X; i++) {
		for (j = 0; j < Map_Y; j++) {
			if (mapCopy[j][i] != mapOrigin[j][i]) {
				mouseXY(i, j);
				switch (mapOrigin[j][i])
				{
				case Snake_Head:
					if (gameEnd == 1) {
						SetConsoleTextAttribute(COUT, RED);
						printf("��");
						SetConsoleTextAttribute(COUT, WHITE);
					}
					else {
						printf("��");
					}
					break;
				case Snake_Body:
					printf("*");
					break;

				case Enemy_Snake_Head:
					SetConsoleTextAttribute(COUT, RED);
					printf("��");
					SetConsoleTextAttribute(COUT, WHITE);
					break;
				case Enemy_Snake_Body:
					SetConsoleTextAttribute(COUT, RED);
					printf("*");
					SetConsoleTextAttribute(COUT, WHITE);
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
				case Spike_Wall:
					SetConsoleTextAttribute(COUT, RED);
					printf("��");
					SetConsoleTextAttribute(COUT, WHITE);
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
		else if (gameEnd != 1 && (key == 'l' || key == 'L') && dash_limit > 0) { // �뽬
			is_dash = 1;
		}
		else if (key == '\r') {
			if (best_score < score) {
				best_score = score;
			}
			gameFlag = 0;
		}
	}

	if (gameEnd != 1) {
		snake_move();
	}
}

void snake_move() {

	if (is_dash == 0) {
		switch (difficulty_level)
		{
		case Easy:
			Sleep(200);
			break;
		case Normal:
			Sleep(150);
			break;
		case Hard:
			if (150 - score > 50) {
				Sleep(150 - score);
			}
			else {
				Sleep(50);
			}
		}
	}
	else if (dash_limit > 0) {
		switch (difficulty_level)
		{
		case Easy:
			Sleep(200-50);
			break;
		case Normal:
			Sleep(150- 50);
			break;
		case Hard:
			if (150 - score - 30 > 50) {
				Sleep(150 - score - 30);
			}
			else {
				Sleep(50);
			}
		}
		dash_limit--;
	}

	//Sleep

	if (spike_spawn_delay == 50-difficulty_level) {
		spike_wall_spawn();
		spike_spawn_delay = 0;
	}
	else {
		spike_spawn_delay += 1;
	}

	struct NODE* cur = head->next_link;
	snake_move_point(Me); //�� �̵� ���� ����


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

		if (cur == head->next_link || (cur != head->next_link && mapOrigin[cur->Y][cur->X] != Snake_Head)) {
			mapOrigin[cur->Y][cur->X] = Empty;
		}

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
			if (mapOrigin[cur->Y][cur->X] == Snake_Body || mapOrigin[cur->Y][cur->X] == Spike_Wall) { // ���� ����, ���ú��� ����?
				gameEnd = 1;
			}
			else if (mapOrigin[cur->Y][cur->X] == Food) { // ���� �̵��� �ڸ��� 'Food'�� ���� ��� body�߰�, 'Food'������
				score += 1;
				add_body();
				food_spawn();
			}
			mapOrigin[cur->Y][cur->X] = Snake_Head;
		}
		else {
			if (mapOrigin[cur->Y][cur->X] == Snake_Head) {
				mapOrigin[cur->Y][cur->X] = Snake_Head;
			}
			else {
				mapOrigin[cur->Y][cur->X] = Snake_Body;
			}
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
	printf("���� �� : %d | ������ �� : %d\n", LW, RW);
	if (gameEnd == 1) {
		printf("���� ����!!!");
	}

}

void add_body() {
	struct NODE* cur = last_node(Me);
	switch (cur->Move) {
	case Left:
		insert_node_last(create_node(cur->X + 1, cur->Y, Left, Me), Me);
		break;
	case Right:
		insert_node_last(create_node(cur->X - 1, cur->Y, Right, Me), Me);
		break;
	case Up:
		insert_node_last(create_node(cur->X, cur->Y + 1, Up, Me), Me);
		break;
	case Down:
		insert_node_last(create_node(cur->X, cur->Y - 1, Down, Me), Me);
		break;
	}
}

void food_spawn() {							// ���� ��ȯ
	while (1) {
		foodX = rand() % (Map_X - 2) + 1;
		foodY = rand() % (Map_Y - 2) + 1;

		if (mapOrigin[foodY][foodX] == Empty) {
			break;
		}
	}

	mapOrigin[foodY][foodX] = Food;
}

void spike_wall_spawn() {					// ���ú� ��ȯ
	struct NODE* cur = head->next_link;
	while (1) {
		spike_wallX = rand() % (Map_X - 2) + 1;
		spike_wallY = rand() % (Map_Y - 2) + 1;

		if (mapOrigin[spike_wallY][spike_wallX] == Empty) {
			if (direction == Left && cur->Y != spike_wallX) {

			}
			else if (direction == Left && cur->Y != spike_wallX) {

			}
			else if (direction == Right && cur->Y != spike_wallX) {
			
			}
			break;
		}
	}

	mapOrigin[spike_wallY][spike_wallX] = Spike_Wall;
}


