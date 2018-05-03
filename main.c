#include <stdio.h>
#include "linkedlist.h"
#include <time.h>



//맨 처음에 선공 후공 선택하게 해서 인공지능을 M, 플레이어(상대)를 P로 저장
//후공일 때도 똑같이 나를 max로 하는

int main(void) {

	int depth = 10;

	char state[6][7] = {
		{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' } };


	//이하의 리스트 채우고 비우는 과정이 게임이 끝나지 않는 동안 반복문 안에 들어가게 됨

	List* childList = createList();
	int gameIsnotTerminal = 1;
	Node* curnode;




	//printf("%d\n", childList->firstnode->score);
	//printf("%d\n", childList->lastnode->score);
	//printf("%d\n", childList->firstnode->nextsearch->nextsearch->score);

	//리스트 작동 체크
	/*appendNextNodes(childList, state, 'M');
	curnode = childList->firstnode;
	while (curnode != NULL) {
	draw(curnode->stateboard);
	curnode = curnode->nextsearch;
	}
	printf("최초 state 상태:\n");
	draw(state);*/

	//Alpha Beta call 테스트
	int maxscore;
	int maxindex;
	int index;
	int win = 0;
	int playerchoice;
	int dummy;
	int turn = 1; //mod 2 1은 선공 차례 0은 후공 차례
				  //이하가 Game Turn 반복문 내부
	int AIfirst; //1이면 AI가 선공, 0이면 AI가 후공
	time_t start; //start 시간과 end 시간
	time_t end;
	double interval = 0; //걸린 시간
	int mode = 0; //Search인가 Rule인가


	printf("선공으로 하시겠습니까? 선공이면 1, 후공이면 0 입력 ");
	scanf("%d", &playerchoice);
	getchar();
	if (playerchoice == 1) {
		AIfirst = 0;
	}
	else if (playerchoice == 0) {
		AIfirst = 1;
	}

	if (AIfirst) {

		while (gameIsnotTerminal) {
			printf("\n<<turn%d>>\n", turn);
			if (turn % 2 == 1) {   //AI의 차례
				printf("Search로 하시겠습니까 Rule로 하시겠습니까? Search면 1, Rule이면 2 입력 ");     //이 자리에 Rule이냐 Search냐 묻는 코드 넣기
				scanf("%d", &mode);
				getchar();
				start = time(NULL);

				//turn==1일 때는 공통 코드 쓰기.
				if (turn == 1) {
					end = time(NULL);
					interval = difftime(end, start);
					dummy = nextState(state, 2, 'M');
					printf("%f초 걸림\n", interval);
					if (mode == 1)
						printf("Search mode: 3번 column에 착수\n");
					else if (mode == 2)
						printf("Rule mode: 3번 column에 착수\n");
					draw(state);
					turn++;

					continue;
				}

				if (mode == 1) {   //Search 모드
					appendNextNodes(childList, state, 'M');
					curnode = childList->firstnode;
					while (curnode != NULL) {
						//printf("%d\n", nodenum);
						//nodenum++;
						//if(!losingMove(curnode->stateboard))  //losing move면 애초에 함수도 안 호출하도록
						curnode->score = AlphaBeta(curnode->stateboard, depth, -1000000000, 1000000000, 'P');
						curnode = curnode->nextsearch;
					}
					curnode = childList->firstnode;
					maxscore = curnode->score;
					index = curnode->col;
					maxindex = index;
					while (curnode != NULL) { //col0~col6까지 순서대로 append했었음.
						printf("%d번째 col에 두면 점수: %d\n", curnode->col + 1, curnode->score);
						if (curnode->score > maxscore) {
							maxindex = curnode->col;
							maxscore = curnode->score;
						}
						curnode = curnode->nextsearch;

					}
					end = time(NULL);
					dummy = nextState(state, maxindex, 'M');
					//결정 후 착수 완료

					interval = difftime(end, start);
					printf("%f초 걸림\n", interval);
					printf("Search mode: %d번 column에 착수\n", maxindex + 1);
					emptyList(childList);
				}
				else if (mode == 2) {  //Rule 모드
					int ruleresult = rule(state, turn, 'M');
					dummy = nextState(state, ruleresult, 'M');
					printf("%f초 걸림\n", interval);
					printf("Rule mode: %d번 column에 착수\n", ruleresult + 1);
				}
			}
			else {
				while (1) {
					printf("당신의 차례입니다. 둘 column을 고르세요:(1-7)");
					scanf("%d", &playerchoice);
					getchar();
					playerchoice--;
					if (state[0][playerchoice] == 'X') {
						dummy = nextState(state, playerchoice, 'P');
						break;
					}
					printf("불가능한 수입니다. 다시 골라주세요.\n");
				}

			}

			draw(state);
			win = winCheck(state);
			if (win == 1) {
				printf("AI M이 승리\n");
				gameIsnotTerminal = 0;
			}
			else if (win == -1) {
				printf("플레이어 P가 승리\n");
				gameIsnotTerminal = 0;
			}
			turn++;
			if (turn == 12) {
				depth = 11;
			}
			else if (turn == 12) {
				depth = 13;
			}
			else if (turn == 15) {
				depth = 14;
			}
			else if (turn == 17) {
				depth = 16;
			}
			else if (turn == 19) {
				depth = 17;
			}
			else if (turn == 21) {
				depth = 19;
			}
			else if (turn == 43) {
				printf("무승부\n");
				break;
			}
		}
	}
	else {
		draw(state);
		while (gameIsnotTerminal) {
			printf("\n<<turn%d>>\n", turn);
			if (turn % 2 == 0) {
				printf("Search로 하시겠습니까 Rule로 하시겠습니까? Search면 1, Rule이면 2 입력 ");     //이 자리에 Rule이냐 Search냐 묻는 코드 넣기
				scanf("%d", &mode);
				getchar();
				start = time(NULL);

				if (turn == 2) {
					end = time(NULL);
					interval = difftime(end, start);
					dummy = nextState(state, 3, 'M');
					printf("%f초 걸림\n", interval);
					if (mode == 1)
						printf("Search mode: 4번 column에 착수\n");
					else if (mode == 2)
						printf("Rule mode: 4번 column에 착수\n");
					draw(state);
					turn++;


					continue;
				}

				if (mode == 1) {  //Search 모드
					appendNextNodes(childList, state, 'M');
					curnode = childList->firstnode;
					while (curnode != NULL) {
						//printf("%d\n", nodenum);
						//nodenum++;
						//if(!losingMove(curnode->stateboard))  //losing move면 애초에 함수도 안 호출하도록
						curnode->score = AlphaBeta(curnode->stateboard, depth, -1000000000, 1000000000, 'P');
						curnode = curnode->nextsearch;
					}
					curnode = childList->firstnode;
					maxscore = curnode->score;
					index = curnode->col;
					maxindex = index;
					while (curnode != NULL) { //col0~col6까지 순서대로 append했었음.
						printf("%d번째 col에 두면 점수: %d\n", curnode->col + 1, curnode->score);
						if (curnode->score > maxscore) {
							maxindex = curnode->col;
							maxscore = curnode->score;
						}
						curnode = curnode->nextsearch;

					}
					end = time(NULL);
					dummy = nextState(state, maxindex, 'M');

					interval = difftime(end, start);
					printf("%f초 걸림\n", interval);
					printf("Search mode: %d번 column에 착수\n", maxindex + 1);
					emptyList(childList);  //현재의 바로 다음 자식들의 점수매김이 끝나고 선택을 마치면 childList를 비워낸다.
				}
				else if (mode == 2) {  //Rule 모드
					int ruletworesult = rule(state, turn, 'P');
					dummy = nextState(state, ruletworesult, 'M');
					printf("%f초 걸림\n", interval);
					printf("Rule mode: %d번 column에 착수\n", ruletworesult + 1);
				}
			}
			else {
				while (1) {
					printf("당신의 차례입니다. 둘 column을 고르세요:(1-7)");
					scanf("%d", &playerchoice);
					getchar();


					while (turn == 1 && playerchoice == 4) {
						printf("선공일 경우, 첫 수에는 가운데에 둘 수 없습니다. 둘 column을 다시 고르세요:(1-7)");
						scanf("%d", &playerchoice);
						getchar();
					}

					playerchoice—;
					if (state[0][playerchoice] == 'X') {
						dummy = nextState(state, playerchoice, 'P');
						break;
					}
					printf("불가능한 수입니다. 다시 골라주세요.\n");
				}

			}

			draw(state);
			win = winCheck(state);
			if (win == 1) {
				printf("AI M이 승리\n");
				gameIsnotTerminal = 0;
			}
			else if (win == -1) {
				printf("플레이어 P가 승리\n");
				gameIsnotTerminal = 0;
			}
			turn++;
			if (turn == 12) {
				depth = 11;
			}
			else if (turn == 12) {
				depth = 13;
			}
			else if (turn == 15) {
				depth = 14;
			}
			else if (turn == 17) {
				depth = 16;
			}
			else if (turn == 19) {
				depth = 17;
			}
			else if (turn == 21) {
				depth = 19;
			}
			else if (turn == 43) {
				printf("무승부\n");
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