#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#pragma warning(disable:4996)

// �Ķ����
#define LEN_MIN			15	// ���� ����
#define LEN_MAX			50
#define PROB_MIN		10	// Ȯ��
#define PROB_MAX		90

void intro() {
	printf("\n");
	printf(" /$$$$$$$\n");
	printf("| $$__  $$\n");
	printf("| $$  \\ $$ /$$   /$$  /$$$$$$$  /$$$$$$  /$$$$$$$\n");
	printf("| $$$$$$$ | $$  | $$ /$$_____/ |____  $$| $$__  $$\n");
	printf("| $$__  $$| $$  | $$|  $$$$$$   /$$$$$$$| $$  \\ $$\n");
	printf("| $$  \\ $$| $$  | $$ \\____  $$ /$$__  $$| $$  | $$\n");
	printf("| $$$$$$$/|  $$$$$$/ /$$$$$$$/|  $$$$$$$| $$  | $$\n");
	printf("|_______/  \\______/ |_______/  \\_______/|__/  |__/\n");
	printf("\n");
	printf("             /$$   /$$                               /$$\n");
	printf("            | $$  | $$                              | $$\n");
	printf("            | $$  | $$  /$$$$$$  /$$$$$$$   /$$$$$$ | $$\n");
	printf("            | $$$$$$$$ /$$__  $$| $$__  $$ /$$__  $$| $$\n");
	printf("            | $$__  $$| $$$$$$$$| $$  \\ $$| $$  \\ $$|__/\n");
	printf("            | $$  | $$| $$_____/| $$  | $$| $$  | $$\n");
	printf("            | $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$$ /$$\n");
	printf("            |__/  |__/ \\_______/|__/  |__/ \\____  $$|__/\n");
	printf("                                           /$$  \\ $$\n");
	printf("                                          |  $$$$$$/\n");
	printf("                                           \\______/\n");

	Sleep(1000);
}

void outro_success() {
	printf("\n");
	printf("  /$$$$$$$ /$$   /$$  /$$$$$$$  /$$$$$$$  /$$$$$$   /$$$$$$$ /$$$$$$$\n");
	printf(" /$$_____/| $$  | $$ /$$_____/ /$$_____/ /$$__  $$ /$$_____//$$_____/\n");
	printf("|  $$$$$$ | $$  | $$| $$      | $$      | $$$$$$$$|  $$$$$$|  $$$$$$\n");
	printf(" \\____  $$| $$  | $$| $$      | $$      | $$_____/ \\____  $$\\____  $$\n");
	printf(" /$$$$$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$|  $$$$$$$ /$$$$$$$//$$$$$$$/\n");
	printf("|_______/  \\______/  \\_______/ \\_______/ \\_______/|_______/|_______/\n");

	Sleep(1000);
}

void outro_gameover() {
	printf("\n");
	printf("  /$$$$$$   /$$$$$$  /$$$$$$/$$$$   /$$$$$$\n");
	printf(" /$$__  $$ |____  $$| $$_  $$_  $$ /$$__  $$\n");
	printf("| $$  \\ $$  /$$$$$$$| $$ \\ $$ \\ $$| $$$$$$$$\n");
	printf("| $$  | $$ /$$__  $$| $$ | $$ | $$| $$_____/\n");
	printf("|  $$$$$$$|  $$$$$$$| $$ | $$ | $$| $$$$$$$\n");
	printf(" \\____  $$ \\_______/|__/ |__/ |__ /\\_______/\n");
	printf(" /$$  \\ $$\n");
	printf("|  $$$$$$/\n");
	printf(" \\______/\n");
	printf("                   /$$$$$$  /$$    /$$ /$$$$$$   /$$$$$$\n");
	printf("                  /$$__  $$|  $$  /$$//$$__  $$ /$$__  $$\n");
	printf("                 | $$  \\ $$ \\  $$/$$/| $$$$$$$$| $$  \\__/\n");
	printf("                 | $$  | $$  \\  $$$/ | $$_____/| $$\n");
	printf("                 |  $$$$$$/   \\  $/  |  $$$$$$$| $$\n");
	printf("                  \\______/     \\_/    \\_______/|__/\n");

	Sleep(1000);
}


void print_train(int len, int C, int Z, int M) {
	for (int i = 0; i < len; i++)
		printf("#");
	printf("\n");

	for (int i = 0; i < len; i++) {
		if (i == 0 || i == len - 1)
			printf("#");
		else if (i == C)
			printf("C");
		else if (i == Z)
			printf("Z");
		else if (i == M)
			printf("M");
		else
			printf(" ");
	}
	printf("\n");

	for (int i = 0; i < len; i++)
		printf("#");
	printf("\n");
	printf("\n");
}

void print_status(int C, int C_move, int Z, int Z_move, int Z_turn) {
	if (C == C_move)
		printf("citizen: stay %d\n", C);
	else
		printf("citizen: %d -> %d\n", C, C_move);

	if (Z == Z_move) {
		if (Z_turn < 0)
			printf("zombie: stay %d (cannot move)\n", Z);
		else
			printf("zombie: stay %d\n", Z);
	}
	else
		printf("zombie: %d -> %d\n", Z, Z_move);
	printf("\n");
}

int move_citizen(int C, int p) {
	int r = rand() % 100;

	if (100 - p > r)
		return C - 1;
	return C;
}

int move_zombie(int Z, int p, int Z_turn) {
	if (Z_turn < 0)
		return Z;

	int r = rand() % 100;

	if (p > r)
		return Z - 1;
	return Z;
}

int is_gameover(int C, int Z) {
	if (C == 1)
		return 1;
	if (C == Z - 1)
		return -1;
	return 0;
}

int main(void) {
	// ��Ʈ��
	intro();

	int len, prob;
	printf("train length(15~50)>> ");
	scanf("%d", &len);

	if (len < LEN_MIN || len > LEN_MAX)
		return 0;

	printf("percentile probability 'p'(10~90)>> ");
	scanf("%d", &prob);

	if (prob < PROB_MIN || prob > PROB_MAX)
		return 0;

	// ���� �ʱ� ���� ���
	int C = len - 6, Z = len - 3, M = len - 2;
	print_train(len, C, Z, M);

	int result = 0;
	int C_move = C, Z_move = Z, Z_turn = 1;
	while (1) {
		// �ù� �̵�
		C_move = move_citizen(C, prob);

		// ���� �̵�
		Z_move = move_zombie(Z, prob, Z_turn);

		// ���� ���� ���
		print_train(len, C_move, Z_move, M);

		// �ù�, ���� ���� ���
		print_status(C, C_move, Z, Z_move, Z_turn);

		C = C_move;
		Z = Z_move;
		Z_turn *= -1;

		result = is_gameover(C, Z);
		if (result == 1) {
			printf("SUCCESS!\ncitizen(s) escaped to the next train\n");
			break;
		}
		else if (result == -1) {
			printf("GAME OVER!\nCitizen(s) has(have) been attacked by a zombie\n");
			break;
		}

		Sleep(4000);
	}

	// �ƿ�Ʈ��
	if (result == 1)
		outro_success();
	else
		outro_gameover();
}

