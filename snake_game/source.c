#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>


#pragma region Define 정의들

#define COUT GetStdHandle(STD_OUTPUT_HANDLE) // 표준 출력 디바이스
#define Map_X 40 // 맵 X크기
#define Map_Y 20 // 맵 Y크기

/* 콘솔 색상 코드 */
#define WHITE 7 
#define RED 4
#define BLUE 1
#define Green 10

/* 뱀의 이동 방향 */
#define Left 1
#define Right 2
#define Up 3
#define Down 4

/* 맵의 구성요소 */
#define Empty 0
#define Ceilling 1
#define Wall 2
#define Food 3
#define Spike_Wall 4
#define Snake_Head -1
#define Snake_Body -2
#define Enemy_Snake_Head -3
#define Enemy_Snake_Body -4

/* 게임 모드 */
#define Solo_mod 1
#define Battle_mod 2

/* 게임 난이도 */
#define Easy 1
#define Normal 2
#define Hard 3

/* 현재 선택한 목록 */
#define Game_mod 1
#define Difficulty_level 2
#define Game_end 3

/*적과 나 구분*/
#define Me 0
#define Enemy 1

#pragma endregion

#pragma region 각종 변수

int best_score = 0;
int score = 0;																				// 게임 스코어 관련 변수
int gameFlag = 0;																			// 게임 시작 여부 관련 변수
int gameEnd = 0;																			// 게임 종료 여부 변수

int XY[2] = { 20, 10 };																		// 최초 시작 좌표 변수(임시 변수라 나중에 수정하면서 삭제해도 괜찮을 듯)
int direction = Left;																		// 뱀의 방향
int enemy_direction = Left;																	// Enemy 뱀의 방향
int mapOrigin[Map_Y][Map_X];																// 실질적 맵
int mapCopy[Map_Y][Map_X];																	// 맵의 복사본(변경 전 행태를 가짐)
int foodX = 0, foodY = 0;																	// 음식의 위치 변수
int spike_wallX = 0, spike_wallY = 0;														// 가시벽의 위치 변수
int spike_spawn_delay = 0;																	// 가시벽 소환 딜레이 변수
int is_dash = 1;																			// 대쉬 사용 유무 변수
int dash_limit = 10;																		// 대쉬 최대 사용량
int dash_count = 0;																			// 대쉬 중 Enemy_snake 움직임 변수
int tail = 2;																				// 현재 꼬리의 개수
int move_delay = 0;																			// 뱀이 움직이는 속도 변수
int can_right, can_left, can_up, can_down;													// Enemy_snake가 움직일 수 있는 방향 변수

int RW = 0, LW = 0; // 테스트용 변수
int game_mod = Solo_mod;																	// 게임 모드
int difficulty_level = Easy;																// 게임의 난이도
int select_menu = Game_mod;																	// 메뉴에서 선택중인 항목
														
#pragma endregion	

#pragma region 선언된 함수들

void map_reset();																			// 최초 시작시 맵 기본 설정 함수
void draw_map();																			// 맵의 요소가 변경될 시 반영하는 함수	
void input_game();																			// 키보드의 입력으로 뱀의 방향을 부여하는 함수
void enemy_input_game();																	// Enemy 뱀의 방향을 부여하는 함수
void snake_move();																			// 뱀의 움직임을 제어하는 함수
void enemy_snake_move();																	// Enemy 뱀의 움직임을 제어하는 함수
void add_body(int is_enemy);																// 뱀의 몸을 늘리는 함수
void food_spawn();																			// 음식 섭취 시 재소환하는 함수
void spike_wall_spawn();																	// 가시벽을 일정 시간마다 소환하는 함수
void print_game_menu();																		// 게임 메뉴 출력 함수
void gameStartScreen();																		// 게임 메뉴 제어 함수
void gamePlayingScreen();																	// 게임 플레이 제어 함수
void inputGameMenu();																		// 게임 메뉴의 선택 기능 함수

#pragma endregion	

#pragma region 뱀 노드 관련 구조체, 몸통 길이 조절

struct NODE* head;																			// Snake의 기본 노드
struct NODE* enemy_head;																	// Enemy_snake의 기본 노드

/*노드 선언*/
struct NODE {
	int X;																					// 뱀의 X좌표
	int Y;																					// 뱀의 Y좌표
	int Move;																				// 뱀이 움직이려는 방향
	struct NODE* before_link;																// 이전 몸
	struct NODE* next_link;																	// 다음 몸
};

/*마지막 노드 확인*/
struct NODE* last_node(int is_enemy) {														
	struct NODE* cur = head;
	if (is_enemy == 1) {																	// last_node를 호출한게 enemy_snake라면 cur = enemy_head
		cur = enemy_head;
	}

	while (cur->next_link != NULL) {
		cur = cur->next_link;
	}
	return cur;																				// 확인된 마지막 노드를 반환
}

/*뱀 몸통 노드를 생성*/
struct NODE* create_node(int X, int Y, int Move, int is_enemy) {
	struct NODE* last = last_node(is_enemy);
	struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));
	new_node->X = X;
	new_node->Y = Y;
	new_node->Move = Move;
	new_node->before_link = last;															// 이전 몸은 last_node를 통해 저장
	new_node->next_link = NULL;																// 다음 몸은 NULL로 저장
	return new_node;																		// 생성한 노드 반환
}

/*마지막에 뱀 몸통을 추가*/
void insert_node_last(struct NODE* new_node, int is_enemy) {
	struct NODE* last = last_node(is_enemy);												// 호출받을 때 매개변수로 is_enemy를 받아 아군, 적을 구분해 추가
	last->next_link = new_node;
}

/*몸통 Move값 전달*/
void snake_move_point(int is_enemy) {
	struct NODE* cur = last_node(is_enemy);
	struct NODE* before_cur = cur->before_link;
	if (is_enemy == Me) {
		while (before_cur != head) {														// before_link가 head라면 뱀 머리이기에 따로 Move값 지정
			cur->Move = before_cur->Move;													// Move값을 뒤쪽 몸으로 전달하여 차례대로 움직으로독 한다
			cur = cur->before_link;
			before_cur = before_cur->before_link;
		}
	}
	else {
		while (before_cur != enemy_head) {													// before_link가 enemy_head라면 뱀 머리이기에 따로 Move값 지정
			cur->Move = before_cur->Move;
			cur = cur->before_link;
			before_cur = before_cur->before_link;
		}
	}
}

/*노드 전부 삭제*/
int delete_node(int is_enemy) {
	struct NODE* prev = head;
	struct NODE* cur = head->next_link;

	if (is_enemy == 1) {
		prev = enemy_head;
		cur = enemy_head->next_link;
	}

	while (cur != NULL) {																	// cur가 NULL이 아니라면 free()
		prev->next_link = cur->next_link;
		free(cur);
		cur = prev->next_link;
	}
	return 0;
}

#pragma endregion

#pragma region 콘솔 커서 숨기는 함수

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;							// 커서숨기는 함수에 사용되는 열거형 

/*커서 숨기기*/
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

/*콘솔 커서 위치 변경 함수*/
void mouseXY(int x, int y) {											
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(COUT, pos);													// 매개변수로 지정한 X, Y 값에 따라 콘솔 커서의 위치를 변경
}

/*main 함수*/
void main() {
	head = (struct NODE*)malloc(sizeof(struct NODE));
	head->next_link = NULL;

	enemy_head = (struct NODE*)malloc(sizeof(struct NODE));
	enemy_head->next_link = NULL;

	srand((unsigned)time(NULL));															// rand() 사용을 위해 srand를 사용해 초기화
	setcursortype(NOCURSOR);																// 콘솔 화면에서 커서를 제거
	while (1) {
		if (gameFlag == 0) {																// 게임이 종료된 상태라면 메뉴 화면을 출력
			gameEnd = 0;
			gameStartScreen();
		}
		else {																				// 게임이 실행됐다면 게임 화면을 출력
			gamePlayingScreen();
		}
	}
}

/*게임 메뉴 관리 함수*/
void gameStartScreen() {
	game_mod = Solo_mod;																	// 메뉴 출력 시 Solo_mod로 초기화
	difficulty_level = Easy;																// 난이도 Easy로 초기화
	print_game_menu();																		// 메뉴UI 출력

	while (gameFlag != 1) {																	// 게임이 실행되지 않았다면 메뉴 조작 함수를 호출
		inputGameMenu();
	}
}

/*게임 화면 관리 함수*/
void gamePlayingScreen() {
	system("cls");																			// 출력 초기화
	map_reset();																			// 맵 출력 전 게임 요소를 초기화
	food_spawn();																			// food를 소환
	draw_map();																				// 게임을 진행할 영역을 출력
	while (gameFlag == 1) {																	// 게임이 종료되지 않았다면 뱀의 움직임 호출
		input_game();
		if (game_mod == Battle_mod) {														// 게임 모드가 Battle_mod 이라면 enemy_input_game()도 같이 호출
			enemy_input_game();
		}
	}
}

/*게임 메뉴UI 출력 함수*/
void print_game_menu() {
	system("cls");
	printf("[현재 스코어]\n");
	printf("Best Score : %d | Last Score : %d\n\n", best_score, score);

	printf("[게임 메뉴]\n");
	printf("▶솔로 모드\n");
	printf("▷Easy\n");
	printf("▷게임종료");
}

/*게임 메뉴 조작 함수*/
void inputGameMenu() {
	char key = getch();																		// getch()로 키보드의 키를 입력

	if (key == 'w' || key == 'W') {															// w, W를 누를 경우
		if (select_menu == Game_end) {														// 선택 메뉴가 Game_end라면 Difficulty_level로 변경
			select_menu = Difficulty_level;
			mouseXY(0, 5);
			printf("▶\n\r▷");																// 화살표를 출력해 선택중인 메뉴를 표시
		}
		else if (select_menu == Difficulty_level) {											// 선택 메뉴가 Diffisulty_level이라면 Game_mod로 변경
			select_menu = Game_mod;
			mouseXY(0, 4);
			printf("▶\n\r▷");
		}
	}
	else if (key == 's' || key == 'S') {													// 위쪽 w, W과 유사
		if (select_menu == Game_mod) {
			select_menu = Difficulty_level;
			mouseXY(0, 4);
			printf("▷\n\r▶");
		}
		else if (select_menu == Difficulty_level) {
			select_menu = Game_end;
			mouseXY(0, 5);
			printf("▷\n\r▶");
		}
	}
	else if (key == 'd' || key == 'D') {													// d, D를 눌러 게임 모드와 난이도를 변경 가능
		if (select_menu == Game_mod) {
			mouseXY(1, 4);
			if (game_mod == Solo_mod) {
				game_mod = Battle_mod;
				printf("배틀 모드");
			}
			else if (game_mod == Battle_mod) {
				game_mod = Solo_mod;
				printf("솔로 모드");
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
	else if (key == 'a' || key == 'A') {													// d, D를 눌렀을 때와 반대로 작동
		if (select_menu == Game_mod) {
			mouseXY(1, 4);
			if (game_mod == Solo_mod) {
				game_mod = Battle_mod;
				printf("배틀 모드");
			}
			else if (game_mod == Battle_mod) {
				game_mod = Solo_mod;
				printf("솔로 모드");
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
	else if (key == '\r') {																	// Enter를 누를 경우 게임을 시작
		switch (select_menu) {																						
		case Game_mod:																		// Game_mod 메뉴를 선택 중일 경우 해당 모드로 게임 시작
			gameFlag = 1;
			break;
		case Game_end:																		// Game_end를 선택 중이라면 프로그램을 종료
			mouseXY(0, 7);
			exit(0);																		// 프로그램을 종료 하는 함수
			break;
		}
	}
}

/*게임 시작 전 요소를 초기화 하는 함수*/
void map_reset() {
	int i, j;

	score = 0;																				// 점수를 초기화
	tail = 2;																				// 꼬리 개수를 2개로 초기화
	dash_limit = tail;																		// 대쉬 가능 수치를 꼬리의 개수로 초기화

	delete_node(Me);																		// 이전 게임에서의 뱀 노드를 삭제
	delete_node(Enemy);																		// Enemy_snake의 노드 삭제

	direction = Left;																		// 기본 이동 방향을 Left로 지정
	spike_spawn_delay = 0;																	// 가시 소환 딜레이 초기화

	for (i = 0; i < Map_X; i++) {															// 맵의 요소를 초기화
		for (j = 0; j < Map_Y; j++) {
			mapOrigin[j][i] = 0;															// 맵들끼리의 차이점을 보고 초기화하기에 origin = 0, copy = 100으로 초기화
			mapCopy[j][i] = 100;
		}
	}

	for (i = 0; i < Map_X; i++) {															// 천장과 바닥 입력
		mapOrigin[0][i] = 1;																// 1은 천장, 바닥
		mapOrigin[Map_Y - 1][i] = Ceilling;
	}

	for (i = 1; i < Map_Y - 1; i++) {														// 양쪽 벽 입력
		mapOrigin[i][0] = 2;																// 2은 벽
		mapOrigin[i][Map_X - 1] = Wall;
	}

	for (i = 0; i < 3; i++) {																// 맵에 뱀 추가
		if (i == 0) {
			mapOrigin[XY[1]][XY[0]] = Snake_Head;
		}
		else {
			mapOrigin[XY[1]][XY[0] + i] = Snake_Body;
		}
		insert_node_last(create_node(20 + i, 10, Left, Me), Me);							// 최초 방향을 Left로 지정
	}

	if (game_mod == Battle_mod) {															// 게임 모드가 Battle모드라면 Enemy뱀 추가
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

/*copy와 origin을 비교해 다른 부분을 출력하는 함수*/
void draw_map() {
	int i, j;
	for (i = 0; i < Map_X; i++) {
		for (j = 0; j < Map_Y; j++) {
			if (mapCopy[j][i] != mapOrigin[j][i]) {											// Copy와 Origin사이에 다른 부분이 있다면 출력
				mouseXY(i, j);
				switch (mapOrigin[j][i])													// switch로 해당하는 요소를 출력
				{
				case Snake_Head:
					if (gameEnd == 1) {
						SetConsoleTextAttribute(COUT, RED);
						printf("◇");
						SetConsoleTextAttribute(COUT, WHITE);
					}
					else {
						printf("◇");
					}
					break;
				case Snake_Body:
					printf("*");
					break;

				case Enemy_Snake_Head:
					if (gameEnd == 1) {
						SetConsoleTextAttribute(COUT, RED);
						printf("◇");
						SetConsoleTextAttribute(COUT, WHITE);
					}
					else {
						SetConsoleTextAttribute(COUT, Green);
						printf("◇");
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
					printf("▩");
					SetConsoleTextAttribute(COUT, WHITE);
					break;
				default:
					break;
				}
			}
		}
	}

	for (i = 0; i < Map_X; i++) {															// 출력이 끝난 후 Copy = Origin으로 저장
		for (j = 0; j < Map_Y; j++) {
			mapCopy[j][i] = mapOrigin[j][i];
		}
	}
}

/*키보드 입력으로 뱀 움직임 방향 제어하는 함수*/
void input_game() {
	char key;

	if (_kbhit()) {
		char key = getch();																	// getch()는 키보드의 하나 키를 입력 받을 수 있게 하는 함수

		if (gameEnd != 1 && (key == 'a' || key == 'A') && direction != Right) {				// 게임이 끝나지 않았다면 입력으로 뱀의 방향 지정
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
		else if (key == '\r') {																// 게임 중 Enter입력시 점수 저장 후 게임 종료
			if (best_score < score) {
				best_score = score;
			}
			gameFlag = 0;
		}

		if (gameEnd != 1 && dash_limit > 0 && (key == 'l' || key == 'L')) {					// 대쉬 사용
			is_dash = 2;
			dash_count = 0;
		}
		else {																				// 대쉬가 종료될 경우 is_dash를 1로 저장
			is_dash = 1;
		}
	}
	else if (dash_limit == 0) {
		is_dash = 1;
	}

	if (gameEnd != 1) {																		// 방향 지정 후 뱀을 움직이는 함수 호출
		snake_move();
	}
}

/*뱀을 움직이는 함수*/
void snake_move() {
	switch (difficulty_level)																// 게임 난이도에 따라 지연 시간 조정
	{
	case Easy:
		move_delay = 200 / is_dash;															// 대쉬 사용으로 is_dash = 2가 되면 지연 속도 / 2
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

	if (spike_spawn_delay == 50-difficulty_level * 5) {										// 뱀이 50-난이도*5 만큼 움직인다면 spike를 소환
		spike_wall_spawn();
		spike_spawn_delay = 0;																// 소환 후 spike_delay 초기화
	}
	else {
		spike_spawn_delay += 1;
	}

	struct NODE* cur = head->next_link;
	snake_move_point(Me);																	//뱀 이동 방향 이전 함수 호출

	if (direction == Left) {																// 함수 호출 후 뱀 머리의 방향 변경
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

	while (cur != NULL) {																	// 맵에서 원래 자리 제거, 각 body좌표 수정

		if (cur == head->next_link || (cur != head->next_link && mapOrigin[cur->Y][cur->X] != Snake_Head)) {
			mapOrigin[cur->Y][cur->X] = Empty;
		}

		switch (cur->Move) {																// 이동 방향에 따라 X, Y값을 변경
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

		if (mapOrigin[cur->Y][cur->X] == Wall) {											// 뱀이 이동 중 벽, 천장과 만나면 좌표를 반대편으로 이동
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

		if (cur == head->next_link) {														// 뱀의 머리일 경우 접촉시 food로 몸 추가, 적 접촉시 게임 종료 등
			if (mapOrigin[cur->Y][cur->X] == Enemy_Snake_Head || mapOrigin[cur->Y][cur->X] == Enemy_Snake_Body || mapOrigin[cur->Y][cur->X] == Snake_Body || mapOrigin[cur->Y][cur->X] == Spike_Wall) { // 뱀이 꼬리, 가시벽과 접촉?
				gameEnd = 1;
			}
			else if (mapOrigin[cur->Y][cur->X] == Food) {									// 뱀이 이동할 자리에 'Food'가 있을 경우 body추가, 'Food'리스폰
				score += 1;
				tail += 1;
				dash_limit = tail;
				add_body(Me);																// 몸을 추가하는 함수 호출
				food_spawn();																// food 소환 함수 호출
			}
			mapOrigin[cur->Y][cur->X] = Snake_Head;
		}
		else {																				// 맵 정보에 뱀의 새로운 좌표 입력
			if (mapOrigin[cur->Y][cur->X] == Snake_Head) {
				mapOrigin[cur->Y][cur->X] = Snake_Head;
			}
			else {
				mapOrigin[cur->Y][cur->X] = Snake_Body;
			}
		}
		cur = cur->next_link;																// cur를 다음 body으로 이전
	}

	draw_map();																				// 맵을 새로 출력해 뱀의 이동을 출력
	mouseXY(0, 20);

	printf("Score : %d\n", score);
	printf("대쉬 게이지 : %d/%d   \n", dash_limit, tail);

	/*테스트용 출력 코드*/
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
	printf("왼쪽 벽 : %d | 오른쪽 벽 : %d\n", LW, RW);
	printf("꼬리 : %d개\n", tail);
	*/

	if (gameEnd == 1) {
		printf("게임 종료!!!");
	}

	if (is_dash == 2) {																		// 대쉬 중 Enemy 뱀 이동 관리
		if (dash_count == 2 && game_mod == Battle_mod) {									// 대쉬 중 2번 이동마다 Enemy_snake는 1번 이동
			enemy_input_game();
			dash_count = 0;
		}
		dash_count += 1;
		dash_limit -= 1;
		input_game();
	}
}

/*Enemy 뱀 이동 방향 정하는 함수*/
void enemy_input_game() {
	struct NODE* cur = enemy_head->next_link;

	/*현재 Enemy 뱀이 움직일 수 있는 방향을 확인*/
	can_right = mapOrigin[cur->Y][cur->X + 1] == Empty || mapOrigin[cur->Y][cur->X + 1] == Food || mapOrigin[cur->Y][cur->X + 1] == Wall ? 1 : 0;
	can_left = mapOrigin[cur->Y][cur->X - 1] == Empty || mapOrigin[cur->Y][cur->X - 1] == Food || mapOrigin[cur->Y][cur->X - 1] == Wall ? 1 : 0;
	can_up = mapOrigin[cur->Y - 1][cur->X] == Empty || mapOrigin[cur->Y - 1][cur->X] == Food || mapOrigin[cur->Y - 1][cur->X] == Ceilling ? 1 : 0;
	can_down = mapOrigin[cur->Y + 1][cur->X] == Empty || mapOrigin[cur->Y + 1][cur->X] == Food || mapOrigin[cur->Y + 1][cur->X] == Ceilling ? 1 : 0;


	int Random = rand() % 2;																// 음식의 위치로 최단거리 이동을 랜덤으로 결정

	if (difficulty_level == Easy) {															// 게임 난이도가 Easy일 경우 뱀과 음식의 좌표값이 동일해지면 피하지 않음
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
	else if (difficulty_level == Normal) {													// 난이도가 Normal일 경우 2/1로 뱀이 완벽하게 회피
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
	else {																					// 난이도가 Hard일 경우 뱀이 완벽하게 회피
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
		enemy_snake_move();																	// 이동 방향이 정해진 후 Enemy뱀 이동 함수를 호출
	}
}

/*Enemy 뱀을 이동하는 함수*/
void enemy_snake_move() {

	struct NODE* cur = enemy_head->next_link;
	snake_move_point(Enemy);																// 적 뱀 이동 방향 이전


	if (enemy_direction == Left) {															// 뱀의 이동 방향을 저장된 변수값에 따라서 변경
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


	while (cur != NULL) {																	// 맵에서 원래 자리 제거, 각 body좌표 수정

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

		if (mapOrigin[cur->Y][cur->X] == Wall) {											// 뱀이 이동 중 벽과 만나면 좌표를 반대편으로 이동
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
			if (mapOrigin[cur->Y][cur->X] == Enemy_Snake_Body || mapOrigin[cur->Y][cur->X] == Snake_Head || mapOrigin[cur->Y][cur->X] == Snake_Body || mapOrigin[cur->Y][cur->X] == Spike_Wall) { // 뱀이 꼬리, 가시벽과 접촉?
				gameEnd = 1;
			}
			else if (mapOrigin[cur->Y][cur->X] == Food) {									// 뱀이 이동할 자리에 'Food'가 있을 경우 body추가, 'Food'리스폰
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
		cur = cur->next_link;																// cur를 다음 body으로 이전
	}

	draw_map();

	/*테스트용 출력 코드*/
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
		printf("게임 종료!!!");
	}
}

/*뱀의 몸을 추가하는 함수*/
void add_body(int is_enemy) {
	struct NODE* cur = last_node(is_enemy);													// 추가할 몸이 아군, 적의 것인지 매개변수로 받아 실행
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

/*food를 소환하는 함수*/
void food_spawn() {
	while (1) {
		foodX = rand() % (Map_X - 2) + 1;													// rand()로 랜덤한 x, y값을 저장
		foodY = rand() % (Map_Y - 2) + 1;

		if (mapOrigin[foodY][foodX] == Empty) {												// 좌표가 Empty가 아니라면 다시 좌표 저장
			break;
		}
	}

	mapOrigin[foodY][foodX] = Food;															// 통과된 좌표에 Food요소 입력
}

/*spike를 소환하는 함수*/
void spike_wall_spawn() {
	struct NODE* cur1 = head->next_link;
	struct NODE* cur2 = head->next_link;
	while (1) {
		spike_wallX = rand() % (Map_X - 2) + 1;												// food와 동일하게 좌표를 저장
		spike_wallY = rand() % (Map_Y - 2) + 1;

		if (mapOrigin[spike_wallY][spike_wallX] == Empty) {									// 소환 장소가 Empty, 뱀의 x, y좌표와 겹치지 않는 장소에 소환
			if (((direction == Left || direction == Right) && cur1->Y == spike_wallY) || ((direction == Down || direction == Up) && cur1->X == spike_wallX)) {
				continue;
			}
			break;
		}
	}

	mapOrigin[spike_wallY][spike_wallX] = Spike_Wall;										// 맵에 Spike_Wall요소 입력
}


