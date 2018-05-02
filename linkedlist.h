#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//main���� �������� state �ϳ��� ��� ����� ��. Ž���� ���� state�� ������ �����ؼ� ���纻 ���. ���� ���� update�� ���� ������ �����ؾ� ��.
//���� ���� ������ ��ƴ� ������ newstate�� return�޾Ƽ� memcpy�� ���� state�� �����ֱ�.
//main���� �ٷ� ���� �ڽĸ���Ʈ ���鿡 ������ ����Ǹ� �� �ݺ��� ���鼭 �� �� score ���� ���� �ָ� bestnode�� �����ϱ�
typedef struct node {
	char stateboard[6][7];
	struct node* nextsearch;
	int score;
	int col;
}Node;

typedef struct list {
	Node* firstnode;
	Node* lastnode;
}List;

List* createList(void);
void append(List*list, Node*node);
void emptyList(List* list); //empty�� ��Ű�� ��� ��Ÿ����� ��
int nextState(char state[6][7], int move, char player);//char nextstate[6][7], 
void appendNextNodes(List* list, char state[6][7], char player);
int AlphaBeta(char state[6][7], int depth, int Alpha, int Beta, char player);
int Eval(char state[6][7]);
void draw(char state[6][7]);
int winCheck(char state[6][7]);
int isFull(char state[6][7]);
