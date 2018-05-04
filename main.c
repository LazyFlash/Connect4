#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "list.h"
#include <time.h>



//맨 처음에 선공 후공 선택. 인공지능을 M, 플레이어(상대)를 P로 저장

int main(void) {

	int depth = 11;

	char state[6][7] = {
		{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' },
	{ 'X', 'X', 'X', 'X', 'X', 'X', 'X' } };


	//이하의 리스트 채우고 비우는 과정이 게임이 끝나지 않는 동안 반복문 안에 들어가게 됨

	List* childList = createList(); // AI의 이번 수 최대 7가지를 저장할 list
	int gameIsnotTerminal = 1;  //1이면 Game이 계속 진행, 아니면 Game 끝

	Node* curnode;  //list 각 수의 점수 저장용과 최선의 수 탐색용
	int maxscore; //list에 저장된 이번 수들의 점수를 비교하며 어느 것이 더 나은 수인지 확인용
	int maxindex;  //list의 수들 중에서 최선의 수의 column 번호 저장용
	int index;  //list 최선의 수 탐색용

	int win = 0;
	int playerchoice;
	int dummy;
	int turn = 1; //mod 2 1은 선공 차례 0은 후공 차례

	int AIfirst; //1이면 AI가 선공, 0이면 AI가 후공
	time_t start; //start 시간과 end 시간
	time_t end;
	double interval = 0; //걸린 시간
	int mode = 0; //Search인가 Rule인가

	int ruleresult;
	int ruletworesult;

	printf("선공으로 하시겠습니까? 선공이면 1, 후공이면 0 입력 ");
	scanf("%d", &playerchoice);
	getchar();
	if (playerchoice == 1) {
		AIfirst = 0;
	}
	else if (playerchoice == 0) {
		AIfirst = 1;
	}

	if (AIfirst) {//Ai 선공인 경우

		while (gameIsnotTerminal) {
			printf("\n<<turn%d>>\n", turn);
			if (turn % 2 == 1) {   //AI의 차례
				printf("Search로 하시겠습니까 Rule로 하시겠습니까? Search면 1, Rule이면 2 입력 ");     //이 자리에 Rule이냐 Search냐 묻는 코드 넣기
				scanf("%d", &mode);
				getchar();
				start = time(NULL);

				//turn==1, 3일 때는 공통 코드 쓰기.
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
				else if (turn == 3) {
					if (state[4][2] == 'P' || state[4][3] == 'P') {//상대의 다음 수 3: 2 또는 4   ->4 선택/4: 3 또는 4 ->4 선택
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
					else if (state[4][2] == 'P' || state[4][5] == 'P') {//	5 : 3 또는 5  -> 3 선택/6 : 2 - 6까지 아무거나-> 3 선택
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
				}


				if (mode == 1) {   //Search 모드
					appendNextNodes(childList, state, 'M');
					curnode = childList->firstnode;
					while (curnode != NULL) {
						curnode->score = AlphaBeta(curnode->stateboard, depth, -1000000000, 1000000000, 'P');
						curnode = curnode->nextsearch;
					}
					curnode = childList->firstnode;
					maxscore = curnode->score;
					index = curnode->col;
					maxindex = index;
					while (curnode != NULL) {
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
					ruleresult = rule(state, turn, 'M');
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
			if (turn == 5) {
				depth = 10;
			}
			else if (turn == 12) {
				depth = 11;
			}
			else if (turn == 15) {
				depth = 13;
			}
			else if (turn == 17) {
				depth = 15;
			}
			else if (turn == 19) {
				depth = 17;
			}
			else if (turn == 20) {
				depth = 20;
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
				printf("Search로 하시겠습니까 Rule로 하시겠습니까? Search면 1, Rule이면 2 입력 ");
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
				else if (turn == 4) {
					if (state[4][2] == 'P') {//상대가 3: 3
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
					else if (state[4][3] == 'P') {//상대가 4: 4
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
				}

				if (mode == 1) {  //Search 모드
					appendNextNodes(childList, state, 'M');
					curnode = childList->firstnode;
					while (curnode != NULL) {
						curnode->score = AlphaBeta(curnode->stateboard, depth, -1000000000, 1000000000, 'P');
						curnode = curnode->nextsearch;
					}
					curnode = childList->firstnode;
					maxscore = curnode->score;
					index = curnode->col;
					maxindex = index;
					while (curnode != NULL) {
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
					ruletworesult = rule(state, turn, 'P');
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
			if (turn == 5) {
				depth = 10;
			}
			else if (turn == 12) {
				depth = 11;
			}
			else if (turn == 15) {
				depth = 13;
			}
			else if (turn == 17) {
				depth = 15;
			}
			else if (turn == 19) {
				depth = 17;
			}
			else if (turn == 20) {
				depth = 20;
			}
			else if (turn == 43) {
				printf("무승부\n");
				break;
			}
		}

	}

	getchar();
	return 0;
}