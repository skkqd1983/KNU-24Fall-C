#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct NODE* head;

struct NODE {
	char name[10];
	int score;
	struct NODE* link;
};

struct NODE* create_node(char name[10], int score) { //��� ����
	struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));
	strcpy_s(new_node->name, 10, name);
	new_node->score = score;
	new_node->link = NULL;

	return new_node;
}

struct NODE* last_node() {
	struct NODE* cur = head;
	while (cur->link != NULL) {
		cur = cur->link;
	}
	return cur;
}

void insert_node_last(struct NODE* new_node) {
	struct NODE* last = last_node();
	struct NODE* prev = head;
	struct NODE* cur = head->link;
	while (cur != NULL) {
		if (new_node->score > cur->score) {
			prev->link = new_node;
			new_node->link = cur;
			return;
		}
		prev = cur;
		cur = cur->link;
	}
	last->link = new_node;
}

void print_node() { // ��ü �л� ���
	struct NODE* cur = head->link;
	printf("---------------------------\n");
	while (cur != NULL) {
		printf("%s\t:\t%d\n", cur->name, cur->score);
		cur = cur->link;
	}
	printf("---------------------------\n");
}

int delete_node(char name[10]) { // �л� ���� ����
	struct NODE* prev = head;
	struct NODE* cur = head->link;
	while (cur != NULL) {
		if (strcmp(cur->name, name) == 0) {
			prev->link = cur->link;
			free(cur);
			return 1;
		}
		prev = cur;
		cur = cur->link;
	}
	return 0;
}

int main() {
	char name[10];
	int input = 0, score = 0;

	head = (struct NODE*)malloc(sizeof(struct NODE));
	head->link = NULL;

	insert_node_last(create_node("ȫ�浿", 99));
	insert_node_last(create_node("���缮", 70));

	while (1) {
		print_node();
		printf("1. �л��� ������ �Է�\n2. ���� ���� ����\n3. ���α׷� ����\ninput : ");
		scanf_s("%d", &input);
		switch (input) {
		case 1:
			printf("���� �̸� : ");
			scanf_s("%s", &name, 10);
			printf("%s�� ���� : ", name);
			scanf_s("%d", &score);
			insert_node_last(create_node(name, score));
			break;
		case 2:
			printf("���� �̸� : ");
			scanf_s("%s", &name, 10);
			delete_node(name);
			break;
		case 3:
			return 0;
		default:
			break;
		}
	}
}