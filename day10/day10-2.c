#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Rank {    // ������ ����
	First = 1,         // �ʱ갪 �Ҵ�
	Second,
	Third,
	Fourth,
	Fifth,
	Sixth,
	Seventh
};


struct Customer* head;


struct Customer {
	char* customerName[10];
	enum Rank rank;
	int order_amount;
	int point;
	struct Customer* prev;
	struct Customer* next;
};

struct Customer* create_node(char Name[10], enum Rank rank, int Order_amount, int Point) { //��� ����
	struct Customer* new_node = (struct Customer*)malloc(sizeof(struct Customer));
	strcpy_s(new_node->customerName, 10, Name);
	new_node->rank = rank;
	new_node->order_amount = Order_amount;
	new_node->point = Point;
	new_node->prev = NULL;
	new_node->next = NULL;

	return new_node;
}

struct Customer* last_node() {
	struct Customer* cur = head;
	while (cur->next != NULL) {
		cur = cur->next;
	}
	return cur;
}

void insert_node_last(struct Customer* new_node) {
	struct Customer* last = last_node();
	struct Customer* prev = head;
	struct Customer* cur = head->next;
	while (cur != NULL) {
		if (new_node->rank < cur->rank) {
			prev->next = new_node;
			new_node->next = cur;

			cur->prev = new_node;
			new_node->prev = prev;
			return;
		}
		else if (new_node->rank == cur->rank) {
			if (new_node->order_amount > cur->order_amount) {
				prev->next = new_node;
				new_node->next = cur;

				cur->prev = new_node;
				new_node->prev = prev;
				return;
			}
			else if (new_node->order_amount == cur->order_amount) {
				if (new_node->point > cur->point) {
					prev->next = new_node;
					new_node->next = cur;

					cur->prev = new_node;
					new_node->prev = prev;
					return;
				}
			}
		}
		prev = cur;
		cur = cur->next;
	}
	last->next = new_node;
	new_node->prev = last;
}

void print_node() { // ��ü �� ���
	struct Customer* cur = head->next;
	printf("---------------------------\n");
	while (cur != NULL) {
		printf("%s\t:\t%d\t%d\t%d\n", cur->customerName, cur->rank, cur->order_amount, cur->point);
		
		cur = cur->next;
	}
	printf("---------------------------\n");
}

int delete_node(char name[10]) { // �� ���� ����
	struct Customer* prev = head;
	struct Customer* cur = head->next;
	while (cur != NULL) {
		if (strcmp(cur->customerName, name) == 0) {
			prev->next = cur->next;
			cur->next->prev = prev;
			free(cur);
			return 1;
		}
		prev = cur;
		cur = cur->next;
	}
	return 0;
}

int main() {
	char name[10];
	int input = 0, order_amount = 0, point = 0, rank = 0;

	head = (struct Customer*)malloc(sizeof(struct Customer));
	head->next = NULL;

	insert_node_last(create_node("A", (enum Rank)1, 100, 100));
	insert_node_last(create_node("B", (enum Rank)2, 200, 200));
	insert_node_last(create_node("C", (enum Rank)1, 30, 300));

	while (1) {
		print_node();
		printf("1. �� ���� �Է�\n2. �� ���� ����\n3. ���α׷� ����\ninput : ");
		scanf_s("%d", &input);
		switch (input) {
		case 1:
			printf("�� �̸� : ");
			scanf_s("%s", &name, 10);
			printf("%s�� ���(1 ~ 7) : ", name);
			scanf_s("%d", &rank);
			printf("%s�� �ֹ��� : ", name);
			scanf_s("%d", &order_amount);
			printf("%s�� ����Ʈ�� : ", name);
			scanf_s("%d", &point);
			insert_node_last(create_node(name, rank, order_amount, point));
			break;
		case 2:
			printf("�� �̸� : ");
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