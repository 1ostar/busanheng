#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#pragma warning(disable:4996)

// 파라미터
#define LEN_MIN		15	// 기차 길이
#define LEN_MAX		50
#define STM_MIN		0	// 마동석 체력
#define STM_MAX		5
#define PROB_MIN	10	// 확률
#define PROB_MAX	90
#define AGGRO_MIN	0	// 어그로 범위
#define AGGRO_MAX	5

// 마동석 이동 방향
#define MOVE_LFET 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE		0
#define ATK_CITIZEN		1
#define ATK_DONGSEOK	2


// 마동석 행동
#define ACTION_REST		0
#define ACTION_PROVOKE	1
#define ACTION_PULL		2

#define CITIZEN		0
#define DONGSEOK	1
#define ZOMBIE		2

#define BEFORE	0
#define AFTER	1

int aggro[2];
int aggro_after[2];
int curr[3][2];
int stamina[2];

void intro() {
	char* intro[] = {
"",
" /$$$$$$$",
"| $$__  $$",
"| $$  \\ $$ /$$   /$$  /$$$$$$$  /$$$$$$  /$$$$$$$",
"| $$$$$$$ | $$  | $$ /$$_____/ |____  $$| $$__  $$",
"| $$__  $$| $$  | $$|  $$$$$$   /$$$$$$$| $$  \\ $$",
"| $$  \\ $$| $$  | $$ \\____  $$ /$$__  $$| $$  | $$",
"| $$$$$$$/|  $$$$$$/ /$$$$$$$/|  $$$$$$$| $$  | $$",
"|_______/  \\______/ |_______/  \\_______/|__/  |__/",
"",
"             /$$   /$$                               /$$",
"            | $$  | $$                              | $$",
"            | $$  | $$  /$$$$$$  /$$$$$$$   /$$$$$$ | $$",
"            | $$$$$$$$ /$$__  $$| $$__  $$ /$$__  $$| $$",
"            | $$__  $$| $$$$$$$$| $$  \\ $$| $$  \\ $$|__/",
"            | $$  | $$| $$_____/| $$  | $$| $$  | $$",
"            | $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$$ /$$",
"            |__/  |__/ \\_______/|__/  |__/ \\____  $$|__/",
"                                           /$$  \\ $$",
"                                          |  $$$$$$/",
"                                           \\______/" };


	for (int i = 0; i < sizeof(intro) / sizeof(intro[0]); i++) {
		printf("%s\n", intro[i]);
		Sleep(100);
	}
	Sleep(1000);
}

void outro_success() {
	char* outro[] = {
"",
"  /$$$$$$$ /$$   /$$  /$$$$$$$  /$$$$$$$  /$$$$$$   /$$$$$$$ /$$$$$$$",
" /$$_____/| $$  | $$ /$$_____/ /$$_____/ /$$__  $$ /$$_____//$$_____/",
"|  $$$$$$ | $$  | $$| $$      | $$      | $$$$$$$$|  $$$$$$|  $$$$$$",
" \\____  $$| $$  | $$| $$      | $$      | $$_____/ \\____  $$\\____  $$",
" /$$$$$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$|  $$$$$$$ /$$$$$$$//$$$$$$$/",
"|_______/  \\______/  \\_______/ \\_______/ \\_______/|_______/|_______/" };

	for (int i = 0; i < sizeof(outro) / sizeof(outro[0]); i++) {
		printf("%s\n", outro[i]);
		Sleep(100);
	}
	Sleep(1000);
}

void outro_gameover() {
	char* outro[] = {
"",
"  /$$$$$$   /$$$$$$  /$$$$$$/$$$$   /$$$$$$",
" /$$__  $$ |____  $$| $$_  $$_  $$ /$$__  $$",
"| $$  \\ $$  /$$$$$$$| $$ \\ $$ \\ $$| $$$$$$$$",
"| $$  | $$ /$$__  $$| $$ | $$ | $$| $$_____/",
"|  $$$$$$$|  $$$$$$$| $$ | $$ | $$| $$$$$$$",
" \\____  $$ \\_______/|__/ |__/ |__ /\\_______/",
" /$$  \\ $$",
"|  $$$$$$/",
" \\______/",
"                   /$$$$$$  /$$    /$$ /$$$$$$   /$$$$$$",
"                  /$$__  $$|  $$  /$$//$$__  $$ /$$__  $$",
"                 | $$  \\ $$ \\  $$/$$/| $$$$$$$$| $$  \\__/",
"                 | $$  | $$  \\  $$$/ | $$_____/| $$",
"                 |  $$$$$$/   \\  $/  |  $$$$$$$| $$",
"                  \\______/     \\_/    \\_______/|__/" };

	for (int i = 0; i < sizeof(outro) / sizeof(outro[0]); i++) {
		printf("%s\n", outro[i]);
		Sleep(100);
	}
	Sleep(1000);
}

void init(int len) {
	aggro[CITIZEN] = 1;
	aggro[DONGSEOK] = 1;

	curr[CITIZEN][BEFORE] = len - 6;
	curr[DONGSEOK][BEFORE] = len - 2;
	curr[ZOMBIE][BEFORE] = len - 3;

	curr[CITIZEN][AFTER] = curr[CITIZEN][BEFORE];
	curr[DONGSEOK][AFTER] = curr[DONGSEOK][BEFORE];
	curr[ZOMBIE][AFTER] = curr[ZOMBIE][BEFORE];
}

int move_citizen(int p) {
	int r = rand() % 100;

	if (100 - p > r) {
		if (aggro[CITIZEN] < AGGRO_MAX)
			aggro_after[CITIZEN] = aggro[CITIZEN] + 1;
		return curr[CITIZEN][BEFORE] - 1;
	}
	if (aggro[CITIZEN] > AGGRO_MIN)
		aggro_after[CITIZEN] = aggro[CITIZEN] - 1;
	return curr[CITIZEN][BEFORE];
}

int move_zombie(int Z_turn) {
	if (Z_turn < 0)
		return curr[ZOMBIE][BEFORE];

	if (aggro[CITIZEN] >= aggro[DONGSEOK]) {
		if (curr[CITIZEN][BEFORE] == curr[ZOMBIE][BEFORE] - 1)
			return curr[ZOMBIE][BEFORE];
		return curr[ZOMBIE][BEFORE] - 1;
	}
	else {
		if (curr[DONGSEOK][BEFORE] == curr[ZOMBIE][BEFORE] + 1)
			return curr[ZOMBIE][BEFORE];
		return curr[ZOMBIE][BEFORE] + 1;
	}

	return curr[ZOMBIE][BEFORE];
}

int move_dongseok() {
	int move = -1;

	if (curr[DONGSEOK][BEFORE] == curr[ZOMBIE][BEFORE] + 1) {
		while (move != 0) {
			printf("madongseok move(0:stay)>> ");
			scanf_s("%d", &move);
		}
	}
	else {
		while (move != 0 && move != 1) {
			printf("madongseok move(0:stay, 1:left)>> ");
			scanf_s("%d", &move);
		}
	}

	if (move == 1) {
		if (aggro[DONGSEOK] < AGGRO_MAX)
			aggro_after[DONGSEOK] = aggro[DONGSEOK] + 1;
		return curr[DONGSEOK][BEFORE] - 1;
	}
	else {
		if (aggro[DONGSEOK] > AGGRO_MIN)
			aggro_after[DONGSEOK] = aggro[DONGSEOK] - 1;
		return curr[DONGSEOK][BEFORE];
	}
}

int act_citizen() {
	if (curr[CITIZEN][BEFORE] == 1)
		return 1;
	printf("citizen does nothing.\n");

	return 0;
}
