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

//32��==> SP:   Space
//13�� ==> CR: ����
//27�� ==> ESC


int x[1000], y[1000]; //x,y ��ǥ���� ���� �� 1000�� 
int food_x, food_y; //�� ��ǥ���� ����
int length; //������
int speed; //������ �ӵ�
int score; //���� ����  --reset�Լ��� ���� �ʱ�ȭ��
int best_score = 0; //�ְ� ���� ���� --reset�Լ��� ���� �ʱ�ȭ ���� ���� 
int last_score = 0; //������ ���� ����  --reset�Լ��� ���� �ʱ�ȭ ���� ����
int dir; //�̵����� ���� 
int key; //�Է¹��� Ű ���� 

void gotoxy(int x, int y, char* s) { //x���� 2x�� ����, ��ǥ���� �ٷ� ���ڿ��� �Է��� �� �ֵ��� printf�Լ� ����  
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //x,y ���� ����, ǥ���ܼ������ �ڵ��� ��ȯ
	printf("%s", s);
}

void title(void); //���� ����ȭ�� 
void reset(void); //������ �ʱ�ȭ 
void draw_map(void); // �����׵θ� 
void move(int dir); //������ �Ӹ� �̵�
void pause(void); //�Ͻ����� 
void game_over(void); //�׾����ϴ� 
void food(void); //�� ���� 
void status(void); // �����ڿ� statusǥ��

int main() {
	title();

	while (1) {
		//kbhit = Ű�� �Էµ� �����ΰ�
		//getch = Ű������ �Է��� �޴� �Լ�
		if (_kbhit()) do { key = _getch(); } while (key == 224); //Ű �Է�
		//Sleep = �۾��� ����ϰ� ���� �� ����ϴ� �Լ�
		Sleep(speed);
		switch (key) { //�Է¹��� Ű ����
		case LEFT: case RIGHT: case UP: case DOWN:
			if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) ||
				(dir == DOWN && key != UP))//180ȸ���̵��� ���� 
				dir = key;
			key = 0; // Ű���� �����ϴ� �Լ��� reset 
			break;
		case Q: // QŰ�� ������ �Ͻ����� 
			pause();
			break;
		case ESC: //ESCŰ�� ������ ���α׷� ���� 
			exit(0);
		}
		move(dir);

		status(); // ����ǥ�� 
	}
}

void title(void) {
	int i, j;

	while (_kbhit()) _getch(); //���ۿ� �ִ� Ű���� ���� 

	draw_map();    //���� ���� ȭ�� 
	for (i = 2 + 1; i < 2 + 20 - 1; i++) { // �� �׵θ� ������ ��ĭ���� ä�� 
		for (j = 3 + 1; j < 3 + 30 - 1; j++) gotoxy(j, i, "  ");
	}
	//������ �׷��� ����
	gotoxy(13, 4, "��");
	for (i = 4; i < 20; i += 2)
	{
		gotoxy(11 + i, 4, "��");
	}

	for (i = 5; i < 20; i += 2)
	{
		gotoxy(9 + i, 5, "��");
	}

	gotoxy(12, 7.4, "��������������������������������������������������������������������");
	gotoxy(12, 8.5, "��            �����̰���          ��");
	gotoxy(12, 9.5, "��������������������������������������������������������������������");

	gotoxy(11, 11, " �� �÷����Ϸ��� �ƹ� Ű�� �����ּ��� �� ");

	gotoxy(13, 13, "�������������������� ���ӹ�� ��������������������");
	gotoxy(13, 14, "��                            ��");
	gotoxy(13, 15, "��   �� ��,��,��,�� : �̵�    ��");
	gotoxy(13, 16, "��   �� Q : �Ͻ�����          ��");
	gotoxy(13, 17, "��   �� ESC : ����            ��");
	gotoxy(13, 18, "��                            ��");
	gotoxy(13, 19, "������������������������������������������������������������");

	while (1) {
		if (_kbhit()) { //Ű�Է¹��� 
			key = _getch();
			if (key == ESC) exit(0); // ESCŰ�� ���� 
			else break; //�ƴϸ� �׳� ��� ���� 
		}
		gotoxy(11, 11, " �� �÷����Ϸ��� �ƹ� Ű�� �����ּ��� �� ");
		Sleep(800);
		gotoxy(11, 11, "                                       ");
		Sleep(400);
		//MAP_WIDTH ����ȭ��ũ��
		//MAP_Y ����ȭ����ġ
	}
	reset(); // ������ �ʱ�ȭ  
}

void reset(void) {
	int i;
	system("cls"); //ȭ���� ���� 
	draw_map(); //�� �׵θ��� �׸� 
	while (_kbhit()) _getch(); //���ۿ� �ִ� Ű���� ���� 

	dir = LEFT; // ���� �ʱ�ȭ  
	speed = 100; // �ӵ� �ʱ�ȭ 
	length = 4; //�� ���� �ʱ�ȭ 
	score = 0; //���� �ʱ�ȭ 
	for (i = 0; i < length; i++) { //�� ���밪 �Է� 
		x[i] = 10 + i;
		y[i] = 5;
		gotoxy(3 + x[i], 2 + y[i], "��");
	}
	gotoxy(3 + x[0], 2 + y[0], "��"); //�� �Ӹ� �׸� 
	food(); // food ����  
}

void draw_map(void) { //���� �׵θ�
	int i;
	gotoxy(2, 0, "��");
	for (i = 0; i < 37; i++) {
		gotoxy(3 + i, 0, "��");
	}
	gotoxy(40, 0, "��");
	for (i = 1; i < 28; i++) {
		gotoxy(2, i, "��");
		gotoxy(40, i, "��");
	}
	gotoxy(2, 27, "��");
	for (i = 0; i < 37; i++) {
		gotoxy(3 + i, 27, "��");
	}
	gotoxy(40, 27, "��");
}

void move(int dir) {
	int i;

	if (x[0] == food_x && y[0] == food_y) { //���̸� �Ծ��� ��
		score += 100; //���� �ϳ��� 100���� ���� ���� 
		food(); //���ο� ���� �߰� 
		length++; //���̸� ���� ������ ���� 1�� ���� 
		x[length - 1] = x[length - 2]; //���θ��� ���뿡 �� �Է� 
		y[length - 1] = y[length - 2];
	}
	if (x[0] == -1 || x[0] == 37 || y[0] == -2 || y[0] == 25) { //���� ����� �� 
		game_over();
		return; //game_over���� ������ �ٽ� �����ϰ� �Ǹ� ���⼭���� �ݺ��ǹǷ� 
				//return�� ����Ͽ� move�� ������ �κ��� ������� �ʵ��� �մϴ�. 
	}
	for (i = 1; i < length; i++) { //���� ��Ҵ��� Ȯ�� 
		if (x[0] == x[i] && y[0] == y[i]) {
			game_over();
			return;
		}
	}

	gotoxy(3 + x[length - 1], 2 + y[length - 1], "  "); //���� �������� ���� 
	for (i = length - 1; i > 0; i--) { //������ǥ�� ��ĭ�� �ű� 
		x[i] = x[i - 1];
		y[i] = y[i - 1];
	}
	gotoxy(3 + x[0], 2 + y[0], "��"); //�Ӹ��� �ִ����� �������� ��ħ 
	if (dir == LEFT) --x[0]; //���⿡ ���� ���ο� �Ӹ���ǥ(x[0],y[0])���� ���� 
	if (dir == RIGHT) ++x[0];
	if (dir == UP) --y[0];
	if (dir == DOWN) ++y[0];
	gotoxy(3 + x[i], 2 + y[i], "��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸� 
}

void pause(void) { // QŰ�� ������ ��� ������ �Ͻ� ���� 
	while (1) {
		if (key == Q) {
			gotoxy(3, 1, "�Ͻ�����");
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

void game_over(void) { //�������� �Լ� 

	gotoxy(13, 10, "��������������������������������������������������������");
	gotoxy(13, 11, "��        �׾����ϴ�.       ��");
	gotoxy(13, 12, "��������������������������������������������������������");

	gotoxy(13, 16.5, " �� �ٽ� ���� : ENTER ");

	if (score > best_score) {
		best_score = score;
		gotoxy(13, 14, "�� �ְ� ���� �� ���� :");
		printf("%d", last_score = score);
	}
	else if (score < best_score) {
		gotoxy(13, 14, " ���� : ");
		printf("%d", last_score = score);
	}
	else if (score == 0) {
		gotoxy(13, 14, " ���� : 0");
	}
	else if (score == best_score) {
		gotoxy(13, 14, " ���� : ");
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

	int food_crush_on = 0;//food�� ������ ������ǥ�� ���� ��� on 
	int r = 0; //���� ������ �絿�Ǵ� ���� 
	gotoxy(41, 0, "����������������������   ������  ����������������������");
	gotoxy(41, 1, "�� �� ���� ���� : "); //��������ǥ�� 
	printf("%3d \t\t  ��", score);
	gotoxy(41, 2, "��                               ��");
	gotoxy(41, 3, "�� �� �ֱ� ���� ���� : "); //�ֱٰ�������ǥ�� 
	printf("%3d \t  ��", last_score);
	gotoxy(41, 4, "��                               ��");
	gotoxy(41, 5, "�� �� �ְ� ���� : "); //�ְ�����ǥ�� 
	printf("%3d\t\t  ��", best_score);
	gotoxy(41, 6, "��"); gotoxy(57, 6, "��");
	gotoxy(41, 7, "��"); gotoxy(57, 7, "��");
	gotoxy(44, 7, "  ����          ����");
	gotoxy(41, 8, "��"); gotoxy(57, 8, "��");
	gotoxy(44, 8, "��������      ��������");
	gotoxy(41, 9, "��"); gotoxy(57, 9, "��");
	gotoxy(43, 9, "������������  ������������");
	gotoxy(41, 10, "��"); gotoxy(57, 10, "��");
	gotoxy(42, 10, " ����������������������������");
	gotoxy(41, 11, "��"); gotoxy(57, 11, "��");
	gotoxy(42, 11, " ����������������������������");
	gotoxy(41, 12, "��"); gotoxy(57, 12, "��");
	gotoxy(42, 12, "  ��������������������������");
	gotoxy(41, 13, "��"); gotoxy(57, 13, "��");
	gotoxy(42, 13, "   ������������������������");
	gotoxy(41, 14, "��"); gotoxy(57, 14, "��");
	gotoxy(42, 14, "     ��������������������");
	gotoxy(41, 15, "��"); gotoxy(57, 15, "��");
	gotoxy(42, 15, "       ����������������");
	gotoxy(41, 16, "��"); gotoxy(57, 16, "��");
	gotoxy(42, 16, "         ������������");
	gotoxy(41, 17, "��"); gotoxy(57, 17, "��");
	gotoxy(42, 17, "           ��������");
	gotoxy(41, 18, "��"); gotoxy(57, 18, "��");
	gotoxy(42, 18, "             ����");
	gotoxy(41, 19, "��"); gotoxy(57, 19, "��");
	gotoxy(41, 20, "��"); gotoxy(57, 20, "��");
	gotoxy(41, 21, "������������������������������������������������������������������");

	while (1) {
		food_crush_on = 0;
		srand((unsigned)time(NULL) + r); //����ǥ���� 
		food_x = (rand() % 19);    //������ ��ǥ���� ���� 
		food_y = (rand() % 25);

		for (i = 0; i < length; i++) { //food�� �� ����� ��ġ���� Ȯ��  
			if (food_x == x[i] && food_y == y[i]) {
				food_crush_on = 1; //��ġ�� food_crush_on �� on 
				r++;
				break;
			}
		}

		if (food_crush_on == 1) continue; //������ ��� while���� �ٽ� ���� 

		gotoxy(3 + food_x, 2 + food_y, "��"); //�Ȱ����� ��� ��ǥ���� food�� ��� 
		speed -= 5; //�ӵ� ���� 
		break;
	}
}

void status(void) { //���� ������ ���� �ִ� �Լ� 
	gotoxy(41, 22, "����������������������  ���罺�� ����������������������");
	gotoxy(41, 23, "�� ��      ǥ : ");
	printf("%2d,%2d            ��", x[0], y[0]);
	gotoxy(41, 24, "�� ��  ��  ǥ : ");
	printf("%2d,%2d            ��", food_x, food_y);
	gotoxy(41, 25, "�� ��  ��  �� : ");
	printf("%2d               ��", length);
	gotoxy(41, 26, "�� �����̼ӵ� : ");
	printf("%3d              ��", speed);
	gotoxy(41, 27, "������������������������������������������������������������������");
}