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

void sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}



void changeBordEl(Location& curentLocation, bool on) {
	if (on)
		setTextColor(10, 0);
	else
		setTextColor(2, 0);
	gotoxy(curentLocation.X * 4, curentLocation.Y * 2);
	printf("+---+");
	gotoxy(curentLocation.X * 4, curentLocation.Y * 2 + 1);
	printf("|");
	gotoxy(curentLocation.X * 4 + 4, curentLocation.Y * 2 + 1);
	printf("|");
	gotoxy(curentLocation.X * 4, curentLocation.Y * 2 + 2);
	printf("+---+");

}

void printBoard() {
	for (int i = 0; i<mapHeight; i++) {
		for (int j = 0; j< mapWith; j++) {
			printf("+---");
		}
		printf("+\n");
		for (int j = 0; j<mapWith; j++) {
			printf("|   ");
		}
		printf("|\n");
	}
	for (int j = 0;  j <mapWith; j++) {
		printf("+---");
	}
	printf("+\n");
	changeBordEl(curentLocation, true);
}




void updateMap() {
	compute_where_can_go();
	for (int i = 0; i < mapWith; i++)
		for (int j = 0; j < mapHeight; j++)
			if (othello_map[i][j] == player1charonmap) {
				gotoxy(i * 4 + 2, j * 2 + 1);
				setTextColor(15, 0);
				printf("O");
			}
			else if (othello_map[i][j] == player2charonmap) {
				gotoxy(i * 4 + 2, j * 2 + 1);
				setTextColor(18, 0);
				printf("O");
			}
			else  if(othello_map[i][j] == nextmovechar) {
				gotoxy(i * 4 + 2, j * 2 + 1);
				setTextColor( 5, 10);
				printf("$");
			}
			else  {
				gotoxy(i * 4 + 2, j * 2 + 1);
				setTextColor(2, 0);
				printf(" ");
			}

			
}

void show_where_can_go(int x , int y ) {
	//cout << "this way is true";
					setTextColor(2, 0);
		gotoxy(x * 4 + 2, y * 2 + 1);
	setTextColor(15, 0);
	printf("Z");
}

void init_map() {
	setTextColor(2, 0);
	for (int i = 0; i < mapWith; i++)
		for (int j = 0; j < mapHeight; j++)
			othello_map[i][j] = 0;
	curentLocation.X = 0;
	curentLocation.Y = 0;
	movecount = 0;

}

bool canGotoThisLocation(const int X , const int Y) {
	if (X >= 0 && X < mapWith)
		if (Y >= 0 && Y < mapHeight)
			return true;
	return false;

}

void printgame_data(int x, int y) {
	gotoxy(x, y);
	printf("player white count : %d", p1count);
	gotoxy(x, y + 1);
	printf("player blue  count : %d", p2count);
	gotoxy(x, y + 2);
	if(movecount%2)
		printf("turn white", p2count);
	else
		printf("turn blue", p2count);

}
void goUp(Location& curentLocation) {

	if (canGotoThisLocation(curentLocation.X, curentLocation.Y - 1)) {
		changeBordEl(curentLocation, false);
		curentLocation.Y = curentLocation.Y - 1;
		changeBordEl(curentLocation, true);
	}
}
void goDown(Location& curentLocation) {
	if (canGotoThisLocation(curentLocation.X, curentLocation.Y + 1)) {
		changeBordEl(curentLocation, false);
		curentLocation.Y = curentLocation.Y + 1;
		changeBordEl(curentLocation, true);
	}
}
void goLeft(Location& curentLocation) {
	if (canGotoThisLocation(curentLocation.X - 1, curentLocation.Y)) {
		changeBordEl(curentLocation, false);
		curentLocation.X = curentLocation.X - 1;
		changeBordEl(curentLocation, true);
	}
}
void goRight(Location& curentLocation) {
	if (canGotoThisLocation(curentLocation.X + 1, curentLocation.Y)) {
		changeBordEl(curentLocation, false);
		curentLocation.X = curentLocation.X + 1;
		changeBordEl(curentLocation, true);
	}
}

void select_location(Location& curentLocation) {
	char playercharonmap;
	char opcharonmap;
	if (movecount % 2) {
		playercharonmap = player1charonmap;
		opcharonmap = player2charonmap;
		p1count ++;
	}
	else {
		playercharonmap = player2charonmap;
		opcharonmap = player1charonmap;
		p2count ++;
	}	



	for (int s = 0; s < 8; s++) {
		int x = curentLocation.X;
		int y = curentLocation.Y;
		bool seetheopp = false;
	//	cout << (int)playercharonmap << othello_map[x][y] << "  " << x << y << "      ";
		while (othello_map[x][y] != playercharonmap && canGotoThisLocation((x = x + wayx[s]), (y = y + wayy[s]))) {
			//if (!canGotoThisLocation((x = x + wayx[s]), (y = y + wayy[s])))
			//	break;
			if (othello_map[x][y] == emptycharmap)
				break;
			if (othello_map[x][y] == opcharonmap) {
				seetheopp = true;
			}
			//cout << "hallod  " << othello_map[x][y] << "  " <<playercharonmap << "  "<< opcharonmap << seetheopp;
		}
	//	cout << "hallod       sd" << othello_map[x][y] << seetheopp;
		if (seetheopp && othello_map[x][y] == playercharonmap) {
			//cout << "hallod               dgfsdf" << s;
			x = curentLocation.X;
			y = curentLocation.Y;
			if (canGotoThisLocation(x, y)) {
				while (othello_map[x][y] != playercharonmap && canGotoThisLocation((x = x + wayx[s]), (y = y + wayy[s]))) {
					othello_map[x][y] = playercharonmap;
					if (movecount % 2) {
						p1count++;
						p2count--;
					}
					else {
						p2count++;
						p1count--;
					}
				}
			}
		}
		
	}

othello_map[curentLocation.X][curentLocation.Y] = playercharonmap;
	

	movecount++;
	updateMap();

	


	printgame_data(20, 20);
}
bool checkneareBlocks(int playercharonmap, int x, int y) {
	return	othello_map[x + 1][y + 1] == playercharonmap ||
			othello_map[x + 1][y	] == playercharonmap ||
			othello_map[x + 1][y - 1] == playercharonmap ||
			othello_map[x 	 ][y + 1] == playercharonmap ||
			othello_map[x 	 ][y	] == playercharonmap ||
			othello_map[x  	 ][y - 1] == playercharonmap ||
			othello_map[x - 1][y + 1] == playercharonmap ||
			othello_map[x - 1][y	] == playercharonmap ||
			othello_map[x - 1][y - 1] == playercharonmap;
}





void compute_where_can_go() {
	char playercharonmap;
	char opcharonmap;
	if (movecount % 2) //player1  should check 9 
	{
		playercharonmap = player1charonmap;
		opcharonmap = player2charonmap;
	}
	else {
		playercharonmap = player2charonmap;
		opcharonmap = player1charonmap;
	}
	for (int i = 0; i < mapWith; i++) {
		for (int j = 0; j < mapHeight; j++) {
			if (othello_map[i][j] == nextmovechar)
				othello_map[i][j] = emptycharmap;
			if (checkneareBlocks(opcharonmap, i, j)) {
				//cout << "ha can go";
				for (int s = 0; s < 8; s++) {
					int x = i;
					int y = j;
					bool seetheopp = false;
					while (othello_map[x][y] != playercharonmap && canGotoThisLocation((x = x + wayx[s]), (y = y + wayy[s]))) {
						if (mapisempty(othello_map[x][y]))
							break;
						if (othello_map[x][y] == opcharonmap) {
							seetheopp = true;
						}
					}
					if (seetheopp && othello_map[x][y] == playercharonmap) {
						x = i;
						y = j;
						if (canGotoThisLocation(x, y) && mapisempty(othello_map[x][y])) {
							othello_map[x][y] = 'z';

						}
					}
				}

			}
		}
	}
}





bool canSelectLocation(Location& curentLocation) {
	return (othello_map[curentLocation.X][curentLocation.Y]== nextmovechar);
		return false;
	return false;
	//return (othello_map[curentLocation.X][curentLocation.Y]== emptycharmap);
}

void changeLocation(Location& curentLocation) {
	char temp = _getch();
	if (temp == -32) {
		temp = _getch();
		switch (temp){
		case 72:goUp(curentLocation); break;//  up 

		case 80:goDown(curentLocation); break;// down

		case 75:goLeft(curentLocation); break; //left

		case 77:goRight(curentLocation); break; //right
		}		
	}
	else if (temp == 13) {
		if(canSelectLocation(curentLocation))
			select_location(curentLocation);
	}
}




void print_game_logo(int x, int y ) {
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
	printf("        _____\\/////____________\\/////_____\\///____\\///_____\\//////////___\\/////////___\\/////////______\\/////____"   );
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

	printgame_data(20 , 20);
	othello_map[2][2] = player1charonmap;
	othello_map[3][3] = player1charonmap;
	othello_map[3][2] = player2charonmap;
	othello_map[2][3] = player2charonmap;
	updateMap();
	while (true) {
		changeLocation(curentLocation);
	}
}