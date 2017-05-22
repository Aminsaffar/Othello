#include <windows.h>
#include <ctime>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <string>

struct Location {
	int X;
	int Y;
};

#define mapHeight 6		//#default value is 6 but free to change
#define mapWith 6		//#default value is 6 but free to change 
#define emptycharmap 0
#define player1charonmap 1
#define player2charonmap 2
#define nextmovechar 'z'
const int wayx[] = { 1, 1 , 1 , 0 , 0 , -1 , -1 , -1 };
const int wayy[] = { 0,-1 , 1 , 1 ,-1 ,  0 , -1 ,  1 };
int p1count = 2;
int p2count = 2;
int othello_map[mapWith][mapHeight];  // x , y
int movecount;
Location curentLocation;

void compute_where_can_go();

bool mapisempty(char s) {
	return s == emptycharmap || s == nextmovechar;
}

void gotoxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorCoord;
	cursorCoord.X = x;
	cursorCoord.Y = y;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);
}

void clrscr()
{
	system("cls");
}

void setTextColor(int textColor, int backColor = 0)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int colorAttribute = backColor << 4 | textColor;
	SetConsoleTextAttribute(consoleHandle, colorAttribute);
}





void print_game_logo(int x, int y) {
	setTextColor(21, 7);
	gotoxy(x, ++y);
	printf("______/\\\\\\\\\\_______________________/\\\\\\___________________________/\\\\\\\\\\\\______/\\\\\\\\\\\\___________________       ");
	gotoxy(x, ++y);
	printf(" ____/\\\\\\///\\\\\\____________________\\/\\\\\\__________________________\\////\\\\\\_____\\////\\\\\\___________________      ");
	gotoxy(x, ++y);
	printf("  __/\\\\\\/__\\///\\\\\\_______/\\\\\\_______\\/\\\\\\_____________________________\\/\\\\\\________\\/\\\\\\___________________     ");
	gotoxy(x, ++y);
	printf("   _/\\\\\\______\\//\\\\\\___/\\\\\\\\\\\\\\\\\\\\\\__\\/\\\\\\______________/\\\\\\\\\\\\\\\\______\\/\\\\\\________\\/\\\\\\_________/\\\\\\\\\\____    ");
	gotoxy(x, ++y);
	printf("    \\/\\\\\\_______\\/\\\\\\__\\////\\\\\\////___\\/\\\\\\\\\\\\\\\\\\\\_____/\\\\\\/////\\\\\\_____\\/\\\\\\________\\/\\\\\\_______/\\\\\\///\\\\\\__   ");
	gotoxy(x, ++y);
	printf("     \\//\\\\\\______/\\\\\\______\\/\\\\\\_______\\/\\\\\\/////\\\\\\___/\\\\\\\\\\\\\\\\\\\\\\______\\/\\\\\\________\\/\\\\\\______/\\\\\\__\\//\\\\\\_  ");
	gotoxy(x, ++y);
	printf("      _\\///\\\\\\__/\\\\\\________\\/\\\\\\_/\\\\___\\/\\\\\\___\\/\\\\\\__\\//\\\\///////_______\\/\\\\\\________\\/\\\\\\_____\\//\\\\\\__/\\\\\\_  ");
	gotoxy(x, ++y);
	printf("       ___\\///\\\\\\\\\\/_________\\//\\\\\\\\\\____\\/\\\\\\___\\/\\\\\\___\\//\\\\\\\\\\\\\\\\\\\\___/\\\\\\\\\\\\\\\\\\___/\\\\\\\\\\\\\\\\\\___\\///\\\\\\\\\\/__ ");
	gotoxy(x, ++y);
	printf("        _____\\/////____________\\/////_____\\///____\\///_____\\//////////___\\/////////___\\/////////______\\/////____");
	setTextColor(2, 0);

}



void start_anim() {
	for (int i = 0; i < 30; i++) {
		print_game_logo(0, i);
		sleep(50);
		clrscr();
	}
}
void main() {
	start_anim();
	init_map();

	printBoard();

	printgame_data(20, 20);
	othello_map[2][2] = player1charonmap;
	othello_map[3][3] = player1charonmap;
	othello_map[3][2] = player2charonmap;
	othello_map[2][3] = player2charonmap;
	updateMap();
	while (true) {
		changeLocation(curentLocation);
	}
}