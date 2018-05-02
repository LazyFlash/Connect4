#include <stdio.h>
#include "linkedlist.h"
#include <time.h>



//�� ó���� ���� �İ� �����ϰ� �ؼ� �ΰ������� M, �÷��̾�(���)�� P�� ����
//�İ��� ���� �Ȱ��� ���� max�� �ϴ� 

int main(void) {
	
	int depth =12;
	
	char state[6][7] = {
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' } };


	//������ ����Ʈ ä��� ���� ������ ������ ������ �ʴ� ���� �ݺ��� �ȿ� ���� ��
	
	List* childList = createList();
	int gameIsnotTerminal=1;
	Node* curnode;

	
	
	
	//printf("%d\n", childList->firstnode->score);
	//printf("%d\n", childList->lastnode->score);
	//printf("%d\n", childList->firstnode->nextsearch->nextsearch->score);
	
	//����Ʈ �۵� üũ
	/*appendNextNodes(childList, state, 'M');
	curnode = childList->firstnode;
	while (curnode != NULL) {
		draw(curnode->stateboard);
		curnode = curnode->nextsearch;
	}
	printf("���� state ����:\n");
	draw(state);*/

	//Alpha Beta call �׽�Ʈ
	int maxscore;
	int maxindex;
	int index;
	int win = 0;
	int playerchoice;
	int dummy;
	int turn=1; //mod 2 1�� ���� ���� 0�� �İ� ����
				//���ϰ� Game Turn �ݺ��� ����
	int AIfirst; //1�̸� AI�� ����, 0�̸� AI�� �İ�
	time_t start; //start �ð��� end �ð�
	time_t end;
	double interval = 0; //�ɸ� �ð�


	printf("�������� �Ͻðڽ��ϱ�? �����̸� 1, �İ��̸� 0 �Է� ");
	scanf_s("%d", &playerchoice);
	getchar();
	if (playerchoice == 1) {
		AIfirst = 0;
	}
	else if (playerchoice == 0) {
		AIfirst = 1;
	}

	if (AIfirst) {

		while (gameIsnotTerminal) {
			printf("<<turn%d>>\n", turn);
			if (turn % 2 == 1) {
				                                                       //�� �ڸ��� Rule�̳� Search�� ���� �ڵ� �ֱ�
								                                       //turn==1�� ���� ���� �ڵ� ����. if(turn==1){} ���Ϸ� if(Rule){} �ֱ�
				start = time(NULL);
				if (turn == 1) {
					dummy = nextState(state, 2, 'M');
					draw(state);
					turn++;
					continue;
				}                                                      
				appendNextNodes(childList, state, 'M');
				curnode = childList->firstnode;
				while (curnode != NULL) {
					//printf("%d\n", nodenum);
					//nodenum++;
					//if(!losingMove(curnode->stateboard))  //losing move�� ���ʿ� �Լ��� �� ȣ���ϵ���
					curnode->score = AlphaBeta(curnode->stateboard, depth, -1000000000, 1000000000, 'P');
					curnode = curnode->nextsearch;
				}
				curnode = childList->firstnode;
				maxscore = curnode->score;
				index = curnode->col;
				maxindex = index;
				while (curnode != NULL) { //col0~col6���� ������� append�߾���.
					printf("%d��° col�� �θ� ����: %d\n", curnode->col+1, curnode->score);
					if (curnode->score > maxscore) {
						maxindex = curnode->col;
						maxscore = curnode->score;
					}
					curnode = curnode->nextsearch;

				}
				dummy = nextState(state, maxindex, 'M');
				//���� �� ���� �Ϸ�
				end = time(NULL);
				interval = difftime(end, start);
				printf("���� �� �������� %f�� �ɸ�\n", interval);
				emptyList(childList);
			}
			else {
				while (1) {
					printf("����� �����Դϴ�. �� column�� ������:(1-7)");
					scanf_s("%d", &playerchoice);
					getchar();
					playerchoice--;
					if (state[0][playerchoice] == 'X') {
						dummy = nextState(state, playerchoice, 'P');
						break;
					}
				}

			}

			draw(state);
			win = winCheck(state);
			if (win == 1) {
				printf("AI M�� �¸�\n");
				gameIsnotTerminal = 0;
			}
			else if (win == -1) {
				printf("�÷��̾� P�� �¸�\n");
				gameIsnotTerminal = 0;
			}
			turn++;
			if (turn == 43) {
				printf("���º�\n");
				break;
			}
		}
	}
	else {
		draw(state);
		while (gameIsnotTerminal) {
			printf("<<turn%d>>\n", turn);
			if (turn % 2 == 0) {
				//�� �ڸ��� Rule�̳� Search�� ���� �ڵ� �ֱ�
				//if(Rule){} �ֱ�

				start = time(NULL);
				appendNextNodes(childList, state, 'M');
				curnode = childList->firstnode;
				while (curnode != NULL) {
					//printf("%d\n", nodenum);
					//nodenum++;
					//if(!losingMove(curnode->stateboard))  //losing move�� ���ʿ� �Լ��� �� ȣ���ϵ���
					curnode->score = AlphaBeta(curnode->stateboard, depth, -1000000000, 1000000000, 'P');
					curnode = curnode->nextsearch;
				}
				curnode = childList->firstnode;
				maxscore = curnode->score;
				index = curnode->col;
				maxindex = index;
				while (curnode != NULL) { //col0~col6���� ������� append�߾���.
					printf("%d��° col�� �θ� ����: %d\n", curnode->col+1, curnode->score);
					if (curnode->score > maxscore) {
						maxindex = curnode->col;
						maxscore = curnode->score;
					}
					curnode = curnode->nextsearch;

				}
				dummy = nextState(state, maxindex, 'M');

				end = time(NULL);
				interval = difftime(end, start);
				printf("���� �� �������� %f�� �ɸ�\n", interval);
				emptyList(childList);  //������ �ٷ� ���� �ڽĵ��� �����ű��� ������ ������ ��ġ�� childList�� �������.
			}
			else {
				while (1) {
					printf("����� �����Դϴ�. �� column�� ������:(1-7)");
					scanf_s("%d", &playerchoice);
					getchar();
					playerchoice--;
					if (state[0][playerchoice] == 'X') {
						dummy = nextState(state, playerchoice, 'P');
						break;
					}
				}

			}

			draw(state);
			win = winCheck(state);
			if (win == 1) {
				printf("AI M�� �¸�\n");
				gameIsnotTerminal = 0;
			}
			else if (win == -1) {
				printf("�÷��̾� P�� �¸�\n");
				gameIsnotTerminal = 0;
			}
			turn++;
			if (turn == 43) {
				printf("���º�\n");
				break;
			}
		}
	
	}
	



	/*curnode = childList->firstnode;
	while (curnode != NULL) {
		printf("%d\n", curnode->score);
		curnode = curnode->nextsearch;
	}*/
	
	
	//printf("%d", childList->firstnode->score);


	getchar();
	return 0;
}