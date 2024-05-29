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

int act_zombie() {
	if (curr[ZOMBIE][BEFORE] - 1 == curr[CITIZEN][BEFORE] && curr[ZOMBIE][BEFORE] + 1 == curr[DONGSEOK][BEFORE]) {
		if (aggro[CITIZEN] > aggro[DONGSEOK]) {
			printf("Zombie attacked citizen (aggro: %d vs. %d)\n", aggro[CITIZEN], aggro[DONGSEOK]);
			printf("GAME OVER! citizen dead...\n");
			return -1;
		}
		else {
			stamina[AFTER] = stamina[BEFORE] - 1;
			printf("Zombie attacked madongseok (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n", aggro[CITIZEN], aggro[DONGSEOK], stamina[BEFORE], stamina[AFTER]);
			stamina[AFTER] = stamina[BEFORE];

			if (stamina[AFTER] == 0) {
				printf("GAME OVER! madongseok is dead...\n");
				return -1;
			}
		}
	}
	else if (curr[ZOMBIE][BEFORE] - 1 == curr[CITIZEN][BEFORE]) {
		printf("GAME OVER! citizen dead...\n");
		return -1;
	}
	else if (curr[ZOMBIE][BEFORE] + 1 == curr[DONGSEOK][BEFORE]) {
		stamina[AFTER] = stamina[BEFORE] - 1;
		printf("Zombie attacked madongseok (madongseok stamina: %d -> %d)\n", stamina[BEFORE], stamina[AFTER]);
		stamina[AFTER] = stamina[BEFORE];

		if (stamina[AFTER] == 0) {
			printf("GAME OVER! madongseok is dead...\n");
			return -1;
		}
	}
	else {
		printf("zonbie attacked nobody.\n");
	}
	return 0;
}

void provoke() {
	printf("\nmadongseok provoked zombie...\n");
	printf("madongseok: %d ", curr[DONGSEOK][BEFORE]);
	if (aggro[DONGSEOK] < AGGRO_MAX) {
		aggro_after[DONGSEOK] = AGGRO_MAX;
		printf("(aggro: %d -> %d, ", aggro[DONGSEOK], aggro_after[DONGSEOK]);
		aggro[DONGSEOK] = aggro_after[DONGSEOK];
	}
	else {
		printf("(aggro: %d, ", aggro[DONGSEOK]);
	}

	printf("stamina: %d)\n", stamina[BEFORE]);
}

void rest() {
	printf("\nmadongseok rests...\n");
	printf("madongseok: %d ", curr[DONGSEOK][BEFORE]);
	if (aggro[DONGSEOK] > AGGRO_MIN) {
		aggro_after[DONGSEOK] = aggro[DONGSEOK] - 1;
		printf("(aggro: %d -> %d, ", aggro[DONGSEOK], aggro_after[DONGSEOK]);
		aggro[DONGSEOK] = aggro_after[DONGSEOK];
	}
	else {
		printf("(aggro: %d, ", aggro[DONGSEOK]);
	}

	if (stamina[BEFORE] < STM_MAX) {
		stamina[AFTER] = stamina[BEFORE] + 1;
		printf("stamina: %d -> %d)\n", stamina[BEFORE], stamina[AFTER]);
		stamina[BEFORE] = stamina[AFTER];
	}
	else {
		printf("stamina: %d)\n", stamina[BEFORE]);
	}
}

int pull(int p, int* Z_turn) {
	int r = rand() % 100;
	int success = 100 - p > r;

	if (success) {
		printf("\nmadongseok pulled zombie... Next turn, it cna't move\n");
		Z_turn = -1;
	}
	else {
		printf("\nmadongseok failed to pull zombie\n");
	}

	printf("madongseok: %d ", curr[DONGSEOK][BEFORE]);
	if (aggro[DONGSEOK] < AGGRO_MAX) {
		aggro_after[DONGSEOK] = aggro[DONGSEOK] + 2;
		if (aggro_after[DONGSEOK] > AGGRO_MAX)
			aggro_after[DONGSEOK] = AGGRO_MAX;
		printf("(aggro: %d -> %d, ", aggro[DONGSEOK], aggro_after[DONGSEOK]);
		aggro[DONGSEOK] = aggro_after[DONGSEOK];
	}
	else {
		printf("(aggro: %d, ", aggro[DONGSEOK]);
	}

	if (stamina[BEFORE] > STM_MIN) {
		stamina[AFTER] = stamina[BEFORE] - 1;
		if (stamina[AFTER] == 0) {
			stamina[AFTER] = 1;
			printf("stamina: %d)\n", stamina[BEFORE]);
		}
		else {
			printf("stamina: %d -> %d)\n", stamina[BEFORE], stamina[AFTER]);
		}
		stamina[BEFORE] = stamina[AFTER];
	}
	else {
		printf("stamina: %d)\n", stamina[BEFORE]);
	}
}

int act_dongseok(int prob, int* Z_turn) {
	int action = -1;

	if (curr[DONGSEOK][BEFORE] != curr[ZOMBIE][BEFORE] + 1) {
		while (action != 0 && action != 1) {
			printf("madongseok action(0.rest, 1.provoke)>> ");
			scanf_s("%d", &action);
		}
	}
	else {
		while (action < 0 || action > 2) {
			printf("madongseok action(0.rest, 1.provoke, 2.pull)>> ");
			scanf_s("%d", &action);
		}
	}

	if (action == 0)
		rest();
	else if (action == 1)
		provoke();
	else
		pull(prob, &Z_turn);
}
void print_train(int len) {
	for (int i = 0; i < len; i++)
		printf("#");
	printf("\n");

	for (int i = 0; i < len; i++) {
		if (i == 0 || i == len - 1)
			printf("#");
		else if (i == curr[CITIZEN][BEFORE])
			printf("C");
		else if (i == curr[ZOMBIE][BEFORE])
			printf("Z");
		else if (i == curr[DONGSEOK][BEFORE])
			printf("M");
		else
			printf(" ");
	}
	printf("\n");

	for (int i = 0; i < len; i++)
		printf("#");
	printf("\n\n");
}

void print_status_citizen() {
	if (curr[CITIZEN][BEFORE] == curr[CITIZEN][AFTER]) {
		printf("citizen: stay %d", curr[CITIZEN][BEFORE]);
	}
	else {
		printf("citizen: %d -> %d", curr[CITIZEN][BEFORE], curr[CITIZEN][AFTER]);
	}

	if (aggro[CITIZEN] == aggro_after[CITIZEN])
		printf(" (aggro: %d)\n", aggro[CITIZEN]);
	else {
		printf(" (aggro: %d -> %d)\n", aggro[CITIZEN], aggro_after[CITIZEN]);
		aggro[CITIZEN] = aggro_after[CITIZEN];
	}
}