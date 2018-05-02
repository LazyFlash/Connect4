#include "linkedlist.h"

//main에서 전역변수 state 하나를 계속 사용할 것. 탐색할 때는 state의 내용을 복사해서 복사본 사용. 수를 고르고 update할 때는 원본을 갱신해야 함.
//만약 원본 갱신이 어렵다 싶으면 newstate를 return받아서 memcpy로 원본 state에 덮어주기.
//main에서 바로 다음 자식리스트 노드들에 점수가 저장되면 또 반복문 돌면서 그 중 score 가장 높은 애를 bestnode에 저장하기

List* createList() {   //게임시작 전 작업
	List* newlist = (List*)malloc(sizeof(List));
	if (newlist) {
		newlist->firstnode = NULL;
		newlist->lastnode = NULL;
	}
	return newlist;
}

void append(List*list, Node*node) {
	if (list->firstnode == NULL)
		list->firstnode = node;
	else
		list->lastnode->nextsearch = node;
	list->lastnode = node;
}

void emptyList(List* list) {
	Node* curnode = list->firstnode;
	Node* deletenode;
	while (curnode != NULL) {
		deletenode = curnode;
		curnode = curnode->nextsearch;
		free(deletenode);
	}
	list->firstnode = NULL;
	list->lastnode = NULL;
}


void appendNextNodes(List* list, char state[6][7], char player) {  //현재 전역변수 state의 가능한 자식 노드들을 리스트에 담음으로써 search를 준비. 

	int col;
	int searhseq[7] = { 3, 2, 4, 1, 5, 0, 6 };


	for (int i = 0; i < 7; i++) {
		col = searhseq[i];
		//move가 가능한지 check: [0][column](해당 열의 맨 윗칸만 비면 됨.)
		if (state[0][col] == 'X') {
			Node* newnode = (Node*)malloc(sizeof(Node));
			if (newnode) {
				memcpy(newnode->stateboard, state, 42);
				int row;
				for (row = 5; row > -1; row--) {
					if (newnode->stateboard[row][col] == 'X') {
						newnode->stateboard[row][col] = player;
						newnode->col = col;
						break;
					}
				}
				newnode->score = 0;
				newnode->nextsearch = NULL;
				append(list, newnode);//list에 append하는 함수 만들면 곧장 append(list, newnode); 추가하기
			}
		}
	}
}

//본격적인 alpha beta로 점수 매기는 함수
/*
01 function alphabeta(node, depth, Alpha, Beta, maximizingPlayer)
02      if depth = 0 or node is a terminal node
03          return the heuristic value of node
04      if maximizingPlayer
05          v := -무한
06          for each child of node
07              v := max(v, alphabeta(child, depth – 1, Alpha, Beta, FALSE))
08              Alpha := max(Alpha, v)
09              if Beta <= Alpha
10                  break (* Beta cut-off *)
11          return v
12      else
13          v := +무한
14          for each child of node
15              v := min(v, alphabeta(child, depth – 1, Alpha, Beta, TRUE))
16              Beta := min(Beta, v)
17              if Beta <= Alpha
18                  break (* Alpha cut-off *)
19          return v
*/
int AlphaBeta(char state[6][7], int depth, int Alpha, int Beta, char player) {
	//main에서 사용: 1. main에서 current state의 자식리스트를 만든다. 
	//2. 리스트 반복문으로 넘어다니면서 isnotTerminal이면 해당 자식노드->score=AlphaBeta(자식노드state, depth, -무한, +무한, T/F)
	//childs of node
	int A = Alpha;
	int B = Beta;

	//printf("Alpha: %d\n", Alpha);
	//printf("Beta: %d\n", Beta);

	if (depth == 0 || winCheck(state) != 0 || isFull(state)) { //또는 terminal 하면(승패 판정 혹은 진짜 꽉 참)

															   //printf("terminal return: %d\n", Eval(state));
															   //printf("terminal\n");
															   //draw(state);
		return Eval(state);
	}
	if (player == 'M') {
		//int v = -1000000000;
		int row;
		//char nextstate[6][7];
		int result = 0;
		int searhseq[7] = { 3, 2, 4, 1, 5, 0, 6 };
		int col;
		for (int i = 0; i<7; i++) { //for each child of node
			col = searhseq[i];
			if (state[0][col] != 'X')
				continue;
			//printf("Max's turn\n");
			//draw(state);
			row = nextState(state, col, 'M');
			result = AlphaBeta(state, depth - 1, A, B, 'P');
			A = max(A, result);
			//Alpha=max(Alpha, AlphaBeta(state, depth-1, Alpha, Beta, 'P'));

			state[row][col] = 'X';

			//int temp = max(Alpha, v);

			if (B <= A) {
				//printf("Max가 prune\n");
				break;
			}

		}
		return A;
		//return Alpha;
	}
	else {
		//int v = 1000000000;
		int row;
		//char nextstate[6][7];
		int result = 0;
		int searhseq[7] = { 3, 2, 4, 1, 5, 0, 6 };
		int col;

		for (int i = 0; i<7; i++) {
			col = searhseq[i];
			if (state[0][col] != 'X')
				continue;
			//printf("Min's turn\n");
			//draw(state);
			row = nextState(state, col, 'P'); //for each child of node
			result = AlphaBeta(state, depth - 1, A, B, 'M');
			B = min(B, result);


			state[row][col] = 'X';
			//int temp = min(Beta, v);


			if (B <= A) {
				//printf("Min이 prune\n");
				//return B;
				break;
			}

		}

		return B;
	}
}

//AlphaBeta의 마지막의 마지막에 list 비우기(각 if문의 마지막에)




//state 넘겨받고부터 수를 결정하기까지 시간 계산 함수 timetaken()

int Eval(char state[6][7]) {//수정해야 할 부분: 공간에 의해서든(이건 이미 winning line에 불포함) 다른 돌에 의해서든 이미 막혀버린 부분은 점수를 그대로 가져가는 것이 부적절함
	int eval = 0;
	int row;
	int col;


	for (row = 0; row < 6; row++) { //보드 가중치
		for (col = 0; col < 7; col++) {

			if (state[row][col] == 'M') {
				if (row < 3)
					eval += row;
				else
					eval += (5 - row);
				if (col < 4)
					eval += col;
				else
					eval += (6 - col);
			}
			/*else if (state[row][col] == 'P') {
			if (row < 3)
			eval -= row;
			else
			eval -= 5 - row;
			if (col < 4)
			eval -= col;
			else
			eval -= 6 - col;

			}*/
		}
	}
	//상대 돌에 의해 막혀버린 부분에 대한 반영: if(3번째==m){} else if(3번째==p){eval=0}
	for (row = 5; row > 2; row--) { //대각선 체크
		for (col = 0; col < 4; col++) {   //오른쪽 위로 향하는 대각선
			if (state[row][col] == 'M') {  //내 돌 두 개가 붙어있는 경우
				if (state[row - 1][col + 1] == 'M') {
					eval += 20;                                                   //+20점
					if (state[row - 2][col + 2] == 'M') {                         //세 개가 붙어있는 경우: 도합 120
						eval += 100;
						if (state[row - 3][col + 3] == 'M') {                    //내 돌 4개
							eval += 110000;
						}
						else if (state[row - 3][col + 3] == 'P') {              //내 돌 3개에 상대 돌 1개로 막힘
							eval -= 120;
						}
					}
					else if (state[row - 2][col + 2] == 'P') {//내 돌 2개에 상대 돌 1개로 막힌 경우
						eval -= 20;
					}
					else if (state[row - 2][col + 2] == 'X' && state[row - 3][col + 3] == 'M') {  // M M X M 꼴일 경우: 도합 110
						eval += 90;
					}

				}
				else if (state[row - 1][col + 1] == 'X' && state[row - 2][col + 2] == 'M' && state[row - 3][col + 3] == 'M') {// M X M M 꼴: 도합 110
					eval += 110;
				}
			}

			else if (state[row][col] == 'P') {
				if (state[row - 1][col + 1] == 'P') {
					eval -= 20;
					if (state[row - 2][col + 2] == 'P') {
						eval -= 100;
						if (state[row - 3][col + 3] == 'P') {
							eval -= 100000;
						}
						else if (state[row - 3][col + 3] == 'M') {
							eval += 120;
						}
					}
					else if (state[row - 2][col + 2] == 'M') {
						eval += 20;
					}
					else if (state[row - 2][col + 2] == 'X' && state[row - 3][col + 3] == 'P') {
						eval -= 90;
					}

				}
				else if (state[row - 1][col + 1] == 'X' && state[row - 2][col + 2] == 'M' && state[row - 3][col + 3] == 'M') {
					eval -= 110;
				}
			}
		}
		for (col = 6; col > 2; col--) {  //왼쪽 위로 향하는 대각선

			if (state[row][col] == 'M') {
				if (state[row - 1][col - 1] == 'M') {
					eval += 20;
					if (state[row - 2][col - 2] == 'M') {
						eval += 100;
						if (state[row - 3][col - 3] == 'M') {
							eval += 110000;
						}
						else if (state[row - 3][col - 3] == 'P') {
							eval -= 120;
						}
					}
					else if (state[row - 2][col - 2] == 'P') {
						eval -= 20;
					}
					else if (state[row - 2][col - 2] == 'X' && state[row - 3][col - 3] == 'M') {
						eval += 90;
					}
				}
				else if (state[row - 1][col - 1] == 'X' && state[row - 2][col - 2] == 'M' && state[row - 3][col - 3] == 'M') {
					eval += 110;
				}
			}
			else if (state[row][col] == 'P') {
				if (state[row - 1][col - 1] == 'P') {
					eval -= 20;
					if (state[row - 2][col - 2] == 'P') {
						eval -= 100;
						if (state[row - 3][col - 3] == 'P')
							eval -= 100000;
						else if (state[row - 3][col - 3] == 'M') {
							eval += 120;
						}
					}
					else if (state[row - 2][col - 2] == 'M') {
						eval += 20;
					}
					else if (state[row - 2][col - 2] == 'X' && state[row - 3][col - 3] == 'P') {
						eval -= 90;
					}
				}
				else if (state[row - 1][col - 1] == 'X' && state[row - 2][col - 2] == 'P' && state[row - 3][col - 3] == 'P') {
					eval -= 110;
				}
			}

		}
	}
	for (row = 5; row > -1; row--) { //가로 체크		 
		for (col = 0; col < 4; col++) {
			if (state[row][col] == 'M') {
				if (state[row][col + 1] == 'M') {
					eval += 20;
					if (state[row][col + 2] == 'M') {
						eval += 100;
						if (state[row][col + 3] == 'M') {
							eval += 110000;
						}
						else if (state[row][col + 3] == 'P') {
							eval -= 120;
						}
					}
					else if (state[row][col + 2] == 'P') {
						eval -= 20;
					}
					else if (state[row][col - 2] == 'X' && state[row][col - 3] == 'M') {
						eval += 90;
					}
				}
				else if (state[row][col - 1] == 'X' && state[row][col - 2] == 'M' && state[row][col - 3] == 'M') {
					eval += 110;
				}
			}
			else if (state[row][col] == 'P') {
				if (state[row][col + 1] == 'P') {
					eval -= 20;
					if (state[row][col + 2] == 'P') {
						eval -= 100;
						if (state[row][col + 3] == 'P') {
							eval -= 100000;
						}
						else if (state[row][col + 3] == 'M') {
							eval += 120;
						}
					}
					else if (state[row][col + 2] == 'M') {
						eval += 20;
					}
					else if (state[row][col - 2] == 'X' && state[row][col - 3] == 'P') {
						eval -= 90;
					}
				}
				else if (state[row][col - 1] == 'X' && state[row][col - 2] == 'P' && state[row][col - 3] == 'P') {
					eval -= 110;
				}
			}
		}
	}

	for (col = 0; col < 7; col++) { //세로 체크
									/*if (state[row][col] == 'M') {
									if (state[row][col + 1] == 'M') {
									eval += 20;
									if (state[row][col + 2] == 'M') {
									eval += 100;
									if (state[row][col + 3] == 'M') {
									eval += 110000;
									}
									else if (state[row][col + 3] == 'P') {
									eval -= 120;
									}
									}
									else if (state[row][col + 2] == 'P') {
									eval -= 20;
									}
									else if (state[row][col - 2] == 'X' && state[row][col - 3] == 'M') {
									eval += 90;
									}
									}
									else if (state[row][col - 1] == 'X' && state[row][col - 2] == 'M' && state[row][col - 3] == 'M') {
									eval += 110;
									}
									}*/
		for (row = 5; row > 2; row--) {
			if (state[row][col] == 'M') {
				if (state[row - 1][col] == 'M') {                    //내 돌 2개가 연속
					eval += 10;
					if (state[row - 2][col] == 'M') {               //내 돌 3개가 연속
						eval += 50;
						if (state[row - 3][col] == 'M') {           //내 돌 4개가 연속. 2개, 3개 연속의 점수가 가로/대각선보다 60점 낮았기 때문에 일단 60 더해줘볼 수도 있음.
							eval += 110000;
						}
						else if (state[row - 3][col] == 'P') {     //내 돌 3개 위에 상대 돌 1개로 막힘
							eval -= 60;
						}
					}
					else if (state[row - 2][col] == 'P') {                                   //내 돌 2개 위에 상대 돌 1개로 막힘
						eval -= 10;
					}
				}
			}
			else if (state[row][col] == 'P') {
				if (state[row - 1][col] == 'P') {
					eval -= 10;
					if (state[row - 2][col] == 'P') {
						eval -= 50;
						if (state[row - 3][col] == 'P') {
							eval -= 100000;
						}
						else if (state[row - 3][col] == 'M') {
							eval += 60;
						}
					}
					else if (state[row - 2][col] == 'M') {
						eval += 10;
					}
				}
			}
		}
	}

	return eval;
}

int nextState(char state[6][7], int col, char player) {

	//memcpy(nextstate, state, 42);
	if (state[0][col] == 'X') {
		int row;
		for (row = 5; row >-1; row--) {
			if (state[row][col] == 'X') {
				state[row][col] = player;
				break;
			}
		}
		return row;
	}
}

void draw(char state[6][7]) {
	int i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			printf("%c ", state[i][j]);
		}
		printf("\n");
	}
	printf("1 2 3 4 5 6 7\n");
	printf("\n\n");
}

int winCheck(char state[6][7]) { //이게 무거우면 직전 move의 col과 row를 받아서 거기 기준으로 winning line들 생겼는지만 체크하도록 하기
	int row;
	int col;
	for (row = 5; row > 2; row--) { //대각선 체크
		for (col = 0; col < 4; col++) {   //오른쪽 위로 향하는 대각선
			if (state[row][col] == 'M' && state[row - 1][col + 1] == 'M' && state[row - 2][col + 2] == 'M' && state[row - 3][col + 3] == 'M')
				return 1;  //M이 이길 때
			else if (state[row][col] == 'P' && state[row - 1][col + 1] == 'P' && state[row - 2][col + 2] == 'P' && state[row - 3][col + 3] == 'P')
				return -1; //P가 이길 때
		}
		for (col = 6; col > 2; col--) {  //왼쪽 위로 향하는 대각선
			if (state[row][col] == 'M' && state[row - 1][col - 1] == 'M' && state[row - 2][col - 2] == 'M' && state[row - 3][col - 3] == 'M')
				return 1;  //M이 이길 때
			else if (state[row][col] == 'P' && state[row - 1][col - 1] == 'P' && state[row - 2][col - 2] == 'P' && state[row - 3][col - 3] == 'P')
				return -1; //P가 이길 때
		}
	}
	for (row = 5; row > -1; row--) { //가로 체크
		for (col = 0; col < 4; col++) {
			if (state[row][col] == 'M' && state[row][col + 1] == 'M' && state[row][col + 2] == 'M' && state[row][col + 3] == 'M')
				return 1;
			else if (state[row][col] == 'P' && state[row][col + 1] == 'P' && state[row][col + 2] == 'P' && state[row][col + 3] == 'P')
				return -1;
		}
	}
	for (col = 0; col < 7; col++) { //세로 체크
		for (row = 5; row > 2; row--) {
			if (state[row][col] == 'M' && state[row - 1][col] == 'M' && state[row - 2][col] == 'M' && state[row - 3][col] == 'M')
				return 1;
			else if (state[row][col] == 'P' && state[row - 1][col] == 'P' && state[row - 2][col] == 'P' && state[row - 3][col] == 'P')
				return -1;
		}
	}
	return 0; //승부 안남
}

int isFull(char state[6][7]) {
	int full = 0;
	for (int col = 0; col < 7; col++) {
		if (state[0][col] != 'X')
			full++;
	}
	if (full == 7)
		full = 1;
	else
		full = 0;
	return full;
}