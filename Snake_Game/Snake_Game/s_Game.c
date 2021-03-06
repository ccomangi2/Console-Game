#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#define _CRT_SECURE_NO_WARNINGS
#define EXAMPLE_MACRO_NAME

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define Q 113
#define ESC 27
#define ENTER 13
#define R 114

//32번==> SP:   Space
//13번 ==> CR: 엔터
//27번 ==> ESC


int x[1000], y[1000]; //x,y 좌표값을 저장 총 1000개 
int food_x, food_y; //별 좌표값을 저장
int length; //몸길이
int speed; //지렁이 속도
int score; //점수 저장  --reset함수에 의해 초기화됨
int best_score = 0; //최고 점수 저장 --reset함수에 의해 초기화 되지 않음 
int last_score = 0; //마지막 점수 저장  --reset함수에 의해 초기화 되지 않음
int dir; //이동방향 저장 
int key; //입력받은 키 저장 

void gotoxy(int x, int y, char* s) { //x값을 2x로 변경, 좌표값에 바로 문자열을 입력할 수 있도록 printf함수 삽입  
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //x,y 값을 저장, 표준콘솔출력의 핸들을 반환
	printf("%s", s);
}

void title(void); //게임 시작화면 
void reset(void); //게임을 초기화 
void draw_map(void); // 게임테두리 
void move(int dir); //지렁이 머리 이동
void pause(void); //일시정지 
void game_over(void); //죽었습니다 
void food(void); //별 생성 
void status(void); // 개발자용 status표시

int main() {
	title();

	while (1) {
		//kbhit = 키가 입력된 상태인가
		//getch = 키보드의 입력을 받는 함수
		if (_kbhit()) do { key = _getch(); } while (key == 224); //키 입력
		//Sleep = 작업을 대기하고 싶을 때 사용하는 함수
		Sleep(speed);
		switch (key) { //입력받은 키 실행
		case LEFT: case RIGHT: case UP: case DOWN:
			if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) ||
				(dir == DOWN && key != UP))//180회전이동을 방지 
				dir = key;
			key = 0; // 키값을 저장하는 함수를 reset 
			break;
		case Q: // Q키를 누르면 일시정지 
			pause();
			break;
		case ESC: //ESC키를 누르면 프로그램 종료 
			exit(0);
		}
		move(dir);

		status(); // 스탯표시 
	}
}

void title(void) {
	int i, j;

	while (_kbhit()) _getch(); //버퍼에 있는 키값을 버림 

	draw_map();    //게임 시작 화면 
	for (i = 2 + 1; i < 2 + 20 - 1; i++) { // 맵 테두리 안쪽을 빈칸으로 채움 
		for (j = 3 + 1; j < 3 + 30 - 1; j++) gotoxy(j, i, "  ");
	}
	//지렁이 그려서 데코
	gotoxy(13, 4, "◎");
	for (i = 4; i < 20; i += 2)
	{
		gotoxy(11 + i, 4, "●");
	}

	for (i = 5; i < 20; i += 2)
	{
		gotoxy(9 + i, 5, "●");
	}

	gotoxy(12, 7.4, "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(12, 8.5, "┃            지렁이게임          ┃");
	gotoxy(12, 9.5, "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(11, 11, " ┃ 플레이하려면 아무 키나 눌러주세요 ┃ ");

	gotoxy(13, 13, "┏━━━━━━━━━ 게임방법 ━━━━━━━━━┓");
	gotoxy(13, 14, "┃                            ┃");
	gotoxy(13, 15, "┃   ▶ ←,→,↑,↓ : 이동    ┃");
	gotoxy(13, 16, "┃   ▶ Q : 일시정지          ┃");
	gotoxy(13, 17, "┃   ▶ ESC : 종료            ┃");
	gotoxy(13, 18, "┃                            ┃");
	gotoxy(13, 19, "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

	while (1) {
		if (_kbhit()) { //키입력받음 
			key = _getch();
			if (key == ESC) exit(0); // ESC키면 종료 
			else break; //아니면 그냥 계속 진행 
		}
		gotoxy(11, 11, " ┃ 플레이하려면 아무 키나 눌러주세요 ┃ ");
		Sleep(800);
		gotoxy(11, 11, "                                       ");
		Sleep(400);
		//MAP_WIDTH 게임화면크기
		//MAP_Y 게임화면위치
	}
	reset(); // 게임을 초기화  
}

void reset(void) {
	int i;
	system("cls"); //화면을 지움 
	draw_map(); //맵 테두리를 그림 
	while (_kbhit()) _getch(); //버퍼에 있는 키값을 버림 

	dir = LEFT; // 방향 초기화  
	speed = 100; // 속도 초기화 
	length = 4; //뱀 길이 초기화 
	score = 0; //점수 초기화 
	for (i = 0; i < length; i++) { //뱀 몸통값 입력 
		x[i] = 10 + i;
		y[i] = 5;
		gotoxy(3 + x[i], 2 + y[i], "●");
	}
	gotoxy(3 + x[0], 2 + y[0], "◎"); //뱀 머리 그림 
	food(); // food 생성  
}

void draw_map(void) { //게임 테두리
	int i;
	gotoxy(2, 0, "┏");
	for (i = 0; i < 37; i++) {
		gotoxy(3 + i, 0, "━");
	}
	gotoxy(40, 0, "┓");
	for (i = 1; i < 28; i++) {
		gotoxy(2, i, "┃");
		gotoxy(40, i, "┃");
	}
	gotoxy(2, 27, "┗");
	for (i = 0; i < 37; i++) {
		gotoxy(3 + i, 27, "━");
	}
	gotoxy(40, 27, "┛");
}

void move(int dir) {
	int i;

	if (x[0] == food_x && y[0] == food_y) { //먹이를 먹었을 때
		score += 100; //먹이 하나당 100점씩 점수 증가 
		food(); //새로운 먹이 추가 
		length++; //먹이를 먹을 때마다 길이 1씩 증가 
		x[length - 1] = x[length - 2]; //새로만든 몸통에 값 입력 
		y[length - 1] = y[length - 2];
	}
	if (x[0] == -1 || x[0] == 37 || y[0] == -2 || y[0] == 25) { //벽에 닿았을 때 
		game_over();
		return; //game_over에서 게임을 다시 시작하게 되면 여기서부터 반복되므로 
				//return을 사용하여 move의 나머지 부분이 실행되지 않도록 합니다. 
	}
	for (i = 1; i < length; i++) { //몸에 닿았는지 확인 
		if (x[0] == x[i] && y[0] == y[i]) {
			game_over();
			return;
		}
	}

	gotoxy(3 + x[length - 1], 2 + y[length - 1], "  "); //몸통 마지막을 지움 
	for (i = length - 1; i > 0; i--) { //몸통좌표를 한칸씩 옮김 
		x[i] = x[i - 1];
		y[i] = y[i - 1];
	}
	gotoxy(3 + x[0], 2 + y[0], "●"); //머리가 있던곳을 몸통으로 고침 
	if (dir == LEFT) --x[0]; //방향에 따라 새로운 머리좌표(x[0],y[0])값을 변경 
	if (dir == RIGHT) ++x[0];
	if (dir == UP) --y[0];
	if (dir == DOWN) ++y[0];
	gotoxy(3 + x[i], 2 + y[i], "◎"); //새로운 머리좌표값에 머리를 그림 
}

void pause(void) { // Q키를 눌렀을 경우 게임을 일시 정지 
	while (1) {
		if (key == Q) {
			gotoxy(3, 1, "일시정지");
			Sleep(400);
			gotoxy(3, 1, "        ");
			Sleep(400);
		}
		else {
			draw_map();
			return;
		}
		if (_kbhit()) {

			do {
				key = _getch();
			} while (key == 224);
		}

	}
}

void game_over(void) { //게임종료 함수 

	gotoxy(13, 10, "┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(13, 11, "┃        죽었습니다.       ┃");
	gotoxy(13, 12, "┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(13, 16.5, " ※ 다시 시작 : ENTER ");

	if (score > best_score) {
		best_score = score;
		gotoxy(13, 14, "★ 최고 점수 ★ 점수 :");
		printf("%d", last_score = score);
	}
	else if (score < best_score) {
		gotoxy(13, 14, " 점수 : ");
		printf("%d", last_score = score);
	}
	else if (score == 0) {
		gotoxy(13, 14, " 점수 : 0");
	}
	else if (score == best_score) {
		gotoxy(13, 14, " 점수 : ");
		printf("%d", last_score = score);
	}
	else {
		gotoxy(13, 11, "********error********");
	}
	Sleep(500);
	while (_kbhit()) _getch();
	key = _getch();
	title();
}

void food(void) {
	int i;

	int food_crush_on = 0;//food가 지렁이 몸통좌표에 생길 경우 on 
	int r = 0; //난수 생성에 사동되는 변수 
	gotoxy(41, 0, "┏━━━━━━━━━━   점수판  ━━━━━━━━━━┓");
	gotoxy(41, 1, "┃ ▶ 현재 점수 : "); //현재점수표시 
	printf("%3d \t\t  ┃", score);
	gotoxy(41, 2, "┃                               ┃");
	gotoxy(41, 3, "┃ ▶ 최근 게임 점수 : "); //최근게임점수표시 
	printf("%3d \t  ┃", last_score);
	gotoxy(41, 4, "┃                               ┃");
	gotoxy(41, 5, "┃ ▶ 최고 점수 : "); //최고점수표시 
	printf("%3d\t\t  ┃", best_score);
	gotoxy(41, 6, "┃"); gotoxy(57, 6, "┃");
	gotoxy(41, 7, "┃"); gotoxy(57, 7, "┃");
	gotoxy(44, 7, "  ♡♡          ♡♡");
	gotoxy(41, 8, "┃"); gotoxy(57, 8, "┃");
	gotoxy(44, 8, "♡♡♡♡      ♡♡♡♡");
	gotoxy(41, 9, "┃"); gotoxy(57, 9, "┃");
	gotoxy(43, 9, "♡♡♡♡♡♡  ♡♡♡♡♡♡");
	gotoxy(41, 10, "┃"); gotoxy(57, 10, "┃");
	gotoxy(42, 10, " ♡♡♡♡♡♡♡♡♡♡♡♡♡♡");
	gotoxy(41, 11, "┃"); gotoxy(57, 11, "┃");
	gotoxy(42, 11, " ♡♡♡♡♡♡♡♡♡♡♡♡♡♡");
	gotoxy(41, 12, "┃"); gotoxy(57, 12, "┃");
	gotoxy(42, 12, "  ♡♡♡♡♡♡♡♡♡♡♡♡♡");
	gotoxy(41, 13, "┃"); gotoxy(57, 13, "┃");
	gotoxy(42, 13, "   ♡♡♡♡♡♡♡♡♡♡♡♡");
	gotoxy(41, 14, "┃"); gotoxy(57, 14, "┃");
	gotoxy(42, 14, "     ♡♡♡♡♡♡♡♡♡♡");
	gotoxy(41, 15, "┃"); gotoxy(57, 15, "┃");
	gotoxy(42, 15, "       ♡♡♡♡♡♡♡♡");
	gotoxy(41, 16, "┃"); gotoxy(57, 16, "┃");
	gotoxy(42, 16, "         ♡♡♡♡♡♡");
	gotoxy(41, 17, "┃"); gotoxy(57, 17, "┃");
	gotoxy(42, 17, "           ♡♡♡♡");
	gotoxy(41, 18, "┃"); gotoxy(57, 18, "┃");
	gotoxy(42, 18, "             ♡♡");
	gotoxy(41, 19, "┃"); gotoxy(57, 19, "┃");
	gotoxy(41, 20, "┃"); gotoxy(57, 20, "┃");
	gotoxy(41, 21, "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

	while (1) {
		food_crush_on = 0;
		srand((unsigned)time(NULL) + r); //난수표생성 
		food_x = (rand() % 19);    //난수를 좌표값에 넣음 
		food_y = (rand() % 25);

		for (i = 0; i < length; i++) { //food가 뱀 몸통과 겹치는지 확인  
			if (food_x == x[i] && food_y == y[i]) {
				food_crush_on = 1; //겹치면 food_crush_on 를 on 
				r++;
				break;
			}
		}

		if (food_crush_on == 1) continue; //겹쳤을 경우 while문을 다시 시작 

		gotoxy(3 + food_x, 2 + food_y, "★"); //안겹쳤을 경우 좌표값에 food를 찍고 
		speed -= 5; //속도 증가 
		break;
	}
}

void status(void) { //각종 스텟을 볼수 있는 함수 
	gotoxy(41, 22, "┏━━━━━━━━━━  현재스텟 ━━━━━━━━━━┓");
	gotoxy(41, 23, "┃ 좌      표 : ");
	printf("%2d,%2d            ┃", x[0], y[0]);
	gotoxy(41, 24, "┃ 별  좌  표 : ");
	printf("%2d,%2d            ┃", food_x, food_y);
	gotoxy(41, 25, "┃ 몸  길  이 : ");
	printf("%2d               ┃", length);
	gotoxy(41, 26, "┃ 지렁이속도 : ");
	printf("%3d              ┃", speed);
	gotoxy(41, 27, "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
}