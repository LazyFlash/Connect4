#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//main에서 전역변수 state 하나를 계속 사용할 것. 탐색할 때는 state의 내용을 복사해서 복사본 사용. 수를 고르고 update할 때는 원본을 갱신해야 함.
//만약 원본 갱신이 어렵다 싶으면 newstate를 return받아서 memcpy로 원본 state에 덮어주기.
//main에서 바로 다음 자식리스트 노드들에 점수가 저장되면 또 반복문 돌면서 그 중 score 가장 높은 애를 bestnode에 저장하기

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))
#define false 0
#define true 1
typedef int bool;

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
void emptyList(List* list); //empty만 시키고 계속 울궈먹으면 됨
int nextState(char state[6][7], int move, char player);//char nextstate[6][7],
void appendNextNodes(List* list, char state[6][7], char player);
int AlphaBeta(char state[6][7], int depth, int Alpha, int Beta, char player);
int Eval(char state[6][7]);
void draw(char state[6][7]);
int winCheck(char state[6][7]);
int isFull(char state[6][7]);

// rule 함수
int rule(char state[6][7], int turn, char player);
bool checkfour(char state[6][7], char player);
bool checkthree(char state[6][7], int row, char player);
int ruleOne(char state[6][7]);
int ruleTwo(char state[6][7]);
int ruleThree(char state[6][7]);
int ruleFour(char state[6][7]);
int ruleFive(char state[6][7]);
int ruleJ(char state[6][7]);
int ruleSix(char state[6][7]);
int ruleSeven(char state[6][7], char player);
int ruleNine(char state[6][7]);
int ruleTen(char state[6][7]);
int checkMajor(char state[6][7], char player);            //board와 player 정보를 받아서 해당 플레이어가 갖고 있는 major threat 개수를 반환
int checkMinor(char state[6][7], char player);            //minor threat check