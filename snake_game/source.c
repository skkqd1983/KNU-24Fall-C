#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>


#pragma region Define ���ǵ�

#define COUT GetStdHandle(STD_OUTPUT_HANDLE) // ǥ�� ��� ����̽�
#define Map_X 40 // �� Xũ��
#define Map_Y 20 // �� Yũ��

/* �ܼ� ���� �ڵ� */
#define WHITE 7 
#define RED 4
#define BLUE 1
#define Green 10

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
int score = 0;																				// ���� ���ھ� ���� ����
int gameFlag = 0;																			// ���� ���� ���� ���� ����
int gameEnd = 0;																			// ���� ���� ���� ����

int XY[2] = { 20, 10 };																		// ���� ���� ��ǥ ����(�ӽ� ������ ���߿� �����ϸ鼭 �����ص� ������ ��)
int direction = Left;																		// ���� ����
int enemy_direction = Left;																	// Enemy ���� ����
int mapOrigin[Map_Y][Map_X];																// ������ ��
int mapCopy[Map_Y][Map_X];																	// ���� ���纻(���� �� ���¸� ����)
int foodX = 0, foodY = 0;																	// ������ ��ġ ����
int spike_wallX = 0, spike_wallY = 0;														// ���ú��� ��ġ ����
int spike_spawn_delay = 0;																	// ���ú� ��ȯ ������ ����
int is_dash = 1;																			// �뽬 ��� ���� ����
int dash_limit = 10;																		// �뽬 �ִ� ��뷮
int dash_count = 0;																			// �뽬 �� Enemy_snake ������ ����
int tail = 2;																				// ���� ������ ����
int move_delay = 0;																			// ���� �����̴� �ӵ� ����
int can_right, can_left, can_up, can_down;													// Enemy_snake�� ������ �� �ִ� ���� ����

int RW = 0, LW = 0; // �׽�Ʈ�� ����
int game_mod = Solo_mod;																	// ���� ���
int difficulty_level = Easy;																// ������ ���̵�
int select_menu = Game_mod;																	// �޴����� �������� �׸�
														
#pragma endregion	

#pragma region ����� �Լ���

void map_reset();																			// ���� ���۽� �� �⺻ ���� �Լ�
void draw_map();																			// ���� ��Ұ� ����� �� �ݿ��ϴ� �Լ�	
void input_game();																			// Ű������ �Է����� ���� ������ �ο��ϴ� �Լ�
void enemy_input_game();																	// Enemy ���� ������ �ο��ϴ� �Լ�
void snake_move();																			// ���� �������� �����ϴ� �Լ�
void enemy_snake_move();																	// Enemy ���� �������� �����ϴ� �Լ�
void add_body(int is_enemy);																// ���� ���� �ø��� �Լ�
void food_spawn();																			// ���� ���� �� ���ȯ�ϴ� �Լ�
void spike_wall_spawn();																	// ���ú��� ���� �ð����� ��ȯ�ϴ� �Լ�
void print_game_menu();																		// ���� �޴� ��� �Լ�
void gameStartScreen();																		// ���� �޴� ���� �Լ�
void gamePlayingScreen();																	// ���� �÷��� ���� �Լ�
void inputGameMenu();																		// ���� �޴��� ���� ��� �Լ�

#pragma endregion	

#pragma region �� ��� ���� ����ü, ���� ���� ����

struct NODE* head;																			// Snake�� �⺻ ���
struct NODE* enemy_head;																	// Enemy_snake�� �⺻ ���

/*��� ����*/
struct NODE {
	int X;																					// ���� X��ǥ
	int Y;																					// ���� Y��ǥ
	int Move;																				// ���� �����̷��� ����
	struct NODE* before_link;																// ���� ��
	struct NODE* next_link;																	// ���� ��
};

/*������ ��� Ȯ��*/
struct NODE* last_node(int is_enemy) {														
	struct NODE* cur = head;
	if (is_enemy == 1) {																	// last_node�� ȣ���Ѱ� enemy_snake��� cur = enemy_head
		cur = enemy_head;
	}

	while (cur->next_link != NULL) {
		cur = cur->next_link;
	}
	return cur;																				// Ȯ�ε� ������ ��带 ��ȯ
}

/*�� ���� ��带 ����*/
struct NODE* create_node(int X, int Y, int Move, int is_enemy) {
	struct NODE* last = last_node(is_enemy);
	struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));
	new_node->X = X;
	new_node->Y = Y;
	new_node->Move = Move;
	new_node->before_link = last;															// ���� ���� last_node�� ���� ����
	new_node->next_link = NULL;																// ���� ���� NULL�� ����
	return new_node;																		// ������ ��� ��ȯ
}

/*�������� �� ������ �߰�*/
void insert_node_last(struct NODE* new_node, int is_enemy) {
	struct NODE* last = last_node(is_enemy);												// ȣ����� �� �Ű������� is_enemy�� �޾� �Ʊ�, ���� ������ �߰�
	last->next_link = new_node;
}

/*���� Move�� ����*/
void snake_move_point(int is_enemy) {
	struct NODE* cur = last_node(is_enemy);
	struct NODE* before_cur = cur->before_link;
	if (is_enemy == Me) {
		while (before_cur != head) {														// before_link�� head��� �� �Ӹ��̱⿡ ���� Move�� ����
			cur->Move = before_cur->Move;													// Move���� ���� ������ �����Ͽ� ���ʴ�� �������ε� �Ѵ�
			cur = cur->before_link;
			before_cur = before_cur->before_link;
		}
	}
	else {
		while (before_cur != enemy_head) {													// before_link�� enemy_head��� �� �Ӹ��̱⿡ ���� Move�� ����
			cur->Move = before_cur->Move;
			cur = cur->before_link;
			before_cur = before_cur->before_link;
		}
	}
}

/*��� ���� ����*/
int delete_node(int is_enemy) {
	struct NODE* prev = head;
	struct NODE* cur = head->next_link;

	if (is_enemy == 1) {
		prev = enemy_head;
		cur = enemy_head->next_link;
	}

	while (cur != NULL) {																	// cur�� NULL�� �ƴ϶�� free()
		prev->next_link = cur->next_link;
		free(cur);
		cur = prev->next_link;
	}
	return 0;
}

#pragma endregion

#pragma region �ܼ� Ŀ�� ����� �Լ�

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;							// Ŀ������� �Լ��� ���Ǵ� ������ 

/*Ŀ�� �����*/
void setcursortype(CURSOR_TYPE c) {
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

/*�ܼ� Ŀ�� ��ġ ���� �Լ�*/
void mouseXY(int x, int y) {											
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(COUT, pos);													// �Ű������� ������ X, Y ���� ���� �ܼ� Ŀ���� ��ġ�� ����
}

/*main �Լ�*/
void main() {
	head = (struct NODE*)malloc(sizeof(struct NODE));
	head->next_link = NULL;

	enemy_head = (struct NODE*)malloc(sizeof(struct NODE));
	enemy_head->next_link = NULL;

	srand((unsigned)time(NULL));															// rand() ����� ���� srand�� ����� �ʱ�ȭ
	setcursortype(NOCURSOR);																// �ܼ� ȭ�鿡�� Ŀ���� ����
	while (1) {
		if (gameFlag == 0) {																// ������ ����� ���¶�� �޴� ȭ���� ���
			gameEnd = 0;
			gameStartScreen();
		}
		else {																				// ������ ����ƴٸ� ���� ȭ���� ���
			gamePlayingScreen();
		}
	}
}

/*���� �޴� ���� �Լ�*/
void gameStartScreen() {
	game_mod = Solo_mod;																	// �޴� ��� �� Solo_mod�� �ʱ�ȭ
	difficulty_level = Easy;																// ���̵� Easy�� �ʱ�ȭ
	print_game_menu();																		// �޴�UI ���

	while (gameFlag != 1) {																	// ������ ������� �ʾҴٸ� �޴� ���� �Լ��� ȣ��
		inputGameMenu();
	}
}

/*���� ȭ�� ���� �Լ�*/
void gamePlayingScreen() {
	system("cls");																			// ��� �ʱ�ȭ
	map_reset();																			// �� ��� �� ���� ��Ҹ� �ʱ�ȭ
	food_spawn();																			// food�� ��ȯ
	draw_map();																				// ������ ������ ������ ���
	while (gameFlag == 1) {																	// ������ ������� �ʾҴٸ� ���� ������ ȣ��
		input_game();
		if (game_mod == Battle_mod) {														// ���� ��尡 Battle_mod �̶�� enemy_input_game()�� ���� ȣ��
			enemy_input_game();
		}
	}
}

/*���� �޴�UI ��� �Լ�*/
void print_game_menu() {
	system("cls");
	printf("[���� ���ھ�]\n");
	printf("Best Score : %d | Last Score : %d\n\n", best_score, score);

	printf("[���� �޴�]\n");
	printf("���ַ� ���\n");
	printf("��Easy\n");
	printf("����������");
}

/*���� �޴� ���� �Լ�*/
void inputGameMenu() {
	char key = getch();																		// getch()�� Ű������ Ű�� �Է�

	if (key == 'w' || key == 'W') {															// w, W�� ���� ���
		if (select_menu == Game_end) {														// ���� �޴��� Game_end��� Difficulty_level�� ����
			select_menu = Difficulty_level;
			mouseXY(0, 5);
			printf("��\n\r��");																// ȭ��ǥ�� ����� �������� �޴��� ǥ��
		}
		else if (select_menu == Difficulty_level) {											// ���� �޴��� Diffisulty_level�̶�� Game_mod�� ����
			select_menu = Game_mod;
			mouseXY(0, 4);
			printf("��\n\r��");
		}
	}
	else if (key == 's' || key == 'S') {													// ���� w, W�� ����
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
	else if (key == 'd' || key == 'D') {													// d, D�� ���� ���� ���� ���̵��� ���� ����
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
	else if (key == 'a' || key == 'A') {													// d, D�� ������ ���� �ݴ�� �۵�
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
	else if (key == '\r') {																	// Enter�� ���� ��� ������ ����
		switch (select_menu) {																						
		case Game_mod:																		// Game_mod �޴��� ���� ���� ��� �ش� ���� ���� ����
			gameFlag = 1;
			break;
		case Game_end:																		// Game_end�� ���� ���̶�� ���α׷��� ����
			mouseXY(0, 7);
			exit(0);																		// ���α׷��� ���� �ϴ� �Լ�
			break;
		}
	}
}

/*���� ���� �� ��Ҹ� �ʱ�ȭ �ϴ� �Լ�*/
void map_reset() {
	int i, j;

	score = 0;																				// ������ �ʱ�ȭ
	tail = 2;																				// ���� ������ 2���� �ʱ�ȭ
	dash_limit = tail;																		// �뽬 ���� ��ġ�� ������ ������ �ʱ�ȭ

	delete_node(Me);																		// ���� ���ӿ����� �� ��带 ����
	delete_node(Enemy);																		// Enemy_snake�� ��� ����

	direction = Left;																		// �⺻ �̵� ������ Left�� ����
	spike_spawn_delay = 0;																	// ���� ��ȯ ������ �ʱ�ȭ

	for (i = 0; i < Map_X; i++) {															// ���� ��Ҹ� �ʱ�ȭ
		for (j = 0; j < Map_Y; j++) {
			mapOrigin[j][i] = 0;															// �ʵ鳢���� �������� ���� �ʱ�ȭ�ϱ⿡ origin = 0, copy = 100���� �ʱ�ȭ
			mapCopy[j][i] = 100;
		}
	}

	for (i = 0; i < Map_X; i++) {															// õ��� �ٴ� �Է�
		mapOrigin[0][i] = 1;																// 1�� õ��, �ٴ�
		mapOrigin[Map_Y - 1][i] = Ceilling;
	}

	for (i = 1; i < Map_Y - 1; i++) {														// ���� �� �Է�
		mapOrigin[i][0] = 2;																// 2�� ��
		mapOrigin[i][Map_X - 1] = Wall;
	}

	for (i = 0; i < 3; i++) {																// �ʿ� �� �߰�
		if (i == 0) {
			mapOrigin[XY[1]][XY[0]] = Snake_Head;
		}
		else {
			mapOrigin[XY[1]][XY[0] + i] = Snake_Body;
		}
		insert_node_last(create_node(20 + i, 10, Left, Me), Me);							// ���� ������ Left�� ����
	}

	if (game_mod == Battle_mod) {															// ���� ��尡 Battle����� Enemy�� �߰�
		for (int i = 0; i < 3; i++) {
			if (i == 0) {
				mapOrigin[XY[1] + 5][XY[0]] = Enemy_Snake_Head;
			}
			else {
				mapOrigin[XY[1] + 5][XY[0] + i] = Enemy_Snake_Body;
			}
			insert_node_last(create_node(XY[0] + i, XY[1] + 5, Left, Enemy), Enemy);
		}
	}
}

/*copy�� origin�� ���� �ٸ� �κ��� ����ϴ� �Լ�*/
void draw_map() {
	int i, j;
	for (i = 0; i < Map_X; i++) {
		for (j = 0; j < Map_Y; j++) {
			if (mapCopy[j][i] != mapOrigin[j][i]) {											// Copy�� Origin���̿� �ٸ� �κ��� �ִٸ� ���
				mouseXY(i, j);
				switch (mapOrigin[j][i])													// switch�� �ش��ϴ� ��Ҹ� ���
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
					if (gameEnd == 1) {
						SetConsoleTextAttribute(COUT, RED);
						printf("��");
						SetConsoleTextAttribute(COUT, WHITE);
					}
					else {
						SetConsoleTextAttribute(COUT, Green);
						printf("��");
						SetConsoleTextAttribute(COUT, WHITE);
					}
					break;
				case Enemy_Snake_Body:
					SetConsoleTextAttribute(COUT, Green);
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

	for (i = 0; i < Map_X; i++) {															// ����� ���� �� Copy = Origin���� ����
		for (j = 0; j < Map_Y; j++) {
			mapCopy[j][i] = mapOrigin[j][i];
		}
	}
}

/*Ű���� �Է����� �� ������ ���� �����ϴ� �Լ�*/
void input_game() {
	char key;

	if (_kbhit()) {
		char key = getch();																	// getch()�� Ű������ �ϳ� Ű�� �Է� ���� �� �ְ� �ϴ� �Լ�

		if (gameEnd != 1 && (key == 'a' || key == 'A') && direction != Right) {				// ������ ������ �ʾҴٸ� �Է����� ���� ���� ����
			direction = Left;
		}
		else if (gameEnd != 1 && (key == 'd' || key == 'D') && direction != Left) {
			direction = Right;
		}
		else if (gameEnd != 1 && (key == 'w' || key == 'W') && direction != Down) {
			direction = Up;
		}
		else if (gameEnd != 1 && (key == 's' || key == 'S') && direction != Up) {
			direction = Down;
		}
		else if (key == '\r') {																// ���� �� Enter�Է½� ���� ���� �� ���� ����
			if (best_score < score) {
				best_score = score;
			}
			gameFlag = 0;
		}

		if (gameEnd != 1 && dash_limit > 0 && (key == 'l' || key == 'L')) {					// �뽬 ���
			is_dash = 2;
			dash_count = 0;
		}
		else {																				// �뽬�� ����� ��� is_dash�� 1�� ����
			is_dash = 1;
		}
	}
	else if (dash_limit == 0) {
		is_dash = 1;
	}

	if (gameEnd != 1) {																		// ���� ���� �� ���� �����̴� �Լ� ȣ��
		snake_move();
	}
}

/*���� �����̴� �Լ�*/
void snake_move() {
	switch (difficulty_level)																// ���� ���̵��� ���� ���� �ð� ����
	{
	case Easy:
		move_delay = 200 / is_dash;															// �뽬 ������� is_dash = 2�� �Ǹ� ���� �ӵ� / 2
		break;
	case Normal:
		move_delay = 150 / is_dash;
		break;
	case Hard:
		if (150 - score > 50) {
			move_delay = (150 - score) / is_dash;
		}
		else {
			move_delay = 50 / is_dash;
		}
	}
	Sleep(move_delay);

	if (spike_spawn_delay == 50-difficulty_level * 5) {										// ���� 50-���̵�*5 ��ŭ �����δٸ� spike�� ��ȯ
		spike_wall_spawn();
		spike_spawn_delay = 0;																// ��ȯ �� spike_delay �ʱ�ȭ
	}
	else {
		spike_spawn_delay += 1;
	}

	struct NODE* cur = head->next_link;
	snake_move_point(Me);																	//�� �̵� ���� ���� �Լ� ȣ��

	if (direction == Left) {																// �Լ� ȣ�� �� �� �Ӹ��� ���� ����
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

	while (cur != NULL) {																	// �ʿ��� ���� �ڸ� ����, �� body��ǥ ����

		if (cur == head->next_link || (cur != head->next_link && mapOrigin[cur->Y][cur->X] != Snake_Head)) {
			mapOrigin[cur->Y][cur->X] = Empty;
		}

		switch (cur->Move) {																// �̵� ���⿡ ���� X, Y���� ����
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

		if (mapOrigin[cur->Y][cur->X] == Wall) {											// ���� �̵� �� ��, õ��� ������ ��ǥ�� �ݴ������� �̵�
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

		if (cur == head->next_link) {														// ���� �Ӹ��� ��� ���˽� food�� �� �߰�, �� ���˽� ���� ���� ��
			if (mapOrigin[cur->Y][cur->X] == Enemy_Snake_Head || mapOrigin[cur->Y][cur->X] == Enemy_Snake_Body || mapOrigin[cur->Y][cur->X] == Snake_Body || mapOrigin[cur->Y][cur->X] == Spike_Wall) { // ���� ����, ���ú��� ����?
				gameEnd = 1;
			}
			else if (mapOrigin[cur->Y][cur->X] == Food) {									// ���� �̵��� �ڸ��� 'Food'�� ���� ��� body�߰�, 'Food'������
				score += 1;
				tail += 1;
				dash_limit = tail;
				add_body(Me);																// ���� �߰��ϴ� �Լ� ȣ��
				food_spawn();																// food ��ȯ �Լ� ȣ��
			}
			mapOrigin[cur->Y][cur->X] = Snake_Head;
		}
		else {																				// �� ������ ���� ���ο� ��ǥ �Է�
			if (mapOrigin[cur->Y][cur->X] == Snake_Head) {
				mapOrigin[cur->Y][cur->X] = Snake_Head;
			}
			else {
				mapOrigin[cur->Y][cur->X] = Snake_Body;
			}
		}
		cur = cur->next_link;																// cur�� ���� body���� ����
	}

	draw_map();																				// ���� ���� ����� ���� �̵��� ���
	mouseXY(0, 20);

	printf("Score : %d\n", score);
	printf("�뽬 ������ : %d/%d   \n", dash_limit, tail);

	/*�׽�Ʈ�� ��� �ڵ�*/
	/*
	cur = head->next_link;
	while (cur != NULL) {
		printf("(%d, %d) ", cur->X, cur->Y);
		cur = cur->next_link;
	}
	printf("\n");

	printf("Food : %d, %d\n", foodX, foodY);

	cur = head->next_link;
	while (cur != NULL) {
		printf("%d, ", cur->Move);
		cur = cur->next_link;
	}
	printf("\n");
	printf("���� �� : %d | ������ �� : %d\n", LW, RW);
	printf("���� : %d��\n", tail);
	*/

	if (gameEnd == 1) {
		printf("���� ����!!!");
	}

	if (is_dash == 2) {																		// �뽬 �� Enemy �� �̵� ����
		if (dash_count == 2 && game_mod == Battle_mod) {									// �뽬 �� 2�� �̵����� Enemy_snake�� 1�� �̵�
			enemy_input_game();
			dash_count = 0;
		}
		dash_count += 1;
		dash_limit -= 1;
		input_game();
	}
}

/*Enemy �� �̵� ���� ���ϴ� �Լ�*/
void enemy_input_game() {
	struct NODE* cur = enemy_head->next_link;

	/*���� Enemy ���� ������ �� �ִ� ������ Ȯ��*/
	can_right = mapOrigin[cur->Y][cur->X + 1] == Empty || mapOrigin[cur->Y][cur->X + 1] == Food || mapOrigin[cur->Y][cur->X + 1] == Wall ? 1 : 0;
	can_left = mapOrigin[cur->Y][cur->X - 1] == Empty || mapOrigin[cur->Y][cur->X - 1] == Food || mapOrigin[cur->Y][cur->X - 1] == Wall ? 1 : 0;
	can_up = mapOrigin[cur->Y - 1][cur->X] == Empty || mapOrigin[cur->Y - 1][cur->X] == Food || mapOrigin[cur->Y - 1][cur->X] == Ceilling ? 1 : 0;
	can_down = mapOrigin[cur->Y + 1][cur->X] == Empty || mapOrigin[cur->Y + 1][cur->X] == Food || mapOrigin[cur->Y + 1][cur->X] == Ceilling ? 1 : 0;


	int Random = rand() % 2;																// ������ ��ġ�� �ִܰŸ� �̵��� �������� ����

	if (difficulty_level == Easy) {															// ���� ���̵��� Easy�� ��� ��� ������ ��ǥ���� ���������� ������ ����
		if (Random == 0) {
			if (foodX - cur->X > 0 && can_right == 1 && enemy_direction != Left) {
				enemy_direction = Right;
			}
			else if (foodX - cur->X < 0 && can_left == 1 && enemy_direction != Right) {
				enemy_direction = Left;
			}
			else if (foodY - cur->Y > 0 && can_down == 1 && enemy_direction != Up) {
				enemy_direction = Down;
			}
			else if (foodY - cur->Y < 0 && can_up == 1 && enemy_direction != Down) {
				enemy_direction = Up;
			}
		}
		else {
			if (foodY - cur->Y > 0 && can_down == 1 && enemy_direction != Up) {
				enemy_direction = Down;
			}
			else if (foodY - cur->Y < 0 && can_up == 1 && enemy_direction != Down) {
				enemy_direction = Up;
			}
			else if (foodX - cur->X > 0 && can_right == 1 && enemy_direction != Left) {
				enemy_direction = Right;
			}
			else if (foodX - cur->X < 0 && can_left == 1 && enemy_direction != Right) {
				enemy_direction = Left;
			}
		}
	}
	else if (difficulty_level == Normal) {													// ���̵��� Normal�� ��� 2/1�� ���� �Ϻ��ϰ� ȸ��
		if (Random == 0) {
			if ((foodX - cur->X > 0 || can_up + can_down == 0) && can_right == 1 && enemy_direction != Left) {
				enemy_direction = Right;
			}
			else if (foodX - cur->X < 0 && can_left == 1 && enemy_direction != Right) {
				enemy_direction = Left;
			}
			else if ((foodY - cur->Y > 0 || can_left + can_right == 0) && can_down == 1 && enemy_direction != Up) {
				enemy_direction = Down;
			}
			else if (foodY - cur->Y < 0 && can_up == 1 && enemy_direction != Down) {
				enemy_direction = Up;
			}
		}
		else 
		{
			if (foodY - cur->Y > 0 && can_down == 1 && enemy_direction != Up) {
				enemy_direction = Down;
			}
			else if ((foodY - cur->Y < 0 || can_left + can_right == 0) && can_up == 1 && enemy_direction != Down) {
				enemy_direction = Up;
			}
			else if (foodX - cur->X > 0 && can_right == 1 && enemy_direction != Left) {
				enemy_direction = Right;
			}
			else if ((foodX - cur->X < 0 || can_up + can_down == 0) && can_left == 1 && enemy_direction != Right) {
				enemy_direction = Left;
			}
		}
	}
	else {																					// ���̵��� Hard�� ��� ���� �Ϻ��ϰ� ȸ��
		if (Random == 0) {
			if ((foodX - cur->X > 0 || can_up + can_down == 0) && can_right == 1 && enemy_direction != Left) {
				enemy_direction = Right;
			}
			else if ((foodX - cur->X < 0 || can_up + can_down == 0) && can_left == 1 && enemy_direction != Right) {
				enemy_direction = Left;
			}
			else if ((foodY - cur->Y > 0 || can_left + can_right == 0) && can_down == 1 && enemy_direction != Up) {
				enemy_direction = Down;
			}
			else if ((foodY - cur->Y < 0 || can_left + can_right == 0) && can_up == 1 && enemy_direction != Down) {
				enemy_direction = Up;
			}
		}
		else {
			if ((foodY - cur->Y > 0 || can_left + can_right == 0) && can_down == 1 && enemy_direction != Up) {
				enemy_direction = Down;
			}
			else if ((foodY - cur->Y < 0 || can_left + can_right == 0) && can_up == 1 && enemy_direction != Down) {
				enemy_direction = Up;
			}
			else if ((foodX - cur->X > 0 || can_up + can_down == 0) && can_right == 1 && enemy_direction != Left) {
				enemy_direction = Right;
			}
			else if ((foodX - cur->X < 0 || can_up + can_down == 0) && can_left == 1 && enemy_direction != Right) {
				enemy_direction = Left;
			}
		}
	}


	if (gameEnd != 1) {
		enemy_snake_move();																	// �̵� ������ ������ �� Enemy�� �̵� �Լ��� ȣ��
	}
}

/*Enemy ���� �̵��ϴ� �Լ�*/
void enemy_snake_move() {

	struct NODE* cur = enemy_head->next_link;
	snake_move_point(Enemy);																// �� �� �̵� ���� ����


	if (enemy_direction == Left) {															// ���� �̵� ������ ����� �������� ���� ����
		cur->Move = Left;
	}
	else if (enemy_direction == Right) {
		cur->Move = Right;
	}
	else if (enemy_direction == Up) {
		cur->Move = Up;
	}
	else if (enemy_direction == Down) {
		cur->Move = Down;
	}


	while (cur != NULL) {																	// �ʿ��� ���� �ڸ� ����, �� body��ǥ ����

		if (cur == enemy_head->next_link || (cur != enemy_head->next_link && mapOrigin[cur->Y][cur->X] != Enemy_Snake_Head)) {
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

		if (mapOrigin[cur->Y][cur->X] == Wall) {											// ���� �̵� �� ���� ������ ��ǥ�� �ݴ������� �̵�
			if (cur->X == Map_X - 1) {
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

		if (cur == enemy_head->next_link) {
			if (mapOrigin[cur->Y][cur->X] == Enemy_Snake_Body || mapOrigin[cur->Y][cur->X] == Snake_Head || mapOrigin[cur->Y][cur->X] == Snake_Body || mapOrigin[cur->Y][cur->X] == Spike_Wall) { // ���� ����, ���ú��� ����?
				gameEnd = 1;
			}
			else if (mapOrigin[cur->Y][cur->X] == Food) {									// ���� �̵��� �ڸ��� 'Food'�� ���� ��� body�߰�, 'Food'������
				add_body(Enemy);
				food_spawn();
			}
			mapOrigin[cur->Y][cur->X] = Enemy_Snake_Head;
		}
		else {
			if (mapOrigin[cur->Y][cur->X] == Enemy_Snake_Head) {
				mapOrigin[cur->Y][cur->X] = Enemy_Snake_Head;
			}
			else {
				mapOrigin[cur->Y][cur->X] = Enemy_Snake_Body;
			}
		}
		cur = cur->next_link;																// cur�� ���� body���� ����
	}

	draw_map();

	/*�׽�Ʈ�� ��� �ڵ�*/
	/*
	mouseXY(0, 29);
	cur = enemy_head->next_link;
	while (cur != NULL) {
		printf("(%d, %d) ", cur->X, cur->Y);
		cur = cur->next_link;
	}
	printf("\n");

	cur = enemy_head->next_link;
	while (cur != NULL) {
		printf("%d, ", cur->Move);
		cur = cur->next_link;
	}
	printf("\n");
	printf("right: %d\nleft: %d\nup: %d\ndown: %d\n", can_right, can_left, can_up, can_down);
	*/

	if (gameEnd == 1) {
		printf("���� ����!!!");
	}
}

/*���� ���� �߰��ϴ� �Լ�*/
void add_body(int is_enemy) {
	struct NODE* cur = last_node(is_enemy);													// �߰��� ���� �Ʊ�, ���� ������ �Ű������� �޾� ����
	switch (cur->Move) {
	case Left:
		insert_node_last(create_node(cur->X + 1, cur->Y, Left, is_enemy), is_enemy);
		break;
	case Right:
		insert_node_last(create_node(cur->X - 1, cur->Y, Right, is_enemy), is_enemy);
		break;
	case Up:
		insert_node_last(create_node(cur->X, cur->Y + 1, Up, is_enemy), is_enemy);
		break;
	case Down:
		insert_node_last(create_node(cur->X, cur->Y - 1, Down, is_enemy), is_enemy);
		break;
	}
}

/*food�� ��ȯ�ϴ� �Լ�*/
void food_spawn() {
	while (1) {
		foodX = rand() % (Map_X - 2) + 1;													// rand()�� ������ x, y���� ����
		foodY = rand() % (Map_Y - 2) + 1;

		if (mapOrigin[foodY][foodX] == Empty) {												// ��ǥ�� Empty�� �ƴ϶�� �ٽ� ��ǥ ����
			break;
		}
	}

	mapOrigin[foodY][foodX] = Food;															// ����� ��ǥ�� Food��� �Է�
}

/*spike�� ��ȯ�ϴ� �Լ�*/
void spike_wall_spawn() {
	struct NODE* cur1 = head->next_link;
	struct NODE* cur2 = head->next_link;
	while (1) {
		spike_wallX = rand() % (Map_X - 2) + 1;												// food�� �����ϰ� ��ǥ�� ����
		spike_wallY = rand() % (Map_Y - 2) + 1;

		if (mapOrigin[spike_wallY][spike_wallX] == Empty) {									// ��ȯ ��Ұ� Empty, ���� x, y��ǥ�� ��ġ�� �ʴ� ��ҿ� ��ȯ
			if (((direction == Left || direction == Right) && cur1->Y == spike_wallY) || ((direction == Down || direction == Up) && cur1->X == spike_wallX)) {
				continue;
			}
			break;
		}
	}

	mapOrigin[spike_wallY][spike_wallX] = Spike_Wall;										// �ʿ� Spike_Wall��� �Է�
}


